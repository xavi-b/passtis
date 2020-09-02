set(MODULE_NAME third-party)

message("Compiling \"${MODULE_NAME}\"")

# clip
set(CLIP_LIB_NAME "clip")
set(CLIP_INC_PATH "${CMAKE_CURRENT_SOURCE_DIR}/${CLIP_LIB_NAME}")

add_subdirectory(${CLIP_LIB_NAME})
