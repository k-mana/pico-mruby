add_executable(hello_world)

target_sources(hello_world PRIVATE
	hello_world.c
	)

target_link_libraries(hello_world PRIVATE
        pico_stdlib
	mruby
	mruby-raspberrypipico
	)

pico_enable_stdio_usb(hello_world 1)
pico_enable_stdio_uart(hello_world 0)

pico_add_extra_outputs(hello_world)
pico_set_program_version(hello_world PICO_MRUBY_VERSION_STRING)
pico_set_program_url(hello_world "https://github.com/k-mana/pico-mruby.git")
