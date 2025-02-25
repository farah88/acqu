cmake_minimum_required (VERSION 2.6)
include(${CMAKE_CURRENT_SOURCE_DIR}/../cmake/subdir-header.cmake)
message(STATUS "Configuring CaLib...")

find_package(ZLIB REQUIRED)
find_package(Curses REQUIRED)

include_directories("include")
include_directories("${CMAKE_SOURCE_DIR}/Tools/a2display/inc")


set(MOD "CaLib")
ROOT_PREPARE_LIB("src" "include" "include" "src/G__${MOD}.cxx" SRCS EXESRCS)
add_library("lib${MOD}" SHARED ${SRCS})
target_link_libraries("lib${MOD}" ${ROOT_LIBRARIES} ${CURSES_LIBRARIES} ${ZLIB_LIBRARIES} Spectrum a2display)
set_target_properties("lib${MOD}" PROPERTIES OUTPUT_NAME ${MOD})

# and we create the calib_manager
add_executable("calib_manager" "src/Main${MOD}Manager.cxx" ${ADDITIONAL_FILES})
target_link_libraries("calib_manager" "lib${MOD}")
