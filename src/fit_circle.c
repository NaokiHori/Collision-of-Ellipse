#include <stdio.h>
#include <stdlib.h>
#include "others.h"
#include "ellipse.h"
#include "example.h"


int fit_circle(void){
  const double e0_a     = 2.0;
  const double e0_b     = 1.5;
  const double e0_x     = 0.5;
  const double e0_y     = 0.5;
  const double e0_theta = 0.2;
  const double e0_xp    = 3.0;
  const double e0_yp    = 2.0;
  // transform coordinate so that
  //   1. the ellipse is located at the origin
  //   2. the ellipse is not rotated
  double e0_xp_, e0_yp_;
  shift_and_rotate(-e0_x, -e0_y, -e0_theta, e0_xp, e0_yp, &e0_xp_, &e0_yp_);
  // find optimum
  double e0_t = ellipse_find_normal_t(e0_a, e0_b, e0_xp_, e0_yp_);
  // find center of evolute
  double e0_xc_ = ellipse_compute_xc(e0_a, e0_b, e0_t);
  double e0_yc_ = ellipse_compute_yc(e0_a, e0_b, e0_t);
  // coordinate transform to the original system
  double e0_xc, e0_yc;
  rotate_and_shift(+e0_x, +e0_y, +e0_theta, e0_xc_, e0_yc_, &e0_xc, &e0_yc);
  // radius of the fitted circle
  double e0_r = 1./ellipse_compute_curvature(e0_a, e0_b, e0_t);
  FILE *fp = fopen("fit-circle.dat", "w");
  fprintf(fp, "e0_a     = % .15f\n", e0_a    );
  fprintf(fp, "e0_b     = % .15f\n", e0_b    );
  fprintf(fp, "e0_x     = % .15f\n", e0_x    );
  fprintf(fp, "e0_y     = % .15f\n", e0_y    );
  fprintf(fp, "e0_theta = % .15f\n", e0_theta);
  fprintf(fp, "e0_xp    = % .15f\n", e0_xp   );
  fprintf(fp, "e0_yp    = % .15f\n", e0_yp   );
  fprintf(fp, "e0_xc    = % .15f\n", e0_xc   );
  fprintf(fp, "e0_yc    = % .15f\n", e0_yc   );
  fprintf(fp, "e0_r     = % .15f\n", e0_r    );
  fclose(fp);
  return 0;
}

