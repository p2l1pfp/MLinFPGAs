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
  /*
  input_t  data_str[N_INPUTS] = {0.44044939, -0.08604704,  0.28499559,  0.1151934,   0.45350668, -0.46141511,
				 -0.45673928, -0.37983865, -0.19626978, -0.24211307,  0.92032003,  0.00543885,
				 -0.3272942,   0.22578914, -0.2764751,  -0.30281451, -0.16354406, -0.45568964,
				 -0.2901558,  -0.10491186,  0.32512018, -0.20537674, -0.1578009,   0.16481732,
				 0.02426484,  0.16045029,  0.08195856};
  std::cout << "res_str: " << res_str[0] << " " << res_str[1] << std::endl;
  std::cout << "keras: 0.9770025   0.02299754" << std::endl;
  */

  
  input_t  data_str[N_INPUTS] = {0};
  int max_nsamples = -1; //-1 for all

  std::ofstream out_file;
  out_file.open("tb_data/res.dat");
  
  int nsamples = 0;
  std::ifstream read_input_file("tb_data/in.dat", std::ifstream::in);
  std::cout << "read in: " << read_input_file.is_open() << std::endl;
  while(read_input_file>>data_str[0]>>data_str[1]>>data_str[2]>>data_str[3]>>data_str[4]>>
	data_str[5]>>data_str[6]>>data_str[7]>>data_str[8]>>data_str[9]>>data_str[10]>>
	data_str[11]>>data_str[12]>>data_str[13]>>data_str[14]>>data_str[15]>>data_str[16]>>
	data_str[17]>>data_str[18]>>data_str[19]>>data_str[20]>>data_str[21]>>data_str[22]>>
	data_str[23]>>data_str[24]>>data_str[25]>>data_str[26]){
    
    nsamples++;
    result_t res_str[N_OUTPUTS] = {0};
    unsigned short size_in, size_out;
    higgs_3layer(data_str, res_str, size_in, size_out);
    
    std::cout << "Sample " << nsamples << " prediction: ";
    for(int i=0; i<N_OUTPUTS; i++){
      std::cout << res_str[i] << " ";
      out_file << res_str[i] << " ";
    }
    std::cout << std::endl;
    out_file << std::endl;
    
    if(nsamples==0) std::cout << "Found network size: " << size_in << "x" << size_out << std::endl;
    if(nsamples == max_nsamples) break;
  }
  read_input_file.close();
  out_file.close();

  return 0;
}
