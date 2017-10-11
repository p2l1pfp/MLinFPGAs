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
#include <math.h>

#include "firmware/parameters.h"
#include "firmware/higgs_2layer.h"
#include "nnet_helpers.h"


int main(int argc, char **argv)
{
  
  input_t  data_str[N_INPUTS] = {0.05216677, 1.78849339, 0.97410184, 0.72014093, 1.34292293, 1.75948036, 1.72571194, 2.03052092, 0.100394, 0.10963588};
  
  // Run the basic neural net block
  result_t res_str[N_OUTPUTS] = {0};
  unsigned short size_in, size_out;
  higgs_2layer(data_str, res_str, size_in, size_out);

  std::cout << "Found network size: " << size_in << "x" << size_out << std::endl;

  std::cout << "Result from FPGA implementation: " << res_str[0] << std::endl;
  std::cout << "Expected from keras training:  0.13149975" << std::endl;

  return 0;
}
