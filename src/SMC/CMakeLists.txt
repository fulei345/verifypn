set(CMAKE_INCLUDE_CURRENT_DIR ON)

add_subdirectory(Simulation)
add_subdirectory(SuccessorGeneration)

add_library(SMC ${HEADER_FILES} Simulation SuccessorGeneration)

target_link_libraries(SMC PUBLIC SMCSimulation SMCSuccessorGeneration)
