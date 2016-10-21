##Kewei Qu and Rachel Xu

##Improvements based on last project feedback
- Based on feedback, we have made sure that no seg faults happens during adding and removing boids. Moreover, we configured our
views so that now we can remove all boids, instead of requiring a minimun of 5 boids.
- The turning view is smoother and less dissy, especially during turning back after hitting the boundary.
- We created boids near boid center to solve the issue of having one outlier dramatically distort the view.
- Turn goal left and right is now calculated based on cross product of goal's direction and up z-axis to make it looks more
natural for viewer. Previously it was implemented as changing the absolute coordinates.
- zNear of perspective is at least 1.

##Extra credit
- Shadow
- Flapping (Note: the wings still flaps when paused for a better look at the flapping feature -- you can see that the shadow indeed
flaps along with the wings, and different boids flaps with a different starting angle).

## Keys
Default view is side view
- 'S'/'s' for side view
- 'C'/'c' for center view
- 'T'/'t' for trailing view
- '+' for adding a boid
- Delete/backspace for removing a boid
- up /down for moving goal up and down
- left / right to turn the goal left and right
- ','/'<' for speed up
- '.'/'>' for slowing down
-'p' for pause
-'d' for stepping when paused, or pause when not paused.

## Data Structures and library
- vector implemeted in vec3.hpp
- Matrix implemented in mat4.hpp
- glTranslate, glRotate, glScale, gluLookat, glPerspective all implemented in gl_replace.cpp
- util.hpp contains all drawing and viewing related auciliary functions
