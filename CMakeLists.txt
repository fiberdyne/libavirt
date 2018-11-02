#-------------------------------------------------------------------------------
#  LIBAVIRT CONFIGURATION LIBRARY
#-------------------------------------------------------------------------------

PROJECT_TARGET_ADD(avirt)

# Target
file(GLOB SOURCE_FILES "src/avirt-*.c")
add_library(avirt STATIC ${SOURCE_FILES})

# Target includes
target_include_directories(avirt PUBLIC include)
