file(GLOB EXAMPLES_SRCS CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

foreach(src ${EXAMPLES_SRCS})
  get_filename_component(name ${src} NAME_WE)
  add_executable(${name} ${src})
  target_compile_features(${name} PRIVATE cxx_std_23)
  target_include_directories(${name} PRIVATE ${CMAKE_SOURCE_DIR}/include)
  
  # Add this line to link Tracy to every example automatically
  if(TARGET TracyClient)
    target_link_libraries(${name} PRIVATE TracyClient)
  endif()
endforeach()