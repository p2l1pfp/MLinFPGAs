Instructions : 
1. Open vivado sdk
2. Choose workspace to be sdk
3. File > new project. Select Xilinx > hardware project. Choose the hdf exporoted (see checked)
4. Add a new board support package: File > new board support package (ensure hardware is linked correctly)
5. File > New application project. Linke to board support package made previously. Choose empty application
6. Right click on teh application project and click on import > file then open test_dnn_sw/src and importa ll files
7. Right click on application project and select refersh. Ensure compliation with no errors
8. Click on Xilinx>program fpga and load bit file (wait til its loaded)
9. Connect to output of board : for zedboard do this by typing minicom in the terminal and the source is /dev/ttyACM0  
10. right click on application and select run > hardware (default debugger). You will now see output from the fpgae coming out from minicom
