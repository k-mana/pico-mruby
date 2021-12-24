cmake_minimum_required(VERSION 3.13)

include(lib/pico-sdk/pico_sdk_init.cmake)

project(pico_mruby)

pico_sdk_init()

set(PICO_MRUBY_VERSION_MAJOR 0)
set(PICO_MRUBY_VERSION_MINOR 1)
set(PICO_MRUBY_VERSION_REVISION 0)
set(PICO_MRUBY_VERSION_STRING "${PICO_MRUBY_VERSION_MAJOR}.${PICO_MRUBY_VERSION_MINOR}.${PICO_MRUBY_VERSION_REVISION}")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -DMRB_BYTECODE_DECODE_OPTION")
add_subdirectory(src)
