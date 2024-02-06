# 定义一个函数来创建可执行文件并设置其属性
function(create_test TARGET_NAME SOURCE_FILE)
    add_executable(${TARGET_NAME} ${SOURCE_FILE})
    set_target_properties(${TARGET_NAME} PROPERTIES
            RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin")
    target_link_libraries(${TARGET_NAME}
            PRIVATE
            base
            net)
    target_include_directories(${TARGET_NAME} PUBLIC
            "${CMAKE_SOURCE_DIR}/muduo/base/include"
            "${CMAKE_SOURCE_DIR}/muduo/net/include")
endfunction()

# 使用函数来创建两个测试程序
create_test(test5 test5.cpp)

# 设置全局包含目录
include_directories(${Boost_INCLUDE_DIRS})
