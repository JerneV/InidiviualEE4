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

/** D E F I N E S ***************************************************/
#define PUSHED 0

/** P R I V A T E   V A R I A B L E S *******************************/
static enum {
    IDLE, SHOW, EXIT
} current_state;

unsigned int timer = 0;
unsigned int counter = 0;

unsigned int nightRider = 50;
unsigned char upCount = 0;
unsigned char downCount = 0;
unsigned int upTimer = 0;
unsigned int downTimer = 0;
unsigned char nCount = 0;

unsigned int ldr_1;

// Boolean! 
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

/********************************************************************
 * Function:        void fsm_game(void)
 * PreCondition:    None
 * Input:           None
 * Output:          None
 * Overview:        An implementation for a simple reaction game
 ********************************************************************/

unsigned char count = 0;

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
            //DO game LOGIc
            LATB = rValues[count];
            
            ldr_1 = ADC_value[count];
            startADC();

            if ( rValues[count] - 20 <= ldr_1 <= rValues[count] + 20){
                current_state = EXIT;
                //SET LATB to 0000 0001 so we can shift bits later on.
                nCount = 0;
                count += 1;
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

void isInBoundary(unsigned int value){
    
}


