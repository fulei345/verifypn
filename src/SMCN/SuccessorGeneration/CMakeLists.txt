set(CMAKE_INCLUDE_CURRENT_DIR ON)

add_library(SMCNSuccessorGeneration ${HEADER_FILES} SMCNSuccessorGenerator.cpp)

target_link_libraries(SMCNSuccessorGeneration bddx spot)
