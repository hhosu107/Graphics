# Install script for directory: E:/Gits/Graphics/HW2/ExternalProjects/tinyobjloader-1.0.6

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "C:/Program Files/CS580")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Release")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "E:/Gits/Graphics/HW2/build/ExternalProjects/tinyobjloader-1.0.6/Debug/tinyobjloader.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "E:/Gits/Graphics/HW2/build/ExternalProjects/tinyobjloader-1.0.6/Release/tinyobjloader.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "E:/Gits/Graphics/HW2/build/ExternalProjects/tinyobjloader-1.0.6/MinSizeRel/tinyobjloader.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "E:/Gits/Graphics/HW2/build/ExternalProjects/tinyobjloader-1.0.6/RelWithDebInfo/tinyobjloader.lib")
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/tinyobjloader/cmake/tinyobjloader-targets.cmake")
    file(DIFFERENT EXPORT_FILE_CHANGED FILES
         "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/tinyobjloader/cmake/tinyobjloader-targets.cmake"
         "E:/Gits/Graphics/HW2/build/ExternalProjects/tinyobjloader-1.0.6/CMakeFiles/Export/lib/tinyobjloader/cmake/tinyobjloader-targets.cmake")
    if(EXPORT_FILE_CHANGED)
      file(GLOB OLD_CONFIG_FILES "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/tinyobjloader/cmake/tinyobjloader-targets-*.cmake")
      if(OLD_CONFIG_FILES)
        message(STATUS "Old export file \"$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/tinyobjloader/cmake/tinyobjloader-targets.cmake\" will be replaced.  Removing files [${OLD_CONFIG_FILES}].")
        file(REMOVE ${OLD_CONFIG_FILES})
      endif()
    endif()
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/tinyobjloader/cmake" TYPE FILE FILES "E:/Gits/Graphics/HW2/build/ExternalProjects/tinyobjloader-1.0.6/CMakeFiles/Export/lib/tinyobjloader/cmake/tinyobjloader-targets.cmake")
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/tinyobjloader/cmake" TYPE FILE FILES "E:/Gits/Graphics/HW2/build/ExternalProjects/tinyobjloader-1.0.6/CMakeFiles/Export/lib/tinyobjloader/cmake/tinyobjloader-targets-debug.cmake")
  endif()
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/tinyobjloader/cmake" TYPE FILE FILES "E:/Gits/Graphics/HW2/build/ExternalProjects/tinyobjloader-1.0.6/CMakeFiles/Export/lib/tinyobjloader/cmake/tinyobjloader-targets-minsizerel.cmake")
  endif()
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/tinyobjloader/cmake" TYPE FILE FILES "E:/Gits/Graphics/HW2/build/ExternalProjects/tinyobjloader-1.0.6/CMakeFiles/Export/lib/tinyobjloader/cmake/tinyobjloader-targets-relwithdebinfo.cmake")
  endif()
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/tinyobjloader/cmake" TYPE FILE FILES "E:/Gits/Graphics/HW2/build/ExternalProjects/tinyobjloader-1.0.6/CMakeFiles/Export/lib/tinyobjloader/cmake/tinyobjloader-targets-release.cmake")
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE FILE FILES "E:/Gits/Graphics/HW2/ExternalProjects/tinyobjloader-1.0.6/tiny_obj_loader.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/doc/tinyobjloader" TYPE FILE FILES "E:/Gits/Graphics/HW2/ExternalProjects/tinyobjloader-1.0.6/LICENSE")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/tinyobjloader/cmake" TYPE FILE FILES
    "E:/Gits/Graphics/HW2/build/ExternalProjects/tinyobjloader-1.0.6/tinyobjloader-config.cmake"
    "E:/Gits/Graphics/HW2/build/ExternalProjects/tinyobjloader-1.0.6/tinyobjloader-config-version.cmake"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/pkgconfig" TYPE FILE FILES "E:/Gits/Graphics/HW2/build/ExternalProjects/tinyobjloader-1.0.6/tinyobjloader.pc")
endif()

