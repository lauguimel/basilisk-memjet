/**
# Laplace pressure equilibrium

A circular droplet of radius $R = 0.2$ is placed at the center of a
unit square domain. Both fluids are at rest. Surface tension $\sigma$
creates a pressure jump across the interface:

$$
\Delta P = \frac{\sigma}{R}
$$

This test verifies that the pressure jump converges to the analytical
value as the mesh is refined. It is the simplest possible two-phase
Basilisk simulation.

## Results

The simulation prints the relative error on the pressure jump to
stderr. A well-configured installation should give errors below 1%
at the default resolution (N = 64).
*/

#include "navier-stokes/centered.h"
#include "two-phase.h"
#include "tension.h"

#define SIGMA 1.0
#define RADIUS 0.2
#define LEVEL 6   // 2^6 = 64 cells per side

int main() {
  size(1.0);
  origin(-0.5, -0.5);
  init_grid(1 << LEVEL);

  rho1 = 1.0;
  rho2 = 1.0;
  mu1 = 0.01;
  mu2 = 0.01;
  f.sigma = SIGMA;

  TOLERANCE = 1e-6;
  DT = 1e-3;
  run();
}

event init(t = 0) {
  fraction(f, sq(RADIUS) - sq(x) - sq(y));
}

/**
We let the simulation run for a short time so that any spurious
currents develop, then check the pressure jump.
*/

event check(t = 0.1) {
  // Measure pressure inside (at center) and outside (at corner)
  double p_in = interpolate(p, 0., 0.);
  double p_out = interpolate(p, 0.45, 0.45);
  double dp_numerical = p_in - p_out;
  double dp_analytical = SIGMA / RADIUS;
  double error = fabs(dp_numerical - dp_analytical) / dp_analytical;

  fprintf(stderr, "# Laplace pressure test\n");
  fprintf(stderr, "# dp_analytical = %g\n", dp_analytical);
  fprintf(stderr, "# dp_numerical  = %g\n", dp_numerical);
  fprintf(stderr, "# relative error = %.4f%%\n", error * 100.);

  // Write pressure field to file
  FILE * fp = fopen("pressure.dat", "w");
  foreach()
    fprintf(fp, "%g %g %g %g\n", x, y, p[], f[]);
  fclose(fp);

  fprintf(stdout, "Laplace test complete. Relative error: %.4f%%\n",
          error * 100.);
}

/**
We also measure the maximum spurious current velocity, which should
be small compared to any physical velocity scale.
*/

event spurious(t = 0.1) {
  double umax = 0.;
  foreach() {
    double un = norm(u);
    if (un > umax) umax = un;
  }
  fprintf(stderr, "# max spurious velocity = %g\n", umax);
}

event end(t = 0.1) {}
