add_executable(GeneralTest
    general.cpp   
)

add_executable(VectorTest
    VectorTest.cpp
)

add_executable(ImagePrinter
    ImagePrinterTest.cpp
)

target_link_libraries(ImagePrinter 
    IODevice
)

add_executable(TexturePrototypeTest
    TexturePrototypeTest.cpp
)

target_link_libraries(TexturePrototypeTest
    IODevice
    Texture
)

add_executable(MipmapPrototype
    MipmapPrototype.cpp
)

add_executable(MipmapLayersDisplay
    MipmapLayersDisplay.cpp
)

target_link_libraries(MipmapLayersDisplay
    Texture
)

add_executable(LoaderPrototypeTest
    LoaderPrototypeTest.cpp
)

target_link_libraries(LoaderPrototypeTest
    Loader
)