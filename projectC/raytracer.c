#include "raytracer.h"

/* === raytracer.c === */

/* hit test utilities */

const hit_test _miss = {MISS,0,{0,0,0},{0,0,0},{0,0,0},{0,0,0}};

int hit(hit_test t)
{
  return t.miss==HIT;
}

int miss(hit_test t)
{
  return t.miss==MISS;
}

/* coordinate system transformation */

location logical_loc(camera c, unsigned int x, unsigned int y)
{
  double pixw, pixh;
  double lx, ly;
  if (c.h>c.w)
    pixh = pixw = 2.0 / c.h;
  else
    pixh = pixw = 2.0 / c.w;
  if (x>c.w || y>c.h) {
    fprintf(stderr, "error (logical_loc): out of bounds (%d, %d)\n", x, y);
    exit(1);
  }
  lx = -1.0 + (pixw * x) + (pixw / 2.0);
  ly =  1.0 - ((pixh * y) + (pixh / 2.0));
  if (c.w>c.h)
    ly -= (c.w-c.h) / 2.0 * pixh;
  else
    lx += (c.h-c.w) / 2.0 * pixw;
  location result = {lx, ly, 0};
  return result;
}

/* ray tracing */

color color_at(object *obj, location loc)
{
	color(*sc)(object*,location) =
		obj->tag == SPHERE ?
		obj->o.s.surface_color:
		obj->tag == CUBE ?
		obj->o.c.surface_color:
    	obj->o.p.surface_color;
  	return (*sc)(obj,loc);
}

color trace_ray(scene s, ray r)
{
  obj_list *objs = s.objects;
  hit_test closest_hit = _miss;
  object *closest_obj = NULL;
  /* walk over object list, keep track of closest object if there is one */
  while (objs) {
    /* for each object, check for intersect */
    hit_test curr_hit = intersect(r,objs->obj);
    /* if it hits AND it's the first hit OR it's closer than closest hit */
    if (hit(curr_hit) && (miss(closest_hit) || (curr_hit.t<closest_hit.t))) {
      // update this to be the closest hit
      closest_hit = curr_hit;
	  closest_hit.surf = color_at(&(objs->obj), curr_hit.hit_point);
      closest_obj = &(objs->obj);
    }
    // advance to next object
    objs = objs->next;
  }
  /* if there is a closest object, return its color, otherwise s.bg */
  //return closest_obj?color_at(closest_obj,closest_hit.hit_point):s.bg;
  return lighting(s,r,closest_hit);
}

/* main loop and ppm creation */

void ppm_header(FILE *f, int height, int width)
{
  fprintf(f,"P3\n");
  fprintf(f,"%d %d\n", width, height);
  fprintf(f,"255\n");
}

void render(FILE *f, stage g)
{
  int row, col;
  camera c = g.c;
  scene sc = g.s;
  ppm_header(f,c.h,c.w);
  /* loop over all locations in the pixel field */
  for(row=0; row<c.h; row++) {
    for(col=0; col<c.w; col++) {
      /* 1: Translate from physical to logical */
      location loc = logical_loc(c,col,row);
      /* 2: Create a directional vector from camera to pixel */
      direction dir = xyz_sub(loc,c.loc);
      /* 3: Normalize this vector */
      direction normdir = xyz_norm(dir);
      /* 4: Create a ray starting at the camera in the direction of normdir */
      ray r = {c.loc,normdir};
      /* 5: trace the ray through the scene and obtain the color */
      color col = trace_ray(sc,r);
      /* 6: print that pixel to the screen (which can be redirected to a file) */
      color_show_bytes(f,col);
    }
  }
}
	

int shadow(location p, light l, obj_list *objs)
{
	ray nray;
	hit_test test;
	direction nudge = xyz_scale(0.0001, l.dir);
	location lifted = xyz_add(p, nudge);
	nray.origin = lifted;
	nray.dir = l.dir;
	
	obj_list *tmp = (obj_list *)malloc(sizeof(obj_list));
	for(tmp = objs; tmp != NULL; tmp = tmp->next){
		test = intersect(nray, tmp->obj);
		if(test.miss == HIT){
			return 1;
		}
	}
	return 0;
}


color check_rgb(color c)
{
	c.r = (c.r > 1.0) ? 1.0 : c.r;
	c.g = (c.g > 1.0) ? 1.0 : c.g;
	c.b = (c.b > 1.0) ? 1.0 : c.b;
	return c;
}


color shine(scene s, ray r, hit_test h)
{
	direction n = h.surf_norm;
	double f;
	direction l = xyz_scale(1/xyz_mag(s.light.dir), s.light.dir);
	color sh = h.shine;
	direction rr = xyz_sub(xyz_scale(2 * xyz_dot(n, l), n), l);
	direction v = xyz_neg(r.dir);
	if (xyz_dot(n, l) > 0){
		if (xyz_dot(rr, v) > 0){
			f = xyz_dot(rr,v)*xyz_dot(rr,v)*xyz_dot(rr,v)*xyz_dot(rr,v)*xyz_dot(rr,v)*xyz_dot(rr,v);
			return check_rgb(color_scale(f, sh));
		} else {
			return color_expr(0.0, 0.0, 0.0);
		}
	}
 	else
		return color_expr(0,0,0);
		
}

color lighting(scene s, ray r, hit_test h)
{
	double max;
	color dcolor, f;
	if(h.miss == MISS) 
		return s.bg;
	else {
		int n = shadow(h.hit_point, s.light, s.objects);
		//printf("H SURF: %s", color_tos(h.surf));
		if(n == 1){
			return color_modulate(h.surf, s.amb);
		} else { 
			//printf("LIGHT\n");
			max = xyz_dot(h.surf_norm, s.light.dir);
			max = (max > 0.0) ? max : 0.0;
			dcolor = color_scale(max ,s.light.c);
			f = color_modulate(h.surf, color_add(dcolor, s.amb));
			return color_add(shine(s,r,h), f);
		}
	}		
}	
