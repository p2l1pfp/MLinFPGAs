#ifndef PARAMETERS_H_
#define PARAMETERS_H_

#include "ap_int.h"

#include <complex>
#include "ap_int.h"
#include "ap_fixed.h"
#include "hls_stream.h"

typedef ap_fixed<32,10> accum_t;
typedef ap_fixed<16,6> input_t;
typedef ap_fixed<13,4> coeff_t;
typedef ap_fixed<16,4> bias_t;
typedef ap_fixed<16,8> result_t;

#define N_INPUTS 10
#define N_OUTPUTS 1
typedef ap_fixed<16,6> input_t ;
typedef ap_fixed<16,8> output_t;

#endif 