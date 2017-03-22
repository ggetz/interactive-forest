# CS 432 Final Project - Interactive Forest by Gabby Getz and Shreya Patel

## Introduction
Here you should provide an overview of your world/project.

## Users Guide
Here you should provide instructions to a user. This includes how to go from scene to scene (that is,
how the “player” can do this) as well as what you can do, and how you can do it, in each scene.
Include screenshots to help illustrate this.

## Technical Details
Here you will talk about how different parts are realized. Things like “how do you do object
selection”, and “how do you do collision detection” should be discussed. Also draw attention to cool
things you did and/or things that aren’t working properly and why you think that may be. You should
also provide a visualization for your scene graph(s).

Shadows were rendered in the scene using shadow mapping as explored in the slides, though using a modified projection when rendering the shadow pass using the directional light. In addition, to fight the shadow map texture aliasing, we used Poisson Sampling to take multple samples from the shadow map. Our shader also allows for a shadow color to passed in instead of just black for aestetic effect.

## Work Allocation
If you chose to work with a partner, you should make clear who is responsible for what in the project.
Based on this you may not end up with the same grade.

## Citations
Procedural tree mesh creation based on [supereggbert/proctree.js](https://github.com/supereggbert/proctree.js)
