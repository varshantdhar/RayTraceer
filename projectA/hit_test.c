#include "raytracer.h"

/*
typedef struct {
  enum {MISS=0,HIT=1} miss;
  double t;
  location hit_point;
  color surf; // not used in project A
  color shine; // not used in project A
  direction surf_norm; // not used in project A
} hit_test;
*/

/* ========================================= */ 
/* === hit_test operations => hit_test.c === */ 
/* ========================================= */ 

const char *_hit_test_fmt = 
  "hit_test(\n  t:%lf\n  hit_point:%s\n  surf:%s\n  shine:%s\n  surf_norm:%s\n)";

char *hit_test_tos(hit_test t)
{
  if (t.miss==MISS)
    return strdup("MISS");
  char buf[1024];
  memset(buf,0,1024);
  char *shp = xyz_tos(t.hit_point);
  char *srf = color_tos(t.surf);
  char *shn = color_tos(t.shine);
  char *nrm = xyz_tos(t.surf_norm);
  snprintf(buf,1024,_hit_test_fmt,t.t,shp,srf,shn,nrm);
  free(shp);
  free(srf);
  free(shn);
  free(nrm);  
  return strdup(buf);
}

void hit_test_show(FILE *f, hit_test t)
{
  char *s = hit_test_tos(t);
  fprintf(f,"%s",s);
  free(s);
}
