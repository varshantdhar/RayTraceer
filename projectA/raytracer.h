#ifndef __RAYTRACER_H__
#define __RAYTRACER_H__

/* raytracer.h
 * CS152 Winter 2016
 * Project A
 * Feb 2016
 */

#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

/* ====================================== */ 
/* ====================================== */ 
/* === STRUCTURE AND TYPE DEFINITIONS === */ 
/* ====================================== */ 
/* ====================================== */ 

typedef unsigned char byte;

typedef struct {
  double x;
  double y;
  double z;
} xyz;

/* Note: Because xyz can be used for both a particular location
 * and a directional vector, we can use the same struct and 
 * typedef for clarity. We may use the three names interchangeably.
 * This allows us to share the basic operations (add, sub, mag, scale).
 * We are not required to use location and direction, it is merely a 
 * tool to help us understand what our (or someone else's) code is doing.
 */

typedef xyz location;
typedef xyz direction;

typedef struct {
  /* each value must be on interval [0,1] */
  double r;
  double g;
  double b;
} color;

typedef struct {
  location origin;
  direction dir;
} ray;

typedef struct _object object;

typedef struct {
  location center;
  double radius;
  color(*surface_color)(object *o, location loc);
  color shine; // not used in project A
  object *o;
} sphere;

typedef struct {
  xyz upper_left;
  double w;
  double h;
  color(*surface_color)(object *poster, location intersection);
  color shine; // not used in project A
  object *o;
} poster;

union object_union {
  sphere s;
  poster p;
};

/* texture used in project B */
typedef struct {
  color **pixels;
  int height;    
  int width;     
} texture;

struct _object{
  enum  { SPHERE, POSTER } tag;
  union object_union o;
  texture *t; // NULL in project A
};

typedef struct obj_list obj_list;
/* convention: NULL is the empty object list */
struct obj_list {
  object obj;
  obj_list *next;
};

/* not used in project A */
typedef struct {
  direction dir; /* note: the vector points AT the light */
  color c;
} light;

typedef struct {
  color bg;
  obj_list *objects;
  light light; // not used in project A
  color amb; // not used in project A
} scene;

typedef struct {
  enum {MISS=0,HIT=1} miss;
  double t;
  location hit_point;
  color surf; // not used in project A
  color shine; // not used in project A
  direction surf_norm; // not used in project A
} hit_test;

typedef struct {
  location loc;
  unsigned int h;
  unsigned int w;
} camera;

typedef struct {
  camera c;
  scene s;
} stage;

/* ================== */ 
/* ================== */ 
/* === OPERATIONS === */ 
/* ================== */ 
/* ================== */ 

/* Please note: the following is not necessarily every operation you
 * will need, nor will you necessarily use every operation in your
 * implementation.  Add and implement additional operations as you
 * require them.
 */

/* ================================== */ 
/* === vector operations => xyz.c === */ 
/* ================================== */ 

xyz xyz_expr(double x, double y, double z);

/* xyz_add : add two vectors */
/* (x,y,z) + (X,Y,Z) = (x+X,y+Y,z+Z) */
xyz xyz_add(xyz v1, xyz v2);

/* xyz_sub : subtract the second vector from the first */
/* (x,y,z) - (X,Y,Z) = (x-X,y-Y,z-Z) */
xyz xyz_sub(xyz v1, xyz v2);

/* xyz_neg : negate a vector */
/* -(x,y,z) = (-x,-y,-z) */
xyz xyz_neg(xyz v);

/* xyz_scale : scale the components of a vector by scalar s */
/* s(x,y,z) = (sx,sy,sz) */
xyz xyz_scale(double s, xyz v);

/* xyz_dot : compute the dot product of two vectors */
/* (x,y,z) dot (X,Y,Z) = xX+yY+zZ */
double xyz_dot(xyz v1, xyz v2);

/* xyz_mag : compute the magnitude of a vector */
/* the square root of the sum of the squares of the components */
double xyz_mag(xyz v);

/* xyz_norm : normalize the vector */
/* i.e., a vector pointing in the same direction whose magnitude is 1 */
/* - normalize a vector by scaling by 1 over its magnitude */
/* - if a vector's magnitude is zero, it can't be normalized; return it */
xyz xyz_norm(xyz v);

char *xyz_tos(xyz v);

void xyz_show(FILE *f, xyz v);

/* =================================== */ 
/* === color operations => color.c === */ 
/* =================================== */ 

/* reject if any component is outside [0.0,1.0] */
color color_expr(double r, double g, double b);

/* color_modulate : multiply the components of two colors */ // <== or -> of
/* (r,g,b) * (R,G,B) = (rR,gG,bB) */
color color_modulate(color c1, color c2);

/* color_scale : scale each component by given scalar, but */
/* don't allow any component in the result to fall outside [0,1] */
color color_scale(double s, color c);

/* add components of colors, but */
/* don't allow any component in the result to exceed 1 */
color color_add(color c1, color c2);

char *color_tos(color c);

void color_show(FILE *f, color c);

/* color_show_bytes */
/* print three integers on [0,255] with spaces in between and a newline */
/* suitable for use in a PPM */
void color_show_bytes(FILE *f, color c);

/* =============================== */ 
/* === ray operations => ray.c === */ 
/* =============================== */ 

/* see web page for ray_position code */
xyz ray_position(ray r, double t);

char *ray_tos(ray r);

void ray_show(FILE *f, ray r);

/* ===================================== */ 
/* === sphere operations => sphere.c === */ 
/* ===================================== */ 

sphere sphere_expr(location v, double r, color(*sc)(object *,location), color sh);

char *sphere_tos(sphere s);

void sphere_show(FILE *f, sphere s);

/* ======================================= */ 
/* === poster operations => poster.c ===== */
/* ======================================= */ 

poster poster_expr(location ulc, double w, double h, color(*sc)(object*,location), color sh);

char *poster_tos(poster s);

void poster_show(FILE *f, poster s);

/* ===================================== */ 
/* === object operations => object.c === */ 
/* ===================================== */ 

char *object_tos(object o);

void object_show(FILE *f, object o);

/* ======================================== */ 
/* === object list ops => obj_list.c === */ 
/* ======================================== */ 

obj_list *ol_cons(object o, obj_list *os);

obj_list *ol_singleton(object s);

char *ol_tos(obj_list *os);

void ol_show(FILE *f, obj_list *os);

void ol_free(obj_list *ss);

/* =================================== */ 
/* === scene operations => scene.c === */ 
/* =================================== */ 

char *scene_tos(scene sc);

void scene_show(FILE *f, scene sc);

/* ========================================= */ 
/* === hit_test operations => hit_test.c === */ 
/* ========================================= */ 

char *hit_test_tos(hit_test t);

void hit_test_show(FILE *f, hit_test t);

/* ===================================== */ 
/* === camera operations => camera.c === */ 
/* ===================================== */ 

char *camera_tos(camera cm);

void camera_show(FILE *f, camera cm);

/* =================================== */ 
/* === stage operations => stage.c === */ 
/* =================================== */ 

char *stage_tos(stage stg);

void stage_show(FILE *f, stage stg);

/* The function get_stage just needs to return a valid stage. */
/* The argument may be used, or not, at your discretion. */
stage get_stage(unsigned int selection);

/* ================================================ */ 
/* ================================================ */ 
/* === RAY TRACER LOGIC and MAIN => raytracer.c === */ 
/* ================================================ */ 
/* ================================================ */ 

/* === project 1 operations === */

/* === intersect.c === */

hit_test intersect(ray r, object obj);

/* === raytracer.c === */

location logical_loc(camera c, unsigned int x, unsigned int y);

color trace_ray(scene s, ray r);

void render(FILE *f, stage g);

/* === rt.c === */

int main(int argc, char *argv[]);

#endif /* __RAYTRACER_H__ */

