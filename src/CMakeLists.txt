add_subdirectory("windows")

set(SOURCE
   ${SOURCE}
   ${CMAKE_CURRENT_SOURCE_DIR}/database.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/encryptor.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/passtis.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
   PARENT_SCOPE
)
set(HEADERS
   ${HEADERS}
   ${CMAKE_CURRENT_SOURCE_DIR}/database.h
   ${CMAKE_CURRENT_SOURCE_DIR}/encryptor.h
   ${CMAKE_CURRENT_SOURCE_DIR}/passtis.h
   PARENT_SCOPE
)