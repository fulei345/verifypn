set(CMAKE_INCLUDE_CURRENT_DIR ON)

add_library(Simulation SMCSimulation.cpp)
target_link_libraries(Simulation)