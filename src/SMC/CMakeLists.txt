set(CMAKE_INCLUDE_CURRENT_DIR ON)

add_subdirectory(SuccessorGeneration)

add_library(SMC ${HEADER_FILES} SMCMain.cpp SMCStubbornSet.cpp)

target_link_libraries(SMC PUBLIC SMCSuccessorGeneration spot bddx)
