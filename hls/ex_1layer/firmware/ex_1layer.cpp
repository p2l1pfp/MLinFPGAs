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
#include "ex_1layer.h"
#include "nnet_layer.h"

#include "weights/weights_1.h"
#include "weights/biases_1.h"

// now doing all parallel inputs
void ex_1layer(
      input_t data[N_INPUTS],
      output_t res[N_OUTPUTS],
      unsigned short &const_size_in,
      unsigned short &const_size_out)
{
    // Remove ap ctrl ports (ap_start, ap_ready, ap_idle, etc) since we only use the AXI-Stream ports
    // #pragma HLS INTERFACE ap_ctrl_none port=return

    // Connect size indicators
    #pragma HLS INTERFACE ap_none port=const_size_in
    #pragma HLS INTERFACE ap_none port=const_size_out
    const_size_in   = N_INPUTS;
    const_size_out  = N_OUTPUTS;

    printf("hi! \n");

    // ****************************************
    // NETWORK INSTATIATION
    // ****************************************

    // One fully connected layer
    nnet::compute_layer<input_t, output_t, coeff_t, bias_t, accum_t, N_INPUTS, N_OUTPUTS>(data, res, weights, biases);
}
