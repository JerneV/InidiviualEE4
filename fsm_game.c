/*********************************************************************
 *
 *                  EE 4 Project - Code Template
 *
 *********************************************************************
 * Processor:       PIC18F25K50
 * Compiler:        XC8 1.35+
 * Author:          Jeroen Van Aken
 * Updated:         03/03/2020
 ********************************************************************/
/** I N C L U D E S *************************************************/
#include "config.h"
#include <stdlib.h>
#include <math.h>

/** D E F I N E S ***************************************************/
#define PUSHED 0

/** P R I V A T E   V A R I A B L E S *******************************/
static enum {
    IDLE, SHOW, EXIT
} current_state;

// Used for the EVEN/UNEVEN LEDs
unsigned int timer = 0;
unsigned int counter = 0;

// All variables for the NIGHRIDER sequence
unsigned int nightRider = 50;
unsigned char upCount = 0;
unsigned char downCount = 0;
unsigned int upTimer = 0;
unsigned int downTimer = 0;
unsigned char nCount = 0;

// Varuables used for logic
unsigned int resistor;
unsigned char currentResistor = 0;


// Booleans
unsigned char isUp = 1;
unsigned char correctValue = 0;

// These values are used for the display of the resistors we expect.
unsigned int rValues[8] = {23,128,176,197,215,223,232,245};


/********************************************************************
 * Function:        void fsm_game_init(void)
 * PreCondition:    None
 * Input:           None
 * Output:          None
 * Overview:        This init sets the start state for this FSM and
 *                  may initialize some counters          
 ********************************************************************/
void fsm_game_init(void) {
    current_state = IDLE;
}


void fsm_game(void) {
    switch (current_state) {
        case(IDLE):
            //blink odd even
            if (timer <= 500) {
                timer += 1;
                LATB = 85;
            } else if (timer > 500) {
                timer += 1;
                LATB = 170;
            }
            if (timer > 1000) {
                timer = 0;
            }

            if (PRG_BUTTON == PUSHED) {
                current_state = SHOW;
                correctValue = 0;
                timer = 0;
            }

            break;

        case(SHOW):
            // Loop through all the values in the rValues array.
           LATB = rValues[currentResistor];
        
            // Read the value of the ADC
            resistor = ADC_value[0];
            // Start ADC for the next Value
            startADC();
                          
            if(88 >= resistor && resistor <= 97 && currentResistor == 0){
                // 1k
                correctValue = TRUE;
                
            }else if(486 >= resistor && resistor <= 537 && currentResistor == 1){
                //10k
                correctValue = TRUE;
                
            }else if(668 >= resistor && resistor <= 739 && currentResistor == 2){
                //22k
                correctValue = TRUE;
                
            }else if(746 >= resistor && resistor <= 825 && currentResistor == 3){
                //33k
                correctValue = TRUE;
                
            }else if(802 >= resistor && resistor <= 886 && currentResistor == 4){
                //47k
                correctValue = TRUE;
                
            }else if(848 >= resistor && resistor <= 937 && currentResistor == 5){
                //68k
                correctValue = TRUE;
                
            }else if(884 >= resistor && resistor <= 977 && currentResistor == 6){
                //100k
                correctValue = TRUE;
                
            }else if(930 >= resistor && resistor <= 1024 && currentResistor == 7){
                //220k
                correctValue = TRUE;
            }
            
            if(correctValue){
                // Set the next state to EXIT
                current_state = EXIT;
                // Reset the amount of times Nightrider has played
                nCount = 0;
                // Go to the next value in the array, this will go out of array boundaries (if it works)
                if(currentResistor <= 7){
                    currentResistor += 1;
                }else{
                    currentResistor = 0;
                }
                //SET LATB to 0000 0001 so we can shift bits later on for the nightrider sequence.
                LATB = 0x01;  
            }
                

            break;

        case(EXIT):
            //show nightrider
            if (isUp == 1) {
                if (upTimer >= nightRider) {
                    if (upCount < 7) {
                        upCount += 1;
                        LATB = LATB << 1;
                        upTimer = 0;
                    } else if (upCount >= 7) {
                        upCount = 0;
                        nCount +=1;
                        isUp = 0;
                        upTimer = 0;
                    }

                } else {
                    upTimer += 1;
                }

            } else if (isUp == 0) {
                if (downTimer >= nightRider) {
                    if (downCount < 7) {
                        downCount += 1;
                        LATB = LATB >> 1;
                        downTimer = 0;
                    } else if (downCount >= 7) {
                        downCount = 0;
                        nCount += 1;
                        isUp = 1;
                        downTimer = 0;
                    }
                } else {
                    downTimer += 1;
                }
            }
            
            if(nCount == 4){
                current_state = IDLE;
            }

            break;


        default:
            current_state = IDLE;
            break;


    }

}



