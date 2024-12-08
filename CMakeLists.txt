cmake_minimum_required(VERSION 3.25)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_COMPILER avr-gcc)
project(
  atmega-snake
  VERSION 0.1
  LANGUAGES C)

set(F_CPU 16000000UL)
set(MCU atmega328p)

add_definitions(-DF_CPU=${F_CPU})
set(CMAKE_C_FLAGS "-mmcu=${MCU}")
set(CMAKE_EXE_LINKER_FLAGS -mmcu=${MCU})

add_library(project_warnings INTERFACE)
target_compile_options(
  project_warnings
  INTERFACE -Wabi
            -Waddr-space-convert
            -Waddress
            -Waggressive-loop-optimizations
            -Warray-bounds
            -Wattributes
            -Wbad-function-cast
            -Wbool-compare
            -Wbuiltin-macro-redefined
            -Wcast-align
            -Wcast-qual
            -Wchar-subscripts
            -Wchkp
            -Wclobbered
            -Wcomment
            -Wcoverage-mismatch
            -Wcpp
            -Wdate-time
            -Wdeprecated
            -Wdeprecated-declarations
            -Wdesignated-init
            -Wdisabled-optimization
            -Wdiscarded-array-qualifiers
            -Wdiscarded-qualifiers
            -Wdiv-by-zero
            -Wdouble-promotion
            -Wempty-body
            -Wendif-labels
            -Wenum-compare
            -Wextra
            -Wfloat-equal
            -Wformat
            -Wformat-contains-nul
            -Wformat-extra-args
            -Wformat-nonliteral
            -Wformat-security
            -Wformat-signedness
            -Wformat-y2k
            -Wformat-zero-length
            -Wfree-nonheap-object
            -Wignored-qualifiers
            -Wimplicit
            -Wimplicit-function-declaration
            -Wimplicit-int
            -Wincompatible-pointer-types
            -Winit-self
            -Winline
            -Wint-conversion
            -Wint-to-pointer-cast
            -Winvalid-memory-model
            -Winvalid-pch
            -Wjump-misses-init
            -Wlogical-not-parentheses
            -Wlogical-op
            -Wlong-long
            -Wmain
            -Wmaybe-uninitialized
            -Wmemset-transposed-args
            -Wmissing-braces
            -Wmissing-declarations
            -Wmissing-field-initializers
            -Wmissing-include-dirs
            -Wmissing-parameter-type
            -Wmissing-prototypes
            -Wmisspelled-isr
            -Wmultichar
            -Wnarrowing
            -Wnested-externs
            -Wnonnull
            -Wodr
            -Wold-style-declaration
            -Wold-style-definition
            -Wopenmp-simd
            -Woverflow
            -Woverlength-strings
            -Woverride-init
            -Wpacked
            -Wpacked-bitfield-compat
            -Wpadded
            -Wparentheses
            -Wpedantic
            -Wpointer-arith
            -Wpointer-sign
            -Wpointer-to-int-cast
            -Wpragmas
            -Wredundant-decls
            -Wreturn-local-addr
            -Wreturn-type
            -Wsequence-point
            -Wshadow
            -Wshift-count-negative
            -Wshift-count-overflow
            -Wsign-compare
            -Wsizeof-array-argument
            -Wsizeof-pointer-memaccess
            -Wstack-protector
            -Wstrict-prototypes
            -Wsuggest-attribute=const
            -Wsuggest-attribute=format
            -Wsuggest-attribute=noreturn
            -Wsuggest-attribute=pure
            -Wsuggest-final-methods
            -Wsuggest-final-types
            -Wswitch
            -Wswitch-bool
            -Wswitch-default
            -Wswitch-enum
            -Wsync-nand
            -Wtrampolines
            -Wtrigraphs
            -Wtype-limits
            -Wundef
            -Wuninitialized
            -Wunknown-pragmas
            -Wunsafe-loop-optimizations
            -Wunsuffixed-float-constants
            -Wunused
            -Wunused-but-set-parameter
            -Wunused-but-set-variable
            -Wunused-function
            -Wunused-label
            -Wunused-local-typedefs
            -Wunused-macros
            -Wunused-parameter
            -Wunused-result
            -Wunused-value
            -Wunused-variable
            -Wvarargs
            -Wvariadic-macros
            -Wvector-operation-performance
            -Wvolatile-register-var
            -Wwrite-strings)

add_subdirectory(src)
