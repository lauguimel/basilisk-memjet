/**
# Coalescence of two droplets

Two identical droplets of radius $R$ are placed close together with a
small gap $\delta$. Surface tension drives the formation and growth of
a capillary bridge between them.

## Parameters

- Droplet radius: $R = 0.25$
- Gap between surfaces: $\delta = 0.02$ (center-to-center = $2R + \delta$)
- Ohnesorge number: $Oh = \mu / \sqrt{\rho \sigma R} = 0.01$ (inertial regime)
- Density ratio: 1000

## Physics

In the inertial regime ($Oh \ll 1$), the bridge radius grows as:
$$r_b \propto \left(\frac{\sigma R}{\rho}\right)^{1/4} t^{1/2}$$

The simulation outputs the bridge radius over time for comparison with
this scaling law.
*/

#include "navier-stokes/centered.h"
#include "two-phase.h"
#include "tension.h"

#define RADIUS 0.25
#define GAP 0.02
#define RHO_L 1.0
#define RHO_G 1e-3
#define SIGMA_COEFF 1.0
#define OH 0.01
#define MU_L (OH * sqrt(RHO_L * SIGMA_COEFF * RADIUS))

#define MAXLEVEL 9
#define MINLEVEL 5

int main() {
  size(2.0);
  origin(-1.0, -1.0);
  init_grid(1 << MINLEVEL);

  rho1 = RHO_L;
  rho2 = RHO_G;
  mu1 = MU_L;
  mu2 = MU_L / 50.;
  f.sigma = SIGMA_COEFF;

  DT = 1e-4;
  TOLERANCE = 1e-5;
  run();
}

event init(t = 0) {
  // Two droplets centered at (±(R + GAP/2), 0)
  double cx = RADIUS + GAP / 2.;
  fraction(f, union(
    sq(RADIUS) - sq(x + cx) - sq(y),
    sq(RADIUS) - sq(x - cx) - sq(y)
  ));
}

/**
Adaptive mesh refinement — refine heavily near the bridge region.
*/

event adapt(i++) {
  adapt_wavelet({f, u.x, u.y}, (double[]){1e-3, 1e-2, 1e-2},
                maxlevel = MAXLEVEL, minlevel = MINLEVEL);
}

event logfile(i += 10) {
  fprintf(stderr, "t = %.6f, i = %d, dt = %g, cells = %ld\n",
          t, i, dt, grid->tn);
}

/**
Measure the bridge radius: scan along x = 0 (the symmetry plane)
and find the extent of the liquid region in the y-direction.
*/

event bridge(t = 0; t += 1e-3; t <= 0.5) {
  static FILE * fp = fopen("bridge.dat", "w");
  if (t == 0)
    fprintf(fp, "# t bridge_radius\n");

  // Find maximum y where f > 0.5 along x ≈ 0
  double rb = 0.;
  foreach() {
    if (fabs(x) < Delta && f[] > 0.5 && fabs(y) > rb)
      rb = fabs(y);
  }
  fprintf(fp, "%g %g\n", t, rb);
  fflush(fp);
}

/**
Output interface shape at regular intervals.
*/

event snapshots(t = 0; t <= 0.5; t += 0.05) {
  char name[80];
  sprintf(name, "interface-%05.3f.dat", t);
  FILE * fp = fopen(name, "w");
  output_facets(f, fp);
  fclose(fp);
}

event end(t = 0.5) {
  fprintf(stdout, "# Coalescence simulation complete.\n");
  fprintf(stdout, "# Oh = %g, R = %g, gap = %g\n", OH, RADIUS, GAP);
  fprintf(stdout, "# Bridge data: bridge.dat\n");
  fprintf(stdout, "# Interface files: interface-*.dat\n");
}
