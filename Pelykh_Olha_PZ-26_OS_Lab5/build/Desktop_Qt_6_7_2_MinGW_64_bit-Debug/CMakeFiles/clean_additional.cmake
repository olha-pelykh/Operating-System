# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\OS_lab5_10_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\OS_lab5_10_autogen.dir\\ParseCache.txt"
  "OS_lab5_10_autogen"
  )
endif()
