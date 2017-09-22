# Install script for directory: /home/arjun/ARJUN/locis/Solvers/KINSOL/kinsol_src/srcdir/src/kinsol

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "")
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

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "1")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  MESSAGE("
Install KINSOL
")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "/home/arjun/ARJUN/locis/Solvers/KINSOL/kinsol_src/builddir/src/kinsol/libsundials_kinsol.a")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/kinsol" TYPE FILE FILES
    "/home/arjun/ARJUN/locis/Solvers/KINSOL/kinsol_src/srcdir/include/kinsol/kinsol_band.h"
    "/home/arjun/ARJUN/locis/Solvers/KINSOL/kinsol_src/srcdir/include/kinsol/kinsol_bbdpre.h"
    "/home/arjun/ARJUN/locis/Solvers/KINSOL/kinsol_src/srcdir/include/kinsol/kinsol_dense.h"
    "/home/arjun/ARJUN/locis/Solvers/KINSOL/kinsol_src/srcdir/include/kinsol/kinsol_direct.h"
    "/home/arjun/ARJUN/locis/Solvers/KINSOL/kinsol_src/srcdir/include/kinsol/kinsol.h"
    "/home/arjun/ARJUN/locis/Solvers/KINSOL/kinsol_src/srcdir/include/kinsol/kinsol_spbcgs.h"
    "/home/arjun/ARJUN/locis/Solvers/KINSOL/kinsol_src/srcdir/include/kinsol/kinsol_spfgmr.h"
    "/home/arjun/ARJUN/locis/Solvers/KINSOL/kinsol_src/srcdir/include/kinsol/kinsol_spgmr.h"
    "/home/arjun/ARJUN/locis/Solvers/KINSOL/kinsol_src/srcdir/include/kinsol/kinsol_spils.h"
    "/home/arjun/ARJUN/locis/Solvers/KINSOL/kinsol_src/srcdir/include/kinsol/kinsol_sptfqmr.h"
    )
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/kinsol" TYPE FILE FILES "/home/arjun/ARJUN/locis/Solvers/KINSOL/kinsol_src/srcdir/src/kinsol/kinsol_impl.h")
endif()

