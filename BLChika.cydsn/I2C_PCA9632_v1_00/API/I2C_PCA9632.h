//**********************************************************************
//
//  File Name: `$INSTANCE_NAME`.h
//  Version `$CY_MAJOR_VERSION`.`$CY_MINOR_VERSION`
//
//  Description:
//    This header file contains registers and constants associated
//    with the I2C PCA9632 component.
//
//**********************************************************************
//  Copyright noritan.org, 2019
//  All Rights Reserved
//  UNPUBLISHED, LICENSED SOFTWARE.
//
// CONFIDENTIAL AND PROPRIETARY INFORMATION
// WHICH IS THE PROPERTY OF noritan.org
//**********************************************************************

#if !defined(CY_I2C_PCA9632_`$INSTANCE_NAME`_H)
#define CY_I2C_PCA9632_`$INSTANCE_NAME`_H

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
#define `$INSTANCE_NAME`_REG_CONTINUOUS (0x80)  
#define `$INSTANCE_NAME`_REG_MODE1 (0x00)  
#define `$INSTANCE_NAME`_REG_PWM0 (0x02) 
#define `$INSTANCE_NAME`_REG_LEDOUT (0x08)

// Configuration bit field
#define `$INSTANCE_NAME`_MODE1_SLEEP (0x10)  

// Register initializer
#define `$INSTANCE_NAME`_MODE1_ENABLE (0x00)  
#define `$INSTANCE_NAME`_MODE1_DISABLE (`$INSTANCE_NAME`_MODE1_SLEEP)  
#define `$INSTANCE_NAME`_LEDOUT_ENABLE (0xAA)  
#define `$INSTANCE_NAME`_LEDOUT_DISABLE (0x00)  

// Default I2C slave address
#define `$INSTANCE_NAME`_DEFAULT_I2C_ADDRESS ((uint8)(`$DefaultI2cAddress`u))

// I2C stream buffer size
#define `$INSTANCE_NAME`_BUFF_SIZE (0x100u)

// MACROs for handling I2C Master API names on SCB and UDB
#if (`$INSTANCE_NAME`_IS_SCB_MASTER_USED)
    #define `$INSTANCE_NAME`_MODE_COMPLETE_XFER         (`$I2cMasterInstanceName`_I2C_MODE_COMPLETE_XFER)
    #define `$INSTANCE_NAME`_WRITE_COMPLETE             (`$I2cMasterInstanceName`_I2C_MSTAT_WR_CMPLT)

    #define `$INSTANCE_NAME`_MasterWriteBuf(slaveAddress, wrData, cnt, mode)  \
        `$I2cMasterInstanceName`_I2CMasterWriteBuf(slaveAddress, wrData, cnt, mode)

    #define `$INSTANCE_NAME`_MasterReadStatus()     `$I2cMasterInstanceName`_I2CMasterStatus()
#else
    #define `$INSTANCE_NAME`_MODE_COMPLETE_XFER         (`$I2cMasterInstanceName`_MODE_COMPLETE_XFER)
    #define `$INSTANCE_NAME`_WRITE_COMPLETE             (`$I2cMasterInstanceName`_MSTAT_WR_CMPLT)

    #define `$INSTANCE_NAME`_MasterWriteBuf(slaveAddress, wrData, cnt, mode)  \
        `$I2cMasterInstanceName`_MasterWriteBuf(slaveAddress, wrData, cnt, mode)

    #define `$INSTANCE_NAME`_MasterReadStatus()     `$I2cMasterInstanceName`_MasterStatus()
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
void `$INSTANCE_NAME`_WriteMode1(uint8 mode1);
void `$INSTANCE_NAME`_WriteLedout(uint8 ledout);
void `$INSTANCE_NAME`_WritePWM(uint8 pwm0, uint8 pwm1, uint8 pwm2, uint8 pwm3);

#endif /* CY_I2C_PCA9632_`$INSTANCE_NAME`_H */

/* [] END OF FILE */
