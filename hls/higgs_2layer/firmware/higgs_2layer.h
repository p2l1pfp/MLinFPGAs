//
//    rfnoc-hls-neuralnet: Vivado HLS code for neural-net building blocks
//
//    Copyright (C) 2017 EJ Kreinar
//
//    This program is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with this program.  If not, see <http://www.gnu.org/licenses/>.
//

#ifndef HIGGS_2LAYER_H_
#define HIGGS_2LAYER_H_

#include <complex>
#include "ap_int.h"
#include "ap_fixed.h"
//#include "hls_stream.h"

#include "parameters.h"

/*
typedef ap_fixed<32,10> accum_t;
typedef ap_fixed<16,6> input_t;
typedef ap_fixed<16,8> result_t;
typedef ap_fixed<16,6> weight_t;
typedef ap_fixed<16,6> bias_t;
typedef ap_fixed<16,8> layer1_t;
typedef ap_fixed<16,8> layer2_t;
*/

typedef ap_fixed<32,10> accum_t;
typedef ap_fixed<32,12> input_t;
typedef ap_fixed<32,12> result_t;
typedef ap_fixed<32,12> weight_t;
typedef ap_fixed<32,12> bias_t;
typedef ap_fixed<32,12> layer1_t;
typedef ap_fixed<32,12> layer2_t;


// Prototype of top level function for C-synthesis
void higgs_2layer(
      input_t data[N_INPUTS],
      result_t res[N_OUTPUTS],
      unsigned short &const_size_in,
      unsigned short &const_size_out);

#endif
