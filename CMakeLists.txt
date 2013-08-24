cmake_minimum_required (VERSION 2.6)
project (test)

set (CMAKE_BUILD_TYPE "Debug")

if (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
  if (MSVC)
    if (${CMAKE_HOST_SYSTEM_PROCESSOR} MATCHES "x86")
      set (IRRLICHT_SYSTEM_NAME "Win32-VisualStudio")
      set (OCULUS_SDK_SYSTEM_NAME "Win32")
    elseif (${CMAKE_HOST_SYSTEM_PROCESSOR} MATCHES "AMD64")
      set (IRRLICHT_SYSTEM_NAME "Win64-VisualStudio")
      set (OCULUS_SDK_SYSTEM_NAME "x64")
    endif ()
  else ()
    set (IRRLICHT_SYSTEM_NAME "Win32-gcc")
    set (OCULUS_SDK_SYSTEM_NAME "Win32")
  endif ()
  set (OCULUS_SDK_LIB_DIR "${CMAKE_SOURCE_DIR}/ThirdParty/OculusSDK/LibOVR/Lib/${OCULUS_SDK_SYSTEM_NAME}/")
elseif (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
  set (IRRLICHT_SYSTEM_NAME "Linux")
  set (OCULUS_SDK_SYSTEM_NAME "Linux")
  set (OCULUS_SDK_LIB_DIR "${CMAKE_SOURCE_DIR}/ThirdParty/OculusSDK/LibOVR/Lib/${OCULUS_SDK_SYSTEM_NAME}/${CMAKE_BUILD_TYPE}/${CMAKE_HOST_SYSTEM_PROCESSOR}/")
endif ()

set (IRRLICHT_LIB_DIR "${CMAKE_SOURCE_DIR}/ThirdParty/irrlicht-1.8/lib/${IRRLICHT_SYSTEM_NAME}/")
set (CMAKE_BINARY_DIR "${CMAKE_SOURCE_DIR}/Binary/")

message ("Irrlicht lib path: ${IRRLICHT_LIB_DIR}")
message ("OculusSDK lib path: ${OCULUS_SDK_LIB_DIR}")

# Platform detection:
# http://www.openguru.com/2009/04/cmake-detecting-platformoperating.html
find_library (
  IRRLICHT_LIB
  NAMES Irrlicht
  HINTS "${IRRLICHT_LIB_DIR}"
)
find_library (
  OCULUS_SDK_LIB
  NAMES ovr ovrd libovr libovrd libovr64 libovr64d
  HINTS "${OCULUS_SDK_LIB_DIR}"
)

include_directories (
  "${CMAKE_SOURCE_DIR}/ThirdParty/irrlicht-1.8/include/"
  "${CMAKE_SOURCE_DIR}/ThirdParty/OculusSDK/LibOVR/Include/"
)

link_directories ("${IRRLICHT_LIB_DIR}" "${OCULUS_SDK_LIB_DIR}")

add_executable (test ${CMAKE_SOURCE_DIR}/Source/test/main.cpp)

if (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
  if (${CMAKE_HOST_SYSTEM_PROCESSOR} MATCHES "x86")
      target_link_libraries (test Irrlicht libovr Winmm)
  elseif (${CMAKE_HOST_SYSTEM_PROCESSOR} MATCHES "AMD64")
      target_link_libraries (test Irrlicht libovr64 Winmm)
  endif ()

elseif (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
  target_link_libraries (test Irrlicht GL X11 Xxf86vm ovr pthread udev Xinerama)
endif ()

install (
  TARGETS test
  DESTINATION ${CMAKE_BINARY_DIR}
)

if (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
  install (
	  FILES "${CMAKE_SOURCE_DIR}/ThirdParty/irrlicht-1.8/bin/${IRRLICHT_SYSTEM_NAME}/Irrlicht.dll"
    DESTINATION ${CMAKE_BINARY_DIR}
  )
endif ()

