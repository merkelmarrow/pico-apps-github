# Specify the name of the executable.
add_executable(lab01)
add_executable(lab01_multicore)

# Specify the source files to be compiled.
target_sources(lab01 PRIVATE "lab01.cpp")
target_sources(lab01_multicore PRIVATE "lab01_multicore.cpp")

# Pull in commonly used features.
target_link_libraries(lab01 PRIVATE pico_stdlib)
target_link_libraries(lab01_multicore PRIVATE pico_stdlib pico_multicore)

# Create map/bin/hex file etc.
pico_add_extra_outputs(lab01)
pico_add_extra_outputs(lab01_multicore)

# Add the URL via pico_set_program_url.
apps_auto_set_url(lab01)
apps_auto_set_url(lab01_multicore) 
