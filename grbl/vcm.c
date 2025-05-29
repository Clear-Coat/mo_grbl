#include "grbl.h"

void vcm_init()
{
    if (MACHINE_TYPE == BAMBOO) 
    {
        DDRD |= (1 << DDD7);
        // Set pin 7 to LOW
        PORTD |= (1 << PORTD7);
    }
}