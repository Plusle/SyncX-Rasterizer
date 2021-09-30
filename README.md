## The Whole Process of the Rasterization

1. Create instances of Loader and Scene, load model information.
2. Create an instance of Renderer, bind the instance of Scene to it.
3. Create an instance of IODevice, which is either ImagePrinter or Terminal, bind the instance of Renderer to it.
4. For each frame, create a temporary Transform, passing to Renderer through IODevice.

