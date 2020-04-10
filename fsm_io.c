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

/** P R I V A T E   V A R I A B L E S *******************************/
static unsigned char servo_counter;
static unsigned char stepper_counter;
static unsigned char pwm_counter;
static enum {FSM_SETIO,
             FSM_UPDATE_SERVO,
             FSM_UPDATE_STEPPER_L,
             FSM_UPDATE_STEPPER_H,
            } current_state;
            
/********************************************************************
 * Function:        void fsm_io_init(void)
 * PreCondition:    None
 * Input:           None
 * Output:          None
 * Overview:        This init sets the start state for this FSM and
 *                  may initialize some counters          
 ********************************************************************/
void fsm_io_init(void) {
	current_state = FSM_SETIO;
    ADC_init();
}

/********************************************************************
 * Function:        fsm_io(void)
 * PreCondition:    None
 * Input:           None
 * Output:          None
 * Overview:        A simple FSM that will start blinking a led on 
 *                  and off once you pushed a button.          
 ********************************************************************/
void fsm_io(void) {
    switch (current_state) {                
        case FSM_SETIO :
        // *** inputs ***
            

        // *** outputs ***
            servo_counter++;
            //LED1_OUT = led1_output;
            
            // incrementing the PWM counter and setting the outputs
            //pwm_counter = (pwm_counter + 1u) % 10;
            // generating a PWM signal on one (or more) of the outputs
            //PWM_OUT = (unsigned)(PWM_duty[0] > pwm_counter);
            
        // *** transitions ***
            // update the servo position only every 10 cycles
            if (servo_counter >= 10) current_state = FSM_UPDATE_SERVO;
            break;
        
        case FSM_UPDATE_SERVO :
        // *** outputs ***
            // loop over all servo channels and set position
            for (char i = 0; i < SERVO_CHANNELS; i++) {
                //SERVO_setPosition(i, servo_setpoint[i]);
            }
            servo_counter = 0;
        // *** transitions ***
            current_state = FSM_SETIO;
            break;
            
        default:
            current_state = FSM_SETIO;
            break;
    }
    
}
