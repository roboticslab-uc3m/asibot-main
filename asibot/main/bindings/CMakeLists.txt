# Created by Juan G Victores 2013 @ UC3M
# Thanks to Paul Fitzpatrick for all the YARP and iCub SWIG stuff for inspiration

cmake_minimum_required(VERSION 2.6)

SET(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR} ${CMAKE_MODULE_PATH})

find_package(YARP REQUIRED)
include_directories(${YARP_INCLUDE_DIRS})
link_directories(${YARP_LIBRARY_DIRS})
link_libraries(${YARP_LIBRARIES})

find_package(ASIBOT REQUIRED)
include_directories(${ASIBOT_INCLUDE_DIRS})
link_directories(${ASIBOT_LINK_DIRS})
link_libraries(${ASIBOT_LIBRARIES})

# Work-around for missing paths to OpenCV libraries
find_package(OpenCV)
if (OpenCV_FOUND)
  link_directories(${OpenCV_LINK_DIRECTORIES} ${OPENCV_LINK_DIRECTORIES})
endif (OpenCV_FOUND)

#MESSAGE(${CMAKE_SOURCE_DIR})
include_directories(${CMAKE_SOURCE_DIR}/../src/libraries/ColorClientLib)

# for yarp.i
include_directories($ENV{YARP_ROOT}/bindings)

set(CREATE_PYTHON FALSE CACHE BOOL "Create Python interface")
set(CREATE_RUBY FALSE CACHE BOOL "Create Ruby interface")
set(CREATE_JAVA FALSE CACHE BOOL "Create Java interface")
SET(CREATE_CSHARP FALSE CACHE BOOL "Create CSharp interface")
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/lib)
make_directory(${LIBRARY_OUTPUT_PATH})

FIND_PACKAGE(SWIG REQUIRED)
INCLUDE(${SWIG_USE_FILE})
MESSAGE(STATUS "${SWIG_USE_FILE}")
SET (MIN_SWIG_VERSION "1.3.29")
IF ("${SWIG_VERSION}" VERSION_LESS "${MIN_SWIG_VERSION}")
  MESSAGE( FATAL_ERROR "Please update your SWIG. Current version: ${SWIG_VERSION}. The minimum version needed: ${MIN_SWIG_VERSION}")
ENDIF ("${SWIG_VERSION}" VERSION_LESS "${MIN_SWIG_VERSION}")

SET_SOURCE_FILES_PROPERTIES(asibot.i PROPERTIES CPLUSPLUS ON)

if (CREATE_PYTHON)
  set(CMAKE_SWIG_OUTDIR "${CMAKE_BINARY_DIR}/lib")
  SET(CMAKE_SWIG_FLAGS "-Wall;-module;asibot;-threads")
  SET(CREATE_PYTHON_VERSION "" CACHE STRING "Control python version used, if you wish." )
  FIND_PACKAGE(PythonLibs ${CREATE_PYTHON_VERSION})
  INCLUDE_DIRECTORIES(${PYTHON_INCLUDE_PATH})
  LINK_LIBRARIES(${PYTHON_LIBRARY})
  
  ## in Windows it seems necessary to declare explictly the link_directory
  if (WIN32)
    get_filename_component(PYTHON_DIR ${PYTHON_LIBRARY} PATH)
    link_directories(${PYTHON_DIR})
  endif()
  
  SWIG_ADD_MODULE(asibot python asibot.i)
  # uncomment if we want a debug version of the library
  #ADD_DEFINITIONS(-DPy_DEBUG)

  # installation path is determined reliably on most platforms using distutils
  FIND_PACKAGE(PythonInterp ${CREATE_PYTHON_VERSION})

  EXECUTE_PROCESS(COMMAND ${PYTHON_EXECUTABLE} 
    -c "from distutils import sysconfig; print(sysconfig.get_python_lib(1,0,prefix='${CMAKE_INSTALL_PREFIX}'))" 
    OUTPUT_VARIABLE PYTHON_INSTDIR OUTPUT_STRIP_TRAILING_WHITESPACE )
  INSTALL(FILES ${CMAKE_BINARY_DIR}/lib/asibot.py ${CMAKE_BINARY_DIR}/lib/_asibot.so 
    DESTINATION ${PYTHON_INSTDIR} )

  IF (WIN32)
    # Check if we have a target called _asibot as SWIG_ADD_MODULE
    # will currently give. If not, we're probably in the distant
    # future, and we'd best not meddle.
    GET_TARGET_PROPERTY(asibot_lib_location _asibot LOCATION)
    IF (asibot_lib_location)
      SET_TARGET_PROPERTIES(_asibot PROPERTIES SUFFIX ".pyd")
    ENDIF ()    
  ENDIF (WIN32)
endif (CREATE_PYTHON)

if (CREATE_RUBY)
  find_package(Ruby REQUIRED)
  set(CMAKE_SWIG_FLAGS "-Wall;-module;asibot")
  include_directories(${RUBY_INCLUDE_PATH})
  swig_add_module(asibot ruby asibot.i)
  swig_link_libraries(asibot ${RUBY_LIBRARY})
  set_target_properties(asibot PROPERTIES PREFIX "")
endif (CREATE_RUBY)

if (CREATE_JAVA)
  set(CMAKE_SWIG_OUTDIR "${CMAKE_CURRENT_BINARY_DIR}/asibot")
  set(CMAKE_SWIG_FLAGS "-package;asibot;-Wall;-module;asibot")
  find_package(JNI REQUIRED)
  include_directories(${JAVA_INCLUDE_PATH} ${JAVA_INCLUDE_PATH2})
  swig_add_module(asibot java asibot.i)
  #option (CREATE_JAVA_CLASS_FILES "Compile java code with javac" ON)
  #if (CREATE_JAVA_CLASS_FILES)
  #  ADD_CUSTOM_COMMAND(TARGET icub
  #    POST_BUILD
  #    COMMAND javac -source 1.3 -target 1.3 "asibot/*.java"
  #    WORKING_DIRECTORY ${CMAKE_BINARY_DIR})
  #endif ()
  if (APPLE)
    set_target_properties(asibot PROPERTIES SUFFIX ".jnilib")
  endif (APPLE)
endif (CREATE_JAVA)

IF (CREATE_CSHARP)
   SET(target_name asibot)
   SET(CMAKE_SWIG_FLAGS "-Wall;-module;asibot")
   SWIG_ADD_MODULE(asibot csharp asibot.i)
ENDIF (CREATE_CSHARP)

