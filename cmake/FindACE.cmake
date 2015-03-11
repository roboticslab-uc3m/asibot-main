# Copyright: (C) 2009 RobotCub Consortium
# Authors: Alexandre Bernardino, Paul Fitzpatrick, Lorenzo Natale
# CopyPolicy: Released under the terms of the LGPLv2.1 or later, see LGPL.TXT

# Variables set:
#   ACE_FOUND
#   ACE_LIBRARIES
#   ACE_INCLUDE_DIRS


# check for "built-in" ACE4YARP - an experimental feature,
# only currently used for building YARP binary distributions.

## script does not work if executed twice bc ACE_LIBRARY get appended
# fix "optimized.lib" problem in windows (Lorenzo Natale)
if (NOT ACE_FOUND)

IF (NOT ACE4YARP_DIR)
  SET (ACE4YARP_DIR ${CMAKE_BINARY_DIR}/ace4yarp)
  IF (NOT EXISTS ${ACE4YARP_DIR})
    SET (ACE4YARP_DIR ${CMAKE_SOURCE_DIR}/ace4yarp)
  ENDIF ()
  SET(ACE4YARP_SRC_DIR "${ACE4YARP_DIR}/src")
  IF (NOT EXISTS ${ACE4YARP_DIR})
    SET (ACE4YARP_DIR ${CMAKE_SOURCE_DIR}/src/libYARP_OS)
    SET(ACE4YARP_SRC_DIR "${ACE4YARP_DIR}/ace_src")
  ENDIF ()
ENDIF ()

IF (EXISTS ${ACE4YARP_SRC_DIR})
  SET(BUILTIN_ACE TRUE CACHE BOOL "compile ACE with YARP")
  SET(BUILTIN_ACE_PREFIX "")
  IF (EXISTS "${ACE4YARP_DIR}/ace_src")
    SET(BUILTIN_ACE_PREFIX "ace_")
  ENDIF ()
  SET(BUILTIN_ACE_SOURCE_DIR "${ACE4YARP_DIR}/${BUILTIN_ACE_PREFIX}src" CACHE STRING "compile ACE source with YARP" FORCE)
  SET(BUILTIN_ACE_INCLUDE_DIR "${ACE4YARP_DIR}/${BUILTIN_ACE_PREFIX}include" CACHE STRING "include ACE headers with YARP" FORCE)
ENDIF ()

IF (BUILTIN_ACE)
	MESSAGE(STATUS "ACE files have been included within YARP")
	MESSAGE(STATUS "Include: ${BUILTIN_ACE_INCLUDE_DIR}")
	MESSAGE(STATUS "Source: ${BUILTIN_ACE_SOURCE_DIR}")
        IF (NOT EXISTS "${BUILTIN_ACE_INCLUDE_DIR}/ace")
	   MESSAGE(FATAL_ERROR "Cannot find builtin ACE headers")
        ENDIF (NOT EXISTS "${BUILTIN_ACE_INCLUDE_DIR}/ace")
        IF (NOT EXISTS "${BUILTIN_ACE_SOURCE_DIR}")
	   MESSAGE(FATAL_ERROR "Cannot find builtin ACE sources")
        ENDIF (NOT EXISTS "${BUILTIN_ACE_SOURCE_DIR}")

	SET(ACE_INCLUDE_DIR "${BUILTIN_ACE_INCLUDE_DIR}")
	IF(UNIX)
		SET(ACE_LIBRARY "-lm -lpthread -ldl")
		FIND_LIBRARY(ACE_RT_LIBRARY NAMES rt PATH /usr/lib /usr/local/lib) 
		IF (NOT ACE_RT_LIBRARY)
			INCLUDE (CheckLibraryExists)
			CHECK_LIBRARY_EXISTS(rt aio_cancel "" ACE_RT_LIBRARY2)
		ENDIF ()
		MARK_AS_ADVANCED(ACE_RT_LIBRARY)
		IF (ACE_RT_LIBRARY OR ACE_RT_LIBRARY2)
			SET(ACE_LIBRARY "${ACE_LIBRARY} -lrt")
		ENDIF ()
		MESSAGE(STATUS "ACE libraries: ${ACE_LIBRARY}")
	ENDIF(UNIX)
	IF(MINGW)
		SET(ACE_LIBRARY m ws2_32 mswsock netapi32 winmm)
	ENDIF(MINGW)
        IF(MSVC)
                ADD_DEFINITIONS(-DACE_AS_STATIC_LIBS=1)
                SET(ACE_LIBRARY winmm)
        ENDIF(MSVC)
	SET(ACE_LINK_FLAGS ${ACE_LIBRARY})
	SET(ACE_FOUND TRUE)
ELSE (BUILTIN_ACE)
#ELSE (EXISTS "${CMAKE_SOURCE_DIR}/src/libYARP_OS/include/ace")
	

########################################################################
## check pkg-config for ace information, if available

SET(ACE_INCLUDE_DIR_GUESS)
SET(ACE_LIBRARY_DIR_GUESS)
SET(ACE_LINK_FLAGS)

INCLUDE(FindPkgConfig)

if (PKG_CONFIG_FOUND)
  pkg_check_modules(ACE ACE)
  if (ACE_FOUND)
    set(ACE_INCLUDE_DIR_GUESS ${ACE_INCLUDE_DIRS})
    set(ACE_LIBRARY_DIR_GUESS ${ACE_LIBRARY_DIRS})
    set(ACE_LINK_FLAGS ${ACE_LDFLAGS})
    set(ACE_C_FLAGS ${ACE_CFLAGS})
  endif (ACE_FOUND)
endif (PKG_CONFIG_FOUND)

SET(ACE_LINK_FLAGS "${ACE_LINK_FLAGS}" CACHE INTERNAL "ace link flags")

########################################################################
##  general find

FIND_PATH(ACE_INCLUDE_DIR ace/ACE.h $ENV{ACE_ROOT} $ENV{ACE_ROOT}/include ${CMAKE_SOURCE_DIR}/../ACE_wrappers/ /usr/include /usr/local/include ${ACE4YARP_DIR} $ENV{ACE4YARP_DIR} DOC "directory containing ace/*.h for ACE library")

# in YARP1, config was in another directory
SET(ACE_INCLUDE_CONFIG_DIR "" CACHE STRING "location of ace/config.h")
MARK_AS_ADVANCED(ACE_INCLUDE_CONFIG_DIR)

FIND_LIBRARY(ACE_LIBRARY NAMES ACE ace ACE4YARP PATHS $ENV{ACE_ROOT}/lib $ENV{ACE_ROOT} ${CMAKE_SOURCE_DIR}/../ACE_wrappers/lib/ /usr/lib /usr/local/lib ${ACE4YARP_DIR} $ENV{ACE4YARP_DIR} DOC "ACE library file")

IF (WIN32 AND NOT CYGWIN)
	SET(CMAKE_DEBUG_POSTFIX "d")
	FIND_LIBRARY(ACE_DEBUG_LIBRARY NAMES ACE${CMAKE_DEBUG_POSTFIX} ace${CMAKE_DEBUG_POSTFIX} PATHS $ENV{ACE_ROOT}/lib $ENV{ACE_ROOT} ${CMAKE_SOURCE_DIR}/../ACE_wrappers/lib/ /usr/lib /usr/local/lib DOC "ACE library file (debug version)")
ENDIF (WIN32 AND NOT CYGWIN)


########################################################################
## OS-specific extra linkage

# Solaris needs some extra libraries that may not have been found already
IF(CMAKE_SYSTEM_NAME STREQUAL "SunOS")
  MESSAGE(STATUS "need to link solaris-specific libraries")
  #  LINK_LIBRARIES(socket rt)
  SET(ACE_LIBRARY ${ACE_LIBRARY} socket rt nsl)
ENDIF(CMAKE_SYSTEM_NAME STREQUAL "SunOS")

# ACE package doesn't specify that pthread and rt are needed, which is 
# a problem for users of GoLD.  Link pthread (just on Linux for now).
IF(CMAKE_SYSTEM_NAME STREQUAL "Linux")
  SET(ACE_LIBRARY ${ACE_LIBRARY} pthread rt)
ENDIF(CMAKE_SYSTEM_NAME STREQUAL "Linux")

# Windows needs some extra libraries
IF (WIN32 AND NOT CYGWIN)
  MESSAGE(STATUS "need to link windows-specific libraries")
  # if ACE found, add winmm dependency
  IF(ACE_LIBRARY)
    SET(ACE_LIBRARY ${ACE_LIBRARY} winmm)
  ENDIF(ACE_LIBRARY)
ENDIF (WIN32 AND NOT CYGWIN)

IF (MINGW)
  MESSAGE(STATUS "need to link windows-specific libraries")
  #LINK_LIBRARIES(winmm wsock32)
  SET(ACE_LIBRARY ${ACE_LIBRARY} winmm wsock32)
ENDIF(MINGW)


########################################################################
## finished - now just set up flags and complain to user if necessary

IF (ACE_INCLUDE_DIR AND ACE_LIBRARY)
	SET(ACE_FOUND TRUE)
ELSE (ACE_INCLUDE_DIR AND ACE_LIBRARY)
	SET(ACE_FOUND FALSE)
ENDIF (ACE_INCLUDE_DIR AND ACE_LIBRARY)

IF (ACE_DEBUG_LIBRARY)
	SET(ACE_DEBUG_FOUND TRUE)
ENDIF (ACE_DEBUG_LIBRARY)

IF (ACE_FOUND)
	IF (NOT ACE_FIND_QUIETLY)
		MESSAGE(STATUS "Found ACE library: ${ACE_LIBRARY} (${CMAKE_SYSTEM_NAME})")
		MESSAGE(STATUS "Found ACE include: ${ACE_INCLUDE_DIR}")
	ENDIF (NOT ACE_FIND_QUIETLY)
ELSE (ACE_FOUND)
	IF (ACE_FIND_REQUIRED)
		MESSAGE(FATAL_ERROR "Could not find ACE")
	ENDIF (ACE_FIND_REQUIRED)
ENDIF (ACE_FOUND)

ENDIF (BUILTIN_ACE)

if (ACE_DEBUG_FOUND)
  if (ACE_LIBRARY)
    set(ACE_LIBRARY optimized ${ACE_LIBRARY} debug ${ACE_DEBUG_LIBRARY})
  endif (ACE_LIBRARY)
endif (ACE_DEBUG_FOUND)

SET(ACE_LIBRARIES ${ACE_LIBRARY})

SET(Ace_FOUND ${ACE_FOUND})
if (NOT ACE_LIBRARIES)
  set(ACE_LIBRARIES ${ACE_LIBRARY})
endif (NOT ACE_LIBRARIES)
if (NOT ACE_INCLUDE_DIRS)
  set(ACE_INCLUDE_DIRS ${ACE_INCLUDE_DIR} ${ACE_INCLUDE_CONFIG_DIR})
endif (NOT ACE_INCLUDE_DIRS)

if (NOT BUILTIN_ACE)
INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(ACE "ACE not found" ACE_LIBRARIES ACE_INCLUDE_DIRS)
endif ()

endif()