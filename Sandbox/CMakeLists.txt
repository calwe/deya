add_executable(Sandbox src/SandboxApp.cpp)

# EXTERNAL LIBRARIES

include_directories(../Deya/vendor/spdlog/include)

# INTERNAL LIBRARIES

include_directories(../Deya)
target_link_libraries(Sandbox Deya)

add_definitions(-DDY_PLATFORM_LINUX)

include_directories(include)
include_directories(../Deya/include)

install(TARGETS Sandbox RUNTIME DESTINATION bin)