#include "raytracer.h"

/* ======================================= */ 
/* === poster operations => poster.c ===== */ 
/* ======================================= */ 

/* poster_expr : construct a poster record,
 * setting object pointer to NULL (unused in this implementation)
 * and texture to NULL (future expansion)
 */
poster poster_expr(location ulc, 
		   double w, 
		   double h, 
		   color(*sc)(object*,location), 
		   color sh)
{
  object *new_obj = (object*)malloc(sizeof(object));
  if (new_obj==NULL) {
    fprintf(stderr,"error (poster_expr): malloc failed\n");
    exit(1);
  }
  if (w<0) {
    fprintf(stderr,"error (poster_expr): negative w: %lf\n",w);
    exit(1);
  }
  if (h<0) {
    fprintf(stderr,"error (poster_expr): negative h: %lf\n",w);
    exit(1);
  }
  new_obj->tag = POSTER;
  new_obj->o.p.upper_left = ulc;
  new_obj->o.p.w = w;
  new_obj->o.p.h = h;
  new_obj->o.p.surface_color = sc;
  new_obj->o.p.shine = sh;
  new_obj->o.p.o = NULL;
  new_obj->t = NULL; // texture reserved for project B
    
  return new_obj->o.p;
}

const char *_pst_fmt =
  "poster(\n  upper_left:%s\n  w:%lf\n  h:%lf\n  surface_color:<FN@%p>\n  shine:%s\n  object:%p\n)";

char *poster_tos(poster s)
{
  char buf[1024];
  memset(buf,0,1024);
  char *ulc = xyz_tos(s.upper_left);
  char *shn = color_tos(s.shine);
  snprintf(buf,1024,_pst_fmt,ulc,s.w,s.h,s.surface_color,shn,s.o);
  free(ulc);
  free(shn);
  return strdup(buf);
}

void poster_show(FILE *f, poster s)
{
  char *t = poster_tos(s);
  fprintf(f,"%s",t);
  free(t);
}
