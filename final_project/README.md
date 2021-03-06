Star Wars Tie Fighter - Graphics 4229 Final Project
============

## Edward Zhu - Undergrad

####Time it took to finish: **60hours-ish**

## Idea

Created a detailed TIE Fighter from Star Wars flying around in space with rocks floating around it and making it able to shoot lasers from it's canons.

When lasers hit the floating rock, the rock will explode! BOOOOM!


Tested and working without errors/warnings on MAC OSX 10.11.1.

## How to build

1. Run `make`
2. Run `./final`

## Main Key bindings:
 *  SPACEBAR   - SHOOT LASERS!!
 *  + or -     - Change field of view of perspective
 *  arrows     - Change view angle
 *  d/D        - Zoom in and out
 *  0          - Reset view angle to (0,0)
 *  ESC        - Exit

## Notes/Review:
 *  Creating the TIE fighter and making sure all points were created perfectly was very, very time consuming.
 *  MOVEMENT in general took a LONG while; the rock movements took some time to code out the right procedure that wouldn't wreck the FPS. I tried 4 different ways that I could do movement over time, and finally found a way that wouldn't substantially drop the FPS. Also the lasers to trigger off a spacebar took me a while to finally get working and right.
 *  Getting the explosion to go off was also tricky to implement smoothly without too many glitches. Getting the code and visuals to happen at the right times and frames took some time to design. 
 * A light source comes from the middle of the explosion.


## Resources Used
- To create cylinders: https://www.opengl.org/discussion_boards/showthread.php/167115-Creating-cylinder and https://stackoverflow.com/questions/26536570/how-do-i-texture-a-cylinder-in-opengl-created-with-triangle-strip
- Wing Texture: http://jkhub.org/topic/1339-starkiller/page-17
- Main body texture: http://3dmodellinganimationlaurence.blogspot.com/2014/11/texturing-tie-fighter-model.html
- Some functionality from the CSCIx229.h library.

