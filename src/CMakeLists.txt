# This file was automatically generated for projects
# without default 'CMakeLists.txt' file.

file(GLOB_RECURSE SOURCES
    "*.cpp"
    "../lib/**/*.cpp"
    "../components/**/*.cpp"
    "../tasks/**/*.cpp"
)

set(INC_DIRS ".")

idf_component_register(
    SRCS ${SOURCES}
    INCLUDE_DIRS ${INC_DIRS}
)