#ifndef PARAMETERS_H_
#define PARAMETERS_H_

#include "ap_int.h"
#include "ap_axi_sdata.h"
#include <complex>
#include "ap_int.h"
#include "ap_fixed.h"
#include "hls_stream.h"


typedef ap_fixed<32,10> accum_t;
typedef ap_fixed<13,4> coeff_t;
typedef ap_fixed<16,4> bias_t;
typedef ap_fixed<16,6> result_t;
//typedef float result_t;

#define N_INPUTS  10
#define N_OUTPUTS 1
#define MCR_SIZE  1024

typedef ap_fixed<32,6> input_t ;
typedef ap_fixed<32,6> output_t;
typedef unsigned int input_o;
typedef unsigned int output_o;
//typedef float input_t;
//typedef float output_t;
typedef ap_axiu<32,4,5,5> apin_t; //The 32,4,5,5 is added to get the linking right
//typedef ap_axiu<16,1,1,1> apin_t; 
#endif 
