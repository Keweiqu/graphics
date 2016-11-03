##Kewei Qu and Rachel Xu

##Design
  - 'P'/'p': toggling parallel and perspective projection
  - 'A'/'a': toggling animation/freeze
  - up/down arrows for zoom in/out
  - 'V'/'v': vertex mode
  - 'E'/'e': edge mode
  - 'F'/'f': face mode
  - 'S'/'s': smooth shading
  - 'F'/'f': flat shading
  - 'K'/'k': phong shading
  The grid is laid out on the x-y plane, with zooming on z-axis. 
  
##Execution instruction
  - make clean; make to compile
  - ./hw5 foo.off bar.off baz.off ... to run the program
  
##NOTE
  - the init time for a large mesh is pretty long, for extralarge/xyzdragon, the init time is 1 minute on a Linux machine. However, we have optimized such that reading the same file does not cost extra time, e.g. if you do ./hw5 xyzdragon xyzdragon it should cost the same amount of time to just init one xyzdragon.

##Extra Credits
  - One or multiple copies of the extra large meshes smoothly animated (frame rate
    only drops when switching to flat mode for xyzdragon)
  - left mouse click-and-drag rotation of the universe. Since it is rotation of the universe, meaning that it works best for 1 mesh, or meshes that filled most of the grid (currrent grid size is limited to perfect square number). Therefore the rotation works well for 1 mesh, 3 meshes, 4 meshes, 7, 8, 9 meshes. However, if you passed in 6 meshes, it will render using the 3 * 3 grid, so that the objects are squished to the left side of the screen, and therefore, the mouse drag rotation looks a little weird.

##What can be done better
  - Our current readFile function does not check for all syntacitcal possible malformed
    input, but we have written a more secure readFile function in our util.cpp file which checks for all syntacical errors (unfortunately, semantics no control at all). As we try to optimize for init time, we decided not to use the more secure version of readFile. The best way to do it is to #define #ifdef SAFE around these two readFile function so that the user can choose whether they want a safer readFile but more init time, or if they are very confident their files are correct, to use the less safer one. However, as one of our group memeber is traveling, we do not have enough time to polish these implementation details and end up not doing it. 
    
