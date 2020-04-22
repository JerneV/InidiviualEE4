/*********************************************************************
 *
 *                  EE 4 Project
 *
 *********************************************************************
 * Processor:       PIC18F25K50
 * Compiler:        XC8 1.35+
 * Author:          Jerne Vingerhoets
 * Updated:         21/04/2020
 * With thanks to Daan Witters for showing me the EXPECTED_VALUES array and why
 * my solution firstly didn't work.
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
unsigned char error = 15;

// Varuables used for logic
unsigned int resistor;
unsigned char currentResistor = 0;
unsigned int randomCounter = 0;


// Booleans
unsigned char isUp = 1;
unsigned char correctValue = FALSE;

// These values are used for the display of the resistors we expect.
static unsigned int LED_RES[8] = {1,10,22,33,47,68,100,220};
// the expected values for the ADC
static unsigned int EXPECTED_VALUES[8] = {93,512,704,786,844,893,930,980};

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
            randomCounter += 1;
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
            //First we set a seed for the random generator. Otherwise we will see the same values
            srand(randomCounter); 
            // Get a new random value
            int r = rand(); 
            // Make LATB show the resistor value we expect in binary
            LATB = LED_RES[r%8];
            // take a random value from our expected values
            int ADC_EXPECTED = EXPECTED_VALUES[r%8];
            // Start ADC for the next value
            startADC();
            // To adjust for possible misreads take in account a certain error value
            if((ADC_value[0] > (ADC_EXPECTED - error) )&&( ADC_value[0] < (ADC_EXPECTED + error) )){
                // Prepare for nightrider
                LATB = 0x01;
                // Go to nighrider
                current_state = EXIT;
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
            
            if(nCount >= 4){
                nCount = 0;
                current_state = IDLE;
            }

            break;


        default:
            current_state = IDLE;
            break;


    }

}



