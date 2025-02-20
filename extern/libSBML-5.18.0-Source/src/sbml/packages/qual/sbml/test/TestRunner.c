/**
 * @file    TestRunner.c
 * @brief   Runs all unit tests in the extension module in the group package
 * @author  Akiya Jouraku
 *
 * $Id: $
 * $HeadURL: $
 *
 * <!--------------------------------------------------------------------------
 * This file is part of libSBML.  Please visit http://sbml.org for more
 * information about SBML, and the latest version of libSBML.
 *
 * Copyright (C) 2019 jointly by the following organizations:
 *     1. California Institute of Technology, Pasadena, CA, USA
 *     2. University of Heidelberg, Heidelberg, Germany
 *
 * Copyright (C) 2013-2018 jointly by the following organizations:
 *     1. California Institute of Technology, Pasadena, CA, USA
 *     2. EMBL European Bioinformatics Institute (EMBL-EBI), Hinxton, UK
 *     3. University of Heidelberg, Heidelberg, Germany
 * 
 * Copyright (C) 2009-2011 jointly by the following organizations: 
 *     1. California Institute of Technology, Pasadena, CA, USA
 *     2. EMBL European Bioinformatics Institute (EMBL-EBI), Hinxton, UK
 *  
 * Copyright (C) 2006-2008 by the California Institute of Technology,
 *     Pasadena, CA, USA 
 *  
 * Copyright (C) 2002-2005 jointly by the following organizations: 
 *     1. California Institute of Technology, Pasadena, CA, USA
 *     2. Japan Science and Technology Agency, Japan
 * 
 * This library is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation.  A copy of the license agreement is provided
 * in the file named "LICENSE.txt" included with this software distribution
 * and also available online as http://sbml.org/software/libsbml/license.html
 * ---------------------------------------------------------------------- -->*/


#include <stdlib.h>
#include <string.h>

#include <sbml/common/extern.h>
#include <sbml/util/memory.h>

#include <check.h>

#ifdef LIBSBML_USE_VLD
  #include <vld.h>
#endif

LIBSBML_CPP_NAMESPACE_USE

BEGIN_C_DECLS

Suite *create_suite_Input (void);
Suite *create_suite_Output (void);
Suite *create_suite_FunctionTerm (void);
Suite *create_suite_Transition (void);
Suite *create_suite_QualitativeSpecies (void);
Suite *create_suite_DefaultTerm (void);
Suite *create_suite_ListOfFunctionTerms (void);
Suite *create_suite_CopyAndClone (void);

END_C_DECLS

/**
 * Global.
 *
 * Declared extern in TestAnnotation suite.
 */
char *TestDataDirectory;

void
setTestDataDirectory (void)
{
  char *srcdir = getenv("srcdir");
  int  length  = (srcdir == NULL) ? 0 : (int)strlen(srcdir);


  /**
   * strlen("/test-data/") = 11 + 1 (for NULL) = 12
   */
  TestDataDirectory = (char *) safe_calloc( length + 12, sizeof(char) );

  if (srcdir != NULL)
  {
    strcpy(TestDataDirectory, srcdir);
    strcat(TestDataDirectory, "/");
  }

  strcat(TestDataDirectory, "test-data/");
}


int
main (int argc, char* argv[]) 
{ 
  int num_failed = 0;

  setTestDataDirectory();

  SRunner *runner = srunner_create(create_suite_Input());
  srunner_add_suite(runner, create_suite_Output());
  srunner_add_suite(runner, create_suite_FunctionTerm());
  srunner_add_suite(runner, create_suite_Transition());
  srunner_add_suite(runner, create_suite_QualitativeSpecies());
  srunner_add_suite(runner, create_suite_DefaultTerm());
  srunner_add_suite(runner, create_suite_ListOfFunctionTerms());
  srunner_add_suite(runner, create_suite_CopyAndClone());

  if (argc > 1 && !strcmp(argv[1], "-nofork"))
  {
    srunner_set_fork_status( runner, CK_NOFORK );
  }

  srunner_run_all(runner, CK_NORMAL);
  num_failed = srunner_ntests_failed(runner);

  srunner_free(runner);
  safe_free(TestDataDirectory);

  return num_failed;
}
