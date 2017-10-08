#include "platform.h"
#include "xparameters.h"
#include "xscugic.h"
#include "xaxidma.h"
#include "xex_1layer.h"
#include "lib_dnn_hw.h"
#include "xil_printf.h"

volatile static int RunExample = 0;
volatile static int ResultExample = 0;

XEx_1layer xex_1layer_dev;

XEx_1layer_Config xex_1layer_config = {
	0,
	XPAR_EX_1LAYER_S_AXI_CONTROL_BUS_BASEADDR
};

//Interrupt Controller Instance
XScuGic ScuGic;

// AXI DMA Instance
extern XAxiDma AxiDma;

int XEx_1LayerSetup(){
	return XEx_1layer_CfgInitialize(&xex_1layer_dev,&xex_1layer_config);
}

void XEx_1LayerStart(void *InstancePtr){
	XEx_1layer *pExample = (XEx_1layer *)InstancePtr;
	XEx_1layer_InterruptEnable(pExample,1);
	XEx_1layer_InterruptGlobalEnable(pExample);
	XEx_1layer_Start(pExample);
}
void XEx_1layerIsr(void *InstancePtr){
	XEx_1layer *pExample = (XEx_1layer *)InstancePtr;

	//Disable the global interrupt
	XEx_1layer_InterruptGlobalDisable(pExample);
	//Disable the local interrupt
	XEx_1layer_InterruptDisable(pExample,0xffffffff);

	// clear the local interrupt
	XEx_1layer_InterruptClear(pExample,1);

	ResultExample = 1;
	// restart the core if it should run again
	if(RunExample){XEx_1layer_Start(pExample);}
}
int XEx_1layerSetupInterrupt() {
	//This functions sets up the interrupt on the ARM
	int result;
	XScuGic_Config *pCfg = XScuGic_LookupConfig(XPAR_SCUGIC_SINGLE_DEVICE_ID);
	if (pCfg == NULL){
		print("Interrupt Configuration Lookup Failed\n\r");
		return XST_FAILURE;
	}
	result = XScuGic_CfgInitialize(&ScuGic,pCfg,pCfg->CpuBaseAddress);
	if(result != XST_SUCCESS){
		return result;
	}
	// self test
	result = XScuGic_SelfTest(&ScuGic);
	if(result != XST_SUCCESS){
		return result;
	}
	// Initialize the exception handler
	Xil_ExceptionInit();
	// Register the exception handler
	//print("Register the exception handler\n\r");
	Xil_ExceptionRegisterHandler(XIL_EXCEPTION_ID_INT,(Xil_ExceptionHandler)XScuGic_InterruptHandler,&ScuGic);
	//Enable the exception handler
	Xil_ExceptionEnable();
	// Connect the Adder ISR to the exception table
	//print("Connect the Adder ISR to the Exception handler table\n\r");
	result = XScuGic_Connect(&ScuGic,XPAR_FABRIC_EX_1LAYER_INTERRUPT_INTR,(Xil_InterruptHandler)XEx_1layerIsr,&xex_1layer_dev);
	if(result != XST_SUCCESS){
		return result;
	}
	//print("Enable the Adder ISR\n\r");
	XScuGic_Enable(&ScuGic,XPAR_FABRIC_EX_1LAYER_INTERRUPT_INTR);
	return XST_SUCCESS;
}
int Setup_HW_Accelerator(float INPUT[N_INPUTS],float OUTPUT[N_OUTPUTS], int dma_size_in,int dma_size_out) {
	int status = XEx_1LayerSetup();
	if(status != XST_SUCCESS){
		print("Error: example setup failed\n");
		return XST_FAILURE;
	}
	status =  XEx_1layerSetupInterrupt();
	if(status != XST_SUCCESS){
		print("Error: interrupt setup failed\n");
		return XST_FAILURE;
	}
	//XMmult_accel_core_SetVal1(&xmmult_dev,val1);
	//XMmult_accel_core_SetVal2(&xmmult_dev,val2);
	XEx_1LayerStart(&xex_1layer_dev);
	//flush the cache
	Xil_DCacheFlushRange((unsigned int)INPUT,dma_size_in);
	Xil_DCacheFlushRange((unsigned int)OUTPUT,dma_size_out);
	//print("\rCache cleared\n\r");

	return 0;
}
void dnn_ref(float input[N_INPUTS], float output[N_OUTPUTS], float weights[N_INPUTS][N_OUTPUTS],float bias[N_INPUTS]) {
  int ia, ib, id;
  for(ia=0; ia < N_OUTPUTS; ia++) output[ia] = 0.;
  for(ia=0; ia < N_INPUTS; ia++) { 
    for(ib=0; ib < N_OUTPUTS; ib++) { 
      output[ib] += input[ia] * weights[ia][ib]; 
    }
  }
  for(ia=0; ia < N_OUTPUTS; ia++) output[ia] += bias[ia];
}
int Start_HW_Accelerator(void) {
	//int status = XMmultSetup();
	//if(status != XST_SUCCESS){
	//	print("Error: example setup failed\n");
	//	return XST_FAILURE;
	//}
	//status =  XMmultSetupInterrupt();
	//if(status != XST_SUCCESS){
	//	print("Error: interrupt setup failed\n");
	//	return XST_FAILURE;
	//}
	//XMmult_accel_core_SetVal1(&xmmult_dev,val1);
	//XMmult_accel_core_SetVal2(&xmmultdev,val2);
	XEx_1LayerStart(&xex_1layer_dev);
	return(XST_SUCCESS);
}
int Run_HW_Accelerator(float  input[N_INPUTS], float output[N_OUTPUTS], int dma_size_in,int dma_size_out) {
	//transfer A to the Vivado HLS block
        int status = XAxiDma_SimpleTransfer(&AxiDma, (unsigned int) input, dma_size_in, XAXIDMA_DMA_TO_DEVICE);
	if (status != XST_SUCCESS) {
	  //print("Error: DMA transfer to Vivado HLS block failed\n");
	  return XST_FAILURE;
	}
	// Wait for transfer to be done
	while (XAxiDma_Busy(&AxiDma, XAXIDMA_DMA_TO_DEVICE)) ;
	
	//get results from the Vivado HLS block
	status = XAxiDma_SimpleTransfer(&AxiDma, (unsigned int) output, dma_size_out,XAXIDMA_DEVICE_TO_DMA);
	if (status != XST_SUCCESS) {
	  //print("Error: DMA transfer from Vivado HLS block failed\n");
	  return XST_FAILURE;
	}
	// Wait for transfer to be done
	while (XAxiDma_Busy(&AxiDma, XAXIDMA_DMA_TO_DEVICE)) ;

	//poll the DMA engine to verify transfers are complete
	/* Waiting for data processing */
	/* While this wait operation, the following action would be done
	 * First: Second matrix will be sent.
	 * After: NN will be computed
	 * Then: Output  will be sent from the accelerator to DDR and
	 * it will be stored at the base address that you set in the first SimpleTransfer
	 */
	while ((XAxiDma_Busy(&AxiDma, XAXIDMA_DEVICE_TO_DMA)) || (XAxiDma_Busy(&AxiDma, XAXIDMA_DMA_TO_DEVICE))) ;

//	while (!ResultExample)
//		print("Waiting for core to finish\n\r");

	return 0;

}
