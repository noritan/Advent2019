//**********************************************************************
//
//  File Name: `$INSTANCE_NAME`.h
//  Version `$CY_MAJOR_VERSION`.`$CY_MINOR_VERSION`
//
//  Description:
//    This header file contains registers and constants associated
//    with the I2C 2SMPB-02E component.
//
//**********************************************************************
//  Copyright noritan.org, 2019
//  All Rights Reserved
//  UNPUBLISHED, LICENSED SOFTWARE.
//
// CONFIDENTIAL AND PROPRIETARY INFORMATION
// WHICH IS THE PROPERTY OF noritan.org
//**********************************************************************

#if !defined(CY_I2C_2SMPB02E_`$INSTANCE_NAME`_H)
#define CY_I2C_2SMPB02E_`$INSTANCE_NAME`_H

#include "cytypes.h"
#include "cyfitter.h"

#include "`$I2cMasterInstanceName`.h"

//======================================================================
//  Conditional Compilation Parameters
//======================================================================
//  This condition checks if I2C Master implemented on SCB
//  block. There is mismatch in function names between I2C Master that is
//  implemented on SCB and I2C Master that is implemented on UDB.
//======================================================================
#if defined(`$I2cMasterInstanceName`_SCB_MODE)
    // In this case always returns - "true"
    #define `$INSTANCE_NAME`_IS_SCB_MASTER_USED     (0x01u == 0x01u)
#else
    // Always "false"
    #define `$INSTANCE_NAME`_IS_SCB_MASTER_USED     (0x01u == 0x00u)
#endif /* `$I2cMasterInstanceName`_SCB_MODE */

#if defined(`$INSTANCE_NAME`_IS_SCB_MASTER_USED)
    // An additional header file for SCB
    #include "`$I2cMasterInstanceName`_I2C.h"
#endif /* `$INSTANCE_NAME`_IS_SCB_MASTER_USED */

//======================================================================
//  Macros
//======================================================================
// Register address
#define `$INSTANCE_NAME`_REG_TEMP_TXD (0xFA)  
#define `$INSTANCE_NAME`_REG_PRESS_TXD (0xF7)  
#define `$INSTANCE_NAME`_REG_CTRL_MEAS (0xF4) 
#define `$INSTANCE_NAME`_REG_DEVICE_STAT (0xF3)
#define `$INSTANCE_NAME`_REG_COE (0xA0)

// Configuration bit field
#define `$INSTANCE_NAME`_STAT_MEASURE (0x08)  

// Register initializer
#define `$INSTANCE_NAME`_CTRL_MEAS_FORCE (0x6D)
#define `$INSTANCE_NAME`_CTRL_MEAS_MORMAL (0x6F)
#define `$INSTANCE_NAME`_CTRL_MEAS_SLEEP (0x6C)

// Default I2C slave address
#define `$INSTANCE_NAME`_DEFAULT_I2C_ADDRESS ((uint8)(`$DefaultI2cAddress`u))

// I2C stream buffer size
#define `$INSTANCE_NAME`_BUFF_SIZE (0x32u)

// MACROs for handling I2C Master API names on SCB and UDB
#if (`$INSTANCE_NAME`_IS_SCB_MASTER_USED)
    #define `$INSTANCE_NAME`_MODE_COMPLETE_XFER         (`$I2cMasterInstanceName`_I2C_MODE_COMPLETE_XFER)
    #define `$INSTANCE_NAME`_WRITE_COMPLETE             (`$I2cMasterInstanceName`_I2C_MSTAT_WR_CMPLT)
    #define `$INSTANCE_NAME`_READ_COMPLETE              (`$I2cMasterInstanceName`_I2C_MSTAT_RD_CMPLT)

    #define `$INSTANCE_NAME`_MasterWriteBuf(slaveAddress, wrData, cnt, mode)  \
        `$I2cMasterInstanceName`_I2CMasterWriteBuf(slaveAddress, wrData, cnt, mode)

    #define `$INSTANCE_NAME`_MasterReadStatus()     `$I2cMasterInstanceName`_I2CMasterStatus()
    #define `$INSTANCE_NAME`_MasterClearStatus()    `$I2cMasterInstanceName`_I2CMasterClearStatus()
    #define `$INSTANCE_NAME`_MasterReadBuf(slaveAddress, rdData, cnt, mode) \
        `$I2cMasterInstanceName`_I2CMasterReadBuf(slaveAddress, rdData, cnt, mode)
    #define `$INSTANCE_NAME`_MasterGetReadBufSize() `$I2cMasterInstanceName`_I2CMasterGetReadBufSize()
#else
    #define `$INSTANCE_NAME`_MODE_COMPLETE_XFER         (`$I2cMasterInstanceName`_MODE_COMPLETE_XFER)
    #define `$INSTANCE_NAME`_WRITE_COMPLETE             (`$I2cMasterInstanceName`_MSTAT_WR_CMPLT)
    #define `$INSTANCE_NAME`_READ_COMPLETE              (`$I2cMasterInstanceName`_MSTAT_RD_CMPLT)

    #define `$INSTANCE_NAME`_MasterWriteBuf(slaveAddress, wrData, cnt, mode)  \
        `$I2cMasterInstanceName`_MasterWriteBuf(slaveAddress, wrData, cnt, mode)

    #define `$INSTANCE_NAME`_MasterReadStatus()     `$I2cMasterInstanceName`_MasterStatus()
    #define `$INSTANCE_NAME`_MasterClearStatus()    `$I2cMasterInstanceName`_MasterClearStatus()
    #define `$INSTANCE_NAME`_MasterReadBuf(slaveAddress, rdData, cnt, mode) \
        `$I2cMasterInstanceName`_MasterReadBuf(slaveAddress, rdData, cnt, mode)
    #define `$INSTANCE_NAME`_MasterGetReadBufSize() `$I2cMasterInstanceName`MasterGetReadBufSize()
#endif /* `$INSTANCE_NAME`_IS_SCB_MASTER_USED */

//======================================================================
//  Global Cariables
//======================================================================
extern uint8 `$INSTANCE_NAME`_initVar;

//======================================================================
//  Function Prototypes
//======================================================================
void `$INSTANCE_NAME`_Init(void);
void `$INSTANCE_NAME`_Enable(void);
void `$INSTANCE_NAME`_Disable(void);
void `$INSTANCE_NAME`_Start(void);
void `$INSTANCE_NAME`_Stop(void);
void `$INSTANCE_NAME`_ReadCoefficient(void);
uint32 `$INSTANCE_NAME`_IsMeasuring(void);
void `$INSTANCE_NAME`_Sleep(void);
void `$INSTANCE_NAME`_Force(void);
void `$INSTANCE_NAME`_ReadRawData(void);
double `$INSTANCE_NAME`_GetTemperature(void);
double `$INSTANCE_NAME`_GetPressure(void);

#endif /* CY_I2C_2SMPB02E_`$INSTANCE_NAME`_H */

/* [] END OF FILE */
