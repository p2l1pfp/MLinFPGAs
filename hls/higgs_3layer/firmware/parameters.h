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

typedef ap_fixed<32,10> accum_t;
typedef ap_fixed<32,12> weight_t;
typedef ap_fixed<32,12> bias_t;
typedef ap_fixed<32,12> input_t;
typedef ap_fixed<32,12> layer1_t;
typedef ap_fixed<32,12> layer2_t;
typedef ap_fixed<32,12> layer3_t;
typedef ap_fixed<32,12> result_t;

#endif 
