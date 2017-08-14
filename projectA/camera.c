#include "raytracer.h"

/*
typedef struct {
  location loc;
  unsigned int h;
  unsigned int w;
} camera;
*/

/* ===================================== */ 
/* === camera operations => camera.c === */ 
/* ===================================== */ 

const char *_cam_fmt = "camera(loc:%s,h:%u,w:%u)";

char *camera_tos(camera cm)
{
  char buf[128];
  memset(buf,0,128);
  char *loc = xyz_tos(cm.loc);
  snprintf(buf,128,_cam_fmt,loc,cm.h,cm.w);
  free(loc);
  return strdup(buf);
}

void camera_show(FILE *f, camera cm)
{
  char *s = camera_tos(cm);
  fprintf(f,"%s",s);
  free(s);
}
