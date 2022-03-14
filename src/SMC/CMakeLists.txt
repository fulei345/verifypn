set(CMAKE_INCLUDE_CURRENT_DIR ON)

add_library(SMC ${HEADER_FILES} SMCSuccessorGenerator.cpp)

target_link_libraries(SMC PUBLIC spot bddx)
