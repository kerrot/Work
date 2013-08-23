cmake_minimum_required (VERSION 2.6)
project (test)

set (IRRLICHT_LIB_DIR "${CMAKE_SOURCE_DIR}/ThirdParty/irrlicht-1.8/lib/${CMAKE_SYSTEM_NAME}/")
set (OCULUS_SDK_LIB_DIR "${CMAKE_SOURCE_DIR}/ThirdParty/OculusSDK/LibOVR/Lib/${CMAKE_SYSTEM_NAME}/Release/${CMAKE_HOST_SYSTEM_PROCESSOR}/")
set (CMAKE_BINARY_DIR "${CMAKE_SOURCE_DIR}/Binary/")

# Platform detection:
# http://www.openguru.com/2009/04/cmake-detecting-platformoperating.html
find_library (
  IRRLICHT_LIB
  NAMES Irrlicht
  HINTS "${IRRLICHT_LIB_DIR}"
)
find_library (
  OCULUS_SDK_LIB
  NAMES ovr
  HINTS "${OCULUS_SDK_LIB_DIR}"
)

include_directories (
  "${CMAKE_SOURCE_DIR}/ThirdParty/irrlicht-1.8/include/"
  "${CMAKE_SOURCE_DIR}/ThirdParty/OculusSDK/LibOVR/Include/"
)

link_directories ("${IRRLICHT_LIB_DIR}" "${OCULUS_SDK_LIB_DIR}")

add_executable (test ${CMAKE_SOURCE_DIR}/Source/test/main.cpp)
target_link_libraries (test Irrlicht GL X11 Xxf86vm ovr pthread udev Xinerama)

install (
  TARGETS test
  RUNTIME DESTINATION ${CMAKE_BINARY_DIR}
)
