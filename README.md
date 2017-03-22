# CS 432 Final Project - Interactive Forest by Gabby Getz and Shreya Patel

## Introduction
We created an interactive forest where you choose where the trees are generated. We create an atmospheric scene with a revolving sun, shadows, and fog. Trees are procedurally generated in the scene where you click on the ground plane.

## Users Guide
Move forwards and backwards in the scene withe the `w` and `s` keys respectively. `a` and `d` will turn you to the left or right. You can move up and down and strife with the arrow keys.

Click on the ground plane with the left mouse button to generate a tree at that location.

Switch between the moving camera and a static birds-eye view camera with spacebar.

## Technical Details
Object picking was accomplished by getting the screen position on click, projecting that into world coordinates on the near camera plane, and creating a vector from the camera eye to that position. Then, the intersection of the vector with the plane was determined to find the location on the plane.

Tree meshes were created procedurally and the generation is based on the webgl library [supereggbert/proctree.js](https://github.com/supereggbert/proctree.js). The port of this library whihc uses L-Systems creates the vertex, normal, and tetxure map locations for a tree mesh based on a random seed and specified tree parameters. We then assign a material to each tree and rendered the using index arrays.

Shadows were rendered in the scene using shadow mapping as explored in the slides, though using a modified projection when rendering the shadow pass using the directional light. In addition, to fight the shadow map texture aliasing, we used Poisson Sampling to take multple samples from the shadow map. Our shader also allows for a shadow color to passed in instead of just black for aestetic effect.

Fog was added to the scene by adding a linear fog function to the mesh shader. It takes the distance from the eye of the camera to the vertex location, and interpolates between the fog color and the fragment color by this amount.

The sun is a directional light that moves through the sky over time. Fog color will also chnage to reflect the time of day.

## Work Allocation
Gabby worked on the shadow implementation, the tree generation port, the skybox, and the fog shader, while Shreya worked on the cameras, the object picking, and the ground plane. Both contributed to constructing the scene.

## Citations
Procedural tree mesh creation based on [supereggbert/proctree.js](https://github.com/supereggbert/proctree.js). 
