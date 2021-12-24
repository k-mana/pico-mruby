if (NOT MRUBY_SRC_PATH)
	set(MRUBY_SRC_PATH ${PROJECT_SOURCE_DIR}/lib/mruby)
endif()

add_library(mruby INTERFACE)
target_include_directories(mruby INTERFACE ${MRUBY_SRC_PATH}/include)
target_include_directories(mruby INTERFACE ${CMAKE_CURRENT_BINARY_DIR}/build/RaspberryPiPico/include)
target_link_libraries(mruby INTERFACE ${CMAKE_CURRENT_BINARY_DIR}/build/RaspberryPiPico/lib/libmruby.a)

add_custom_command(
	OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/build/RaspberryPiPico/lib/libmruby.a
	COMMAND rake MRUBY_BUILD_DIR=${CMAKE_CURRENT_BINARY_DIR}/build MRUBY_CONFIG=${CMAKE_CURRENT_SOURCE_DIR}/RaspberryPiPico.rb
	WORKING_DIRECTORY ${MRUBY_SRC_PATH}
	)

add_custom_target(build_mruby
	DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/build/RaspberryPiPico/lib/libmruby.a
	)

add_dependencies(mruby build_mruby)
