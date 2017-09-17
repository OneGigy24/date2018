add_library(tooling
            ToolnumberoneTool.cpp
            CompilationDatabaseUtils.cpp
            FrontendActions.cpp
            )

target_include_directories(tooling
                           PRIVATE ${CMAKE_SOURCE_DIR}/include
                           )
target_link_libraries(tooling
		      core
		      )
