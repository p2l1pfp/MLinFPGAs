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
#include "higgs_3layer.h"

#include "nnet_layer.h"
#include "nnet_activation.h"

#include "weights/w1.h"
#include "weights/b1.h"
#include "weights/w2.h"
#include "weights/b2.h"
#include "weights/w3.h"
#include "weights/b3.h"
#include "weights/w4.h"
#include "weights/b4.h"

// AXI-Stream port type is compatible with pointer, reference, & array input / ouputs only
// See UG902 Vivado High Level Synthesis guide (2014.4) pg 157 Figure 1-49
void higgs_3layer(
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

    //This is preventing pipelining. Comment out.
    //#pragma HLS DATAFLOW

    //Connect size indicators
    //#pragma HLS INTERFACE ap_none port=const_size_in
    //#pragma HLS INTERFACE ap_none port=const_size_out
    const_size_in   = N_INPUTS;
    const_size_out  = N_OUTPUTS;

    // ****************************************
    // NETWORK INSTATIATION
    // ****************************************

    // IN->LAYER 1
    layer1_t logits1[N_LAYER_1];
    layer1_t  layer1_out[N_LAYER_1];
    #pragma HLS ARRAY_PARTITION variable=logits1 complete
    #pragma HLS ARRAY_PARTITION variable=layer1_out complete
    
    nnet::compute_layer<input_t, layer1_t, weight_t, bias_t, accum_t, N_INPUTS, N_LAYER_1>(data, logits1, w1, b1);
    nnet::relu<layer1_t, layer1_t, N_LAYER_1>(logits1, layer1_out);

    // LAYER 1 -> LAYER 2
    layer2_t logits2[N_LAYER_2];
    layer2_t  layer2_out[N_LAYER_2];
    #pragma HLS ARRAY_PARTITION variable=logits2 complete
    #pragma HLS ARRAY_PARTITION variable=layer2_out  complete

    nnet::compute_layer<layer1_t, layer2_t, weight_t, bias_t, accum_t, N_LAYER_1, N_LAYER_2>(layer1_out, logits2, w2, b2);
    nnet::sigmoid<layer2_t, layer2_t, N_LAYER_2, 1024>(logits2,layer2_out);

    // LAYER 2 -> LAYER 3
    layer3_t logits3[N_LAYER_3];
    layer3_t  layer3_out[N_LAYER_3];
    #pragma HLS ARRAY_PARTITION variable=logits3 complete
    #pragma HLS ARRAY_PARTITION variable=layer3_out  complete
    nnet::compute_layer<layer2_t, layer3_t, weight_t, bias_t, accum_t, N_LAYER_2, N_LAYER_3>(layer2_out, logits3, w3, b3);
    nnet::sigmoid<layer3_t, layer3_t, N_LAYER_3, 1024>(logits3,layer3_out);


    // LAYER 3 -> LAYER 4 (softmax, out)
    result_t logits4[N_OUTPUTS];
    #pragma HLS ARRAY_PARTITION variable=logits4 complete
    nnet::compute_layer<layer3_t, result_t, weight_t, bias_t, accum_t, N_LAYER_3, N_OUTPUTS>(layer3_out, logits4, w4, b4);
    nnet::softmax<result_t, result_t, N_OUTPUTS, 1024>(logits4,res);

}
