add_executable(pico-mruby)

target_sources(pico-mruby PRIVATE
        pico_mruby.c
        stdio_usb_descriptors.c
        msc_disk.c
	diskio.c
	${PICO_SDK_PATH}/src/rp2_common/pico_stdio_usb/stdio_usb.c
	${PICO_SDK_PATH}/src/rp2_common/pico_stdio_usb/reset_interface.c
	)

target_include_directories(pico-mruby PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}
	${PICO_SDK_PATH}/src/rp2_common/pico_stdio_usb/include
        )

target_link_libraries(pico-mruby PRIVATE
        pico_stdlib
        tinyusb_device_unmarked
        pico_stdio
        pico_time
	pico_unique_id
	pico_usb_reset_interface_headers
        mruby
	mruby-raspberrypipico
	fatfs
	)

pico_enable_stdio_uart(pico-mruby 0)
# Please don't enable stdio_usb.
pico_enable_stdio_usb(pico-mruby 0)

pico_add_extra_outputs(pico-mruby)
pico_set_program_version(pico-mruby PICO_MRUBY_VERSION_STRING)
pico_set_program_url(pico-mruby "https://github.com/k-mana/pico-mruby.git")
