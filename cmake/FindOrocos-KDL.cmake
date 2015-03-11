IF(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
  ####################### WINDOWS ####################################
FIND_PATH(KDL_INCLUDE_DIR kdl "C:/Archivos de programa/Orocos-KDL/include")
IF ( KDL_INCLUDE_DIR )
    MESSAGE(STATUS "-- Looking for Orocos-KDL headers - found")
    #    SET(KDL_CFLAGS "${KDL_CFLAGS} -I\"${EIGEN2_INCLUDE_DIR}\"" CACHE INTERNAL "")
ELSE ( KDL_INCLUDE_DIR )
    MESSAGE(FATAL_ERROR "-- Looking for Orocos-KDL headers - not found")
ENDIF ( KDL_INCLUDE_DIR )

#FIND_PATH(Orocos-KDL_LIBRARIES orocos-kdl "C:/Archivos de programa/Orocos-KDL/bin")
SET(KDL_LIBRARIES "C:/Archivos de programa/Orocos-KDL/lib/orocos-kdl.lib")
IF ( KDL_LIBRARIES )
    MESSAGE(STATUS "-- Looking for Orocos-KDL libraries - found")
    #    SET(KDL_CFLAGS "${KDL_CFLAGS} -I\"${EIGEN2_INCLUDE_DIR}\"" CACHE INTERNAL "")
ELSE ( KDL_LIBRARIES )
    MESSAGE(FATAL_ERROR "-- Looking for Orocos-KDL libraries - not found")
ENDIF ( KDL_LIBRARIES )
ELSEIF(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
  ######################## LINUX ####################################3

#  set(ENV{PKG_CONFIG_PATH} "/usr/local/lib/pkgconfig/") #CHANGE THIS
  message( "Looking for KDL in: /usr/local")
  
#      message("   Includes in: ${Orocos-KDL_INCLUDE_DIRS}")
#      message(    "Compiler flags: ${Orocos-KDL_CFLAGS}")  
#      message("   Libraries: ${Orocos-KDL_LIBRARIES}")
#      message("   Libraries in: ${Orocos-KDL_LIBRARY_DIRS}")
#      message("   Linker flags : ${Orocos-KDL_LD_FLAGS}")
#      message("   Defines: ${Orocos-KDL_DEFINES}")

#  ELSE(Orocos-KDL_FOUND)  
#    IF(Orocos-KDL_FIND_REQUIRED)
#      MESSAGE(FATAL_ERROR "Could not find KDL")
#    ENDIF(Orocos-KDL_FIND_REQUIRED)
#  ENDIF ( Orocos-KDL_FOUND )
#ELSE  ( PKG_CONFIG_FOUND )

#  MESSAGE( FATAL_ERROR "Can't find KDL without pkgconfig !")

#ENDIF ( PKG_CONFIG_FOUND )

SET(KDL_LIBRARIES "/usr/local/lib/liborocos-kdl.so")
IF ( KDL_LIBRARIES )
    MESSAGE(STATUS "-- Looking for Orocos-KDL libraries - found")
    #    SET(KDL_CFLAGS "${KDL_CFLAGS} -I\"${EIGEN2_INCLUDE_DIR}\"" CACHE INTERNAL "")
ELSE ( KDL_LIBRARIES )
    MESSAGE(FATAL_ERROR "-- Looking for Orocos-KDL libraries - not found")
ENDIF ( KDL_LIBRARIES )

ENDIF(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
