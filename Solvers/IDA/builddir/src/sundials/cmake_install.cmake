# Install script for directory: /home/arjun/ARJUN/locis/Solvers/IDA/IDA_src/srcdir/src/sundials

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
Install shared components
")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/sundials" TYPE FILE FILES
    "/home/arjun/ARJUN/locis/Solvers/IDA/IDA_src/srcdir/include/sundials/sundials_band.h"
    "/home/arjun/ARJUN/locis/Solvers/IDA/IDA_src/srcdir/include/sundials/sundials_dense.h"
    "/home/arjun/ARJUN/locis/Solvers/IDA/IDA_src/srcdir/include/sundials/sundials_direct.h"
    "/home/arjun/ARJUN/locis/Solvers/IDA/IDA_src/srcdir/include/sundials/sundials_iterative.h"
    "/home/arjun/ARJUN/locis/Solvers/IDA/IDA_src/srcdir/include/sundials/sundials_math.h"
    "/home/arjun/ARJUN/locis/Solvers/IDA/IDA_src/srcdir/include/sundials/sundials_nvector.h"
    "/home/arjun/ARJUN/locis/Solvers/IDA/IDA_src/srcdir/include/sundials/sundials_fnvector.h"
    "/home/arjun/ARJUN/locis/Solvers/IDA/IDA_src/srcdir/include/sundials/sundials_pcg.h"
    "/home/arjun/ARJUN/locis/Solvers/IDA/IDA_src/srcdir/include/sundials/sundials_sparse.h"
    "/home/arjun/ARJUN/locis/Solvers/IDA/IDA_src/srcdir/include/sundials/sundials_spbcgs.h"
    "/home/arjun/ARJUN/locis/Solvers/IDA/IDA_src/srcdir/include/sundials/sundials_spfgmr.h"
    "/home/arjun/ARJUN/locis/Solvers/IDA/IDA_src/srcdir/include/sundials/sundials_spgmr.h"
    "/home/arjun/ARJUN/locis/Solvers/IDA/IDA_src/srcdir/include/sundials/sundials_sptfqmr.h"
    "/home/arjun/ARJUN/locis/Solvers/IDA/IDA_src/srcdir/include/sundials/sundials_types.h"
    )
endif()

