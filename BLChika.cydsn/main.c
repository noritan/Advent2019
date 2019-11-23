/* ========================================
 *
 * Copyright noritan.org, 2019
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF noritan.org.
 *
 * ========================================
*/
#include "project.h"

#define     LED_ADDRESS     (0xC4)
#define     REG_MODE1       (0x00)
#define     REG_PWM         (0x02)
#define     REG_LEDOUT      (0x08)
#define     REG_CONTINUOUS  (0x80)
#define     MODE1_ENABLE    (0x00)
#define     MODE1_DISABLE   (0x10)
#define     LEDOUT_ENABLE   (0xAA)
#define     LEDOUT_DISABLE  (0x00)
#define     BUFF_SIZE       (16)

static uint8_t  buff[BUFF_SIZE];

void SendBuffer(uint32_t size) {
    I2CM_I2CMasterWriteBuf(
        LED_ADDRESS >> 1,
        buff,
        size,
        I2CM_I2C_MODE_COMPLETE_XFER
    );
    while (!(I2CM_I2CMasterStatus() & I2CM_I2C_MSTAT_WR_CMPLT)) {
        // Wait until I2C Master finishes transaction
    }
}

int main(void) {
    CyGlobalIntEnable; /* Enable global interrupts. */

    I2CM_Start();
    
    // Setup LCD display
    I2C_LCD_Start();
    I2C_LCD_PrintString("Hello, World!");
    
    // Setup Backlight LED
    buff[0] = REG_MODE1;
    buff[1] = MODE1_ENABLE;
    SendBuffer(2);
    buff[0] = REG_LEDOUT;
    buff[1] = LEDOUT_ENABLE;
    SendBuffer(2);
    buff[0] = REG_CONTINUOUS | REG_PWM;
    buff[1] = 0x33;
    buff[2] = 0x33;
    buff[3] = 0x88;
    SendBuffer(4);
    
    for (;;) {
        /* Place your application code here. */
    }
}

/* [] END OF FILE */
