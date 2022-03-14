set(CMAKE_INCLUDE_CURRENT_DIR ON)

add_library(Simulation ${HEADER_FILES} SMCSimulation.cpp)
target_link_libraries(Simulation)