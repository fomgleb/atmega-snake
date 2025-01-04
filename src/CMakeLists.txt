set(APP_NAME "atmega-snake")
set(ELF_FILE ${APP_NAME}.elf)
set(HEX_FILE ${APP_NAME}.hex)
set(SOURCE_DIR ".")
set(PRIVATE_HEADER_DIR ".")
file(GLOB_RECURSE SOURCES "${SOURCE_DIR}/*.c")

add_executable(${ELF_FILE} ${SOURCES})
add_custom_command(
  TARGET ${ELF_FILE}
  POST_BUILD
  COMMAND avr-objcopy -j .text -j .data -O ihex ${ELF_FILE} ${HEX_FILE}
  COMMENT "Generating hex file")

target_include_directories(${ELF_FILE} PRIVATE ${PRIVATE_HEADER_DIR})

target_link_libraries(${ELF_FILE} PRIVATE atmega_snake_warnings)
