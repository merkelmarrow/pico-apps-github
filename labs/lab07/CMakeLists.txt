# Specify the name of the executable.
add_executable(lab07)

# Specify the source files to be compiled.
target_sources(lab07 PRIVATE lab07.c lab07.S)

# Pull in commonly used features.
target_link_libraries(lab07 PRIVATE pico_stdlib pico_multicore)

# Create map/bin/hex file etc.
pico_add_extra_outputs(lab07)

pico_enable_stdio_uart(lab07 0)
pico_enable_stdio_usb(lab07 1)

# Add the URL via pico_set_program_url.
apps_auto_set_url(lab07)
