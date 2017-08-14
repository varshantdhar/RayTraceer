#include "raytracer.h"

/* =============================== */ 
/* === ray operations => ray.c === */ 
/* =============================== */ 

/* see web page for ray_position code */
xyz ray_position(ray r, double t)
{
  return xyz_add(r.origin, xyz_scale(t, r.dir));
}

const char *_ray_fmt = "ray(origin:%s,dir:%s)";

char *ray_tos(ray r)
{
  char buf[256];
  memset(buf,0,256);
  char *ost = xyz_tos(r.origin);
  char *dst = xyz_tos(r.dir);
  snprintf(buf,256,_ray_fmt,ost,dst);
  free(ost);
  free(dst);
  return strdup(buf);
}

void ray_show(FILE *f, ray r)
{
  char *s = ray_tos(r);
  fprintf(f,"%s",s);
  free(s);
}


