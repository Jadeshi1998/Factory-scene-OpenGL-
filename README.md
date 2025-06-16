# Computer Graphics Factory  
**Name:** Yumeng Shi  
**CLICK THE REPORT TO CHECK **
(*https://github.com/Jadeshi1998/Factory-scene-OpenGL-/blob/main/COSC363-Assessment1.pdf*)
---

## Overview  

This project is a 3D animated factory scene built using **OpenGL with C++**, showcasing dynamic animations, lighting effects, and user interactions. The scene includes:

- A moving conveyor belt with bricks and material spheres  
- A hammer and sprinkler that process the material  
- A sorting box and output teapots  
- Chimneys with smoke using a particle system  
- A dynamic spotlight and skybox environment  

---

## Features  

- **Dynamic Spotlight** rotating around the conveyor belt  
- **Planar Shadows** cast by major objects (chimneys, red gate)  
- **Particle System** simulating smoke from chimneys  
- **Non-linear Conveyor Belt** using custom sweep surface  
- **Keyboard & Camera Controls** for interaction  

---

## Controls  

- `W` / `S` – Move camera forward / backward  
- Arrow keys – Rotate view or move camera up/down  
- `Q` – Toggle polygon mode (wireframe/solid)  

---

## Build & Run Instructions (CSSE Lab Machines)  

1. Open terminal and navigate to your project folder  
2. Run the following commands:

```bash
cmake .
make
./Factory.out
