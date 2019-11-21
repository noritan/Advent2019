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

    for (;;) {
        CyDelay(500L);
        Pin_LED_Write(!Pin_LED_Read());
    }
}

/* [] END OF FILE */
