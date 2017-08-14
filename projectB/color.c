#include <string.h>
#include "raytracer.h"

/* =================================== */ 
/* === color operations => color.c === */ 
/* =================================== */ 

void _color_expr_check(char c, double x)
{
  if (x<0||x>1) {
    fprintf(stderr,"error (color_expr): %c out of range: %lf\n",c,x);
    exit(1);
  }
}

/* reject if any component is outside [0.0,1.0] */
color color_expr(double r, double g, double b)
{
  _color_expr_check('r',r);
  _color_expr_check('g',g);
  _color_expr_check('b',b);
  color c = {r,g,b};
  return c;
}

/* color_modulate : multiply the components of two colors */
/* (r,g,b) * (R,G,B) = (rR,gG,bB) */
color color_modulate(color c1, color c2)
{
  return color_expr(c1.r*c2.r,c1.g*c2.g,c1.b*c2.b);
}

/* _clamp : restrict x to [0,1] by clipping */
double _clamp(double x)
{
  return x<0?0:(x>1?1:x);
}

/* color_scale : scale each component by given scalar, but */
/* don't allow any component in the result to fall outside [0,1] */
color color_scale(double s, color c)
{
  return color_expr(_clamp(s*c.r),_clamp(s*c.g),_clamp(s*c.b));
}

/* add components of colors, but */
/* don't allow any component in the result to exceed 1 */
color color_add(color c1, color c2)
{
  return color_expr(_clamp(c1.r+c2.r),_clamp(c1.g+c2.g),_clamp(c1.b+c2.b));
}

const char *_color_fmt = "color(%lf,%lf,%lf)";

char *color_tos(color c)
{
  char buf[128];
  memset(buf,0,128);
  snprintf(buf,128,_color_fmt,c.r,c.g,c.b);
  return strdup(buf);
}

void color_show(FILE *f, color c)
{
  fprintf(f,_color_fmt,c.r,c.g,c.b);
}

unsigned char _byte(double x)
{
  return (unsigned char)(x*255.0);
}

/* color_show_bytes */
/* print three integers on [0,255] with spaces in between and a newline */
/* suitable for use in a PPM */
void color_show_bytes(FILE *f, color c)
{
  fprintf(f,"%u %u %u\n",_byte(c.r),_byte(c.g),_byte(c.b));
}

