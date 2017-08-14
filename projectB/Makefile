# Makefile (starter)
# CS152 Winter 2016
# Project B

.PHONY: clean

SRC = xyz.c color.c ray.c sphere.c poster.c object.c obj_list.c scene.c light.c camera.c hit_test.c stage.c intersect.c raytracer.c rt.c texture.c

trace: raytracer.h $(SRC)
	clang -g -O3 -Wall -o rt $(SRC) -lm

clean:
	rm -rf rt *.dSYM *.ppm
