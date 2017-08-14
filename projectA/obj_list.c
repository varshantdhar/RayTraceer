#include <string.h>
#include "raytracer.h"

/* ======================================*/ 
/* === object list ops => obj_list.c === */ 
/* ======================================*/ 

obj_list *ol_cons(object o, obj_list *os)
{
  obj_list *new_list = (obj_list*)malloc(sizeof(obj_list));
  if (new_list==NULL) {
    fprintf(stderr,"error (ol_cons): malloc failed\n");
    exit(1);
  }
  new_list->obj = o;
  new_list->next = os;
  return new_list;
}

obj_list *ol_singleton(object s)
{
  return ol_cons(s,NULL);
}

/* helper function to concatenate three strings */
char *_cat3(char *s1, char *s2, char *s3)
{
  int i;
  unsigned int len1 = strlen(s1);
  unsigned int len2 = strlen(s2);
  unsigned int len3 = strlen(s3);
  char *s4 = (char*)malloc(len1+len2+len3+1);
  for (i=0; i<len1; i++)
    s4[i] = s1[i];
  for (i=0; i<len2; i++)
    s4[len1+i] = s2[i];
  for (i=0; i<len3; i++)
    s4[len1+len2+i] = s3[i];
  s4[len1+len2+len3]='\0';
  return s4;
}

char *ol_tos(obj_list *os)
{
  char *s = strdup("obj_list[");
  char *t = strdup("");
  while (os) {
    char *o = object_tos(os->obj);
    t = s;
    s = _cat3(s,o,os->next?":":"");
    os=os->next;
    free(o);
    free(t);
  }
  t = s;
  s = _cat3(s,"]","");
  free(t);
  return s;
}

void ol_show(FILE *f, obj_list *os)
{
  char *s = ol_tos(os);
  fprintf(f,"%s",s);
  free(s);
}

void ol_free(obj_list *ss)
{
  if (ss) {
    ol_free(ss->next);
    free(ss);
  }
}
