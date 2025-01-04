include(FetchContent)

# Add avr-utils library
FetchContent_Declare(
  avr_utils
  GIT_REPOSITORY https://github.com/fomgleb/avr-utils
  GIT_TAG v0.1
  GIT_SHALLOW TRUE
  SYSTEM)
FetchContent_MakeAvailable(avr_utils)
