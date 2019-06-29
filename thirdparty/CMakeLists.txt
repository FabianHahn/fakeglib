if(FAKEGLIB_VENDOR_GOOGLETEST)
	add_subdirectory(googletest)
	add_library(googletest::gtest ALIAS gtest)
endif()
