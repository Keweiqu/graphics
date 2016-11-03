##Kewei Qu and Rachel Xu

##Features
  - 'P'/'p': toggling parallel and perspective projection
  - 'A'/'a': toggling animation/freeze
  - up/down arrows for zoom in/out
  - 'V'/'v': vertex mode
  - 'E'/'e': edge mode
  - 'F'/'f': face mode
  - 'S'/'s': smooth shading
  - 'F'/'f': flat shading
  - 'K'/'k': phong shading
  - The grid is laid out on the x-y plane, with zooming on z-axis. 

##Data Struture
Assuming there is no flat mode, the data structure for rendering the meshes is simple. For multiple meshes, we only keep one VAO that contains two VBOs(vertex vbo, normal vbo). To render each mesh with correct offset, we maintain a metadata structure for each mesh that record the number of indices to render and the correct offset in the vbo. In the draw call, we passed in the modelview for each mesh that gives the correct layout.
For flat, we keep another vao with two vbos(vertex vbo and normal vbo).

##Execution instruction
  - make clean; make to compile
  - ./hw5 foo.off bar.off baz.off ... to run the program
  
##NOTE
  - the init time for a large mesh is pretty long, for extralarge/xyzdragon, the init time is 1 minute on a Linux machine. However, we have optimized such that reading the same file does not cost extra time, e.g. if you do ./hw5 xyzdragon xyzdragon it should cost the same amount of time to just init one xyzdragon.

##Extra Credits
  - One or multiple copies of the extra large meshes smoothly animated (frame rate
    only drops when switching to flat mode for xyzdragon). Rate also drops when rendering more than 3 xyzdragons. For other extralarge, the program works fine. 
  - left mouse click-and-drag rotation of the universe. Since it is rotation of the universe, meaning that it works best for 1 mesh, or meshes that filled most of the grid (currrent grid size is limited to perfect square number). Therefore the rotation works well for 1 mesh, 3 meshes, 4 meshes, 7, 8, 9 meshes. However, if you passed in 6 meshes, it will render using the 3 * 3 grid, so that the objects are squished to the left side of the screen, and therefore, the mouse drag rotation looks a little weird.
  - [question] the mouse drag rotate does not quite work for vertical drag + horizontal drag. We realized that the vertical rotate should always happen around the "absolute" (1.0, 0.0, 0.0) from the viewer's point of view. Our algorithm is such that after left and right rotation, we find the axis that is now at the position of (1.0, 0.0, 0.0). If the rotation is alpha horizontally, then the new axis should be (cos(alpha), 0.0, sin(alpha)). However it doesn't work well for drag rotate larger than 180 degree. You can comment out the y part for universe_rotate matrix in main.cpp the 3rd line in while(!window shouldClose), and verify that horizontal drag rotate works fine on its own. We leave the y part in it so that you can see what's wrong and give us suggestions on it. It's a little more correct when we do the axis as (cos(alpha), 0.0, sin(-alpha)), that's why we have a negative sign there.

##What can be done better
  - Our current readFile function does not check for all syntacitcal possible malformed
    input, but we have written a more secure readFile function in our util.cpp file which checks for all syntacical errors (unfortunately, semantics no control at all). As we try to optimize for init time, we decided not to use the more secure version of readFile. The best way to do it is to #define #ifdef SAFE around these two readFile function so that the user can choose whether they want a safer readFile but more init time, or if they are very confident their files are correct, to use the less safer one. However, as one of our group memeber is traveling, we do not have enough time to polish these implementation details and end up not doing it. 
    
