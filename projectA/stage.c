#include "raytracer.h"

/* =================================== */ 
/* === stage operations => stage.c === */ 
/* =================================== */ 

const char *_stage_fmt = "*** begin stage\n\n%s\n\n%s\n*** end stage\n";

char *stage_tos(stage stg)
{
  char buf[1024];
  memset(buf,0,1024);
  char *c = camera_tos(stg.c);
  char *s = scene_tos(stg.s);
  snprintf(buf,1024,_stage_fmt,c,s);
  free(c);
  free(s);
  return strdup(buf);
}

void stage_show(FILE *f, stage stg)
{
  char *s = stage_tos(stg);
  fprintf(f,"%s",s);
  free(s);
}

/* constant coloring functions for easy testing */

color sc_red(object *o, location loc)
{
  return color_expr(1,0,0);
}

color sc_green(object *o, location loc)
{
  return color_expr(0,1,0);
}

/* sc_blue : surface color function that returns blue, always */
color sc_blue(object *o, location loc)
{
  return color_expr(0,0,1);
}

/* sc_maroon : surface color function that returns maroon, always */
color sc_maroon(object *o, location loc)
{
  return color_expr(0.5,0,0);
}

color sc_purple(object *o, location loc)
{
  return color_expr(0.5,0,0.5);
}

color sc_gray(object *o, location loc)
{
  return color_expr(0.5,0.5,0.5);
}

/* coloring functions specified on web page */

/* _dist : compute Euclidean distance in 3-space */
double _dist(xyz a, xyz b)
{
  double dx = a.x-b.x;
  double dy = a.y-b.y;
  double dz = a.z-b.z;
  return sqrt(dx*dx+dy*dy+dz*dz);
}

/* poster_red_distance - calculate distance from upper-left-corner, use to 
 * scale from black at upper-left to red at lower-right
 */
color poster_red_distance(object *o, location loc)
{
  if (o->tag!=POSTER) {
    fprintf(stderr,"error (poster_red_distance): not a poster\n");
    exit(1);
  }
  location ulc = o->o.p.upper_left;
  /* compute lower right corner */
  location lrc = {ulc.x+o->o.p.w,ulc.y+o->o.p.h,ulc.z}; 
  /* take the distance from between loc and ulc, and divide by total distance
   * from the ulc to rlc. place that result as the red in a color.
   */
  return color_expr(_dist(ulc,loc)/_dist(ulc,lrc),0,0);
}

/* poster_rgb_distance - calculate distance from upper-left-corner, use to 
 * scale from all red at upper-left to all blue at lower-left, with 
 * green in the middle, and gradations between them.
 */
color poster_rgb_distance(object *o, location loc)
{
  if (o->tag!=POSTER) {
    fprintf(stderr,"error (poster_rgb_distance): not a poster\n");
    exit(1);
  }
  location ulc = o->o.p.upper_left;
  /* compute lower right corner */
  location lrc = {ulc.x+o->o.p.w,ulc.y+o->o.p.h,ulc.z}; 
  /* relative ratio within poster */
  double ratio = _dist(ulc,loc)/_dist(ulc,lrc);
  /* for the first half, we blend from red to green */
  if (ratio<=0.5)
    return color_expr(1-ratio*2,ratio*2,0);
  /* for the first half, we blend from green to blue */
  else 
    return color_expr(0,2*(1-ratio),2*ratio-1);
}


/* sphere_loc_to_rgb - scale the location so that it maps x, y, z onto r, g, b in 
 * a way that shows gradations in the sphere. You are being given flexibility to 
 * do this in a way that looks interesting to you.
 */
/* Note: Yours does NOT need to be exactly like this - this is one solution */
color sphere_loc_to_rgb(object *o, location loc)
{
  // separately find relative x, y, z (relative to sphere),                                           
  // then map onto 0->1 or r, g, b                                                                    
  sphere *s = &(o->o.s);

  // 0 to 1 - where are x and y within poster?                                                        
  double x_within_sphere = (s->center.x+s->radius - loc.x)/
    (s->radius*2);
  double y_within_sphere = (s->center.y+s->radius - loc.y)/
    (s->radius*2);
  double z_within_sphere = (s->center.z+s->radius - loc.z)/
    (s->radius*2);

  return color_expr(x_within_sphere, y_within_sphere, z_within_sphere);

}

/* sphere_stripes - color the sphere with 4 blue stripes on a white background.
 * the pattern is: WBWBWBWBW. The stripes and background are roughly equal height.
 */
color sphere_horiz_stripes(object *o, location loc)
{
  if (o->tag!=SPHERE) {
    fprintf(stderr,"error (sphere_horiz_stripes): not a sphere\n");
    exit(1);
  }    
  location ctr = o->o.s.center;
  double rad = o->o.s.radius;
  double dy = loc.y-ctr.y;
  double s = (dy+rad)/(2*rad); /* s is on [0,1] */
  int n = (int)floor(s*9);     /* n is on [0,9] */
  if (n==9) n--;               /* n is on [0,8] */
  return n%2?color_expr(0,0,1):color_expr(1,1,1);
}


/* object constructors */

object *sphere_obj(location v, double r, color(*sc)(object*,location), color sh)
{
  object *new_obj = (object*)malloc(sizeof(object));
  if (new_obj==NULL) {
    fprintf(stderr,"error (sphere_obj): malloc failed\n");
    exit(1);
  }
  new_obj->tag = SPHERE;
  new_obj->o.s.center = v;
  new_obj->o.s.radius = r;
  new_obj->o.s.surface_color = sc;
  new_obj->o.s.shine = sh;
  new_obj->o.s.o = NULL;
  new_obj->t = NULL;
  return new_obj;
}

object *poster_obj(location ulc, 
		   double w,
		   double h, 
		   color(*sc)(object*,location), 
		   color sh)
{
  object *new_obj = (object*)malloc(sizeof(object));
  if (new_obj==NULL) {
    fprintf(stderr,"error (poster_obj): malloc failed\n");
    exit(1);
  }
  new_obj->tag = POSTER;
  new_obj->o.p.upper_left = ulc;
  new_obj->o.p.w = w;
  new_obj->o.p.h = h;
  new_obj->o.p.surface_color = sc;
  new_obj->o.p.shine = sh;
  new_obj->o.s.o = NULL;
  new_obj->t = NULL;
  return new_obj;
}

/* The function get_stage just needs to return a valid stage. */
/* The argument may be used, or not, at your discretion. */
stage get_stage(unsigned int selection)
{
  if (selection==1) {
    /* gallery image 1 */
    camera cam = {{0,0,-5},120,120};
    location ulc = {-1.5,1.5,4.5};
    object *pst = poster_obj(ulc,3,3,&poster_red_distance,color_expr(0,0,0));
    scene scn = {{0,0,0.25},ol_singleton(*pst)};
    /* valgrind caught this -- it's because the poster is copied into
     * the object list, as opposed to its pointer being copied
     */
    free(pst);
    stage stg = {cam,scn};
    return stg;
  }
  if (selection==2) {
    /* gallery image 2 */
    camera cam = {{0,0,-5},120,120};
    location ulc = {-1.5,1.5,4.5};
    object *pst = poster_obj(ulc,3,3,&poster_rgb_distance,color_expr(0,0,0));
    scene scn = {{0,0,0.25},ol_singleton(*pst)};
    free(pst);
    stage stg = {cam,scn};
    return stg;
  }
  if (selection==3) {
    /* gallery image 3 */
    camera c = {{0,0,-5},120,120};
    object *sph = sphere_obj(xyz_expr(1,0,4),1,&sphere_loc_to_rgb,color_expr(0,0,0));
    scene s = {{0,0,0.25},NULL};
    s.objects = ol_singleton(*sph);
    free(sph);
    stage g = {c,s};
    return g;
  }
  if (selection==4) {
    /* gallery image 4 */
    camera c = {{0,0,-5},120,120};
    object *sph0 = sphere_obj(xyz_expr(0,0,3),1,&sphere_loc_to_rgb,color_expr(0,0,0));
    object *sph1 = sphere_obj(xyz_expr(-0.5,0.7,3),0.5,&sc_purple,color_expr(0,0,0));
    scene s = {{0,0,0.25},NULL};
    s.objects = ol_cons(*sph0,ol_singleton(*sph1));
    free(sph0);
    free(sph1);
    stage g = {c,s};
    return g;
  } 
  if (selection==100) {
    /* striped sphere */
    camera c = {{0,0,-5},120,120};
    object *sph = sphere_obj(xyz_expr(0,0,4),1,&sphere_horiz_stripes,color_expr(0,0,0));
    scene s = {{0,0,0.25},NULL};
    s.objects = ol_singleton(*sph);
    free(sph);
    stage g = {c,s};
    return g;
  } 
  if (selection==101) {
    /* let's get a better look at the striped sphere... */
    camera c = {{0,0,-5},500,800};
    object *sph = sphere_obj(xyz_expr(0,0,4),1,&sphere_horiz_stripes,color_expr(0,0,0));
    scene s = {{0,0,0.25},NULL};
    s.objects = ol_singleton(*sph);
    free(sph);
    stage g = {c,s};
    return g;
  } 
  if (selection==103) {
    /* based on gallery image 3 */
    camera c = {{0,0,-5},120,120};
    object *sph = sphere_obj(xyz_expr(1,0,4),1,&sc_red,color_expr(0,0,0));
    scene s = {{0,0,0.25},NULL};
    s.objects = ol_singleton(*sph);
    free(sph);
    stage g = {c,s};
    return g;
  } 
  if (selection==104) {
    /* based on gallery image 4 */
    camera c = {{0,0,-5},120,120};
    object *sph0 = sphere_obj(xyz_expr(0,0,3),1,&sc_blue,color_expr(0,0,0));
    object *sph1 = sphere_obj(xyz_expr(-0.5,0.7,3),0.5,&sc_red,color_expr(0,0,0));
    scene s = {{0,0,0.25},NULL};
    s.objects = ol_cons(*sph0,ol_singleton(*sph1));    
    free(sph0);
    free(sph1);
    stage g = {c,s};
    return g;
  }

  /* some simple examples for students */
  if (selection==15200) {
    camera c = {{0,0,-5},300,300};
    object *sph = sphere_obj(xyz_expr(0,0,3),1,&sc_blue,color_expr(0,0,0));
    scene s = {{0,0,0},NULL};
    s.objects = ol_singleton(*sph);
    free(sph);
    stage g = {c,s};
    return g;
  } 
  if (selection==15201) {
    camera c = {{0,0,-5},600,300};
    object *sph = sphere_obj(xyz_expr(0,0,3),1,&sc_blue,color_expr(0,0,0));
    scene s = {{0,0,0},NULL};
    s.objects = ol_singleton(*sph);
    free(sph);
    stage g = {c,s};
    return g;
  } 
  if (selection==15202) {
    camera c = {{0,0,-5},300,600};
    object *sph = sphere_obj(xyz_expr(0,0,3),1,&sc_blue,color_expr(0,0,0));
    scene s = {{0,0,0},NULL};
    s.objects = ol_singleton(*sph);
    free(sph);
    stage g = {c,s};
    return g;
  } 
  if (selection==15203) {
    camera c = {{0,0,-5},300,300};
    object *pst = poster_obj(xyz_expr(-1,1,3),1,1,&sc_blue,color_expr(0,0,0));
    scene s = {{0,0,0},NULL};
    s.objects = ol_singleton(*pst);
    free(pst);
    stage g = {c,s};
    return g;
  } 
  if (selection==15204) {
    camera c = {{0,0,-5},300,300};
    object *pst = poster_obj(xyz_expr(-1,1,3),2,1,&sc_blue,color_expr(0,0,0));
    scene s = {{0,0,0},NULL};
    s.objects = ol_singleton(*pst);
    free(pst);
    stage g = {c,s};
    return g;
  } 
  if (selection==15205) {
    camera c = {{0,0,-5},300,300};
    object *pst = poster_obj(xyz_expr(-1,1,3),1,2,&sc_blue,color_expr(0,0,0));
    scene s = {{0,0,0},NULL};
    s.objects = ol_singleton(*pst);
    free(pst);
    stage g = {c,s};
    return g;
  } 
  if (selection==15206) {
    camera c = {{0,0,-5},200,400};
    object *sph0 = sphere_obj(xyz_expr(-1,0,3),0.3,&sc_blue,color_expr(0,0,0));
    object *sph1 = sphere_obj(xyz_expr(1,0,3),0.3,&sc_maroon,color_expr(0,0,0));    
    scene s = {{0,0,0},NULL};
    s.objects = ol_cons(*sph0,ol_singleton(*sph1));
    free(sph0);
    free(sph1);
    stage g = {c,s};
    return g;
  } 

  fprintf(stderr,"error (get_stage): undefined selection %u\n"
                 "(see get_stage in stage.c for available selections)\n",selection);
  exit(1);
}
