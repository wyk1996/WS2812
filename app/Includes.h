#ifndef _INCLUDES_H_
#define _INCLUDES_H_


#include <stdio.h>
#include <string.h>
#include <math.h>
#include "gd32e230c_eval.h"
#include "gd32e23x_libopt.h"
#include "systick.h"
#include "gd32e23x.h"

#include "customer_config.h"
#include "customer_device_type.h"




//Data variable type header file
#include "IndependentDataType.h"

//Driver header file
#include "drv_common.h"

//Logical driver layer header file
#include "hw_api_usart.h"


#define EPSILON		0.000001f
#define FLOAT_EQUAL(a, b) (fabs((a) - (b)) < EPSILON)
#define FLOAT_GREATER(a, b) (((a) - (b)) > EPSILON)
#define FLOAT_LESS(a, b) (((b) - (a)) > EPSILON)
#define FLOAT_GREATER_EQUAL(a, b) (FLOAT_GREATER(a, b) || FLOAT_EQUAL(a, b))
#define FLOAT_LESS_EQUAL(a, b) (FLOAT_LESS(a, b) || FLOAT_EQUAL(a, b))

#if LOG_LEVEL <= LOG_LEVEL_DEBUG
#define log_d(format, ...) printf( format, ##__VA_ARGS__)
#else
#define log_d(format, ...)
#endif

#if 0
#if LOG_LEVEL <= LOG_LEVEL_INFO
#define log_i(format, ...) printf("[INFO] " format, ##__VA_ARGS__)
#else
#define log_i(format, ...)
#endif

#if LOG_LEVEL <= LOG_LEVEL_WARNING
#define log_w(format, ...) printf("[WARNING] " format, ##__VA_ARGS__)
#else
#define log_w(format, ...)
#endif

#if LOG_LEVEL <= LOG_LEVEL_ERROR
#define log_e(format, ...) printf("[ERROR] " format, ##__VA_ARGS__)
#else
#define log_e(format, ...)
#endif
#endif




#endif


