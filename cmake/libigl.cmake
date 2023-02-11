if(TARGET igl::core)
    return()
endif()

include(FetchContent)
FetchContent_Declare(
    libigl
    GIT_REPOSITORY https://github.com/libigl/libigl.git
    GIT_TAG 4a91b88f81aa094d076ad4f1460ae663295ac518
)
FetchContent_MakeAvailable(libigl)