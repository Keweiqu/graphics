## Homework 7
## Kewei Qu and Rachel Xu

## Running the Program
`./tracer testfiles/[some_test_file]` under default mode
`./tracer testfiles/[some_test_file] integer1 integer2` under debug mode

## Test Files and Rendered Images
the test files are in the "testfiles" folder and the rendered images are in the "results" folder
test1.in -> test1.ppm
test2.in -> test2.ppm
test3.in -> test3.ppm
test4.in -> test4.ppm
test5.in -> test5.ppm
test6.in -> test6.ppm
test10.in -> test10.ppm (scale and translate)
test11.in -> test11.ppm (mesh)
test12.in -> test12.ppm (scale and translate)
test13.in -> test13.ppm (image texture)
(testfiles below are for performance measurement)
demo2.in -> demo2.ppm
demo3.in -> demo3.ppm
demo4.in -> demo4.ppm
demo5.in -> demo5.ppm

The most complex scene we have rendered so far is demo2.in, which consists of 40 spheres
and 2 polyhedra. It took 63 minutes and 42 seconds to render (with anti-aliasing).

## Basic Features
- Spheres and planes
- Solid colored objects
- Ambient, diffuse and specular shading

## Intermediate Features
- Convex polyhedra
- Anti-aliasing (super-sampling 9 rays a pixel)
- Refraction
- Checkerboard textured objects

## Extra Credit
- Transformations:
  - Sphere: scale and translate
  - Plane: translate
- Image textured objects
- If the position of the eye and light are both inside of a sphere/polyhedron/plane,
  we still light the pixels by reversing the normal of intersection points.
- Triangle meshes
  - We attempted at implementing triangle meshes, but we failed to render the meshes
    correctly. We printed out some debugging information and found that the coordinates
    of the intersection points were not correct, but we weren't sure where and what
    kind of mistakes were made in the code. Could you please take a look at our code
    for triangle meshes? We are really curious why it didn't work and would appreciate
    any feedback. (for example, test11 is test12 plus an incorrectly rendered tetrahedron)

## Debug Mode
To turn on debug mode, change `#define DEBUG 0` in `common.hpp` to `#define DEBUG 1`
