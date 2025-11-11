/*
 * Generated with the FPGA Interface C API Generator 21.3
 * for NI-RIO 21.3 or later.
 * This file needs to be generated with the API for the bitfile to use
 */
#ifndef __NiFpga_FPGA_Main_h__
#define __NiFpga_FPGA_Main_h__

#ifndef NiFpga_Version
   #define NiFpga_Version 213
#endif

#include "NiFpga.h"

/**
 * The filename of the FPGA bitfile.
 *
 * This is a #define to allow for string literal concatenation. For example:
 *
 *    static const char* const Bitfile = "C:\\" NiFpga_FPGA_Main_Bitfile;
 */
#define NiFpga_FPGA_Main_Bitfile "PXIe7847R_SelfTest.lvbitx"

/**
 * The signature of the FPGA bitfile.
 */
static const char* const NiFpga_FPGA_Main_Signature = "DC163AC6287FBFB65DEEBBBE683470C3";

#if NiFpga_Cpp
extern "C"
{
#endif

typedef enum
{
   NiFpga_FPGA_Main_IndicatorU8_Connector0Port_1 = 0x1800A,
   NiFpga_FPGA_Main_IndicatorU8_Connector0Port_3 = 0x1801A,
   NiFpga_FPGA_Main_IndicatorU8_Connector1Port_1 = 0x18022,
   NiFpga_FPGA_Main_IndicatorU8_Connector1Port_3 = 0x1802A,
   NiFpga_FPGA_Main_IndicatorU8_Connector2Port_1 = 0x18032,
   NiFpga_FPGA_Main_IndicatorU8_Connector2Port_3 = 0x1803A,
   NiFpga_FPGA_Main_IndicatorU8_Connector3Port_1 = 0x18042,
   NiFpga_FPGA_Main_IndicatorU8_Connector3Port_3 = 0x1804A
} NiFpga_FPGA_Main_IndicatorU8;

typedef enum
{
   NiFpga_FPGA_Main_ControlBool_Start = 0x1800E,
   NiFpga_FPGA_Main_ControlBool_Stop = 0x18012
} NiFpga_FPGA_Main_ControlBool;

typedef enum
{
   NiFpga_FPGA_Main_ControlU8_Connector0Port_0 = 0x18006,
   NiFpga_FPGA_Main_ControlU8_Connector0Port_2 = 0x18016,
   NiFpga_FPGA_Main_ControlU8_Connector1Port_0 = 0x1801E,
   NiFpga_FPGA_Main_ControlU8_Connector1Port_2 = 0x18026,
   NiFpga_FPGA_Main_ControlU8_Connector2Port_0 = 0x1802E,
   NiFpga_FPGA_Main_ControlU8_Connector2Port_2 = 0x18036,
   NiFpga_FPGA_Main_ControlU8_Connector3Port_0 = 0x1803E,
   NiFpga_FPGA_Main_ControlU8_Connector3Port_2 = 0x18046
} NiFpga_FPGA_Main_ControlU8;

typedef enum
{
   NiFpga_FPGA_Main_IndicatorArrayI64_AIs = 0x18000
} NiFpga_FPGA_Main_IndicatorArrayI64;

typedef enum
{
   NiFpga_FPGA_Main_IndicatorArrayI64Size_AIs = 8
} NiFpga_FPGA_Main_IndicatorArrayI64Size;


#if NiFpga_Cpp
}
#endif

#endif
