#include "raytracer.h"

/*
typedef struct {
  color bg;
  obj_list *objects;
  light light; // not used in project A
  color amb; // not used in project A
} scene;
*/

/* =================================== */ 
/* === scene operations => scene.c === */ 
/* =================================== */ 

const char *_scene_fmt = 
  "--- begin scene\nbg:    %s\nlight: (unused)\namb:   (unused)\n"
  "objs:  %s\n--- end scene\n";

char *scene_tos(scene sc)
{
  char buf[1024];
  memset(buf,0,1024);
  char *bgs  = color_tos(sc.bg);
  char *os   = ol_tos(sc.objects);
  snprintf(buf,1024,_scene_fmt,bgs,os);
  char *result = strdup(buf);
  free(bgs);
  free(os);
  return result;
}

void scene_show(FILE *f, scene sc)
{
  char *s = scene_tos(sc);
  fprintf(f,"%s",s);
  free(s);
}
