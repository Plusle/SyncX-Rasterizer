# SynchronizX's Software Rasterizer 

## Introduction
This is a software rasterizer which is designed just for studying of rasterizaion pipeline. 
There are several software rasterizer I can find on github, but I still decided to develop a new one, because:
1. I do like make wheel by myself. It makes everything clarified to me, and I can notice many details that I can't realize when I was reading the textbook.
2. I have seen 2 or 3 projects of softrasterizer, but none of them are designed to show the whole processes of traditional pipeline. 

## The goal of this project
To imitate the whole stages of rasterzation, especially what have been done behand the stage when we create an application by Graphics API like GL/Vulkan/DX,
besides the most ordinary stage like vertex processing, clipping, rasterization, fragment rendering and depth test, I also want to assemble raw data 
in more intuitive way, and do some pre-process like generating mipmap. 

## The whole stage of this application (temperoary)
1. Create instances of Loader and Scene, load model information.
    * Load both vertex information of model and texture.
2. Create an instance of Renderer, bind the instance of Scene to it.
3. Create an instance of IODevice, which is either Printer or Terminal, bind the instance of Renderer to it.
    * IODevice controls the behavior of renderer directly.
4. For each frame, a camera transform stored by IODevice will be passed to renderer to render the scene by current direction.

