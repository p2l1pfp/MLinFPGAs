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
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "firmware/parameters.h"
#include "firmware/ex_1layer.h"
#include "nnet_helpers.h"

int main(int argc, char **argv) {

  // define inputs/outputs
  input_t  data_i[N_INPUTS];
  output_t data_o[N_OUTPUTS];
  
  //Now define the streams
  apin_t sdata_i[N_INPUTS];
  apin_t sdata_o[N_OUTPUTS];
  ap_uint<1> hw_trig;

  // fill with random numbers at some point...
  for (int i = 0; i < N_INPUTS; ++i)  data_i[i] = 0;
  for (int i = 0; i < N_OUTPUTS; ++i) data_o[i] = 0;

  //Axi-stream it up
  for (int i = 0; i < N_INPUTS;  ++i)  sdata_i[i] = push_stream(data_i[i],i==(N_INPUTS-1));
  for (int i = 0; i < N_OUTPUTS; ++i) sdata_o[i] = push_stream(data_o[i],i==(N_OUTPUTS-1));

  // top function!
  ex_1layer(sdata_i, sdata_o, &hw_trig);
  // do some validation down here
  // like compare results to full floating point
}

