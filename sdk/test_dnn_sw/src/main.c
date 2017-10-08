/*
 * main() - performs timing tests on hardware and software NN evalutaions
 *
 *
 * Stolen Notes:
 *    1) the hardware timer provides a 64 bit count solution
 *    2) each timer count = 10ns
 *    3) the upper 32 bits are not used as the preliminary tests show that they
 *       are never used
 *
 */

#include <stdio.h>
#include "xil_io.h"
#include "platform.h"
#include "xparameters.h"
#include "xscugic.h"
#include "xaxidma.h"
#include "xex_1layer.h"
#include "xil_printf.h"
#include "xex_1layer_hw.h"
#include "xtmrctr.h"
#include "temp.h"
#include "xgpiops.h"
#include "lib_dnn_hw.h"

// simplified device IDs and system parameters
#define GPIO_DEVICE_ID  	XPAR_XGPIOPS_0_DEVICE_ID
#define NUM_TESTS           1024
#define UPDATE_PACE         (100000000)
#define N_INPUTS  10
#define N_OUTPUTS 1
typedef  float coeff_t; 
typedef  float bias_t; 
#include "weights/biases_1.h"
#include "weights/weights_1.h"
#undef  USE_HIGH_LEVEL_API_TIMERS

// --- define framework for hardware
XGpioPs_Config *ConfigPtrPS;
XAxiDma         AxiDma;			// AXI DMA Instance
XTmrCtr         timer_dev;		// axi_TIMER Instance
XGpioPs         PS_GPIOs;		// refers to all PS_GPIOs (EMIO and MIO)

// define the EMIO-GPIO usage
#define TIMER_HOLD_OE_LOW     0x0000
#define TIMER_CAPTURE_OE_LOW  0x0001
#define TIMER_HOLD_OE_HI      0x0002
#define TIMER_CAPTURE_OE_HI   0x0003
#define TIMER_FORCE_RESET     0x0004

#define ALL_32_OFF                      0
#define ALL_32_ON              0xFFFFFFFF
#define CONTROL_BANK        XGPIOPS_BANK2
#define DATA_BANK           XGPIOPS_BANK3


// function prototypes
void putnum (int);
void writeInt (int);
int  init_dma();

// macros for making output easier
#define writeStr_NL(x)    xil_printf("%s\n\r",x);
#define writeStr          xil_printf
#define writeNL           xil_printf("\n\r")
#define writeHex          putnum
#define writeInt(x)       xil_printf("%d",x)
#define writeInt_NL(x)    xil_printf("%d\n\r",x);
#define writeFloat(x)     writeInt((int)x); writeStr("."); writeInt_NL((int)(x * 1000) % 1000)
#define writeFloat_NL(x)  writeInt((int)x); writeStr("."); writeInt_NL((int)(x * 1000) % 1000); xil_printf("\n\r");


// ********** main() starts here **********
int main(){
  int   i, k, status, err=0;
  float INPUT[N_INPUTS];
  float OUTPUT_hw[N_OUTPUTS]; //hard
  float OUTPUT_sw[N_OUTPUTS]; //soft

  u32   dma_size_in  = N_INPUTS  * sizeof(float);
  u32   dma_size_out = N_OUTPUTS * sizeof(float);
  u32   per_loop_overhead;
  u32   function_call_overhead;
  u32   nn_sw_start_axiTimer, nn_sw_end_axiTimer;
  u32   nn_hw_start_axiTimer, nn_hw_end_axiTimer;
  u32   run_time_sw_axiTimer, run_time_sw_timerSub;
  u32   run_time_hw_axiTimer, run_time_hw_timerSub;
  
  // initialize the platform
  init_platform();
  
  writeStr_NL("\r***************************************************");
  writeStr_NL("\r*1 Layer DNN in FPGA Higgs Tagger by Racist J. Wu *");
  writeStr_NL("\r***************************************************");
  
  // initialize and configure the GPIO banks to control timer sub-system
  ConfigPtrPS = XGpioPs_LookupConfig(GPIO_DEVICE_ID);
  status = XGpioPs_CfgInitialize(&PS_GPIOs, ConfigPtrPS, ConfigPtrPS->BaseAddr);
  if (status != XST_SUCCESS) {
    writeStr_NL("Failed to initialize the PS GPIO");
    while (1); // die here
  }
  
  // EMIO_GPIO_consolidated has lower 32 bits as outputs and the upper 32 bits as inputs
  // XGpioPs_SetDirectionPin(&device, pin numbers, zero for input - one for output)
  // PH: At some point we should switch this to 16 bit inputs
  XGpioPs_SetDirection(&PS_GPIOs, DATA_BANK, ALL_32_OFF);			// upper 32 bits are inputs
  XGpioPs_SetDirection(&PS_GPIOs, CONTROL_BANK, ALL_32_ON);		// lower 32 bits are outputs
  XGpioPs_SetOutputEnable(&PS_GPIOs, CONTROL_BANK, ALL_32_ON);	//    and must be driven
  XGpioPs_Write(&PS_GPIOs, CONTROL_BANK, TIMER_HOLD_OE_LOW);		// hold the timer subsystem so that the next rising edge will trigger good capture
  
  // Initialize the DMA
  status = init_dma();
  if(status != XST_SUCCESS){
    writeStr_NL("\rError: DMA init failed");
    err = 1;
    return XST_FAILURE;
  }
  //	Initialize AXI Timer  
  status = TmrCtrLowLevelExample(XPAR_TMRCTR_0_BASEADDR, TIMER_COUNTER_0);
  if (status != XST_SUCCESS) {
    return XST_FAILURE;
  }
  // Timer calibration - compute overhead to call a function and increment a loop (subtract later)
  {
    // define variables local to this block
    unsigned int fn_test_start,  fn_test_end;
    unsigned int loop_test_start,loop_test_end;
      
    // Calibrate AXI timer
    fn_test_start = XTmrCtr_GetTimerCounterReg(XPAR_TMRCTR_0_BASEADDR, TIMER_COUNTER_0);
    fn_test_end   = XTmrCtr_GetTimerCounterReg(XPAR_TMRCTR_0_BASEADDR, TIMER_COUNTER_0);
    function_call_overhead = fn_test_end - fn_test_start;   			// determine how long it takes to call the timer routine
    writeStr("   overhead for function call:     "); writeInt_NL(function_call_overhead);
    
    // Loop measurement - figures overhead for looping
    loop_test_start = XTmrCtr_GetTimerCounterReg(XPAR_TMRCTR_0_BASEADDR, TIMER_COUNTER_0);
    for (i = 0; i<1000; i++);
    loop_test_end = XTmrCtr_GetTimerCounterReg(XPAR_TMRCTR_0_BASEADDR, TIMER_COUNTER_0);
    // compute and display calibration results
    per_loop_overhead = (loop_test_end - loop_test_start + 500) / 1000;			// +500 for rounding
    writeStr("   overhead for looping (per loop):"); writeInt_NL(per_loop_overhead);
  }
  // Initialize 
  for(i = 0; i < N_INPUTS; i++) INPUT[i] = (float) i*1.5;
  // Run the software version of the DNN for some number of iterations
  for (k = 0; k < 1; k++) {
    writeStr("Running Software NN eval for a baseline on "); writeInt(NUM_TESTS); writeStr_NL("# of tests:");
    //Start the clock
    nn_sw_start_axiTimer = XTmrCtr_GetTimerCounterReg(XPAR_TMRCTR_0_BASEADDR, TIMER_COUNTER_0); // get the current time from the AXI timer
    XGpioPs_Write(&PS_GPIOs, CONTROL_BANK, TIMER_FORCE_RESET);
    XGpioPs_Write(&PS_GPIOs, CONTROL_BANK, TIMER_CAPTURE_OE_LOW);// reset and start the timer subsystem, output the low DW to the data bank
    for (i = 0; i < NUM_TESTS; i++) dnn_ref(INPUT,OUTPUT_sw,weights,biases);
    XGpioPs_Write(&PS_GPIOs, CONTROL_BANK, 0); // stop the timer subsystem and hold the count
    nn_sw_end_axiTimer = XTmrCtr_GetTimerCounterReg(XPAR_TMRCTR_0_BASEADDR, TIMER_COUNTER_0);	// get the current time from the AXI timer
    writeStr("  here ");
#ifdef PERFORMANCE_CHECK
    {
      // define variables local to this block
      float time_in_nsecs_per_nn;
      float n_nns_per_sec;
      u32   overhead = (NUM_TESTS * function_call_overhead) + (2 * function_call_overhead) +
	(per_loop_overhead * NUM_TESTS);		// attempt to compensate for the 3 function calls and the loop overhead
      
      // Read timers and compute amount of time taken to do the software nn evaluation
      run_time_sw_axiTimer   = nn_sw_end_axiTimer - nn_sw_start_axiTimer;	// raw value for AXI timer
      run_time_sw_axiTimer  -= overhead;
      writeStr("\tTotal run time (in 10ns cycles) for SW on Processor (AXI_TIMER):       ");
      writeInt_NL(run_time_sw_axiTimer);
      // read the duration to compute the performance
      run_time_sw_timerSub  -= overhead - (2 * function_call_overhead);
      writeStr("\tTotal run time (in 10ns cycles) for SW on Processor (timer_subsystem): ");
      run_time_sw_timerSub = XGpioPs_Read(&PS_GPIOs,DATA_BANK);          	        // collect the lower 32 bits from the timer subsystem
      writeInt_NL(run_time_sw_timerSub);			
      // compute number of matrices per second
      time_in_nsecs_per_nn = run_time_sw_timerSub * 10.0 / (float)(NUM_TESTS);
      n_nns_per_sec       = 1000000000.0 / time_in_nsecs_per_nn;
      writeStr("\tNumber of NN evals per second:                                         ");
      writeFloat_NL(n_nns_per_sec);
    }
#endif
    // Setup the HW Accelerator
    writeStr("  here setting up HW");
    Setup_HW_Accelerator(INPUT,OUTPUT_hw, dma_size_in,dma_size_out);
    nn_hw_start_axiTimer = XTmrCtr_GetTimerCounterReg(XPAR_TMRCTR_0_BASEADDR, TIMER_COUNTER_0);	// get the current time from the AXI timer
    XTmrCtr_GetTimerCounterReg(XPAR_TMRCTR_0_BASEADDR, TIMER_COUNTER_0);	// get the current time from the AXI timer
    XGpioPs_Write(&PS_GPIOs, CONTROL_BANK, 0);
    XGpioPs_Write(&PS_GPIOs, CONTROL_BANK, TIMER_FORCE_RESET);
    XGpioPs_Write(&PS_GPIOs, CONTROL_BANK, TIMER_CAPTURE_OE_LOW);	//
    writeStr("  HW tests");
    for (i = 0; i < NUM_TESTS; i++) {
      if(i % 2 == 0) writeInt(i);
      Start_HW_Accelerator();
      //XGpioPs_Write(&PS_GPIOs, CONTROL_BANK, TIMER_CAPTURE_OE_LOW);	//
      Run_HW_Accelerator(INPUT, OUTPUT_hw, dma_size_in,dma_size_out);
      //writeFloat_NL(OUTPUT_hw[0]);
      //XGpioPs_Write(&PS_GPIOs, CONTROL_BANK, 0);
    }
    //XGpioPs_Write(&PS_GPIOs, CONTROL_BANK, 0);  //e-assert the application running signal and capture the end timer value
    run_time_hw_timerSub = XGpioPs_Read(&PS_GPIOs,DATA_BANK);
    XTmrCtr_GetTimerCounterReg(XPAR_TMRCTR_0_BASEADDR, TIMER_COUNTER_0);
    nn_hw_end_axiTimer   = XTmrCtr_GetTimerCounterReg(XPAR_TMRCTR_0_BASEADDR, TIMER_COUNTER_0);
    writeStr("  HW tests done");		
#ifdef PERFORMANCE_CHECK
    {
      // variables local to this block
      float acc_factor;
      float time_in_nsecs_per_nn;
      float n_nns_per_sec;
      u32   overhead = (NUM_TESTS * function_call_overhead) + (2 * function_call_overhead) +
	(per_loop_overhead * NUM_TESTS);		// attempt to compensate for the 3 function calls and the loop overhead
      // Read timers and compute amount of time taken to do the software matrix multiply
      run_time_hw_axiTimer   = nn_hw_end_axiTimer - nn_hw_start_axiTimer;	 // raw value for AXI timer
      run_time_hw_axiTimer  -= overhead;
      writeStr("\tTotal run time (in 10ns cycles) for HW Accelerator (AXI_TIMER):       ");
      writeInt_NL(run_time_hw_axiTimer);

      // read the duration to compute the performance
      writeStr("\tTotal run time (in 10ns cycles) for HW Accelerator (timer_subsystem): ");
      //run_time_hw_timerSub = XGpioPs_Read(&PS_GPIOs,DATA_BANK);    // collect the lower 32 bits from the timer subsystem
      writeInt_NL(run_time_hw_timerSub);															  // compute number of nns per second
      time_in_nsecs_per_nn = run_time_hw_timerSub * 10.0 / (float)(NUM_TESTS);
      n_nns_per_sec       = 1000000000.0 / time_in_nsecs_per_nn;
      writeStr("\tNumber of NNs per second:                                         ");
      writeFloat_NL(n_nns_per_sec);
      
      // HW vs. SW speedup factors
      writeStr_NL("Acceleration Factor...");
      acc_factor = (float)(run_time_sw_axiTimer) / (float)(run_time_hw_axiTimer);
      writeStr("\taccording to software timer:                                          ");
      writeFloat_NL(acc_factor);
      acc_factor = (float)(run_time_sw_timerSub) / (float)(run_time_hw_timerSub);
      writeStr("\taccording to hardware timer:                                          ");
      writeFloat_NL(acc_factor);
    }
#endif

#ifdef BEHAVIOR_CHECK
		//Compare the results from sw and hw
		for (i = 0; i < N_OUTPUTS; i++) {
		  if (OUTPUT_sw[i] != OUTPUT_hw[i]) {
		    err = 1;
		    break;
		  }
		}
		// report error analysis
		if (err == 0) { writeStr_NL("\rSW and HW results match!"); }
		else 		  { writeStr_NL("\rERROR: results mismatch");  }
#endif

  }
  writeStr_NL("\r *** END OF TEST\n");
  cleanup_platform();
  return  err;
}
// Initializes the AXI_DMA hardware
int init_dma(){
	XAxiDma_Config *CfgPtr;
	int status;

	CfgPtr = XAxiDma_LookupConfig(XPAR_DMA_CONTROLLER_DEVICE_ID);
	if(!CfgPtr){
		print("Error looking for AXI DMA config\n\r");
		return XST_FAILURE;
	}
	status = XAxiDma_CfgInitialize(&AxiDma,CfgPtr);
	if(status != XST_SUCCESS){
		print("Error initializing DMA\n\r");
		return XST_FAILURE;
	}
	//check for scatter gather mode
	if(XAxiDma_HasSg(&AxiDma)){
		print("Error DMA configured in SG mode\n\r");
		return XST_FAILURE;
	}
	/* Disable interrupts, we use polling mode */
	XAxiDma_IntrDisable(&AxiDma, XAXIDMA_IRQ_ALL_MASK,XAXIDMA_DEVICE_TO_DMA);
	XAxiDma_IntrDisable(&AxiDma, XAXIDMA_IRQ_ALL_MASK,XAXIDMA_DMA_TO_DEVICE);

	return XST_SUCCESS;
}

