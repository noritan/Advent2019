//**********************************************************************
//
//  File Name: `$INSTANCE_NAME`.c
//  Version `$CY_MAJOR_VERSION`.`$CY_MINOR_VERSION`
//
//  Description:
//    This file provides source code for the I2C 2SMPB-02E component's API.
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
//  Internal context
//======================================================================
struct `$INSTANCE_NAME`_Context {
    double temp;
    double pressure;
    double b00;
    double bt1;
    double bt2;
    double bp1;
    double b11;
    double bp2;
    double b12;
    double b21;
    double bp3;
    double a0;
    double a1;
    double a2;
    double dt;
    double dp;
    double tr;
    double pr;
} `$INSTANCE_NAME`_context;


//======================================================================
//  Internal function ptototypes
//======================================================================
static void `$INSTANCE_NAME`_WriteDataToBuff(uint8 dByte);
static void `$INSTANCE_NAME`_SendSequence(void);
static uint32 `$INSTANCE_NAME`_ReceiveSequence(uint8 *rbuf, uint32 length);


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
    `$INSTANCE_NAME`_ReadCoefficient();
}


//======================================================================
//  Function Name: `$INSTANCE_NAME`_Enable
//======================================================================
//
//  Summary:
//    Enable the sensor.
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
}


//======================================================================
//  Function Name: `$INSTANCE_NAME`_Disable
//======================================================================
//
//  Summary:
//    Disable sensor.
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
    // Enter to SLEEP mode
    `$INSTANCE_NAME`_Sleep();
}


//======================================================================
//  Function Name: `$INSTANCE_NAME`_Start
//======================================================================
//
//  Summary:
//    This function initializes the sensor module as follows:
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

    // Enable the sensor
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


static void `$INSTANCE_NAME`_CalculateK(uint8 *coe, double a, double s, double *result) {
        int32 x = (coe[0] << 8) | coe[1];
        if (x & 0x8000) { x -= 0x10000u; } // Sign extend
        *result = a + (s * x) / 32768.0;
}
    
//======================================================================
//  Function Name: `$INSTANCE_NAME`_ReadCoefficient
//======================================================================
//
//  Summary:
//    Read the Coefficient parameters from the sensor.
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
void `$INSTANCE_NAME`_ReadCoefficient(void) {
    struct {
        uint8 b00[2];  // A0
        uint8 bt1[2];
        uint8 bt2[2];
        uint8 bp1[2];
        uint8 b11[2];  // A8
        uint8 bp2[2];
        uint8 b12[2];
        uint8 b21[2];
        uint8 bp3[2];  // B0
        uint8 a0[2];
        uint8 a1[2];
        uint8 a2[2];
        uint8 b00_a0_ex;  // B8
    } coe;

    // Set sub-address to COE register
    `$INSTANCE_NAME`_WriteDataToBuff(
        `$INSTANCE_NAME`_REG_COE
    );
    `$INSTANCE_NAME`_SendSequence();
    // Read a packet
    `$INSTANCE_NAME`_ReceiveSequence(
        (uint8 *)&coe,
        sizeof coe
    );
    
    {
        int32 x;
        x = (coe.a0[0] << 12) | (coe.a0[1] << 4) | (coe.b00_a0_ex & 0x0F);
        if (x & 0x80000) { x -= 0x100000u; } // Sign extend
        `$INSTANCE_NAME`_context.a0 = (double)x / 16.0d;
        
        `$INSTANCE_NAME`_CalculateK(coe.a1, -6.3e-03,  4.3e-04, &`$INSTANCE_NAME`_context.a1);
        `$INSTANCE_NAME`_CalculateK(coe.a2, -1.9e-11,  1.2e-10, &`$INSTANCE_NAME`_context.a2);
        
        x = (coe.b00[0] << 12) | (coe.b00[1] << 4) | ((coe.b00_a0_ex & 0xF0) >> 4);
        if (x & 0x80000) { x -= 0x100000u; } // Sign extend
        `$INSTANCE_NAME`_context.b00 = (double)x / 16.0d;

        `$INSTANCE_NAME`_CalculateK(coe.bt1,  1.0e-01,  9.1e-02, &`$INSTANCE_NAME`_context.bt1);
        `$INSTANCE_NAME`_CalculateK(coe.bt2,  1.2e-08,  1.2e-06, &`$INSTANCE_NAME`_context.bt2);
        `$INSTANCE_NAME`_CalculateK(coe.bp1,  3.3e-02,  1.9e-02, &`$INSTANCE_NAME`_context.bp1);
        `$INSTANCE_NAME`_CalculateK(coe.b11,  2.1e-07,  1.4e-07, &`$INSTANCE_NAME`_context.b11);
        `$INSTANCE_NAME`_CalculateK(coe.bp2, -6.3e-10,  3.5e-10, &`$INSTANCE_NAME`_context.bp2);
        `$INSTANCE_NAME`_CalculateK(coe.b12,  2.9e-13,  7.6e-13, &`$INSTANCE_NAME`_context.b12);
        `$INSTANCE_NAME`_CalculateK(coe.b21,  2.1e-15,  1.2e-14, &`$INSTANCE_NAME`_context.b21);
        `$INSTANCE_NAME`_CalculateK(coe.bp3,  1.3e-16,  7.9e-17, &`$INSTANCE_NAME`_context.bp3);
    }
}


//======================================================================
//  Function Name: `$INSTANCE_NAME`_IsMeasuring
//======================================================================
//
//  Summary:
//    Return if the sensor in measurement sequence.
//
//  Parameters:
//    None
//
//  Return:
//    true: if sensor is measring
//    false: if sensor is not measuring
//
//  Reentrant:
//    No
//
//======================================================================
uint32 `$INSTANCE_NAME`_IsMeasuring(void) {
    return 0;
}


//======================================================================
//  Function Name: `$INSTANCE_NAME`_Sleep
//======================================================================
//
//  Summary:
//    Enter to the SLEEP mode.
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
void `$INSTANCE_NAME`_Sleep(void) {
    `$INSTANCE_NAME`_WriteDataToBuff(
        `$INSTANCE_NAME`_REG_CTRL_MEAS
    );
    `$INSTANCE_NAME`_WriteDataToBuff(
        `$INSTANCE_NAME`_CTRL_MEAS_SLEEP
    );
    `$INSTANCE_NAME`_SendSequence();    
}


//======================================================================
//  Function Name: `$INSTANCE_NAME`_Force
//======================================================================
//
//  Summary:
//    Enter to the FORCE mode.
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
void `$INSTANCE_NAME`_Force(void) {
    `$INSTANCE_NAME`_WriteDataToBuff(
        `$INSTANCE_NAME`_REG_CTRL_MEAS
    );
    `$INSTANCE_NAME`_WriteDataToBuff(
        `$INSTANCE_NAME`_CTRL_MEAS_FORCE
    );
    `$INSTANCE_NAME`_SendSequence();    
}


//======================================================================
//  Function Name: `$INSTANCE_NAME`_ReadRawData
//======================================================================
//
//  Summary:
//    Read the raw data.
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
void `$INSTANCE_NAME`_ReadRawData(void) {
    struct {
        uint8 dp[3];
        uint8 dt[3];
    } txd;
    
    // Set sub-address to COE register
    `$INSTANCE_NAME`_WriteDataToBuff(
        `$INSTANCE_NAME`_REG_PRESS_TXD
    );
    `$INSTANCE_NAME`_SendSequence();
    // Read a packet
    `$INSTANCE_NAME`_ReceiveSequence(
        (uint8 *)&txd,
        sizeof txd
    );
    
    int32 t = ((txd.dt[0] << 16) | (txd.dt[1] << 8) | (txd.dt[2] << 0)) - 0x800000;
    int32 p = ((txd.dp[0] << 16) | (txd.dp[1] << 8) | (txd.dp[2] << 0)) - 0x800000;
    
    `$INSTANCE_NAME`_context.dt = (double)t;
    `$INSTANCE_NAME`_context.dp = (double)p;
}


//======================================================================
//  Function Name: `$INSTANCE_NAME`_GetTemperature
//======================================================================
//
//  Summary:
//    Return the calculated temperature value.
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
double `$INSTANCE_NAME`_GetTemperature(void) {
    struct `$INSTANCE_NAME`_Context *p = &`$INSTANCE_NAME`_context;    
    p->tr = (p->a2 * p->dt + p->a1) * p->dt + p->a0;
    p->temp = p->tr / 256.0;
    return p->temp;
}


//======================================================================
//  Function Name: `$INSTANCE_NAME`_GetPressure
//======================================================================
//
//  Summary:
//    Return the calculated pressure value.
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
double `$INSTANCE_NAME`_GetPressure(void) {
    struct `$INSTANCE_NAME`_Context *p = &`$INSTANCE_NAME`_context;
    double x3 = p->bp3;
    double x2 = p->b21 * p->tr + p->bp2;
    double x1 = (p->b12 * p->tr + p->b11) * p->tr + p->bp1;
    double x0 = (p->bt2 * p->tr + p->bt1) * p->tr + p->b00;
    p->pr = ((x3 * p->dp + x2) * p->dp + x1) * p->dp + x0;
    p->pressure = p->pr / 100.0;
    return p->pressure;
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
    // Clear status flags
    `$INSTANCE_NAME`_MasterClearStatus();
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


//======================================================================
//  Function Name: `$INSTANCE_NAME`_ReceiveSequence
//======================================================================
//
//  Summary:
//    Receives data from the I2C device and waits while transaction
//    completes.
//
//  Parameters:
//    rbuf: Buffer to store the received data.
//    length: Length of the buffer.
//
//  Return:
//    The length of received data.
//
//  Reentrant:
//    No
//
//======================================================================
static uint32 `$INSTANCE_NAME`_ReceiveSequence(uint8 *rbuf, uint32 length) {
    // Clear status flags
    `$INSTANCE_NAME`_MasterClearStatus();
    // Receive a I2C packet
    (void) `$INSTANCE_NAME`_MasterReadBuf(
        `$INSTANCE_NAME`_address,
        rbuf,
        length,
        `$INSTANCE_NAME`_MODE_COMPLETE_XFER
    );
    
    while (!(`$INSTANCE_NAME`_MasterReadStatus() & `$INSTANCE_NAME`_READ_COMPLETE)) {
        // Wait until I2C Master finishes transaction
    }
    
    // Return the data length
    return `$INSTANCE_NAME`_MasterGetReadBufSize();
}


/* [] END OF FILE */
