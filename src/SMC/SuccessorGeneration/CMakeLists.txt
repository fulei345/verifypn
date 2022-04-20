set(CMAKE_INCLUDE_CURRENT_DIR ON)

add_library(SMCSuccessorGeneration ${HEADER_FILES} SMCSuccessorGenerator.cpp SMCReducingSuccessorGenerator.cpp)

target_link_libraries(SMCSuccessorGeneration bddx spot)
