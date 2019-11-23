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

int main(void) {
    CyGlobalIntEnable; /* Enable global interrupts. */

    I2CM_Start();
    
    // Setup LCD display
    I2C_LCD_Start();
    I2C_LCD_PrintString("Hello, World!");
    
    // Setup Backlight LED
    I2C_LED_Start();
    I2C_LED_WritePWM(0x33, 0x33, 0x88, 0x00);
    
    for (;;) {
        /* Place your application code here. */
    }
}

/* [] END OF FILE */
