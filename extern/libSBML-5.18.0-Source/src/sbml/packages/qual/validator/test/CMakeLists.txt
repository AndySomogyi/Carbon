## @file    CMakeLists.txt
## @brief   CMake build script
## @author  Frank T. Bergmann
##
## <!--------------------------------------------------------------------------
## This file is part of libSBML.  Please visit http://sbml.org for more
## information about SBML, and the latest version of libSBML.
##
## Copyright (C) 2013-2018 jointly by the following organizations:
##     1. California Institute of Technology, Pasadena, CA, USA
##     2. EMBL European Bioinformatics Institute (EMBL-EBI), Hinxton, UK
##     3. University of Heidelberg, Heidelberg, Germany
## 
## Copyright (C) 2009-2013 jointly by the following organizations: 
##     1. California Institute of Technology, Pasadena, CA, USA
##     2. EMBL European Bioinformatics Institute (EMBL-EBI), Hinxton, UK
##  
## Copyright (C) 2006-2008 by the California Institute of Technology,
##     Pasadena, CA, USA 
##  
## Copyright (C) 2002-2005 jointly by the following organizations: 
##     1. California Institute of Technology, Pasadena, CA, USA
##     2. Japan Science and Technology Agency, Japan
## 
## This library is free software; you can redistribute it and/or modify it
## under the terms of the GNU Lesser General Public License as published by
## the Free Software Foundation.  A copy of the license agreement is provided
## in the file named "LICENSE.txt" included with this software distribution
## and also available online as http://sbml.org/software/libsbml/license.html
## ------------------------------------------------------------------------ -->

file(GLOB C_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.c)

foreach(c_file ${C_FILES})
	set_source_files_properties(${c_file} PROPERTIES LANGUAGE CXX)
endforeach()

file(GLOB CPP_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp )
file(GLOB H_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.h )

# figure out whether we are doing an individual build, or whether 
# this package has already been integrated
set (TEST_LIB)
if (EXISTS "${LIBSBML_ROOT_SOURCE_DIR}/VERSION.txt")
  # integrated
  set (TEST_LIB ${LIBSBML_LIBRARY}-static)
else()
  # individual test build
  set (TEST_LIB qual)
endif()

set(TEST_LIBSBML_LIBS ${TEST_LIB} ${LIBCHECK_LIBRARY})

set(TEST_FILES ${CPP_FILES} ${C_FILES} ${H_FILES})


include_directories(BEFORE ${LIBSBML_ROOT_SOURCE_DIR}/src)
include_directories(BEFORE ${LIBSBML_ROOT_BINARY_DIR}/src)

include_directories(${LIBCHECK_INCLUDE_DIR})
add_executable(test_qual_validator ${TEST_FILES})
target_link_libraries(test_qual_validator ${TEST_LIBSBML_LIBS})
add_test(test_qual_validator_run ${CMAKE_CURRENT_BINARY_DIR}/test_qual_validator ${CMAKE_CURRENT_SOURCE_DIR})
set_tests_properties(test_qual_validator_run PROPERTIES ENVIRONMENT 
	"srcdir=${CMAKE_CURRENT_SOURCE_DIR}")


