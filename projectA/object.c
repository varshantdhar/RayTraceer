#include "raytracer.h"

/* ===================================== */ 
/* === object operations => object.c === */ 
/* ===================================== */ 

char *object_tos(object o)
{
  char buf[1024];
  memset(buf,0,1024);
  char *s;
  switch (o.tag) {
  case SPHERE:
    s = sphere_tos(o.o.s);
    break;
  case POSTER:
    s = poster_tos(o.o.p);
    break;
  default:
    fprintf(stderr,"error (object_tos): unexpected tag in object %d",o.tag);
    exit(1);
  }
  snprintf(buf,1024,"object[\n%s,\n  texture:%p\n]",s,o.t);
  free(s);
  return strdup(buf);    
}

void object_show(FILE *f, object o)
{
  char *s = object_tos(o);
  fprintf(f,"%s",s);
  free(s);
}
