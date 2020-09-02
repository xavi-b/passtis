set(MODULE_NAME passtis)

message("Compiling \"${MODULE_NAME}\"")

add_executable(${MODULE_NAME} "")

# dependencies
find_package(Boost COMPONENTS program_options REQUIRED)
find_package(OpenSSL REQUIRED)
find_package(yaml-cpp REQUIRED)
find_package(Curses REQUIRED)
find_library(MENU_LIBRARY menu)
find_library(FORM_LIBRARY form)

target_include_directories(${MODULE_NAME}
    PUBLIC
        ${PROJECT_SOURCE_DIR}/third-party/clip
)

target_link_libraries(${MODULE_NAME}
    PUBLIC
        ${Boost_LIBRARIES}
        ${OPENSSL_CRYPTO_LIBRARY}
        ${YAML_CPP_LIBRARIES}
        ${CURSES_LIBRARIES}
        ${MENU_LIBRARY}
        ${FORM_LIBRARY}
        clip
)

# sources
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src)

target_include_directories(${MODULE_NAME}
    PUBLIC
        external
        "${CMAKE_CURRENT_SOURCE_DIR}/include"
)