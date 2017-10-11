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
#include "firmware/higgs_2layer.h"
#include "nnet_helpers.h"


int main(int argc, char **argv)
{
  
  input_t  data_str[N_INPUTS] = {0};
  //input_t  data_str[N_INPUTS] = {0.05216677, 1.78849339, 0.97410184, 0.72014093, 1.34292293, 1.75948036, 1.72571194, 2.03052092, 0.100394, 0.10963588};
  
  int max_nsamples = -1; //-1 for all
  
  int nsamples = 0;
  std::ifstream read_input_file("tb_data/in.dat", std::ifstream::in);
  std::cout << "read in: " << read_input_file.is_open() << std::endl;
  while(read_input_file>>data_str[0]>>data_str[1]>>data_str[2]>>data_str[3]>>data_str[4]>>
	data_str[5]>>data_str[6]>>data_str[7]>>data_str[8]>>data_str[9]){
    nsamples++;

    result_t res_str[N_OUTPUTS] = {0};
    unsigned short size_in, size_out;
    higgs_2layer(data_str, res_str, size_in, size_out);

    std::cout << "Sample " << nsamples << " prediction: ";
    for(int i=0; i<N_OUTPUTS; i++){
      std::cout << res_str[i] << " ";
    }
    std::cout << std::endl;

    if(nsamples==0) std::cout << "Found network size: " << size_in << "x" << size_out << std::endl;
    if(nsamples == max_nsamples) break;
  }

  //std::cout << "Result from FPGA implementation: " << res_str[0] << std::endl;
  //std::cout << "Expected from keras training:  0.13149975" << std::endl;

  return 0;
}
