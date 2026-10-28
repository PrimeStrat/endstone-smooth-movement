cmake_minimum_required(VERSION 3.15)

project(smooth_movement CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
    endstone
    GIT_REPOSITORY https://github.com/EndstoneMC/endstone.git
    GIT_TAG v0.11
)
FetchContent_MakeAvailable(endstone)

endstone_add_plugin(
    ${PROJECT_NAME}
    src/Main.cpp
    src/SmoothMovement.cpp
    src/Movement/MovementRewriter.cpp
    src/Movement/MovePlayerPacket.cpp
    src/Movement/MoveActorDeltaPacket.cpp
    src/Movement/MoveActorAbsolutePacket.cpp
)
