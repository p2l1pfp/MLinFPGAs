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
#include "ap_axi_sdata.h"
//#include "hls_stream.h"

#include "parameters.h"

typedef ap_fixed<32,10> accum_t;
typedef ap_fixed<16,6> input_t;
typedef ap_fixed<16,8> result_t;
typedef ap_fixed<16,6> weight_t;
typedef ap_fixed<16,6> bias_t;
typedef ap_fixed<16,8> layer1_t;
typedef ap_fixed<16,8> layer2_t;
typedef unsigned int input_o;
typedef unsigned int output_o;
typedef ap_axiu<32,4,5,5> apin_t; //The 32,4,5,5 is added to get the linking right

// Prototype of top level function for C-synthesis
void higgs_2layer_hw(
      input_t data[N_INPUTS],
      result_t res[N_OUTPUTS],
      unsigned short &const_size_in,
      unsigned short &const_size_out);

result_t pop_stream(apin_t const &e);
apin_t  push_stream(result_t const &v, bool last = false);
void    higgs_2layer(apin_t in_stream[N_INPUTS*MCR_SIZE],apin_t out_stream[N_OUTPUTS*MCR_SIZE]);

#endif
