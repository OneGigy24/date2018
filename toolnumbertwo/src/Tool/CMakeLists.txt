add_library(tool
           ToolnumbertwoTool.cpp 
          )
target_include_directories(tool
                           PRIVATE ${CMAKE_SOURCE_DIR}/include
                          )

target_link_libraries(tool
                      core)
