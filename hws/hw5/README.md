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

##Extra Credits
  - One or multiple copies of the extra large meshes smoothly animated (frame rate
    only drops when switching to flat mode for xyzdragon)
  - left mouse click-and-drag rotation of the universe

##What can be done better
  - Our current readFile function might result in segmentation fault on Malformed
    input, but we have written a more secure readFile function (won't crash on bad
     input) in our util.cpp file. We are not using the more secure version because
     it does more checks and therefore costs more time. We had originally planned
     to prompt the user to choose between the two versions but we ran out of time.
