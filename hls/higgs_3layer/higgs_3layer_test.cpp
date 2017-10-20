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
#include <fstream>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "firmware/parameters.h"
#include "firmware/higgs_3layer.h"
#include "nnet_helpers.h"


int main(int argc, char **argv)
{
  
  //input_t  data_str[N_INPUTS] = {0};
  input_t  data_str[N_INPUTS] = {0.44044939, -0.08604704,  0.28499559,  0.1151934,   0.45350668, -0.46141511,
				 -0.45673928, -0.37983865, -0.19626978, -0.24211307,  0.92032003,  0.00543885,
				 -0.3272942,   0.22578914, -0.2764751,  -0.30281451, -0.16354406, -0.45568964,
				 -0.2901558,  -0.10491186,  0.32512018, -0.20537674, -0.1578009,   0.16481732,
				 0.02426484,  0.16045029,  0.08195856};
  result_t res_str[N_OUTPUTS] = {0};
  unsigned short size_in, size_out;
  higgs_3layer(data_str, res_str, size_in, size_out);
  std::cout << "res_str: " << res_str[0] << " " << res_str[1] << std::endl;
  std::cout << "keras: 0.9770025   0.02299754" << std::endl;
  
  return 0;
}
