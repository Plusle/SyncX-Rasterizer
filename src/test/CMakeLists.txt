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
    Core
)

add_executable(TextureInterpolationTest
    TextureInterpolationTest.cpp
)

target_link_libraries(TextureInterpolationTest
    Core
)