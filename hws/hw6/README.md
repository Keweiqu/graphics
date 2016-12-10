## Homework 6
## Kewei Qu and Rachel Xu

## World Layout
The environment of the world is an ocean surrounded by ice mountains.(Four sides and ceiling are textured with environment) Objects on the ocean spread in a triangle fashion with two islands and a sphere marking the three vertices of the triangle. On the stand alone sphere there stands a bear. The two island meshes are textured using plain image texture as icebergs. On each island, there supports/hovers an additional sphere with a statue stands on it. The peak island supports Athena statue(the one holds shield and armored), the valley island hovers Nike statue(the one that is headless and with wings). All spheres and statue meshes are environmental mapped. 
## Islands
Initially we were having trouble clamping the island edges to the ground, there for we tried two ways to solve it. Since our world is of side length 40,000, the islands automatically increase the number of triangles if your distance to the island is less than 20,000.

### Peak algorithm
We modified the perturbation so that it is based on the dist between the current location and the center, this results in a peak look island.
### Valley algorithm
Clamp the edges to the ground level, which results in a valley look island. 

## View mode
We have implemented 4 types of viewing:
- side view(S)
- trailing view(T)
- First person view(F)
- *Free view*(E)
The first three are implemented as descriped in the hw6 requirements. We decided to get rid of the center view in favor of the Free view. Free view is such that the look is centered at (0, 0) while the eye position rotates around the entire plane. This view offers to view the entire layout of our world clearly. 

## Pre-programed flight sequence
Default is brownian motion we have done for previous homework. Route 1 flies to and circles around Athena statue. Route 2 flies to and circles around Nike statue. Route 3 flies to and circles around bear. Press 0 to resume brownian motion. 
Since these flight circles around the meshes. We recommend using side view / first person to gain a better look at the island, sphere and object on the sphere. Trailing does not work well as you are always tracing the back of the flock. 

## Extra credit
- Fog 
Since the environment is mostly a frozen world, we feel it is more realistic to have light fog, therefore, it can be best seen using the Free view. 
- ocean wave?
To simulate the ocean wave, we interpolated two textures cropped from a single image of the surface of ocean water so that the ocean changes with time.
- attempts for realism and aesthetic effect? :D


## Keys
Default view is side view
- 'S'/'s' for side view
- 'F'/'f' for first person view
- 'T'/'t' for trailing view
- 'E'/'e' for free view
- 'p' for pause
- 'I'/'i' for zooming in
- 'O'/'o' for zooming out
- 'N'/'n' to reset zoom level to default
- 1 to circle around Nike
- 2 to circle around Athena
- 3 to circle around Bear
- 0 for Brownian motion
- 'Q'/'q' to quit the program

In first person mode:
- up /down for move the look up and down
- left / right to move the look left and right

We know that turning the flock left and right is no longer required for this homework, however we tried to tune it and would like to get feedback on it. The following key only works for brownian motion. 
- 'A'/'a' to turn flock left
- 'D'/'d' to turn flock right
