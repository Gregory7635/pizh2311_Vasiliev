add_library(ITMLparse 
    parser.cpp 
    parser.h
)

target_include_directories(ITMLparse PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Enable C++17 features
set_target_properties(ITMLparse PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)