/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/
#include "project.h"
#include <stdio.h>

int main(void) {
    CyGlobalIntEnable; /* Enable global interrupts. */

    I2CM_Start();
    
    // Setup LCD display
    I2C_LCD_Start();
    
    // Setup Backlight LED
    I2C_LED_Start();
    I2C_LED_WritePWM(0x33, 0x33, 0x88, 0x00);

    // Startup Pressure Sensor
    I2C_SENSOR_Start();
    
    for (;;) {
        double temp;
        double pressure;
        char sbuf[64];
        
        I2C_SENSOR_Force();
        CyDelay(1000);
        I2C_SENSOR_ReadRawData();
        temp = I2C_SENSOR_GetTemperature();
        pressure = I2C_SENSOR_GetPressure();
        sprintf(sbuf, "T:%7.2fdegC", temp);
        I2C_LCD_Position(0, 0x80);  // locate to 0,0
        I2C_LCD_PrintString(sbuf);
        sprintf(sbuf, "P:%7.2fmBar", pressure);
        I2C_LCD_Position(0, 0xA8);  // locate to 1,0
        I2C_LCD_PrintString(sbuf);
    }
}

/* [] END OF FILE */
