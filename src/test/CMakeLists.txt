file(GLOB DX_TEST_SOURCES "${PROJECT_SOURCE_DIR}/src/test/*.cpp")

include_directories(${SERVER_INCLUDE})

foreach (test_source ${DX_TEST_SOURCES})
    # Create a human readable name.
    get_filename_component(test_filename ${test_source} NAME)
    string(REPLACE ".cpp" "" test_name ${test_filename})
    set(file_name "${test_name}_test")
    add_executable(${file_name} EXCLUDE_FROM_ALL ${test_source})

    set(libpath "${CMAKE_SOURCE_DIR}/build/lib/lib")
    string(APPEND libpath ${test_name})
    string(APPEND libpath ".a")

    target_link_libraries(${file_name} ${libpath}) #${CMAKE_SOURCE_DIR}/build/lib/libstack_pra.a
    # Set test target properties and dependencies.
    set_target_properties(${file_name}
            PROPERTIES
            RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
            COMMAND ${file_name}
            )

endforeach (test_source ${DX_TEST_SOURCES})