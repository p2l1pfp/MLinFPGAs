#ifndef H_LIB_EXAMPLE_HW_H
#define H_LIB_EXAMPLE_HW_H

#define N_INPUTS    10
#define N_OUTPUTS   1

int Start_HW_Accelerator(void);
int Setup_HW_Accelerator(unsigned int INPUT[N_INPUTS],unsigned int OUTPUT[N_OUTPUTS], int dma_size_in,int dma_size_out);
int Run_HW_Accelerator(unsigned int  input[N_INPUTS], unsigned int output[N_OUTPUTS], int dma_size_in,int dma_size_out);
void dnn_ref(unsigned int input[N_INPUTS], unsigned int output[N_OUTPUTS], float weights[N_INPUTS][N_OUTPUTS],float bias[N_INPUTS]);

#endif
