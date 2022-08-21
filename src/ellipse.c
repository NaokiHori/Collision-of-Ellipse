#include <math.h>
#include "others.h"
#include "ellipse.h"


// compute x-coordinate of evolute
double ellipse_compute_xc(const double a, const double b, const double t){
  return a*(1.-pow(b/a, 2.))*pow(cos(t), 3.);
}

// compute y-coordinate of evolute
double ellipse_compute_yc(const double a, const double b, const double t){
  return b*(1.-pow(a/b, 2.))*pow(sin(t), 3.);
}

double ellipse_compute_curvature(const double a, const double b, const double t){
  const double num = a*b;
  const double den = pow(pow(a*sin(t), 2.)+pow(b*cos(t), 2.), 1.5);
  return num/den;
}

// find t, with which a vector from (a cos(t), b sin(t)) to (x, y)
//   becomes a normal vector to the ellipse
// https://blog.chatfield.io/simple-method-for-distance-to-ellipse/
double ellipse_find_normal_t(const double a, const double b, const double xp, const double yp){
  const double small = 1.e-8;
  const int itermax = 20;
  // consider in the 1st quadrant
  double abs_xp = fabs(xp);
  double abs_yp = fabs(yp);
  double t = 0.25*M_PI;
  for(int iter = 0; iter < itermax; iter++){
    // point on ellipse
    double xe = a*cos(t);
    double ye = b*sin(t);
    // evolute of the given t
    double xc = ellipse_compute_xc(a, b, t);
    double yc = ellipse_compute_yc(a, b, t);
    // compute residual
    double dxe =     xe-xc;
    double dye =     ye-yc;
    double dxp = abs_xp-xc;
    double dyp = abs_yp-yc;
    double norme = fmax(hypot(dxe, dye), small);
    double normp = fmax(hypot(dxp, dyp), small);
    double dc = norme*asin((dxe*dyp-dye*dxp)/(norme*normp));
    double dt = dc/sqrt(a*a+b*b-xe*xe-ye*ye);
    // update
    t += dt;
    // limit to the 1st quadrant
    t = fmin(0.5*M_PI, t);
    t = fmax(      0., t);
    if(fabs(dt) < small){
      break;
    }
  }
  // for the other quadrants
  if(xp < 0.) t = M_PI-t;
  if(yp < 0.) t =     -t;
  return t;
}

