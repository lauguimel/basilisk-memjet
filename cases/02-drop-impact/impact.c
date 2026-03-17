/**
# Axisymmetric drop impact on a solid surface

A spherical droplet of diameter $D = 1$ impacts a flat solid surface
at velocity $U = 1$. The simulation is axisymmetric (2D with `axi.h`).

Inspired by the Basilisk test case `fall.c` (Figueiredo et al., 2016).

## Conventions with `axi.h`

With `axi.h`, the x-axis is the **axis of symmetry** and the y-axis
is the **radial** direction. The wall (solid surface) is at the
**left** boundary (x = 0).

## Dimensionless parameters

All quantities are made dimensionless using $D$, $U$, and $\rho_l$:

- $Re = \rho_l U D / \mu_l = 100$
- $We = \rho_l U^2 D / \sigma = 50$ (moderate spreading)
- $Fr = U / \sqrt{g D} = 2.26$ (same as fall.c)
- Density ratio: $\rho_l / \rho_g = 1000$
- Viscosity ratio: $\mu_l / \mu_g = 1000$
*/

#include "axi.h"
#include "navier-stokes/centered.h"
#include "two-phase.h"
#include "tension.h"

#define RE 100.
#define WE 50.
#define FR 2.26
#define RHO_r 0.001     // rho_g / rho_l
#define MU_r  0.001     // mu_g / mu_l
#define LEVEL 7

/**
The drop comes from the right. We allow outflow there. */

u.n[right] = neumann(0);
p[right]   = dirichlet(0);

/**
The wall is at the left side. No-slip + non-wetting for VOF. */

u.t[left] = dirichlet(0);
f[left]   = 0.;

int main() {
  size(2.6);
  init_grid(1 << LEVEL);

  rho1 = 1.;
  rho2 = RHO_r;
  mu1 = 1. / RE;
  mu2 = MU_r / RE;
  f.sigma = 1. / WE;

  DT = 2e-3;
  TOLERANCE = 1e-4;
  run();
}

event init(t = 0) {
  /**
  The drop is centered at (2, 0) with radius 0.5 (diameter 1). */

  fraction(f, sq(0.5) - sq(x - 2.) - sq(y));

  /**
  The initial velocity of the droplet is -1 (toward the wall). */

  foreach()
    u.x[] = -f[];
}

/**
Gravity acts in the negative x-direction (toward the wall). */

event acceleration(i++) {
  face vector av = a;
  foreach_face(x)
    av.x[] -= 1. / sq(FR);
}

/**
Adaptive mesh refinement based on the interface and velocity. */

#if TREE
event adapt(i++) {
  adapt_wavelet({f, u.x, u.y}, (double[]){1e-2, 5e-3, 5e-3},
                maxlevel = LEVEL, minlevel = LEVEL - 2);
}
#endif

/**
Track the spreading diameter of the droplet on the wall. */

event logfile(i += 20; t <= 5.) {
  scalar pos[];
  position(f, pos, {0, 1});
  double dmax = 2. * statsf(pos).max;
  fprintf(stderr, "t = %.4f, spread = %.4f, dt = %g, cells = %ld\n",
          t, dmax, dt, grid->tn);
}

/**
Output the interface shape at regular intervals. */

event snapshots(t = 0; t <= 5.; t += 0.5) {
  char name[80];
  sprintf(name, "interface-%04.2f.dat", t);
  FILE * fp = fopen(name, "w");
  output_facets(f, fp);
  fclose(fp);
}

event end(t = 5.) {
  fprintf(stdout, "# Drop impact simulation complete.\n");
  fprintf(stdout, "# Re = %g, We = %g, Fr = %g\n", RE, WE, FR);
}
