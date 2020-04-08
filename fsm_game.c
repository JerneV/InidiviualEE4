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
static enum {LED_OFF, LED_EVEN, LED_ODD
            } current_state;
            
unsigned int timer = 0;

unsigned int ADC_READ = 0;
            
/********************************************************************
 * Function:        void fsm_game_init(void)
 * PreCondition:    None
 * Input:           None
 * Output:          None
 * Overview:        This init sets the start state for this FSM and
 *                  may initialize some counters          
 ********************************************************************/
void fsm_game_init(void) {
	current_state = LED_OFF;
}

/********************************************************************
 * Function:        void fsm_game(void)
 * PreCondition:    None
 * Input:           None
 * Output:          None
 * Overview:        An implementation for a simple reaction game
 ********************************************************************/
void fsm_game(void) {
    //For now we'll just read it each time.
    
    
    switch (current_state) { 
        case LED_OFF :
        // *** outputs ***
            LATB = 0;
                        
            //if (PRG_BUTTON == PUSHED){
            if(ADC_value[0] > 400){
                current_state = LED_EVEN;
                
            }
            break;
            
        case LED_EVEN :
            if(timer <= 1000){
                timer +=1;
                LATB = 85;

            }else{
                current_state = LED_ODD;
                timer = 0;
            }
                
            
            break;
            
        case LED_ODD:
            if(timer <= 1000){
                timer += 1;
                LATB = 170;
            }else{
                current_state = LED_EVEN;
                timer = 0;
            }
            
            
            break;

        default:
            current_state = LED_OFF;
            break;
    }
    
}
