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
#include <stdio.h>
#include <stdlib.h>
#include "parameters.h"
#include "ex_1layer.h"
#include "nnet_layer.h"

#include "weights/weights_1.h"
#include "weights/biases_1.h"

// now doing all parallel inputs
void ex_1layer_hw(
      input_t data[N_INPUTS],
      output_t res[N_OUTPUTS],
      unsigned short &const_size_in,
      unsigned short &const_size_out)
{

    #pragma HLS ARRAY_PARTITION variable=data complete
    #pragma HLS ARRAY_PARTITION variable=res complete
    #pragma HLS pipeline II=1
    // Remove ap ctrl ports (ap_start, ap_ready, ap_idle, etc) since we only use the AXI-Stream ports
    // #pragma HLS INTERFACE ap_ctrl_none port=return

    // Connect size indicators
    #pragma HLS INTERFACE ap_none port=const_size_in
    #pragma HLS INTERFACE ap_none port=const_size_out
    const_size_in   = N_INPUTS;
    const_size_out  = N_OUTPUTS;

    //printf("hi! \n");

    // ****************************************
    // NETWORK INSTATIATION
    // ****************************************

    // One fully connected layer
    nnet::compute_layer<input_t, output_t, coeff_t, bias_t, accum_t, N_INPUTS, N_OUTPUTS>(data, res, weights, biases);
    return;
}
input_t pop_stream(apin_t const &e) {
#pragma HLS INLINE off
  input_t ret    = input_t(e.data);
  volatile ap_uint<4>  strb = e.strb;
  volatile ap_uint<4>  keep = e.keep;
  volatile ap_uint<4>  user = e.user;
  volatile ap_uint<1>  last = e.last;
  volatile ap_uint<5>  id   = e.id;
  volatile ap_uint<5>  dest = e.dest;
  return ret;
}
apin_t push_stream(output_t const &v, bool last) {
#pragma HLS INLINE off
  apin_t e;
  e.data = v;
  e.strb = (1<<2)-1;
  e.keep = (1<<2)-1;
  e.user = 0;
  e.last = last ? 1 : 0;
  e.id   = 0;
  e.dest = 0;
  return e;
}
void ex_1layer(apin_t in_stream[N_INPUTS*MCR_SIZE],apin_t out_stream[N_OUTPUTS*MCR_SIZE], volatile ap_uint<1> *hw_trig) {
  // Map ports to Vivado HLS interfaces                                                                                                                
#pragma HLS INTERFACE s_axilite        port=return bundle=CONTROL_BUS
#pragma HLS INTERFACE axis             port=in_stream
#pragma HLS INTERFACE axis             port=out_stream
#pragma HLS INTERFACE ap_none register port=hw_trig

  //Setup Trigger bit
  ap_uint<1> logic_zero = 0;
  ap_uint<1> logic_one = 1;
  *hw_trig = logic_one;

  //Readers to put to the hardware
  input_t  data  [N_INPUTS];
  output_t output[N_OUTPUTS];
  unsigned short size_in, size_out;

  // stream in first matrix                                                                                                                            
  for(int i=0; i<N_INPUTS; i++) {
#pragma HLS PIPELINE II=1
    data[i] = pop_stream(in_stream[i]);
  }
  // do multiplication                                                                                                                 
  ex_1layer_hw(data,output,size_in,size_out);

 for(int i=0; i<N_OUTPUTS; i++) {
#pragma HLS PIPELINE II=1
   out_stream[i] = push_stream(output,i==(N_OUTPUTS-1));
  }
 *hw_trig = logic_zero; 
 return;
}
 
