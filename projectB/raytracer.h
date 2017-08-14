#ifndef __RAYTRACER_H__
#define __RAYTRACER_H__

/* raytracer.h
 * CS152 Winter 2015
 * Project B
 * Feb 2015
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
  color shine;
  object *o; // optional - you decide whether to use it
} sphere;

typedef struct {
  xyz upper_left;
  double w;
  double h;
  color(*surface_color)(object *poster, location intersection);
  color shine;
  object *o; // optional - you decide whether to use it
} poster;

union object_union {
  sphere s;
  poster p;
};

typedef struct {
        color **pixels;
        int height;
        int width;
} texture;

struct _object{
  enum  { SPHERE, POSTER } tag;
  union object_union o;
  texture *t;
};

typedef struct obj_list obj_list;
/* convention: NULL is the empty object list */
struct obj_list {
  object obj;
  obj_list *next;
};

typedef struct {
  direction dir; /* note: the vector points AT the light */
  color c;
} light;

typedef struct {
  color bg;
  obj_list *objects;
  light light;
  color amb;
} scene;

typedef struct {
  enum {MISS=0,HIT=1} miss;
  double t;
  location hit_point;
  color surf; 
  color shine; // not used in project B
  direction surf_norm;
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

/* =============================== */ 
/* === color operations => color.c === */ 
/* =============================== */ 

/* reject if any component is outside [0.0,1.0] */
color color_expr(double r, double g, double b);

/* color_modulate : multiply the components or two colors */
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

/* ===================================== */ 
/* === slab operations => poster.c ===== */ 
/* ===================================== */ 

poster poster_expr(location ulc, double w, double h, 
		   color(*sc)(object*,location), color sh);

char *poster_tos(poster s);

void poster_show(FILE *f, poster s);

/* ===================================== */ 
/* === object operations => object.c === */ 
/* ===================================== */ 

char *object_tos(object o);

void object_show(FILE *f, object o);

/* ======================================== */ 
/* === object list ops => obj_list.c ====== */ 
/* ======================================== */ 

obj_list *ol_cons(object o, obj_list *os);

obj_list *ol_singleton(object s);

char *ol_tos(obj_list *os);

void ol_show(FILE *f, obj_list *os);

void ol_free(obj_list *ss);

/* =================================== */ 
/* === light operations => light.c === */ 
/* =================================== */ 

char *light_tos(light l);

void light_show(FILE *f, light l);

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

/* Functions from project A */
location logical_loc(camera c, unsigned int x, unsigned int y);
color trace_ray(scene s, ray r);
void render(FILE *f, stage g);

/* Functions from project B */

/* Given an intersection location p, light source, and object list,
 * check each object to see if it intersects the shadow ray. 
 * return nonzero if the location p is in shadow. Return 0 if not.
 */
int shadow(location p, light l, obj_list *objs);

/* Given a scene, ray, and hit_test, determine the color of the pixel.
 * The hit_test already calculated the surface color of the shape in the 
 * absence of lighting. Here, you take into account shadow and ambient
 * light to adjust the color. */
color lighting(scene s, ray r, hit_test h);

/* === rt.c === */

int main(int argc, char *argv[]);

/* === texture.c === */
/* Functions from project A that you already implemented */

/* poster_red_distance - calculate distance from upper-left-corner, use to 
 * scale from black at upper-left to red at lower-right
 */
color poster_red_distance(object *o, location loc); 
/* poster_rgb_distance: calculate distance from upper-left-corner, use to 
 * scale from all red at upper-left to all blue at lower-right, with 
 * green in the middle, and gradations between them.
 */
color poster_rgb_distance(object *o, location loc); 
/* sphere_loc_to_rgb: scale the location so that it maps x, y, z onto r, g, b 
 * in a way that shows gradations in the sphere. 
 */
color sphere_loc_to_rgb(object *o, location loc); 
/* sphere_stripes: color the sphere with 4 blue stripes on a white background.*/
color sphere_horiz_stripes(object *o, location loc); 

/* Functions in project B that you are supposed to implement  - see
 * assignment description for details on each function. */
color map_texture_on_poster(object *o, location intersection);
color map_texture_on_sphere(object *o, location intersection);
texture *make_horiz_stripes(color c1, color c2, int num_stripes);
texture *make_vert_stripes(color c1, color c2, int num_stripes);
texture *make_waves(int length, int x_offset, int y_offset, color col);

#endif /* __RAYTRACER_H__ */

