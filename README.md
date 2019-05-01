# ClothSimulation
First attempt of cloth simulation using OpenGL 3.3, GLFW. Implemented physics simulation based on the euler steps method and the damped mass spring model.

Got help from here: https://graphics.stanford.edu/~mdfisher/cloth.html

---update April 30th, 2019---
When using explicit euler steps, you have to add a LOT of damping (In my case, air resistance, damped spring). You have to lower the total energy per step via these damping factors. Otherwise it will explode no matter how small the time-step is.

https://www.youtube.com/watch?v=tA5-Y_i0c2A&feature=youtu.be

---To Add---
Collision detection with a sphere.

---update May 1st, 2019---
Added a sphere within the shirt to make it look like a breast.

https://www.youtube.com/watch?v=fn-xVN4-P2s&feature=youtu.be

---To Add---
Now apply the shirt to a BVH model -> Make it walk around with the shirt on.
