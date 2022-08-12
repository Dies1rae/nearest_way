PROJECT( neares_way )

cmake_minimum_required (VERSION 3.10)
SET(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
add_definitions("-Wall -Wsign-conversion -Wextra -Weffc++")

if (MSVC)
    # warning level 4 and all warnings as errors
    add_compile_options(/W4 /WX)
else()
    # lots of warnings and all warnings as errors
    add_compile_options(-Wall -Wextra -pedantic)
endif()

#SET(CMAKE_VERBOSE_MAKEFILE ON)
SET(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake ${CMAKE_INSTALL_PREFIX}/share/cmake)
SET(cmake_utils ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

message(neares_way: EXECUTABLE_OUTPUT_PATH=${EXECUTABLE_OUTPUT_PATH})

if (NOT (DEFINED EXECUTABLE_OUTPUT_PATH) OR (EXECUTABLE_OUTPUT_PATH STREQUAL ""))
  SET (EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/game)
endif ()
if (NOT (DEFINED LIBRARY_OUTPUT_PATH) OR (LIBRARY_OUTPUT_PATH STREQUAL ""))
  SET (LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib${LIBSUFFIX})
endif ()
if (NOT (DEFINED BINARY_OUTPUT_PATH) OR (BINARY_OUTPUT_PATH STREQUAL ""))
  SET (BINARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib${LIBSUFFIX})
endif ()


SET(SRCS
  main.cpp
  common.h
  )

SET(LIBS
)

INCLUDE_DIRECTORIES(
  ${CMAKE_SOURCE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}/
)


ADD_EXECUTABLE(neares_way
  ${SRCS}
)

message(PROJECT_BINARY_DIR=${PROJECT_BINARY_DIR})

TARGET_LINK_LIBRARIES(neares_way
  ${LIBS}
)

INSTALL(TARGETS neares_way DESTINATION game)
