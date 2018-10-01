#*************************************************************************
#
#  FIBERDYNE CONFIDENTIAL
#  __________________
#
#   [2010] - [2017] Fiberdyne Systems Pty Ltd
#   All Rights Reserved.
#
#  NOTICE:  All information contained herein is, and remains
#  the property of Fiberdyne Systems Pty Ltd and its suppliers,
#  if any.  The intellectual and technical concepts contained
#  herein are proprietary to Fiberdyne Systems Pty Ltd
#  and its suppliers and may be covered by U.S. and Foreign Patents,
#  patents in process, and are protected by trade secret or copyright law.
#  Dissemination of this information or reproduction of this material
#  is strictly forbidden unless prior written permission is obtained
#  from Fiberdyne Systems Pty Ltd.
#
#*************************************************************************

#-------------------------------------------------------------------------------
#  LIBAVIRT CONFIGURATION LIBRARY
#-------------------------------------------------------------------------------

# Defines
add_definitions(-D_GNU_SOURCE)

# Target
add_library(avirt STATIC src/avirt_*.c)

# Target includes
target_include_directories(avirt PUBLIC include)
