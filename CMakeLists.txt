cmake_minimum_required (VERSION 2.6)
project (pm)

set (CMAKE_BUILD_TYPE "Debug")

if (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
  if (MSVC)
    set (IRRLICHT_SYSTEM_NAME "Win32-VisualStudio")
    set (OCULUS_SDK_SYSTEM_NAME "Win32")
    set (LEAP_SDK_SYSTEM_NAME "x86")
  else ()
    set (IRRLICHT_SYSTEM_NAME "Win32-gcc")
    set (OCULUS_SDK_SYSTEM_NAME "Win32")
    set (LEAP_SDK_SYSTEM_NAME "x86")
  endif ()
  set (OCULUS_SDK_LIB_DIR "${CMAKE_SOURCE_DIR}/ThirdParty/OculusSDK/LibOVR/Lib/${OCULUS_SDK_SYSTEM_NAME}/")
elseif (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
  set (IRRLICHT_SYSTEM_NAME "Linux")
  if (${CMAKE_HOST_SYSTEM_PROCESSOR} MATCHES "i386")
    set (LEAP_SDK_SYSTEM_NAME "x86")
  elseif (${CMAKE_HOST_SYSTEM_PROCESSOR} MATCHES "x86_64")
    set (LEAP_SDK_SYSTEM_NAME "x64")
  endif ()
  set (OCULUS_SDK_SYSTEM_NAME "Linux")
  set (OCULUS_SDK_LIB_DIR "${CMAKE_SOURCE_DIR}/ThirdParty/OculusSDK/LibOVR/Lib/${OCULUS_SDK_SYSTEM_NAME}/${CMAKE_BUILD_TYPE}/${CMAKE_HOST_SYSTEM_PROCESSOR}/")
endif ()

set (IRRLICHT_LIB_DIR "${CMAKE_SOURCE_DIR}/ThirdParty/irrlicht-1.8/lib/${IRRLICHT_SYSTEM_NAME}/")
set (LEAP_LIB_DIR "${CMAKE_SOURCE_DIR}/ThirdParty/LeapSDK/lib/${LEAP_SDK_SYSTEM_NAME}/")
set (CMAKE_BINARY_DIR "${CMAKE_SOURCE_DIR}/Binary/")

message ("Irrlicht lib path: ${IRRLICHT_LIB_DIR}")
message ("OculusSDK lib path: ${OCULUS_SDK_LIB_DIR}")
message ("LeapSDK lib path: ${LEAP_LIB_DIR}")

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
find_library (
  LEAP_SDK_LIB
  NAMES Leap Leapd
  HINTS "${LEAP_LIB_DIR}"
)

include_directories (
  "${CMAKE_SOURCE_DIR}/ThirdParty/irrlicht-1.8/include/"
  "${CMAKE_SOURCE_DIR}/ThirdParty/OculusSDK/LibOVR/Include/"
  "${CMAKE_SOURCE_DIR}/ThirdParty/LeapSDK/include/"
)

link_directories ("${IRRLICHT_LIB_DIR}" "${OCULUS_SDK_LIB_DIR}" "${LEAP_LIB_DIR}")

add_executable (
  pm
  ${CMAKE_SOURCE_DIR}/Source/main.cpp
  ${CMAKE_SOURCE_DIR}/Source/COVRSceneManagerDecorator.cpp
  ${CMAKE_SOURCE_DIR}/Source/PrimateMurder.cpp
  ${CMAKE_SOURCE_DIR}/Source/GameWorld.cpp
  ${CMAKE_SOURCE_DIR}/Source/LeapDevice.cpp
  ${CMAKE_SOURCE_DIR}/Source/GameObjectFactory.cpp
  ${CMAKE_SOURCE_DIR}/Source/GameObject.cpp
  ${CMAKE_SOURCE_DIR}/Source/GamePhysics.cpp
  ${CMAKE_SOURCE_DIR}/Source/GameUtil.cpp
  ${CMAKE_SOURCE_DIR}/Source/GameMatrix4.cpp
  ${CMAKE_SOURCE_DIR}/Source/BasketObject.cpp
  ${CMAKE_SOURCE_DIR}/Source/GravityObject.cpp
  ${CMAKE_SOURCE_DIR}/Source/PMType.cpp
  ${CMAKE_SOURCE_DIR}/Source/ResistanceObject.cpp
  ${CMAKE_SOURCE_DIR}/Source/LeafObject.cpp
  ${CMAKE_SOURCE_DIR}/Source/WindObject.cpp
  ${CMAKE_SOURCE_DIR}/Source/WindowInterface.cpp
  ${CMAKE_SOURCE_DIR}/Source/CollidableObject.cpp
  ${CMAKE_SOURCE_DIR}/Source/AvatarObject.cpp
  ${CMAKE_SOURCE_DIR}/Source/HandObject.cpp
  ${CMAKE_SOURCE_DIR}/Source/HeadObject.cpp
  ${CMAKE_SOURCE_DIR}/Source/GameTime.cpp
  ${CMAKE_SOURCE_DIR}/Source/ButtonObject.cpp
  ${CMAKE_SOURCE_DIR}/Source/MainWindowInterFace.cpp
  ${CMAKE_SOURCE_DIR}/Source/MenuUIObject.cpp
  ${CMAKE_SOURCE_DIR}/Source/ComicUIObject.cpp
  ${CMAKE_SOURCE_DIR}/Source/OVRMachine.cpp
)

if (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
  target_link_libraries (pm Irrlicht libovr Leapd Winmm)
elseif (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
  target_link_libraries (pm Irrlicht GL X11 Xxf86vm ovr pthread udev Xinerama Leap)
endif ()

set_target_properties (
  pm
  PROPERTIES
  INSTALL_RPATH "./"
)

if (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
  install (
    FILES
      "${CMAKE_SOURCE_DIR}/ThirdParty/irrlicht-1.8/bin/${IRRLICHT_SYSTEM_NAME}/Irrlicht.dll"
      "${CMAKE_SOURCE_DIR}/ThirdParty/LeapSDK/lib/${LEAP_SDK_SYSTEM_NAME}/Leapd.dll"
    DESTINATION ${CMAKE_BINARY_DIR}
  )
elseif (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
  install (
    FILES ${LEAP_SDK_LIB}
    DESTINATION ${CMAKE_BINARY_DIR}
  )
endif ()

install (
  TARGETS pm
  DESTINATION ${CMAKE_BINARY_DIR}
)

