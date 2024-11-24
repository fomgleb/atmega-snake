set(APP_NAME "atmega-snake")
set(ELF_FILE ${APP_NAME}.elf)
set(SOURCE_DIR ".")
set(PRIVATE_HEADER_DIR ".")
file(GLOB_RECURSE SOURCES "${SOURCE_DIR}/*.c")

add_executable(${ELF_FILE} ${SOURCES})

target_include_directories(${ELF_FILE} PRIVATE ${PRIVATE_HEADER_DIR})

target_link_libraries(${ELF_FILE} PRIVATE project_warnings)
