//**********************************************************************
//
//  File Name: `$INSTANCE_NAME`.c
//  Version `$CY_MAJOR_VERSION`.`$CY_MINOR_VERSION`
//
//  Description:
//    This file provides source code for the I2C PCA9632 component's API.
//
//**********************************************************************
//  Copyright noritan.org, 2019
//  All Rights Reserved
//  UNPUBLISHED, LICENSED SOFTWARE.
//
// CONFIDENTIAL AND PROPRIETARY INFORMATION
// WHICH IS THE PROPERTY OF noritan.org
//**********************************************************************

//======================================================================
//  Header file inclusion
//======================================================================
#include "`$INSTANCE_NAME`.h"


//======================================================================
//  Variable declaration
//======================================================================
uint8 `$INSTANCE_NAME`_initVar = 0u;

//  Buffer that holds data to be send to I2C slave
static uint8 `$INSTANCE_NAME`_buff[`$INSTANCE_NAME`_BUFF_SIZE];

/* Need to have `$INSTANCE_NAME`_buffIndex of different size to avoid MISRA
* warning when SCB based I2C Master is used. 
*/
#if(`$INSTANCE_NAME`_IS_SCB_MASTER_USED)
    /* Variable used for buffer indexing */
    static uint32 `$INSTANCE_NAME`_buffIndex = 0u;
    /* Variable stores the I2C address */
    static uint32 `$INSTANCE_NAME`_address = `$INSTANCE_NAME`_DEFAULT_I2C_ADDRESS;
#else
    /* Variable used for buffer indexing */
    static uint8 `$INSTANCE_NAME`_buffIndex = 0u;
    /* Variable stores the I2C address */
    static uint8 `$INSTANCE_NAME`_address = `$INSTANCE_NAME`_DEFAULT_I2C_ADDRESS;
#endif /* (`$INSTANCE_NAME`_IS_SCB_MASTER_USED) */


//======================================================================
//  Internal function ptototypes
//======================================================================
static void `$INSTANCE_NAME`_WriteDataToBuff(uint8 dByte);
static void `$INSTANCE_NAME`_SendSequence(void);


//======================================================================
//  Function Name: `$INSTANCE_NAME`_Init
//======================================================================
//
//  Summary:
//    Performs initialization required for the component's normal work.
//
//  Parameters:
//    None
//
//  Return:
//    None
//
//  Reentrant:
//    No
//
//======================================================================
void `$INSTANCE_NAME`_Init(void) {
}


//======================================================================
//  Function Name: `$INSTANCE_NAME`_Enable
//======================================================================
//
//  Summary:
//    Turns on the LED.
//
//  Parameters:
//    None
//
//  Return:
//    None
//
//  Reentrant:
//    No
//
//======================================================================
void `$INSTANCE_NAME`_Enable(void) {
    // Enter to Normal mode
    `$INSTANCE_NAME`_WriteMode1(
        `$INSTANCE_NAME`_MODE1_ENABLE
    );
    // Enable all LED as PWM
    `$INSTANCE_NAME`_WriteLedout(
        `$INSTANCE_NAME`_LEDOUT_ENABLE
    );
}


//======================================================================
//  Function Name: `$INSTANCE_NAME`_Disable
//======================================================================
//
//  Summary:
//    Turns off the LED.
//
//  Parameters:
//    None
//
//  Return:
//    None
//
//  Reentrant:
//    No
//
//======================================================================
void `$INSTANCE_NAME`_Disable(void) {
    // Disable all LED
    `$INSTANCE_NAME`_WriteLedout(
        `$INSTANCE_NAME`_LEDOUT_DISABLE
    );
    // Enter to Sleep mode
    `$INSTANCE_NAME`_WriteMode1(
        `$INSTANCE_NAME`_MODE1_DISABLE
    );
}


//======================================================================
//  Function Name: `$INSTANCE_NAME`_Start
//======================================================================
//
//  Summary:
//    This function initializes the LED hardware module as follows:
//     Initialize the I2C bus if it hasn't already been
//
//  Parameters:
//    `$INSTANCE_NAME`_initVar - global variable.
//
//  Return:
//    `$INSTANCE_NAME`_initVar - global variable.
//
//  Reentrant:
//    No
//
//======================================================================
void `$INSTANCE_NAME`_Start(void) {
    // If not initialized then perform initialization
    if (!`$INSTANCE_NAME`_initVar) {
        `$INSTANCE_NAME`_Init();
        `$INSTANCE_NAME`_initVar = 1u;
    }

    // Turn on the LED
    `$INSTANCE_NAME`_Enable();
}


//======================================================================
//  Function Name: `$INSTANCE_NAME`_Stop
//======================================================================
//
//  Summary:
//    Turns off the LED but does not stop the I2C Master component.
//
//  Parameters:
//    None
//
//  Return:
//    None
//
//  Reentrant:
//    No
//
//======================================================================
void `$INSTANCE_NAME`_Stop(void) {
    `$INSTANCE_NAME`_Disable();
}


//======================================================================
//  Function Name: `$INSTANCE_NAME`_WriteMode1
//======================================================================
//
//  Summary:
//    Set MODE1 register.
//
//  Parameters:
//    mode - Byte value to be written to MODE1
//
//  Return:
//    None
//
//  Reentrant:
//    No
//
//======================================================================
void `$INSTANCE_NAME`_WriteMode1(uint8 mode1) {
    `$INSTANCE_NAME`_WriteDataToBuff(`$INSTANCE_NAME`_REG_MODE1);
    `$INSTANCE_NAME`_WriteDataToBuff(mode1);
    `$INSTANCE_NAME`_SendSequence();
}

//======================================================================
//  Function Name: `$INSTANCE_NAME`_WriteMode1
//======================================================================
//
//  Summary:
//    Set LEDOUT register.
//
//  Parameters:
//    ledout - Byte value to be written to LEDOUT
//
//  Return:
//    None
//
//  Reentrant:
//    No
//
//======================================================================
void `$INSTANCE_NAME`_WriteLedout(uint8 ledout) {
    `$INSTANCE_NAME`_WriteDataToBuff(`$INSTANCE_NAME`_REG_LEDOUT);
    `$INSTANCE_NAME`_WriteDataToBuff(ledout);
    `$INSTANCE_NAME`_SendSequence();
}


//======================================================================
//  Function Name: `$INSTANCE_NAME`_WriteMode1
//======================================================================
//
//  Summary:
//    Set PWM registers.
//
//  Parameters:
//    pwm0 - PWM value for LED0
//    pwm1 - PWM value for LED1
//    pwm2 - PWM value for LED2
//    pwm3 - PWM value for LED3
//
//  Return:
//    None
//
//  Reentrant:
//    No
//
//======================================================================
void `$INSTANCE_NAME`_WritePWM(
    uint8 pwm0,
    uint8 pwm1,
    uint8 pwm2,
    uint8 pwm3
) {
    `$INSTANCE_NAME`_WriteDataToBuff(
        `$INSTANCE_NAME`_REG_PWM0 | `$INSTANCE_NAME`_REG_CONTINUOUS
    );
    `$INSTANCE_NAME`_WriteDataToBuff(pwm0);
    `$INSTANCE_NAME`_WriteDataToBuff(pwm1);
    `$INSTANCE_NAME`_WriteDataToBuff(pwm2);
    `$INSTANCE_NAME`_WriteDataToBuff(pwm3);
    `$INSTANCE_NAME`_SendSequence();
}


//======================================================================
//   Function Name: `$INSTANCE_NAME`_WriteDataToBuff
//======================================================================
//
//  Summary:
//    Writes a byte of data to the I2C buffer.
//
//  Parameters:
//    dByte: the byte containing to be written to the I2C device.
//
//  Return:
//    None
//
//  Reentrant:
//    No
//
//======================================================================
static void `$INSTANCE_NAME`_WriteDataToBuff(uint8 dByte) {
    // Put one byte of data to buffer at location of `$INSTANCE_NAME`_buffIndex
    `$INSTANCE_NAME`_buff[`$INSTANCE_NAME`_buffIndex] = dByte;
    // Increment `$INSTANCE_NAME`_buffIndex to point to next free position
    `$INSTANCE_NAME`_buffIndex++;
}


//======================================================================
//  Function Name: `$INSTANCE_NAME`_SendSequence
//======================================================================
//
//  Summary:
//    Sends the content of the buffer to the I2C device and waits
//    while transaction completes. In the end resets index to point
//    to the start of the buffer.
//
//  Parameters:
//    None
//
//  Return:
//    None
//
//  Reentrant:
//    No
//
//======================================================================
static void `$INSTANCE_NAME`_SendSequence(void) {
    // Send command pattern to I2C device
    (void) `$INSTANCE_NAME`_MasterWriteBuf(
        `$INSTANCE_NAME`_address,
        `$INSTANCE_NAME`_buff,
        `$INSTANCE_NAME`_buffIndex,
        `$INSTANCE_NAME`_MODE_COMPLETE_XFER
    );

    while (!(`$INSTANCE_NAME`_MasterReadStatus() & `$INSTANCE_NAME`_WRITE_COMPLETE)) {
        // Wait until I2C Master finishes transaction
    }

    // Reset buffer index
    `$INSTANCE_NAME`_buffIndex = 0u;
}


/* [] END OF FILE */
