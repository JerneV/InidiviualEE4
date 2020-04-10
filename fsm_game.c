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
unsigned char count = 0;


// Boolean! For nightrider 
unsigned char isUp = 1;

//unsigned int rValues[8] = {0x7A, 0xC0, 0x83,0xE8, 0xFA, 0xFE, 0xE5, 0xEB};
unsigned int rValues[8] = {0x5E, 0x03, 0xC1, 0x17, 0x5F, 0x7F, 0xAF, 0xD7};

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
                timer = 0;
            }

            break;

        case(SHOW):
            // For now: just show the first value in the array
            LATB = rValues[count];
            
            // Read the next value of the ADC
            resistor = ADC_value[0];
            startADC();
            
            unsigned int error = floor(resistor * 0.10);
            unsigned int expected = rValues[count];
            
            // we want the measured value to be in a certain range
            if(expected - error <= resistor <= expected + error){
            //if(resistor == rValues[count]){
                // Set the next state to EXIT
                current_state = EXIT;
                // Reset the amount of times Nightrider has played
                nCount = 0;
                // Just so I can see all the values in the array
                count += 1;
                //SET LATB to 0000 0001 so we can shift bits later on.
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



