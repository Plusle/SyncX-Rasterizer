# SynchronizX's Software Rasterizer 

## Introduction

This is a software rasterizer which is designed just for studying of rasterization pipeline. 
There are several software rasterizer I can find on GitHub, but I still decided to develop a new one, because:

1. I do like make wheel by myself. It makes everything clarified to me, and I can notice many details that I can't realize when I was reading the textbook.
2. I have seen 2 or 3 projects of softrasterizer, but none of them are designed to show the whole processes of traditional pipeline. 



## The goal of this project

To imitate the whole stages of rasterization, especially what have been done behand the stage when we create an application by Graphics API like GL/Vulkan/DX,
besides the most ordinary stage like vertex processing, clipping, rasterization, fragment rendering and depth test, I also want to assemble raw data 
in more intuitive way, and do some pre-process like generating mipmap. 



## Procedure

As always, the application start from the `main` function. Conceptually, there are 5 stages in `main` function:

1. Initializing an instance of `Scene`
2. Initializing an instance of `Loader` for the instance of `Scene`
3. Listing a series of file/folder to load all render targets.
4. Initializing instances of specified type of `IODevice` and `Renderer`. 
5. Executing the `play(Scene*)` member function of `IODevice` to begin the rendering.



## Structure of `Scene`, the warehouse for rendering stage

`Scene` contains a series of buffer.

```cpp
class Scene {
public:
    vector<Model> m_Models;
private:
    vector<Vertex> m_Vertices;
    vector<Face> m_Faces;
    vector<Texture> m_Textures;
};
```

In the terminology of OpenGL, `m_Vertices`, `m_Faces`, `m_Textrues` are equivalent to VBO, EBO, Texture buffer. Since I store vertex data in struct `Vertex`, I don't need a VAO. We usually render several models by invoking draw call on each model. To do so, I also add an indices of `Model`, which stores the index(subscription in `m_Faces`) of faces. Model data loade

Normally, initializing an instance of `Scene` need doing nothing. `Scene` only exposes its `m_Models` because I think modification on buffer should only be accessible through the API. F

Note that `Scene` only provide models for its interface.  That is because in the stage of shader, vertices should be loaded with the form so-called 'stream'. In code using graphics API, the `Model` class usually has a member function called `render()` to invoke drawcall with particular configuration and shader program. Hence, in this application, vertices will be collected within struct `Model` in advance.





## `Loader`, the true beginning

`Loader` need to be binded with a specified `Scene`.

```cpp
class Loader {
public:
    Loader(Scene* sc) : m_Scene(sc) {}
    void Load(const char* path) const;
    
    Scene* m_Scene;
};
```

Inside the `Load` function, there are several steps to load model file to `Scene`.

First, there is a helper function to handle the path and get the list of filename.

```cpp
static vector<string> GetAllFilenames(const char* path); 

void Load(const char* path) const {
    // 1st step
	vector<string> filenames = GetAllFilenames(path);    
    
    // ... 
}
```

With this list, `Load` function will read them sequentially. Moreover, to make the parser simple, `Load` function will use the filename of .obj file to create a load list under a convention. For a file named `model.obj`, function will also try to find image file with filename `model_suffix.ext` at the same location. The suffix could be `diffuse`, `specular`, `roughness`, etc. Extension could be `jpg`, `png` or `tga`.

For each filename, there are several pipeline stages in the function. In the end, the pipeline appends a new `Model` to `Scene`.

For a rasterization pipeline, the `Model` looks like this:

```cpp
struct Model {
    int32_t m_FaceIdxBeg, m_FaceIdxEnd;
    int32_t m_Diffuse, m_Specular;
    // may contain other textures
};
```

Therefore, the stages within pipeline are:

1. Create a `Model` called model.
2. Call `LoadObjFileRoutine(filename, model)`. It will append vertices and indices to `Scene` and record the bound within the array.
3. Call `LoadTexture(filename, model)`. This function tries to find an appropriate extension for each suffix and load those images. Then record the index of `Texture` within the array of `Scene`.
4. Call `push_back(model)` of `std::vector` to append the new `Model`.

### Loading model data

Except `usemtl` and `g` in .obj file, there are `v` for vertex, `vt` for texture coordinate, `vn` for normal, `f` for face as well. Parser reads lines sequentially. Within each line, it checks the keyword, then creates an appropriate data (`Vector2f` or `Vector3f`)  of that line and appends it to corresponding array of `Scene`. Before and after the parsing, function call `m_Array.size()` of `Scene` to record index boundary of current model inside. As the result of this, a series of vertices/face/texture will be appended, and a corresponding  `Model` will also be created.

### Load textures for model

As said before, this parser won't parse `usemtl`, hence, material information will be loaded by suffix convention. 

Given a filename, the code of this function looks like

```cpp
void LoadTexture(const string& filename, Model model) {
	LoadTexture(filename, "_diffuse", model.m_Diffuse);
    LoadTexture(filename, "_specular", model.m_Specular);
    // Call more if other texture in Model exists
}

void LoadTexture(const string& filename, const string& suffix, int32_t& texture) {
    extensions = { ".jpg", ".png", ".tga" };
    texture = -1;
    for (ext : extensions) {
        if (Exist(filename + suffix + ext)) {
            texture = m_Scene->m_Texture.size();
            m_Scene->m_Textures.push_back(Texture);
            break;
        }
    }
}
```



## Structure of `Texture`

`Texture` is the first complicate structure in this application. Given a filename, the constructor would read the image file and reconstruct in byte form. The interface of `Texture` looks like this:

```cpp
class Texture {
public:
    Texture(const char* filename, );
    
    Vector3f GetColorBilinear(Vector2f uv) const;
    Vector3f GetColorTrilinear(Vector2f uv, Vector2f duv_dx, Vector2f duv_dy) const;
private:
    Vector3f GetColorBilinearHelper(Vector2f uv, int32_t level) const;
};
```

Bilinear interpolation only need the texture coordinate on that location (pixel), while trilinear interpolation need $duv/dx$ and $duv/dy$, additionally. Both of them calls the `GetColorBilinearHelper` to interpolate in 2D. Bilinear interpolation call this with level 0, while Trilinear interpolation calls it twice with two level computed by itself, then interpolates between them.

Moreover, to use Mipmap, the mipmap data need to be generated during the execution of constructor. Configuration of whether Mipmap need to be generated is set to be true by default.

```cpp
class Texture {
public:
    // omit interface
    int32_t m_Width, m_Height, m_Channels;
    uint8_t* m_Data;
    int32_t m_MaxMipmapLevel;
    std::vector<uint32_t> m_MipmapOffset;    
};
```

The only need to be explained is `m_MipmapOffset`. Since the data of image is stored in `m_Data` linearly, to move to start position of particular mipmap level. It can be ensured that `m_MipmapOffset.size() = m_MaxMipmapLevel + 1`. 

### Procedure of constructor

The 1st version of `Texture` converts png and jpeg files to unsigned char form by invoking `stbi_load()` function of `stb_image.h`. I'm also considering adding the support of TGA image format from scratch, since I think it's kind of easy to implement and worth trying to implement a parser of binary data.

In the body of constructor, the first step is figure out how many channels does the image file have. I do it by checking the extension of file name. The result is either 3 for jpeg or 4 for png. Then, getting the converted data by invoking: `uint8* data = stbi_load()`. The next step is the hardest step in this procedure, generating Mipmap, So I wrote a individual function to isolate it from the constructor. In `void GenerateMipmap()`, function first determines the maximum level of Mipmap, then generates Mipmap level by level.

The first step is quite clear, compressing the width and height into half until one of them equals 1. The times of compression the max level.

```c++
m_MipmapMaxLevel = -1;
for (int32_t width = m_Width, height = m_Height; 
     width != 1 && height != 1; 
     ++m_MipmapMaxLevel, width >>= 1, height >>= 1) {
    layer_compacity.push_back(m_Channels * width * height);
}
```

Then, since the Mipmap is stored in single `uint8` pointer in sequential form, it is useful to get the offset of each level on runtime. 

```c++
m_MipmapOffset = std::vector<uint32_t>(m_MipmapMaxLevel + 1, 0);
for (int32_t level = 1; level <= m_MipmapMaxLevel; ++level) {
    m_MipmapOffset[level] = m_MipmapOffset[level - 1] + layer_compacity[level - 1];
}
```

Also, by accumulating all level's compacity, the total required memory can be calculated.

```c++
uint32_t byte_sum = std::accumulate(layer_compacity.cbegin(), layer_compacity.cend());
```

With all those stuffs, it's time to begin the generating. First, allocating the memory.

```c++
uint8_t* mipmap = new uint8_t[byte_sum];
std::memcpy(mipmap, m_Data, layer_compacity[0]);
```

This also copies the first level, the original data, to destination. Then, every new level is created by the former one, by computing the arithmetical average of four conjunct grids.

```c++
for (int32_t level = 1, src_width = m_Width, src_height = m_Height; level <= m_MipmapMaxLevel; 
     ++level, src_height >>= 1, src_width >>= 1) {
    uint8_t* current_src = &mipmap_data[m_MipmapOffset[level - 1]];
    uint8_t* current_dst = &mipmap_data[m_MipmapOffset[level]];
    for (int32_t i = 0; i < src_height - src_height % 2; i += 2) {
        for (int32_t j = 0; j < src_width - src_width % 2; j += 2) {
            uint8_t* tl = &current_src[i * src_width * m_Channels + j * m_Channels];
            uint8_t* tr = tl + m_Channels;
            uint8_t* bl = tl + src_width * m_Channels;
            uint8_t* br = bl + m_Channels;
            for (int32_t k = 0; k < m_Channels; ++k) {
                current_dst[k] = (tl[k] + tr[k] + bl[k] + br[k]) / 4;
            }
            current_dst += m_Channels;
        }
    }
    assert(current_dst == &mipmap_data[m_MipmapOffset[level] + layer_compacity[level]]);
}
```

The code is self-explaining, so I think it's no need to explain it. The only need to notice is the second expression in `for` statement. The boundary of width and height in each level is the even number below them, which means the last pixel will be discarded if that dimension has a odd magnitude.

Finally, deallocating the original memory and leting the data pointer points to the new one.

```c++
STBI_FREE(m_Data);
m_Data = mipmap;
```

Now, the object of `Texture` has been instantized completely. The details about how to use it will be introduced in the part of rendering pipeline.

## `IODevice`, the interface between rendering and interacting

Until now, the whole part above is about details of the front-end . In the sense of this application, front-end means things that is not corresponding to graphics issues, which basically includes how to load stuffs and preprocessing needed by rendering. From now on, the part of rendering begins.

The first part is `IODevice`, which is still a interface standing before the rendering pipeline. In the sense of this application, however, it is still the front-end, I decide to introduce it now, because it is the entity that actually invoking the draw calls.

Imaging that you are stand before a printer or a DVD player, after you inserted the materials you want to print/play, the printing/playing begins. `IODevice` is such facility that can represent different output device like printer for a image or a video player for an animation, abstractedly.
