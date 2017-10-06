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

#ifndef EX_1LAYER_H_
#define EX_1LAYER_H_

#include "parameters.h"

// Prototype of top level function for C-synthesis
void ex_1layer_hw(
      input_t data[N_INPUTS],
      output_t res[N_OUTPUTS],
      unsigned short &const_size_in,
      unsigned short &const_size_out);

input_t pop_stream(apin_t const &e);
apin_t push_stream(output_t const &v, bool last = false);
void ex_1layer(apin_t in_stream[N_INPUTS*MCR_SIZE],apin_t out_stream[N_OUTPUTS*MCR_SIZE], volatile ap_uint<1> *hw_trig);

#endif
