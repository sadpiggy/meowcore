file(GLOB files_to_remove
    "${CMAKE_BINARY_DIR}/*.o"
    "${CMAKE_BINARY_DIR}/*/CMakeFiles/*.o"
    "${CMAKE_BINARY_DIR}/*/*/*.o"
    "${CMAKE_BINARY_DIR}/main"
    "${CMAKE_BINARY_DIR}/memory_test"
)

foreach(file ${files_to_remove})
    if(EXISTS "${file}")
        message(STATUS "Removing file: ${file}")
        file(REMOVE "${file}")
    endif()
endforeach()
