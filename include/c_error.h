/*
 * mx_error.h
 *
 *  Created on: Oct 3, 2017
 *      Author: andy
 */

#ifndef SRC_MX_ERROR_H_
#define SRC_MX_ERROR_H_

#include "c_port.h"

#include <exception>

struct CError {
    HRESULT err;
    const char* msg;
    int lineno;
    const char* fname;
    const char* func;
};

enum CError_Options {
    // also set the python error when setting the Carbon error.
    CERROR_SET_PYTHON_ERROR = 1 << 0
};

#define mx_error(code, msg) CErr_Set(code, msg, __LINE__, __FILE__, MX_FUNCTION)

#define c_error(code, msg) CErr_Set(code, msg, __LINE__, __FILE__, MX_FUNCTION)

#define C_ERR(code, msg) CErr_Set(code, msg, __LINE__, __FILE__, MX_FUNCTION)

#define c_exp(e) CExp_Set(e, "", __LINE__, __FILE__, MX_FUNCTION)

#define c_exp_msg(e, msg) CExp_Set(e, msg, __LINE__, __FILE__, MX_FUNCTION)

#define C_RETURN_EXP(e) CExp_Set(e, "", __LINE__, __FILE__, MX_FUNCTION); return NULL

#define C_EXP(e) CExp_Set(e, "", __LINE__, __FILE__, MX_FUNCTION)

CAPI_FUNC(HRESULT) CErr_Set(HRESULT code, const char* msg, int line, const char* file, const char* func);

CAPI_FUNC(HRESULT) CExp_Set(const std::exception&, const char* msg, int line, const char* file, const char* func);

CAPI_FUNC(CError*) CErr_Occurred();


CAPI_FUNC(void) CErr_SetOptions(uint32_t options);


/**
 * Clear the error indicator. If the error indicator is not set, there is no effect.
 */
CAPI_FUNC(void) CErr_Clear();




#endif /* SRC_MX_ERROR_H_ */
