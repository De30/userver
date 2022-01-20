# AUTOGENERATED, DON'T CHANGE THIS FILE!

 
              
 
 

include(FindPackageHandleStandardArgs)

find_library(Nghttp2_LIBRARIES_nghttp2 NAMES
  nghttp2
  )
list(APPEND Nghttp2_LIBRARIES
  ${Nghttp2_LIBRARIES_nghttp2}
)
  
find_path(Nghttp2_INCLUDE_DIRS_nghttp2_nghttp2_h NAMES
  nghttp2/nghttp2.h
  )
list(APPEND Nghttp2_INCLUDE_DIRS
  ${Nghttp2_INCLUDE_DIRS_nghttp2_nghttp2_h}
)
  
 

 
 
find_package_handle_standard_args(
  Nghttp2
    REQUIRED_VARS
      Nghttp2_LIBRARIES
      Nghttp2_INCLUDE_DIRS
      
    FAIL_MESSAGE
      "Could not find `Nghttp2` package. Debian: sudo apt update && sudo apt install libnghttp2-dev MacOS: brew install nghttp2"
)
mark_as_advanced(
  Nghttp2_LIBRARIES
  Nghttp2_INCLUDE_DIRS
  
)
 
if(NOT Nghttp2_FOUND)
  message(FATAL_ERROR "Could not find `Nghttp2` package. Debian: sudo apt update && sudo apt install libnghttp2-dev MacOS: brew install nghttp2")
elseif(Nghttp2_version_result)
  if(Nghttp2_version_result EQUAL 0)
    if(Nghttp2_VERSION VERSION_LESS None)
      message(FATAL_ERROR "Found but version is Nghttp2_VERSION. Could not find `Nghttp2` package. Debian: sudo apt update && sudo apt install libnghttp2-dev MacOS: brew install nghttp2")
    endif()
  else()
    message(WARNING "Can not determine version. ${Nghttp2_version_error}")
  endif()
endif()

 
if (NOT TARGET Nghttp2)
  add_library(Nghttp2 INTERFACE IMPORTED GLOBAL)
   target_include_directories(Nghttp2 INTERFACE ${Nghttp2_INCLUDE_DIRS})
   target_link_libraries(Nghttp2 INTERFACE ${Nghttp2_LIBRARIES})
  endif(NOT TARGET Nghttp2)