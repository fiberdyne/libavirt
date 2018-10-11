#-------------------------------------------------------------------------------
#  LIBAVIRT CONFIGURATION LIBRARY
#-------------------------------------------------------------------------------

# Target
file(GLOB SOURCE_FILES "src/avirt-*.c")
add_library(avirt STATIC ${SOURCE_FILES})

# Target includes
target_include_directories(avirt PUBLIC include)
