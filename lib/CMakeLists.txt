add_library(pico-lcd16x2_i2c INTERFACE)

target_include_directories(pico-lcd16x2_i2c
  INTERFACE
  ${CMAKE_CURRENT_LIST_DIR}/include
)

target_link_libraries(pico-lcd16x2_i2c
  INTERFACE
  hardware_i2c
)

target_sources(pico-lcd16x2_i2c
  INTERFACE
  ${CMAKE_CURRENT_LIST_DIR}/lcd16x2_i2c.c
  ${CMAKE_CURRENT_LIST_DIR}/include/lcd16x2_i2c.h
)
