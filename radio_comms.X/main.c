/*
 * File:   main.c
 * Author: FSAE
 *
 * Created on December 15, 2017, 7:18 PM
 */


#include <xc.h>
#include "mcc_generated_files/mcc.h";

uCAN_MSG canMessage;

void radio_timeout() {
    AUDIO_ON_SetHigh();
    LED_RED_SetLow();
    LED_BLUE_SetHigh();
    LED_GREEN_SetHigh();
}

void main(void) {
    SYSTEM_Initialize();
    
    //CAN config 
    CIOCONbits.CLKSEL = 1;
    CIOCONbits.ENDRHI = 1;
    CIOCONbits.TX2SRC = 0;
    CIOCONbits.TX2EN = 0; 
    TMR1_SetInterruptHandler(&radio_timeout);
    
    //Connect to bluetooth headphone
    __delay_ms(10000);
    EUSART1_Write('R');
    EUSART1_Write('I');
    EUSART1_Write('N');
    EUSART1_Write('G');
    EUSART1_Write('\n');
    EUSART1_Write('\r');
    
    AUDIO_ON_SetLow();
    __delay_ms(2000);
    AUDIO_ON_SetHigh();
    
    while (1) {
        if (CAN_receive(&canMessage)) {
            if (canMessage.frame.id == 0x643) {
                if(canMessage.frame.data0 >> 7) {
                    AUDIO_ON_SetLow();
                    LED_GREEN_SetLow();
                    LED_BLUE_SetHigh();
                    TMR1_StartTimer();
                } else {
                    AUDIO_ON_SetHigh();
                    LED_BLUE_SetLow();
                    LED_GREEN_SetHigh();
                    TMR1_StopTimer();
                }
                
            }
        }
    }
    
    return;
}
