/* ========================================
 *
 * Copyright noritan.org, 2019
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF noritn.org.
 *
 * ========================================
*/
#include "project.h"

int main(void) {
    CyGlobalIntEnable; /* Enable global interrupts. */

    I2CM_Start();
    I2C_LCD_Start();
    
    for (uint32_t i = 0x30; i <= 0x6F; i++) {
        I2C_LCD_PutChar(i);
    }

    for (;;) {
        /* Place your application code here. */
    }
}

/* [] END OF FILE */
