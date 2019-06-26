cmake_minimum_required(VERSION 3.1.2)

project(passtis)

add_subdirectory("src")
add_subdirectory("third-party")

add_executable(passtis ${SOURCE} ${HEADER})

target_include_directories(passtis PRIVATE ${THIRD_PARTY_INCLUDE})
target_link_libraries(passtis yaml-cpp crypto ncurses menu form boost_program_options xcb pthread png ${THIRD_PARTY_LIB})