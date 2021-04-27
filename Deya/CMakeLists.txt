add_library(Deya
    src/Deya/Application.cpp
)

include_directories(include)
include_directories(include/Deya)

add_definitions(-DDY_PLATFORM_LINUX)

set_target_properties(Deya PROPERTIES PREFIX "") # REMOVE "lib" FROM OUTPUT FILE

install(TARGETS Deya
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib)