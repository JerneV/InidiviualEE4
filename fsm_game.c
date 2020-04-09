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
static enum {LED_OFF, LED_UP, LED_DOWN
            } current_state;
            
unsigned int timer = 0;

//unsigned int ADC_READ = ADC_value[0];

unsigned int nightRider = 50;

unsigned char upCount = 0;
unsigned char downCount = 0;

unsigned int upTimer = 0;
unsigned int downTimer = 0;
            
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
    switch (current_state) { 
        case LED_OFF :
        // *** outputs ***
            LATB = 0x00;
                        
            //if (PRG_BUTTON == PUSHED){
            if(ADC_value[0] > 50){
                current_state = LED_UP;
                LATB = 0x01;
                
            }
            break;
            
        case LED_UP :
            if(upTimer >= nightRider){
                if(upCount < 7){
                    upCount +=1;
                    LATB = LATB << 1;
                    upTimer = 0;
                }else if(upCount >= 7){
                    upCount = 0;
                    current_state = LED_DOWN;
                    upTimer = 0;
                }
            }else{
                upTimer += 1;
            }
            break;
            
        case LED_DOWN:
            if(downTimer >= nightRider){
               if(downCount < 7){
                downCount +=1;
                LATB = LATB >> 1;
                downTimer = 0;
            }else if(downCount >= 7){
                downCount = 0;
                current_state = LED_UP;
                downTimer = 0;
            }           
             
            }else{
                downTimer += 1;
            }
            break;
            
        default: 
            current_state = LED_OFF;
            break;
            

    }
    
}
