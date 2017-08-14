/* Varshant Dhar, varshant
 * CS152, Winter 2016
 * Project A - rt.c
 */

#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include "raytracer.h"

color poster_red_distance(object *o, location loc){
	
	double max_dist, dist, ulc_x, ulc_y, squared_x, squared_y, w, h;
	ulc_x = o->o.p.upper_left.x;
	ulc_y = o->o.p.upper_left.y;
	squared_x = (loc.x - ulc_x)*(loc.x - ulc_x);
	squared_y = (loc.y - ulc_y)*(loc.y - ulc_y);
	dist = sqrt(squared_x + squared_y);
	w = o->o.p.w;
	h = o->o.p.h;
	max_dist = sqrt(w*w+h*h);
	color new_color;
	new_color.r = dist/max_dist;
	new_color.g = 0;
	new_color.b = 0;
	return new_color;
	
	
}

color poster_rgb_distance(object *o, location loc){
	
	double max_dist, dist, ulc_x, ulc_y, squared_x, squared_y, w, h;
	ulc_x = o->o.p.upper_left.x;
	ulc_y = o->o.p.upper_left.y;
	squared_x = (loc.x - ulc_x)*(loc.x - ulc_x);
	squared_y = (loc.y - ulc_y)*(loc.y - ulc_y);
	dist = sqrt(squared_x + squared_y);
	w = o->o.p.w;
	h = o->o.p.h;
	max_dist = sqrt(w*w+h*h);
	color new_color;
	new_color.r = ((dist) < (max_dist/2) ? ((-2.0 / max_dist) * dist + 1) : 0.0);
	new_color.g = ((dist) < (max_dist/2) ? ((2/max_dist) * dist) : ((-2.0/max_dist) * dist + 2.0));
	new_color.b = ((dist) < (max_dist/2) ? 0.0 : ((2.0/max_dist) * dist - 1));
	return new_color;
	
}

color sphere_loc_to_rgb(object *o, location loc){
	
		double rad, x, y, z;
		rad = o->o.s.radius;
		x = o->o.s.center.x;
		y = o->o.s.center.y;
		z = o->o.s.center.z;
		color new_color;
		new_color.r = (1/rad)*(loc.x + rad - x);
		new_color.g = (1/rad)*(loc.y + rad - y);
		new_color.b = (1/rad)*(loc.z + rad - z);
		return new_color;
				
}

color sphere_horiz_stripes(object *o, location loc){
	
		double rad, y;
		color new_color;
		rad = o->o.s.radius;
		y = o->o.s.center.y;
		if ((loc.y >= y + (1/9) * rad && loc.y <= y + (3/9) * rad) ||
			(loc.y >= y + (5/9) * rad && loc.y <= y + (7/9) * rad) ||
			(loc.y >= y - (5/9) * rad && loc.y <= y - (7/9) * rad) ||
			(loc.y >= y - (3/9) * rad && loc.y <= y - (1/9) * rad)) {
				new_color.r = 0.0;
				new_color.g = 0.0;
				new_color.b = 1.0;
				return new_color;
			} else {
				new_color.r = 1.0;
				new_color.g = 1.0;
				new_color.b = 1.0;
				return new_color;
			}	
}



int main(int argc, char *argv[]){
	
	render(stdout, get_stage(1));
	return 0;
}
