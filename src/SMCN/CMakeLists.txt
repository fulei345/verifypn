set(CMAKE_INCLUDE_CURRENT_DIR ON)

add_subdirectory(SuccessorGeneration)

add_library(SMCN ${HEADER_FILES} SMCNMain.cpp)

target_link_libraries(SMCN PUBLIC SMCNSuccessorGeneration spot bddx)
