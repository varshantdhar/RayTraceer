#include <stdio.h>
#include <string.h>
#include "raytracer.h"

/* ===================================== */ 
/* === sphere operations => sphere.c === */ 
/* ===================================== */ 

/* sphere_expr : construct a sphere record,
 * setting object pointer to NULL (unused in this implementation)
 * and texture to NULL (future expansion)
 */
sphere sphere_expr(location v, 
		   double r, 
		   color(*sc)(object *,location), 
		   color sh)
{
  object *new_obj = (object*)malloc(sizeof(object));
  if (new_obj==NULL) {
    fprintf(stderr,"error (sphere_expr): malloc failed\n");
    exit(1);
  }
  new_obj->tag = SPHERE;
  new_obj->o.s.center = v;
  new_obj->o.s.radius = r;
  new_obj->o.s.surface_color = sc;
  new_obj->o.s.shine = sh;
  new_obj->o.s.o = NULL;
  new_obj->t = NULL; // texture reserved for project B
  return new_obj->o.s;
}

const char *_sph_fmt =
  "sphere(\n  center:%s\n  radius:%lf\n  surface_color:<FN@%p>\n"
   "  shine:%s\n  object:%p\n)";

char *sphere_tos(sphere s)
{
  char buf[1024];
  memset(buf,0,1024);
  char *ctr = xyz_tos(s.center);
  char *shn = color_tos(s.shine);
  snprintf(buf,1024,_sph_fmt,ctr,s.radius,s.surface_color,shn,s.o);
  free(ctr);
  free(shn);
  return strdup(buf);
}

void sphere_show(FILE *f, sphere s)
{
  char *t = sphere_tos(s);
  fprintf(f,"%s",t);
  free(t);
}
