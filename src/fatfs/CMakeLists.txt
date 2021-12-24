set(TOP ${PROJECT_SOURCE_DIR}/lib/fatfs)

add_library(fatfs)

target_include_directories(fatfs PUBLIC
	${TOP}/source
	)

target_sources(fatfs PRIVATE
	${TOP}/source/ff.c
	)
