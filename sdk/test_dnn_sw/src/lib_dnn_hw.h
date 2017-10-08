#ifndef H_LIB_EXAMPLE_HW_H
#define H_LIB_EXAMPLE_HW_H

#define N_INPUTS    10
#define N_OUTPUTS   1

int Start_HW_Accelerator(void);
int Setup_HW_Accelerator(float INPUT[N_INPUTS],float OUTPUT[N_OUTPUTS], int dma_size_in,int dma_size_out);
int Run_HW_Accelerator(float  input[N_INPUTS], float output[N_OUTPUTS], int dma_size_in,int dma_size_out);
void dnn_ref(float input[N_INPUTS], float output[N_OUTPUTS], float weights[N_INPUTS][N_OUTPUTS],float bias[N_INPUTS]);

#endif
