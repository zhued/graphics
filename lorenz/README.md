# Edward Zhu's Lorenz Attractor

Many basic functions used throughout transfered from gears assignment.

## Basics of Lorenz Attractor
The calculations of the lorenz attractor is as follows:
- dx/dt = (Prandtl)(y-x)
- dy/dt = x((Rayleigh)-z)-y
- dz/dt = xy - (beta)

## How to build

1. Run `make`
2. Run `./lorenz`

## Key bindings:

- UP ARROW: Rotate X axis +
- DOWN ARROW: Rotate X axis -
- LEFT ARROW: Rotate Y axis +
- RIGHT ARROW: Rotate Y axis -
- r: Restart animation
- f: Finish animation
- t: Animation speed +
- g: Animation speed -
- e: Zoom Z axis +
- d: Zoom Z axis -