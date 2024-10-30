#include "mo.h"
#include "grbl.h"

volatile int numCalls = 0;

void mc_alarm() {
    sys.state = ALARM_HOMING_FAIL_DOOR;  // Set system state to alarm
    report_alarm_message(ALARM_HOMING_FAIL_DOOR);  // Report hard limit alarm to the user (you can choose another alarm code)
    st_go_idle();  // Stop steppers or take the machine to an idle state
}

// Check the state of pin PC3
void check_pc3_state() {
    pinMode(17, INPUT);
    int pc3State = digitalRead(17); // Assuming PC3 is configured correctly
    if (pc3State == LOW && sys.state != ALARM_HOMING_FAIL_DOOR) {
        mc_alarm();  // Call the function to set the system state to alarm
    } 
    // } else if (pc3State == HIGH && sys.state == ALARM_HOMING_FAIL_DOOR) {
    //     sys.state = STATE_IDLE;  // Set system state to idle
    // }
}

/**
 * Pinch timer is set at 1000hz (the minimum)
 */
void setup_pinch_timer() {
    TCCR2A = 0; // set entire TCCR2A register to 0
    TCCR2B = 0; // same for TCCR2B
    TCNT2  = 0; // initialize counter value to 0
    // set compare match register for 1000 Hz increments
    OCR2A = 249; // = 16000000 / (64 * 1000) - 1 (must be <256)
    // turn on CTC mode
    TCCR2B |= (1 << WGM21);
    // Set CS22, CS21 and CS20 bits for 64 prescaler
    TCCR2B |= (1 << CS22) | (0 << CS21) | (0 << CS20);
    // enable timer compare interrupt
    TIMSK2 |= (1 << OCIE2A);
}

// ISR for Timer2
ISR(TIMER2_COMPA_vect) {
    numCalls += 1;
    if (numCalls > 1000) {
        check_pc3_state();  // Call the function to check the PC3 state
        numCalls = 0;
    }
}
