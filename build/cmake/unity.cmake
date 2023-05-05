include_guard(GLOBAL)

# -----------------------------------------------------------------------------
# This file defines unity related files and paths
# -----------------------------------------------------------------------------

# path to unity home dir
set(UNITY_HOME_DIR          $ENV{UNITY_HOME})
set(UNITY_EXTRAS_HOME_DIR   $ENV{UNITY_HOME}/extras)
set(UNITY_FIXTURE_HOME_DIR  ${UNITY_EXTRAS_HOME_DIR}/fixture)
set(UNITY_CONFIG_DIR        $ENV{UNITY_CONFIG_PATH})

# -----------------------------------------------------------------------------
# Unity
# -----------------------------------------------------------------------------
set(UNITY_LIB               Unity)

# path to unity sources
set(UNITY_SOURCES
    ${UNITY_HOME_DIR}/src/unity.c
)

# unity include directories
set(UNITY_INCLUDES
    ${UNITY_HOME_DIR}/src
    ${UNITY_CONFIG_DIR}
)

# -----------------------------------------------------------------------------
# Unity Fixtures
# -----------------------------------------------------------------------------
set(UNITY_FIXTURES_LIB      UnityFixtures)

# path to unity sources
set(UNITY_FIXTURES_SOURCES
    ${UNITY_FIXTURE_HOME_DIR}/src/unity_fixture.c
    ${UNITY_EXTRAS_HOME_DIR}/memory/src/unity_memory.c
)

# unity include directories
set(UNITY_FIXTURES_INCLUDES
    ${UNITY_FIXTURE_HOME_DIR}/src
    ${UNITY_EXTRAS_HOME_DIR}/memory/src
)