cmake_minimum_required(VERSION 3.10)
project(MQForge)

option(TARGET_PLATFORM "Target platform (Windows or Linux)" "Linux")

if(TARGET_PLATFORM STREQUAL "Windows")
	set(CMAKE_SYSTEM_NAME Windows)
	set(CMAKE_C_COMPILER i686-w64-mingw32-gcc)
	set(CMAKE_CXX_COMPILER i686-w64-mingw32-c++)
	set(CMAKE_FIND_ROOT_PATH /usr/i686-w64-mingw32/sys-root/mingw)

	add_executable(compiler
		globals.c
		main.c
		utils.c
		basic_json_parse.c
		file_utils.c
		file_orderer.c
		file_importer.c
		compiler.c
		sys_interactions_windows.c
	)

	set_target_properties(compiler PROPERTIES
		OUTPUT_NAME "MQForge"
		SUFFIX ".exe"
	)

elseif(TARGET_PLATFORM STREQUAL "Linux")
	set(CMAKE_SYSTEM_NAME Linux)
	set(CMAKE_C_COMPILER gcc)
	set(CMAKE_CXX_COMPILER g++)

	add_executable(compiler
		globals.c
		main.c
		utils.c
		basic_json_parse.c
		file_utils.c
		file_orderer.c
		file_importer.c
		compiler.c
		sys_interactions_linux.c
	)

	set_target_properties( compiler PROPERTIES OUTPUT_NAME "MQForge" )

else()
	message(FATAL_ERROR "Unsupported TARGET_PLATFORM: ${TARGET_PLATFORM}")

endif()



# target_compile_options(compiler PRIVATE -m32)
