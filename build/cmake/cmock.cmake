include_guard(GLOBAL)

# -----------------------------------------------------------------------------
# This file defines CMock related files and paths
# -----------------------------------------------------------------------------

# path to unity home dir
set(CMOCK_HOME_DIR          $ENV{CMOCK_HOME})

set(CMOCK_LIB               CMock)

# path to unity library
set(CMOCK_SOURCES
    ${CMOCK_HOME_DIR}/src/cmock.c
)

# unity include directory
set(CMOCK_INCLUDES
    ${CMOCK_HOME_DIR}/src
)
