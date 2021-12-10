add_library(Loader
    loader.cpp
)

add_library(Scene
    scene.cpp
)

add_library(Texture
    texture.cpp
)

add_library(Pipeline
    pipeline.cpp
)

target_link_libraries(Pipeline
    Scene
)

target_link_libraries(Loader
    Scene
    Texture
)

add_library(IODevice
    io_interface.cpp
)

target_link_libraries(IODevice
    Texture
    Renderer
)

add_library(Renderer
    renderer.cpp
)

target_link_libraries(Renderer
    Pipeline
)