#include <stdio.h>
#include <string.h>
#include "raytracer.h"

/* ===================================== */ 
/* === cube operations => cube.c === */ 
/* ===================================== */ 

/* cube_expr : construct a cube record,
 * setting object pointer to NULL (unused in this implementation)
 * and texture to NULL (future expansion)
 */
cube cube_expr(location ftl, 
		   double l, 
		   color(*sc)(object *,location), 
		   color sh)
{
  object *new_obj = (object*)malloc(sizeof(object));
  if (new_obj==NULL) {
    fprintf(stderr,"error (sphere_expr): malloc failed\n");
    exit(1);
  }
  new_obj->tag = CUBE;
  new_obj->o.c.front_top_left = ftl;
  new_obj->o.c.length = l;
  new_obj->o.c.surface_color = sc;
  new_obj->o.c.shine = sh;
  new_obj->o.c.o = NULL;
  new_obj->t = NULL; // texture reserved for project B
  return new_obj->o.c;
}

const char *_cub_fmt =
  "cube(\n  front-top-left:%s\n  length:%lf\n  surface_color:<FN@%p>\n"
   "  shine:%s\n  object:%p\n)";

char *cube_tos(cube c)
{
  char buf[1024];
  memset(buf,0,1024);
  char *ctr = xyz_tos(c.front_top_left);
  char *shn = color_tos(c.shine);
  snprintf(buf,1024,_cub_fmt,ctr,c.length,c.surface_color,shn,c.o);
  free(ctr);
  free(shn);
  return strdup(buf);
}

void cube_show(FILE *f, cube c)
{
  char *t = cube_tos(c);
  fprintf(f,"%s",t);
  free(t);
}
