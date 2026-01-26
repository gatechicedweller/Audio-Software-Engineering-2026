## set defines for operating system and platform

if(APPLE)
	set(GTCMT_MACOSX 1)
	# Detect Apple Silicon (M1/M2/M3/M4)
	if(CMAKE_SYSTEM_PROCESSOR MATCHES "arm64")
		set(GTCMT_MACOSX_ARM 1)
	endif()
endif()

if(UNIX AND NOT APPLE)
	## get bit depth
	if(CMAKE_SIZEOF_VOID_P EQUAL 4)
		set(GTCMT_LINUX32 1)
	else()
		set(GTCMT_LINUX64 1)
	endif()
endif()

if(MINGW)
	if(CMAKE_SIZEOF_VOID_P EQUAL 4)
		set(GTCMT_WIN32 1)
	else()
		set(GTCMT_WIN64 1)
	endif()
endif()

if(MSVC)
	if(CMAKE_SIZEOF_VOID_P EQUAL 4)
		set(GTCMT_WIN32 1)
	else()
		set(GTCMT_WIN64 1)
	endif()

	# Set runtime library (static for static builds)
	if(NOT BUILD_SHARED_LIBS)
		set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
	endif()

	foreach(flag_var
			CMAKE_C_FLAGS CMAKE_C_FLAGS_DEBUG CMAKE_C_FLAGS_RELEASE
			CMAKE_C_FLAGS_MINSIZEREL CMAKE_C_FLAGS_RELWITHDEBINFO
			CMAKE_CXX_FLAGS CMAKE_CXX_FLAGS_DEBUG CMAKE_CXX_FLAGS_RELEASE
			CMAKE_CXX_FLAGS_MINSIZEREL CMAKE_CXX_FLAGS_RELWITHDEBINFO)

		if(NOT BUILD_SHARED_LIBS AND NOT gtest_force_shared_crt)
			string(REPLACE "/MD" "-MT" ${flag_var} "${${flag_var}}")
		endif()

		# More strict warnings
		string(REPLACE "/W3" "/W4" ${flag_var} "${${flag_var}}")

		# Prevent D9025 warning
		string(REPLACE "/EHsc" "/fp:precise" ${flag_var} "${${flag_var}}")

		set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG}" CACHE STRING "" FORCE)
		set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE}" CACHE STRING "" FORCE)
	endforeach()
endif()