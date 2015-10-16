# Texture and Lighting - HW6
## Edward Zhu
## CSCI 4229

Demonstrates basic lighting and textures with houses and neighborhoods. 
Code skeleton taken from my HW5, which was inspired by example 13 from moodle

Time it took to complete: **6 hours**

## Idea

I created a neighborhood of houses located around a corner. There is also a light source that orbits around the scene, giving the different sides of object shine depending on where it is located. And I also added some textures.

## How to build

1. Run `make`
2. Run `./hw6`

## Main Key bindings:
 *  l          Toggles lighting
 *  m          Toggles light movement
 *  < or >     when light movement toggled off, it moves the light source
 *  []         Lower/rise light
 *  3          Toggle light distance (10/2)
 *  p          Toggles ortogonal/perspective projection
 *  + or -     Change field of view of perspective
 *  x          Toggle axes
 *  arrows     Change view angle
 *  PgDn/PgUp  Zoom in and out
 *  0          Reset view angle
 *  ESC        Exit

Other options:
 *  a/A        Decrease/increase ambient light
 *  d/D        Decrease/increase diffuse light
 *  s/S        Decrease/increase specular light
 *  e/E        Decrease/increase emitted light
 *  n/N        Decrease/increase shininess
 *  1          Toggle smooth/flat shading
 *  2          Toggle local viewer mode
 *  4          Change ball increment
 *  5          Invert bottom normal
