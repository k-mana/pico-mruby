add_library(mruby-raspberrypipico)

target_sources(mruby-raspberrypipico PRIVATE
	src/raspberrypipico.c
	)

target_include_directories(mruby-raspberrypipico PUBLIC
	src
	)

target_link_libraries(mruby-raspberrypipico PRIVATE
	pico_runtime
	pico_time
	mruby
	)
