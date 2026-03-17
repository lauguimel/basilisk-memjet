/**
# Axisymmetric drop impact on a solid surface

A spherical droplet of diameter $D$ impacts a flat solid surface at
velocity $U$. The simulation is axisymmetric (2D with `axi.h`).

## Parameters

- Diameter: $D = 1$
- Impact velocity: $U = 1$
- Density ratio: $\rho_l / \rho_g = 1000$
- Reynolds number: $Re = \rho_l U D / \mu_l = 1000$
- Weber number: $We = \rho_l U^2 D / \sigma = 250$

## Geometry

The drop center starts at $(0, 1.5 D/2)$ above the surface (y = 0).
The domain is $[0, 4] \times [0, 4]$, axisymmetric around $y = 0$
(which is actually the axis of symmetry in Basilisk's `axi.h`).

Note: with `axi.h`, the x-axis is the axis of symmetry and the
y-axis is the radial direction. So we set up the drop along x.
*/

#include "axi.h"
#include "navier-stokes/centered.h"
#include "two-phase.h"
#include "tension.h"

// Physical parameters
#define DIAMETER 1.0
#define RADIUS (DIAMETER / 2.0)
#define U0 (-1.0)     // downward velocity
#define RHO_L 1.0
#define RHO_G 1e-3
#define RE 1000.
#define WE 250.
#define MU_L (RHO_L * fabs(U0) * DIAMETER / RE)
#define SIGMA_COEFF (RHO_L * sq(U0) * DIAMETER / WE)

#define MAXLEVEL 8
#define MINLEVEL 4

int main() {
  size(4.0);
  origin(0., 0.);
  init_grid(1 << MINLEVEL);

  rho1 = RHO_L;
  rho2 = RHO_G;
  mu1 = MU_L;
  mu2 = MU_L / 50.;
  f.sigma = SIGMA_COEFF;

  DT = 1e-3;
  TOLERANCE = 1e-4;
  run();
}

// No-slip on the left boundary (the solid wall in axisymmetric coords)
u.t[left] = dirichlet(0.);
u.n[left] = dirichlet(0.);

// Outflow on top and right
u.n[right] = neumann(0.);
p[right] = dirichlet(0.);
u.n[top] = neumann(0.);
p[top] = dirichlet(0.);

event init(t = 0) {
  // Drop centered at (1.5*RADIUS, 0) — above the wall at x = 0
  fraction(f, sq(RADIUS) - sq(x - 3. * RADIUS) - sq(y));

  // Initial downward velocity inside the drop
  foreach()
    u.x[] = f[] * U0;
}

/**
Gravity acts in the negative x-direction (toward the wall).
*/

event acceleration(i++) {
  face vector av = a;
  foreach_face(x)
    av.x[] -= 9.81;
}

/**
Adaptive mesh refinement based on the interface and velocity.
*/

event adapt(i++) {
  adapt_wavelet({f, u.x, u.y}, (double[]){1e-3, 1e-2, 1e-2},
                maxlevel = MAXLEVEL, minlevel = MINLEVEL);
}

event logfile(i += 20) {
  fprintf(stderr, "t = %.4f, i = %d, dt = %g, cells = %ld\n",
          t, i, dt, grid->tn);
}

/**
Output the interface shape at regular intervals.
*/

event snapshots(t = 0; t <= 3.0; t += 0.25) {
  char name[80];
  sprintf(name, "interface-%04.2f.dat", t);
  FILE * fp = fopen(name, "w");
  output_facets(f, fp);
  fclose(fp);
}

event end(t = 3.0) {
  fprintf(stdout, "# Drop impact simulation complete.\n");
  fprintf(stdout, "# Re = %g, We = %g\n", RE, WE);
  fprintf(stdout, "# Interface files: interface-*.dat\n");
}
