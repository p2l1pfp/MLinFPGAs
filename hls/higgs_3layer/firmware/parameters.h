#ifndef PARAMETERS_H_
#define PARAMETERS_H_

#include <complex>
#include "ap_int.h"
#include "ap_fixed.h"

#define N_INPUTS      27
#define N_LAYER_1     64
#define N_LAYER_2     32
#define N_LAYER_3     32
#define N_OUTPUTS      2

/*
typedef ap_fixed<32,10> accum_t;
typedef ap_fixed<16,6> weight_t;
typedef ap_fixed<16,6> bias_t;
typedef ap_fixed<16,6> input_t;
typedef ap_fixed<16,6> layer1_t;
typedef ap_fixed<16,6> layer2_t;
typedef ap_fixed<16,6> layer3_t;
typedef ap_fixed<16,6> result_t;
*/

typedef ap_fixed<32,10> accum_t;
typedef ap_fixed<32,8> weight_t;
typedef ap_fixed<32,8> bias_t;
typedef ap_fixed<32,8> input_t;
typedef ap_fixed<32,8> layer1_t;
typedef ap_fixed<32,8> layer2_t;
typedef ap_fixed<32,8> layer3_t;
typedef ap_fixed<32,8> result_t;

#endif 
