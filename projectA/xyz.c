#include <string.h>
#include <math.h>
#include "raytracer.h"

/* ================================== */ 
/* === vector operations => xyz.c === */ 
/* ================================== */ 

xyz xyz_expr(double x, double y, double z)
{
  xyz v = {x,y,z};
  return v;
}

/* xyz_add : add two vectors */
/* (x,y,z) + (X,Y,Z) = (x+X,y+Y,z+Z) */
xyz xyz_add(xyz v1, xyz v2) 
{
  return xyz_expr(v1.x+v2.x,v1.y+v2.y,v1.z+v2.z);
}

/* xyz_sub : subtract the second vector from the first */
/* (x,y,z) - (X,Y,Z) = (x-X,y-Y,z-Z) */
xyz xyz_sub(xyz v1, xyz v2)
{
  return xyz_expr(v1.x-v2.x,v1.y-v2.y,v1.z-v2.z);
}

/* xyz_neg : negate a vector */
/* -(x,y,z) = (-x,-y,-z) */
xyz xyz_neg(xyz v)
{
  return xyz_expr(-v.x,-v.y,-v.z);
}

/* xyz_scale : scale the components of a vector by scalar s */
/* s(x,y,z) = (sx,sy,sz) */
xyz xyz_scale(double s, xyz v)
{
  return xyz_expr(s*v.x,s*v.y,s*v.z);
}

/* xyz_dot : compute the dot product of two vectors */
/* (x,y,z) dot (X,Y,Z) = xX+yY+zZ */
double xyz_dot(xyz v1, xyz v2)
{
  return v1.x*v2.x+v1.y*v2.y+v1.z*v2.z;
}

/* xyz_mag : compute the magnitude of a vector */
/* the square root of the sum of the squares of the components */
double xyz_mag(xyz v)
{
  return sqrt(v.x*v.x+v.y*v.y+v.z*v.z);
}

/* xyz_norm : normalize the vector */
/* i.e., a vector pointing in the same direction whose magnitude is 1 */
/* - normalize a vector by scaling by 1 over its magnitude */
/* - if a vector's magnitude is zero, it can't be normalized; return it */
xyz xyz_norm(xyz v)
{
  double m = xyz_mag(v);
  return m==0 ? v : xyz_scale(1.0/m,v);
}

const char *_xyz_fmt = "xyz(%lf,%lf,%lf)";

char *xyz_tos(xyz v)
{
  char buf[128];
  memset(buf,0,128);
  snprintf(buf,128,_xyz_fmt,v.x,v.y,v.z);
  return strdup(buf);
}

void xyz_show(FILE *f, xyz v)
{
  fprintf(f,_xyz_fmt,v.x,v.y,v.z);
}

