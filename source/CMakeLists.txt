add_library(
    MyLibrary1
        "src/MyLibrary1.cpp" 
        "inc/MyLibrary1.h"
)

target_include_directories(
    MyLibrary1 
        PUBLIC "inc"
)
