# We want to analyze the functional tests as well
set(dev_essential_cmake_enable_integrated_tests ON)

# for find_package(GTest)
set(CMAKE_FIND_PACKAGE_PREFER_CONFIG TRUE)

# Enables/disables the self contained header tests via environment variable
set(dev_essential_cmake_enable_self_contained_header_tests $ENV{ENABLE_SELF_CONTAINED_HEADER_TESTS})
