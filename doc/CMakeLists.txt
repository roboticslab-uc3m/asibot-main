# Generated Juan G Victores 2013
# Thanks stefan majewsky for http://majewsky.wordpress.com/2010/08/14/tip-of-the-day-cmake-and-doxygen/

cmake_minimum_required(VERSION 2.6)

IF (GENERATE_doc_html)

find_package(Doxygen)
if(DOXYGEN_FOUND)
  configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile @ONLY)
  add_custom_target(doc ALL
    ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    # WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/../
    COMMENT "Generating API documentation with Doxygen" VERBATIM
  )
endif(DOXYGEN_FOUND)

ENDIF (GENERATE_doc_html)

