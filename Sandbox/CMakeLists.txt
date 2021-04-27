add_executable(Sandbox src/SandboxApp.cpp)

include_directories(../Deya)
target_link_libraries(Sandbox Deya)

add_definitions(-DDY_PLATFORM_LINUX)

include_directories(include)
include_directories(../Deya/include)

install(TARGETS Sandbox RUNTIME DESTINATION bin)