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

#include "parameters.h"
#include "higgs_2layer.h"

#include "nnet_layer.h"
#include "nnet_activation.h"

#include "weights/layer1_weights_10x32.h"
#include "weights/layer1_biases_32x1.h"
#include "weights/layer2_weights_32x1.h"
#include "weights/layer2_biases_1x1.h"

// AXI-Stream port type is compatible with pointer, reference, & array input / ouputs only
// See UG902 Vivado High Level Synthesis guide (2014.4) pg 157 Figure 1-49
void higgs_2layer(
		  input_t data[N_INPUTS],
		  result_t res[N_OUTPUTS],
		  unsigned short &const_size_in,
		  unsigned short &const_size_out)
{
    #pragma HLS ARRAY_PARTITION variable=data complete
    #pragma HLS ARRAY_PARTITION variable=res complete

    #pragma HLS pipeline II=1 

    //Remove ap ctrl ports (ap_start, ap_ready, ap_idle, etc) since we only use the AXI-Stream ports
    //#pragma HLS INTERFACE ap_ctrl_none port=return

    #pragma HLS DATAFLOW

    //Connect size indicators
    //#pragma HLS INTERFACE ap_none port=const_size_in
    //#pragma HLS INTERFACE ap_none port=const_size_out
    const_size_in   = N_INPUTS;
    const_size_out  = N_OUTPUTS;

    // ****************************************
    // NETWORK INSTATIATION
    // ****************************************

    // LAYER 1
    layer1_t logits1[N_LAYER_1];// = {0}; 
    layer1_t layer1[N_LAYER_1];// = {0};
    #pragma HLS ARRAY_PARTITION variable=logits1 complete
    #pragma HLS ARRAY_PARTITION variable=layer1 complete
    
    nnet::compute_layer<input_t, layer1_t, weight_t, bias_t, accum_t, N_INPUTS, N_LAYER_1>(data, logits1, weights1, biases1);
    nnet::relu<layer1_t, layer1_t, N_LAYER_1>(logits1, layer1);

    // LAYER 2
    nnet::compute_layer<layer1_t, layer2_t, weight_t, bias_t, accum_t, N_LAYER_1, N_OUTPUTS>(layer1, res, weights2, biases2);
}
