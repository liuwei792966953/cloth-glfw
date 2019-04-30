# ClothSimulation
First attempt of cloth simulation using OpenGL 3.3, GLFW. Implemented physics simulation based on the euler steps method and the damped mass spring model.

Got help from here: https://graphics.stanford.edu/~mdfisher/cloth.html

---update May 1st, 2019---
When using explicit euler steps, you have to add a LOT of damping (air resistance, damped spring, etc). You have to lower the total energy per step via these damping factors. Otherwise it will explode no matter how small the time-step is.

---To Add---
Collision detection with a sphere.
