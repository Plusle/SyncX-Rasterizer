# SynchronixZ's Software Renderer 

## Introduction
This is a software renderer which is designed just for studying of rasterizaion pipeline. 
There are several software renderer I can find on github, but I still decided to develop a new one, because:
1. I do like make wheel by myself. It makes everything clarified to me, and I can notice many details that I can't realize when I was reading the textbook.
2. I have seen 2 or 3 projects of softrenderer, 

## The Whole Process of the Rasterization

1. Create instances of Loader and Scene, load model information.
2. Create an instance of Renderer, bind the instance of Scene to it.
3. Create an instance of IODevice, which is either ImagePrinter or Terminal, bind the instance of Renderer to it.
4. For each frame, create a temporary Transform, passing to Renderer through IODevice.

