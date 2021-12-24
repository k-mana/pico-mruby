add_executable(pico-mirb)

target_sources(pico-mirb PRIVATE
	pico_mirb.c
	)

target_link_libraries(pico-mirb PRIVATE
        pico_stdlib
	mruby
	mruby-raspberrypipico
	)

pico_enable_stdio_usb(pico-mirb 1)
pico_enable_stdio_uart(pico-mirb 0)

pico_add_extra_outputs(pico-mirb)
pico_set_program_version(pico-mirb PICO_MRUBY_VERSION_STRING)
pico_set_program_url(pico-mirb "https://github.com/k-mana/pico-mruby.git")
