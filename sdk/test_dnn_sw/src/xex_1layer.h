// ==============================================================
// File generated by Vivado(TM) HLS - High-Level Synthesis from C, C++ and SystemC
// Version: 2016.4
// Copyright (C) 1986-2017 Xilinx, Inc. All Rights Reserved.
// 
// ==============================================================

#ifndef XEX_1LAYER_H
#define XEX_1LAYER_H

#ifdef __cplusplus
extern "C" {
#endif

/***************************** Include Files *********************************/
#ifndef __linux__
#include "xil_types.h"
#include "xil_assert.h"
#include "xstatus.h"
#include "xil_io.h"
#else
#include <stdint.h>
#include <assert.h>
#include <dirent.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stddef.h>
#endif
#include "xex_1layer_hw.h"

/**************************** Type Definitions ******************************/
#ifdef __linux__
typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
#else
typedef struct {
    u16 DeviceId;
    u32 Control_bus_BaseAddress;
} XEx_1layer_Config;
#endif

typedef struct {
    u32 Control_bus_BaseAddress;
    u32 IsReady;
} XEx_1layer;

/***************** Macros (Inline Functions) Definitions *********************/
#ifndef __linux__
#define XEx_1layer_WriteReg(BaseAddress, RegOffset, Data) \
    Xil_Out32((BaseAddress) + (RegOffset), (u32)(Data))
#define XEx_1layer_ReadReg(BaseAddress, RegOffset) \
    Xil_In32((BaseAddress) + (RegOffset))
#else
#define XEx_1layer_WriteReg(BaseAddress, RegOffset, Data) \
    *(volatile u32*)((BaseAddress) + (RegOffset)) = (u32)(Data)
#define XEx_1layer_ReadReg(BaseAddress, RegOffset) \
    *(volatile u32*)((BaseAddress) + (RegOffset))

#define Xil_AssertVoid(expr)    assert(expr)
#define Xil_AssertNonvoid(expr) assert(expr)

#define XST_SUCCESS             0
#define XST_DEVICE_NOT_FOUND    2
#define XST_OPEN_DEVICE_FAILED  3
#define XIL_COMPONENT_IS_READY  1
#endif

/************************** Function Prototypes *****************************/
#ifndef __linux__
int XEx_1layer_Initialize(XEx_1layer *InstancePtr, u16 DeviceId);
XEx_1layer_Config* XEx_1layer_LookupConfig(u16 DeviceId);
int XEx_1layer_CfgInitialize(XEx_1layer *InstancePtr, XEx_1layer_Config *ConfigPtr);
#else
int XEx_1layer_Initialize(XEx_1layer *InstancePtr, const char* InstanceName);
int XEx_1layer_Release(XEx_1layer *InstancePtr);
#endif

void XEx_1layer_Start(XEx_1layer *InstancePtr);
u32 XEx_1layer_IsDone(XEx_1layer *InstancePtr);
u32 XEx_1layer_IsIdle(XEx_1layer *InstancePtr);
u32 XEx_1layer_IsReady(XEx_1layer *InstancePtr);
void XEx_1layer_EnableAutoRestart(XEx_1layer *InstancePtr);
void XEx_1layer_DisableAutoRestart(XEx_1layer *InstancePtr);


void XEx_1layer_InterruptGlobalEnable(XEx_1layer *InstancePtr);
void XEx_1layer_InterruptGlobalDisable(XEx_1layer *InstancePtr);
void XEx_1layer_InterruptEnable(XEx_1layer *InstancePtr, u32 Mask);
void XEx_1layer_InterruptDisable(XEx_1layer *InstancePtr, u32 Mask);
void XEx_1layer_InterruptClear(XEx_1layer *InstancePtr, u32 Mask);
u32 XEx_1layer_InterruptGetEnabled(XEx_1layer *InstancePtr);
u32 XEx_1layer_InterruptGetStatus(XEx_1layer *InstancePtr);

#ifdef __cplusplus
}
#endif

#endif
