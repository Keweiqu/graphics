##HW3 Boids##
###Kewei Qu, Rachel Xu###
- *NOTE*
  In this project we used a gsl library, which we manually installed in each of our own home directory. Also we only did the linux version of installation. Therefore, there is a chance that the program won't compile on your computer, if you cannot access our home dirs, which are /home/kqu and /home/rxu1 respectively or you are not running a linux os. We are happy to schedule a demo session and run valgrind under our accounts, using lab machines.
  
- Design
  - Press left and right arrow to change the x-axis of goal
  - Press up and down to move goal up and down
  - Press v and b to speed up and slow down goal, w/ ceiling speed of 100 units and floor speed of 20 units
  - Each boid has a speed limit of 30 units
  - Press C for center view, T for trailing view, and S for side view
  - Press P for pause and then use D to step, at each step the center of all boids, and the location of the goal is printed.
- Extra credits
  - flapping wings
  - shadows
  
- What can be done better
  - It's probably a mistake to use the gsl_vector lib, which is definitely a overkill for vectors, as it malloc'd all vector's regardless, and the syntax is really cumbersome. We spent sometime trying to fix all memo leaks created by these gsk_vectors.
  - Views also changes dramatically when the goal hit the bound/ or created a bird from really far away, which is not a good implementation, and requires some more parameters tuning. For the bird created far away, the work around is to find the centroid of the flock and add a bird there. For the goal hitting the bound, we can compute the cross prod of goal's speed and bound's up vector(0,0,1) and turn in that direction, instead of a sharp 180 turn.
