/*
 * carbon.h
 *
 *  Created on: Jun 30, 2015
 *      Author: andy
 */

#ifndef _INCLUDED_CARBON_H_
#define _INCLUDED_CARBON_H_


#include <c_port.h>
#include <c_object.h>
#include <c_type.h>
#include <c_descrobject.h>
#include <c_listwrap.h>
#include <c_error.h>
#include <c_compartment.h>
#include <c_kineticlaw.h>
#include <c_reaction.h>
#include <c_species.h>
#include <c_speciesreference.h>
#include <c_event.h>
#include <c_util.h>

#ifdef __cplusplus
#include <c_object.hpp>
#include <CLogger.hpp>
#endif

/**
 * Initialize the entire runtime.
 *
 * Callers of the Carbon lib need to initialize it, this creates the Python
 * module and also initializes Python if not already initialized.
 */
CAPI_FUNC(HRESULT) C_Initialize(int);


/**
 * Shutdown and cleanup.
 */
CAPI_FUNC(HRESULT) C_Finalize();

/**
 * The carbon module, initialized with C_Initialize;
 */
CAPI_DATA(PyObject*) Carbon_ModulePtr;

/**
 * Called by Python runtime to create and import the module
 */
PyMODINIT_FUNC PyInit_carbon(void);


#endif /* _INCLUDED_CARBON_H_ */
