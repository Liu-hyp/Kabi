set(LIB_NAME common)
#aux_source_directory(${PROJECT_SOURCE_DIR} PB_FILES)
file(GLOB SOURCES
*.cpp
*.h)
add_library(${LIB_NAME} STATIC ${SOURCES}) 