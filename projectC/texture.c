/* Varshant Dhar, varshant
 * CS152, Winter 2016
 * Project B - texture.c
 */

#include <stdlib.h>
#include <stdio.h>
#include "raytracer.h"
#include <math.h>
#include <unistd.h>

#define PI 3.14159265358979323846


color map_texture_on_sphere(object *o, location intersection)
{
	double alt = o->o.s.center.y - o->o.s.radius;
	double y = intersection.y - alt;
	double pixh = (y/(o->o.s.radius*2))*o->t->height;
	y = (o->o.s.center.y) - intersection.y;
	xyz added = xyz_add(o->o.s.center,xyz_expr(0,y,0));
	xyz polar = xyz_sub(intersection, added);
	double theta = atan(polar.x/polar.z);
	double pixw = ((theta+PI)/(2*PI))*o->t->width;
	int pixel_width = (int)(pixw);
	int pixel_height = (int)(pixh);
	return o->t->pixels[pixel_width][pixel_height];
	
}

color map_texture_on_poster(object *o, location intersection)
{
	double x = intersection.x - o->o.p.upper_left.x;
	double y = o->o.p.upper_left.y - intersection.y;
	double pixel_width = (x/o->o.p.w)*o->t->width;
	double pixel_height = (y/o->o.p.h)*o->t->height;
	int pixw = (int) (pixel_width);
	int pixh = (int) (pixel_height);
	return o->t->pixels[pixw][pixh];
	
}


int findside(cube c, location intersection)
{
		double length = c.length;
		xyz ftl = c.front_top_left;
		
		if(ftl.z - 0.1 <= intersection.z && ftl.z + 0.1 >= intersection.z) {
			return 1; //Face
		}
		if( ftl.x - 0.1 <= intersection.x && ftl.x + 0.1 >= intersection.x) {
			return 2; //Left
		}
		if(ftl.y - 0.1 <= intersection.y && ftl.y + 0.1 >= intersection.y) {
			return 3; //Top
		}
		if(ftl.x + length - 0.1 <= intersection.x && ftl.x + length + 0.1 >= intersection.x) {
			return 4; //Right
		}
		if(ftl.y + length - 0.1 <= intersection.y && ftl.y + length + 0.1 >= intersection.y) {
			return 5; //Bottom
		}
		fprintf(stderr,"error (cube_side): Intersection point is invalid.");
		exit(1);									
}

color map_texture_on_cube_faces(object* o, location intersection)
{
	double x, y, xpix, ypix;
	xyz ftl = o->o.c.front_top_left;
	double length = o->o.c.length;
	int side = findside(o->o.c, intersection);
	switch(side){
		case 1:
		{
			x = fabs(intersection.x - ftl.x);
			y = fabs(ftl.y - intersection.y);
			break;
			
		}
		case 2:
		{
			x = fabs(intersection.z - (ftl.z + length));
			y = fabs(ftl.y - intersection.y);
			break;
		}
		case 3:
		{
			x = fabs(intersection.x - ftl.x);
			y = fabs((ftl.z + length) - intersection.z);
			break;
		}
		case 4:
		{
			x = fabs(intersection.z - ftl.z);
			y = fabs(ftl.y - intersection.y);
			break;
		}
		case 5:
		{
			x = fabs(intersection.x - ftl.x);
			y = fabs(ftl.z - intersection.z);
			break;
		}
		default:
		{
			fprintf(stderr, "error (map_texture_on_cube_faces): Invalid side.");
			exit(1);
		}
	}
	xpix = (x/length)*(o->t->width);
	ypix = (y/length)*(o->t->height);
	return o->t->pixels[(int)xpix][(int)ypix];
}

color map_texture_around_cube(object* o, location intersection)
{
	xyz ftl = o->o.c.front_top_left;
	double x, y, xpix, ypix;
	double length = o->o.c.length;
	int w, h, side;
	int firstw = 0, firsth = 0;
	w = (o->t->width)/3;
	h = (o->t->height)/3;
	side = findside(o->o.c,intersection);
	
	switch (side){
		case 1:
		{
			firstw = w;
			x = fabs(intersection.x - ftl.x);
			y = fabs(ftl.y - intersection.y);
			break;
		}
		
		case 2:
		{
			firsth = h;
			x = fabs(intersection.z - (ftl.z + length));
			y = fabs(ftl.y - intersection.y);
			break;
		}
		
		case 3:
		{
			firstw = w;
			x = fabs(intersection.x - ftl.x);
			y = fabs((ftl.z + length) - intersection.z);
			break;
		}
		
		case 4:
		{
			firstw = 2*w;
			x = fabs(intersection.z - ftl.z);
			y = fabs(ftl.y - intersection.y);
			break;
		}
		
		case 5:
		{
			firstw = w;
			x = fabs(intersection.x - ftl.x);
			y = fabs(ftl.z - intersection.z);
			break;
		}
		default:
		{
			fprintf(stderr, "error (map_texture_around_cube): Invalid side.");
			exit(1);
		}
	}
	
	xpix = firstw + (x/length)*w;
	ypix = firsth + (y/length)*h;
	return o->t->pixels[(int)xpix][(int)ypix];
}


texture *make_horiz_stripes(color c1, color c2, int num_stripes)
{
	texture *bitmap = (texture*)malloc(sizeof(texture));
    if (bitmap==NULL) {
      fprintf(stderr,"error (make_horiz_stripes): malloc failed for bitmap\n");
      exit(1);
    }
	bitmap->pixels = (color **)malloc(sizeof(color *));
	bitmap->pixels[0] = (color *)malloc(sizeof(color) * num_stripes);
	
	
	for (int i = 0; i < num_stripes; i++){
			if (i%2) {
				bitmap->pixels[0][i] = c2;
			} else { 
				bitmap->pixels[0][i] = c1;
			}
	}
	
	bitmap->height = num_stripes;
	bitmap->width = 1;
	
	return bitmap;
}	
texture *make_vert_stripes(color c1, color c2, int num_stripes)
{
	texture *bitmap = (texture *)malloc(sizeof(texture));
   if (bitmap==NULL) {
      fprintf(stderr,"error (make_vert_stripes): malloc failed for bitmap\n");
      exit(1);
    }
	bitmap->pixels = (color **)malloc(sizeof(color *));
	
	
	for (int i = 0; i < num_stripes; i++){
		
	bitmap->pixels[i] = (color *)malloc(sizeof(color));
			if (i%2) {
				bitmap->pixels[i][0] = c2;
			} else {
				bitmap->pixels[i][0] = c1;
			}	
	}
	bitmap->height = 1;
	bitmap->width = num_stripes;
	
	return bitmap;
}	

double dist(double x0, double y0, double x1, double y1){

	double distance = sqrt((x1-x0)*(x1-x0)+(y1-y0)*(y1-y0));
	return distance;

}

texture *make_waves(int length, int x_offset, int y_offset, color col)
{
	texture* waves = (texture *)malloc(sizeof(texture));
		int x, y, distance;
		double b;
		double new_length = (double) (length + 0.5);
		int center = (int) (new_length/2);
		int center_x = center + x_offset;
		int center_y = center + y_offset;
		waves->pixels = (color **)malloc(sizeof(color*)*length);
		for(int i = 0; i<length; i++) {
			waves->pixels[i] = (color *)malloc(sizeof(color)*length);
		}
		for(int j = 0; j<length*length; j++) {
			x = j/length;
			y = j%length;
			distance = dist(center_x,center_y,x,y);
			b = sin(distance)+1;
			waves->pixels[x][y] = color_scale(b,col);
		}
		waves->width = length;
		waves->height = length;
		return waves;
	
	
}	


texture *make_bitmap_from_file(char *filename)
{
	double red, green, blue;
	int r, g, b;
	int h, w, scale;
	
	if (access(filename, F_OK) == -1) {
		fprintf(stderr, "error (make_bitmap_from_file) : file does not exist");
		exit(1);
	}
	
	FILE *f = fopen(filename, "r");
	
	texture *new = (texture *)malloc(sizeof(texture));
	fscanf(f, "%d %d\n", &h, &w);
	fscanf(f, "%d\n", &scale);
	new->height = h;
	new->width = w;
	new->pixels = (color **)malloc(sizeof(color *) * h);
	
	if (f == NULL){
		fprintf(stderr, "error (make_bitmap_from_file) : error opening file");
		exit(1);
	}
	
	for(int i = 0; i < h; i++){
		new->pixels[i] = (color *)malloc(sizeof(color) * w);
		for(int j = 0; j < w; j++){
			fscanf(f, "%d %d %d\n", &r, &g, &b);
			red = ((double)r)/scale;
			green = ((double)g)/scale;
			blue = ((double)b)/scale;
			new->pixels[i][j] = color_expr(red,green,blue);
		}
	}
	
	fclose(f);
	return new;
}
