#include <math.h>
#include "raytracer.h"

/* === intersect.c === */

/* to fill in slots not used in project A */
color _c0 = {0,0,0};
xyz   _v0 = {0,0,0};

hit_test sph_int(ray r, sphere s)
{
  xyz A = xyz_sub(r.origin,s.center);
  double B = xyz_dot(A,r.dir);
  double C = xyz_dot(A,A)-s.radius*s.radius;
  double D = B*B-C;
  double t = (-B)-sqrt(D);
  if (D>0&&t>0) {
    hit_test ht = {HIT,t,ray_position(r,t),_c0,s.shine,_v0};
	xyz v = xyz_sub(ray_position(r,t), s.center);
	ht.surf_norm = xyz_norm(v);
    return ht;
  } else {
    // if it doesn't hit, then it's a miss, and nothing else in the struct matters
    hit_test ht = {MISS,0,_v0,_c0,_c0,_v0};
    return ht;
  }
}

/* pre: the location's z matches the poster's */
/* (this is an outcome of the math given)     */
int on_poster(poster *p, location loc)
{
  return (loc.x>=p->upper_left.x) &&
         (loc.x<=(p->upper_left.x+p->w)) &&
         (loc.y<=p->upper_left.y) &&
         (loc.y>=(p->upper_left.y-p->h)) &&
		 (loc.z>=p->upper_left.z) &&
		 (loc.z<=(p->upper_left.z+p->h));
}

hit_test pst_int(ray r, poster p, xyz n, double d)
{
  double t = -(xyz_dot(r.origin,n)+d)/xyz_dot(r.dir,n);
  location hp = ray_position(r,t);
  if (t>0&&on_poster(&p,hp)) {
    hit_test ht = {HIT, t, hp, {0,0,0}, p.shine, n};
    return ht;
  } 
  hit_test ht = {MISS,0,{0,0,0},{0,0,0},{0,0,0},{0,0,0}};
  return ht; 
}


hit_test cube_int(ray r, cube c)
{
	poster p;
	p.w = c.length;
	p.h = c.length;
	p.upper_left = c.front_top_left;
	location corner = c.front_top_left;
	double length = c.length;
	hit_test ht = pst_int(r,p,xyz_expr(0,0,-1),p.upper_left.z);
	//Face
	if(ht.miss == HIT)
		return ht;
	corner = xyz_expr(corner.x + length, corner.y, corner.z);
	p.upper_left = corner;
	//Right Side
	ht = pst_int(r,p,xyz_expr(1,0,0),-p.upper_left.x);
	if(ht.miss == HIT)
		return ht;
	p.upper_left = c.front_top_left;
	//Left Side
	ht = pst_int(r,p,xyz_expr(-1,0,0),p.upper_left.x);
	if(ht.miss == HIT)
		return ht;
	//Top
	ht = pst_int(r,p,xyz_expr(0,1,0),-p.upper_left.y);
	if(ht.miss == HIT)
		return ht;
	corner = xyz_expr(c.front_top_left.x ,c.front_top_left.y - length, c.front_top_left.z); 
	p.upper_left = corner;
	//Bottom
	ht = pst_int(r,p,xyz_expr(0,-1,0),p.upper_left.y);
	return ht;
}

hit_test intersect(ray r, object obj)
{
  switch (obj.tag) {
  case SPHERE:
    return sph_int(r,obj.o.s);
  case POSTER:
    return pst_int(r,obj.o.p, xyz_expr(0,0,-1), obj.o.p.upper_left.z);
  case CUBE:
	return cube_int(r,obj.o.c);	
  default:
    fprintf(stderr,"error (intersect): unknown object tag %d\n",obj.tag);
    exit(1);
  }
}
