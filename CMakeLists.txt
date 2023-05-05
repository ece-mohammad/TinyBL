cmake_minimum_required(VERSION 3.18)


set(PROJECT_HOME_DIR        ${CMAKE_SOURCE_DIR})


set(CMAKE_MODULE_PATH       
    ${PROJECT_HOME_DIR}/build/cmake
)


project(TinyBL
    VERSION         0.1
    DESCRIPTION     ""
    HOMEPAGE_URL    ""
    LANGUAGES       C ASM
)

# -----------------------------------------------------------------------------
# Includes
# -----------------------------------------------------------------------------

include(CTest)
include(unity)
include(cmock)


include_directories(
    BEFORE
    ${PROJECT_HOME_DIR}/Inc
)


add_compile_options(
    "--coverage"
    "-Wall"
    "-Wextra"
    "-Wpedantic"
    "--std=c99"
    "-fshort-enums"
)

add_link_options(
    "--coverage"
)

# -----------------------------------------------------------------------------
# Unity lib
# -----------------------------------------------------------------------------
add_library(${UNITY_LIB}
    STATIC
    ${UNITY_SOURCES}
)

target_include_directories(${UNITY_LIB}
    PUBLIC
    ${UNITY_INCLUDES}
)

# -----------------------------------------------------------------------------
# Unity Fixture lib
# -----------------------------------------------------------------------------
add_library(${UNITY_FIXTURES_LIB}
    STATIC
    ${UNITY_FIXTURES_SOURCES}
)

target_include_directories(${UNITY_FIXTURES_LIB}
    PUBLIC
    ${UNITY_FIXTURES_INCLUDES}
)

target_link_libraries(${UNITY_FIXTURES_LIB}
    ${UNITY_LIB}
)


# -----------------------------------------------------------------------------
# CMock lib
# -----------------------------------------------------------------------------
add_library(${CMOCK_LIB}
    STATIC
    ${CMOCK_SOURCES}
)

target_include_directories(${CMOCK_LIB}
    PUBLIC
    ${CMOCK_INCLUDES}
    ${UNITY_INCLUDES}
)

# -----------------------------------------------------------------------------
# Flash lib
# -----------------------------------------------------------------------------

set(FLASH_TARGET    Flash)
set(FLASH_LIB       ${FLASH_TARGET})

set(FLASH_SOURCE_FILES
    ${PROJECT_HOME_DIR}/Src/Flash/Flash.c
)

set(FLASH_INCLUDES
    ${PROJECT_HOME_DIR}/Inc/Flash
)

add_library(${FLASH_LIB})

target_sources(${FLASH_LIB}
    PUBLIC
    ${FLASH_SOURCE_FILES}
)

target_include_directories(${FLASH_LIB}
    PUBLIC
    ${FLASH_INCLUDES}
)

# -----------------------------------------------------------------------------
# Flash unit test lib
# -----------------------------------------------------------------------------

set(FLASH_TEST_TARGET  FlashTest)
set(FLASH_TEST_LIB     ${FLASH_TEST_TARGET})

set(FLASH_TEST_SOURCE_FILES
    ${PROJECT_HOME_DIR}/Test/Src/Flash/FlashTest.c
    ${PROJECT_HOME_DIR}/Test/TestRunners/Flash/FlashTestRunner.c
)

set(FLASH_TEST_INCLUDES
    ${FLASH_INCLUDES}
    ${PROJECT_HOME_DIR}/Test/Inc/Flash
)

add_library(${FLASH_TEST_LIB})

target_sources(${FLASH_TEST_LIB}
    PUBLIC
    ${FLASH_TEST_SOURCE_FILES}
)

target_include_directories(${FLASH_TEST_LIB}
    PUBLIC
    ${FLASH_TEST_INCLUDES}
    ${UNITY_INCLUDES}
    ${UNITY_FIXTURES_INCLUDES}
)

add_dependencies(${FLASH_TEST_LIB}
    ${FLASH_LIB}
)

# -----------------------------------------------------------------------------
# UART lib
# -----------------------------------------------------------------------------


# -----------------------------------------------------------------------------
# UART unit test lib
# -----------------------------------------------------------------------------


# -----------------------------------------------------------------------------
# Timer lib
# -----------------------------------------------------------------------------

# -----------------------------------------------------------------------------
# Timer unit test lib
# -----------------------------------------------------------------------------


# -----------------------------------------------------------------------------
# TinySerialize lib
# -----------------------------------------------------------------------------

set(TINY_SERIALIZE_TARGET            TinySerialize)
set(TINY_SERIALIZE_LIB               ${TINY_SERIALIZE_TARGET})

set(TINY_SERIALIZE_SOURCE_FILES
    ${PROJECT_HOME_DIR}/Src/TinySerialize/TinySerialize.c
)

set(TINY_SERIALIZE_INCLUDES
    ${PROJECT_HOME_DIR}/Inc/TinySerialize
)

add_library(${TINY_SERIALIZE_LIB})

target_sources(${TINY_SERIALIZE_LIB}
    PUBLIC
    ${TINY_SERIALIZE_SOURCE_FILES}
)

target_include_directories(${TINY_SERIALIZE_LIB}
    PUBLIC
    ${TINY_SERIALIZE_INCLUDES}
)

# -----------------------------------------------------------------------------
# TinySerialize unit test lib
# -----------------------------------------------------------------------------

set(TINY_SERIALIZE_TEST_TARGET  TinySerializeTest)
set(TINY_SERIALIZE_TEST_LIB     ${TINY_SERIALIZE_TEST_TARGET})

set(TINY_SERIALIZE_TEST_SOURCE_FILES
    ${PROJECT_HOME_DIR}/Test/Src/TinySerialize/TinySerializeTest.c
    ${PROJECT_HOME_DIR}/Test/TestRunners/TinySerialize/TinySerializeTestRunner.c
)

set(TINY_SERIALIZE_TEST_INCLUDES
    ${TINY_SERIALIZE_INCLUDES}
    ${PROJECT_HOME_DIR}/Test/Inc/TinySerialize
)

add_library(${TINY_SERIALIZE_TEST_LIB})

target_sources(${TINY_SERIALIZE_TEST_LIB}
    PUBLIC
    ${TINY_SERIALIZE_TEST_SOURCE_FILES}
)

target_include_directories(${TINY_SERIALIZE_TEST_LIB}
    PUBLIC
    ${TINY_SERIALIZE_TEST_INCLUDES}
    ${UNITY_INCLUDES}
    ${UNITY_FIXTURES_INCLUDES}
)

add_dependencies(${TINY_SERIALIZE_TEST_LIB}
    ${TINY_SERIALIZE_LIB}
)

# -----------------------------------------------------------------------------
# TinyMessage lib
# -----------------------------------------------------------------------------

set(TINY_MESSAGE_TARGET            TinyMessage)
set(TINY_MESSAGE_LIB               ${TINY_MESSAGE_TARGET})

set(TINY_MESSAGE_SOURCE_FILES
    ${PROJECT_HOME_DIR}/Src/TinyMessage/TinyMessage.c
)

set(TINY_MESSAGE_INCLUDES
    ${PROJECT_HOME_DIR}/Inc/TinyMessage
    ${TINY_SERIALIZE_INCLUDES}
)

add_library(${TINY_MESSAGE_LIB})

target_sources(${TINY_MESSAGE_LIB}
    PUBLIC
    ${TINY_MESSAGE_SOURCE_FILES}
)

target_include_directories(${TINY_MESSAGE_LIB}
    PUBLIC
    ${TINY_MESSAGE_INCLUDES}
)

# -----------------------------------------------------------------------------
# TinyMessage unit test lib
# -----------------------------------------------------------------------------

set(TINY_MESSAGE_TEST_TARGET  TinyMessageTest)
set(TINY_MESSAGE_TEST_LIB     ${TINY_MESSAGE_TEST_TARGET})

set(TINY_MESSAGE_TEST_SOURCE_FILES
    ${PROJECT_HOME_DIR}/Test/Src/TinyMessage/TinyMessageTest.c
    ${PROJECT_HOME_DIR}/Test/TestRunners/TinyMessage/TinyMessageTestRunner.c
)

set(TINY_MESSAGE_TEST_INCLUDES
    ${TINY_MESSAGE_INCLUDES}
    ${PROJECT_HOME_DIR}/Test/Inc/TinyMessage
)

add_library(${TINY_MESSAGE_TEST_LIB})

target_sources(${TINY_MESSAGE_TEST_LIB}
    PUBLIC
    ${TINY_MESSAGE_TEST_SOURCE_FILES}
)

target_include_directories(${TINY_MESSAGE_TEST_LIB}
    PUBLIC
    ${TINY_MESSAGE_TEST_INCLUDES}
    ${UNITY_INCLUDES}
    ${UNITY_FIXTURES_INCLUDES}
)

add_dependencies(${TINY_MESSAGE_TEST_LIB}
    ${TINY_MESSAGE_LIB}
)


# -----------------------------------------------------------------------------
# Fletcher16 lib
# -----------------------------------------------------------------------------

set(FLETCHER16_TARGET   Fletcher16)
set(FLETCHER16_LIB      ${FLETCHER16_TARGET})

set(FLETCHER16_SOURCE_FILES
${PROJECT_HOME_DIR}/Src/Fletcher16/Fletcher16.c
)

set(FLETCHER16_INCLUDES
    ${PROJECT_HOME_DIR}/Inc/Fletcher16
)


add_library(${FLETCHER16_LIB})

target_sources(${FLETCHER16_LIB}
    PUBLIC
    ${FLETCHER16_SOURCE_FILES}
)

target_include_directories(${FLETCHER16_LIB}
    PUBLIC
    ${FLETCHER16_INCLUDES}
)


# -----------------------------------------------------------------------------
# Fletcher16 unit test lib
# -----------------------------------------------------------------------------

set(FLETCHER16_TEST_TARGET  Fletcher16Test)
set(FLETCHER16_TEST_LIB     ${FLETCHER16_TEST_TARGET})

set(FLETCHER16_TEST_SOURCE_FILES
    ${PROJECT_HOME_DIR}/Test/Src/Fletcher16/Fletcher16Test.c
    ${PROJECT_HOME_DIR}/Test/TestRunners/Fletcher16/Fletcher16TestRunner.c
)

set(FLETCHER16_TEST_INCLUDES
    ${FLETCHER16_INCLUDES}
    ${PROJECT_HOME_DIR}/Test/Inc/Fletcher16
)

add_library(${FLETCHER16_TEST_LIB})

target_sources(${FLETCHER16_TEST_LIB}
    PUBLIC
    ${FLETCHER16_TEST_SOURCE_FILES}
)

target_include_directories(${FLETCHER16_TEST_LIB}
    PUBLIC
    ${FLETCHER16_TEST_INCLUDES}
    ${UNITY_INCLUDES}
    ${UNITY_FIXTURES_INCLUDES}
)

add_dependencies(${FLETCHER16_TEST_LIB}
    ${FLETCHER16_LIB}
)


# -----------------------------------------------------------------------------
# CRC32 lib
# -----------------------------------------------------------------------------

set(CRC32_TARGET            CRC32)
set(CRC32_LIB               ${CRC32_TARGET})

set(CRC32_SOURCE_FILES
    ${PROJECT_HOME_DIR}/Src/CRC32/CRC32.c
)

set(CRC32_INCLUDES
    ${PROJECT_HOME_DIR}/Inc/CRC32
)

add_library(${CRC32_LIB})

target_sources(${CRC32_LIB}
    PUBLIC
    ${CRC32_SOURCE_FILES}
)

target_include_directories(${CRC32_LIB}
    PUBLIC
    ${CRC32_INCLUDES}
)


# -----------------------------------------------------------------------------
# CRC32 unit test lib
# -----------------------------------------------------------------------------

set(CRC32_TEST_TARGET  CRC32Test)
set(CRC32_TEST_LIB     ${CRC32_TEST_TARGET})

set(CRC32_TEST_SOURCE_FILES
    ${PROJECT_HOME_DIR}/Test/Src/CRC32/CRC32Test.c
    ${PROJECT_HOME_DIR}/Test/TestRunners/CRC32/CRC32TestRunner.c
)

set(CRC32_TEST_INCLUDES
    ${CRC32_INCLUDES}
    ${PROJECT_HOME_DIR}/Test/Inc/CRC32
)

add_library(${CRC32_TEST_TARGET})

target_sources(${CRC32_TEST_TARGET}
    PUBLIC
    ${CRC32_TEST_SOURCE_FILES}
)

target_include_directories(${CRC32_TEST_TARGET}
    PUBLIC
    ${CRC32_TEST_INCLUDES}
    ${UNITY_INCLUDES}
    ${UNITY_FIXTURES_INCLUDES}
)

add_dependencies(${CRC32_TEST_TARGET}
    ${CRC32_LIB}
)

# -----------------------------------------------------------------------------
# Transport lib
# -----------------------------------------------------------------------------

set(TRANSPORT_TARGET   Transport)
set(TRANSPORT_LIB      ${TRANSPORT_TARGET})

set(TRANSPORT_SOURCE_FILES
${PROJECT_HOME_DIR}/Src/Transport/Transport.c
)

set(TRANSPORT_INCLUDES
    ${PROJECT_HOME_DIR}/Inc/Transport
)


add_library(${TRANSPORT_LIB})

target_sources(${TRANSPORT_LIB}
    PUBLIC
    ${TRANSPORT_SOURCE_FILES}
)

target_include_directories(${TRANSPORT_LIB}
    PUBLIC
    ${TRANSPORT_INCLUDES}
)


# -----------------------------------------------------------------------------
# Fletcher16 unit test lib
# -----------------------------------------------------------------------------

set(TRANSPORT_TEST_TARGET  TransportTest)
set(TRANSPORT_TEST_LIB     ${TRANSPORT_TEST_TARGET})

set(TRANSPORT_TEST_SOURCE_FILES
    ${PROJECT_HOME_DIR}/Test/Src/Transport/TransportTest.c
    ${PROJECT_HOME_DIR}/Test/TestRunners/Transport/TransportTestRunner.c
)

set(TRANSPORT_TEST_INCLUDES
    ${TRANSPORT_INCLUDES}
    ${PROJECT_HOME_DIR}/Test/Inc/Transport
)

add_library(${TRANSPORT_TEST_LIB})

target_sources(${TRANSPORT_TEST_LIB}
    PUBLIC
    ${TRANSPORT_TEST_SOURCE_FILES}
)

target_include_directories(${TRANSPORT_TEST_LIB}
    PUBLIC
    ${TRANSPORT_TEST_INCLUDES}
    ${UNITY_INCLUDES}
    ${UNITY_FIXTURES_INCLUDES}
)

add_dependencies(${TRANSPORT_TEST_LIB}
    ${TRANSPORT_LIB}
)


# -----------------------------------------------------------------------------
# TinyBL lib
# -----------------------------------------------------------------------------

set(TINYBL_TARGET   TinyBL)
set(TINYBL_LIB      ${TINYBL_TARGET})

set(TINYBL_SOURCE_FILES
    ${PROJECT_HOME_DIR}/Src/TinyBL/TinyBL.c
)

set(TINYBL_INCLUDES
    ${PROJECT_HOME_DIR}/Inc/TinyBL
    ${FLASH_INCLUDES}
    ${TINY_SERIALIZE_INCLUDES}
    ${TINY_MESSAGE_INCLUDES}
    ${CRC32_INCLUDES}
)

add_library(${TINYBL_LIB})

target_sources(
    ${TINYBL_LIB}
    PUBLIC
    ${TINYBL_SOURCE_FILES}
)

target_include_directories(
    ${TINYBL_LIB}
    PUBLIC
    ${TINYBL_INCLUDES}
)


# -----------------------------------------------------------------------------
# TinyBL unit test lib
# -----------------------------------------------------------------------------

set(TINYBL_TEST_TARGET      TinyBLTest)
set(TINYBL_TEST_LIB         ${TINYBL_TEST_TARGET})

set(TINYBL_TEST_SOURCE_FILES
    ${PROJECT_HOME_DIR}/Test/Src/TinyBL/TinyBLTest.c
    ${PROJECT_HOME_DIR}/Test/TestRunners/TinyBL/TinyBLTestRunner.c
)

set(TINYBL_TEST_INCLUDES
    ${PROJECT_HOME_DIR}/Test/Inc/TinyBL
)

add_library(${TINYBL_TEST_LIB})

target_sources(${TINYBL_TEST_LIB}
    PUBLIC
    ${TINYBL_TEST_SOURCE_FILES}
)

target_include_directories(${TINYBL_TEST_LIB}
    PUBLIC
    ${TINYBL_INCLUDES}
    ${TINYBL_TEST_INCLUDES}
    ${UNITY_INCLUDES}
    ${UNITY_FIXTURES_INCLUDES}
)

target_link_libraries(${TINYBL_TEST_LIB}
    ${TINYBL_LIB}
    ${UNITY_LIB}
    ${UNITY_FIXTURES_LIB}
)


# -----------------------------------------------------------------------------
# Bootloader lib
# -----------------------------------------------------------------------------

# -----------------------------------------------------------------------------
# Bootloader unit test lib
# -----------------------------------------------------------------------------

# set_target_properties(${PROJECT_NAME}
#     PROPERTIES
#         C_STANDARD          11
#         C_STANDARD_REQUIRED ON
#         C_EXTENSIONS        OFF
#         PUBLIC_HEADER       "${${PROJECT_NAME}_PUBLIC_HEADERS}"
#         EXPORT_NAME         framework
# )

# -----------------------------------------------------------------------------
# Application lib
# -----------------------------------------------------------------------------

# -----------------------------------------------------------------------------
# Application unit test
# -----------------------------------------------------------------------------

# -----------------------------------------------------------------------------
# All tests
# -----------------------------------------------------------------------------
set(ALL_TESTS_TARGTET   AllTests)

add_executable(${ALL_TESTS_TARGTET})

target_sources(${ALL_TESTS_TARGTET}
    PUBLIC
    ${PROJECT_HOME_DIR}/Test/Src/AllTests.c
)

target_include_directories(${ALL_TESTS_TARGTET}
    PUBLIC
    ${UNITY_FIXTURES_INCLUDES}
    ${FLETCHER16_TEST_INCLUDES}
)

target_link_libraries(${ALL_TESTS_TARGTET}
    ${UNITY_LIB}
    ${UNITY_FIXTURES_LIB}
    ${FLASH_LIB}
    ${FLASH_TEST_LIB}
    ${TINY_SERIALIZE_LIB}
    ${TINY_SERIALIZE_TEST_LIB}
    ${TINY_MESSAGE_LIB}
    ${TINY_MESSAGE_TEST_LIB}
    ${FLETCHER16_LIB}
    ${FLETCHER16_TEST_LIB}
    ${CRC32_LIB}
    ${CRC32_TEST_LIB}
    ${TINYBL_LIB}
    ${TINYBL_TEST_LIB}
)

# Flash test
add_test(NAME ${FLASH_TARGET} COMMAND ${ALL_TESTS_TARGTET} -v -g ${FLASH_TARGET})

# Fletcher16 test
add_test(NAME ${FLETCHER16_TARGET} COMMAND ${ALL_TESTS_TARGTET} -v -g ${FLETCHER16_TARGET})

# TinySerialize test
add_test(NAME ${TINY_SERIALIZE_TARGET} COMMAND ${ALL_TESTS_TARGTET} -v -g ${TINY_SERIALIZE_TARGET})

# TinyMessage test
add_test(NAME ${TINY_MESSAGE_TARGET} COMMAND ${ALL_TESTS_TARGTET} -v -g ${TINY_MESSAGE_TARGET})

# CRC32 test
add_test(NAME ${CRC32_TARGET} COMMAND ${ALL_TESTS_TARGTET} -v -g ${CRC32_TARGET})

# TinyBL test
add_test(NAME ${TINYBL_TARGET} COMMAND ${ALL_TESTS_TARGTET} -v -g ${TINYBL_TARGET})

# Transport test
add_test(NAME ${TRANSPORT_TARGET} COMMAND ${ALL_TESTS_TARGTET} -v -g ${TRANSPORT_TARGET})
