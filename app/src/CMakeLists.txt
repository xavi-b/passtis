add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/windows)

target_sources(${MODULE_NAME}
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/database.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/encryptor.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/passtis.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
)