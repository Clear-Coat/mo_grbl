#include "grbl.h"

#define CS1_PIN PC0
#define CS2_PIN PC1

#define DEBUG_SPI

#ifdef DEBUG_SPI
void debug_print_read(uint8_t addr, uint8_t data) {
    serial_write('R');
    serial_write('[');
    if (addr < 16) serial_write('0');
    serial_write((addr < 10) ? ('0' + addr) : ('A' + addr - 10));
    serial_write(']');
    serial_write('=');

    // Print data as raw hex
    uint8_t *data_ptr = &data;
    for (int i = 0; i < sizeof(data); i++) {
        uint8_t byte = *(data_ptr + i);
        if (byte < 16) serial_write('0');
        serial_write((byte >> 4) < 10 ? ('0' + (byte >> 4)) : ('A' + (byte >> 4) - 10));
        serial_write((byte & 0xF) < 10 ? ('0' + (byte & 0xF)) : ('A' + (byte & 0xF) - 10));
    }
    serial_write('\n');
}
#endif

void spi_disable(void) {
    // Disable SPI by clearing the SPE (SPI Enable) bit in SPCR
    SPCR &= ~(1 << SPE);
    
    // Optional: Set SPI pins as inputs to save power
    // SS (PB2), MOSI (PB3), MISO (PB4), SCK (PB5) on ATmega328P
    DDRB &= ~((1 << PB2) | (1 << PB3) | (1 << PB4) | (1 << PB5));
    
    // Optional: Disable internal pull-ups on SPI pins
    PORTB &= ~((1 << PB2) | (1 << PB3) | (1 << PB4) | (1 << PB5));
}

void SPI_init() {
    // ? Set PB4 to input
    DDRB &= ~(1 << PB4);
    // ? Set PB3, PB5, PB2 to output
    DDRB |= (1 << PB3) | (1 << PB5) | (1 << PB2);
    DDRC |= (1 << CS1_PIN) | (1 << CS2_PIN);
    PORTC |= (1 << CS1_PIN) | (1 << CS2_PIN);
    // Slower SPI clock: SPR1=1, SPR0=1 = fosc/128
    SPCR = (1 << SPE) | (1 << MSTR) | (1 << SPR1) | (1 << SPR0) | (1 << CPHA);
}

void SPI_write(uint8_t cs_pin, uint8_t addr, uint8_t data) {
    // ? First frame should be 00<address bits>
    // ? Second frame should be all data bits
    uint16_t frame = ((addr & 0x3F) << 8) | data;
    
    // ? Set CS pin low
    PORTC &= ~(1 << cs_pin);
    
    SPDR = frame >> 8;
    while(!(SPSR & (1 << SPIF)));
    
    // ? Mask out high byte, 0xFF = 0000000011111111
    // ? Send low byte
    SPDR = frame & 0xFF;
    while(!(SPSR & (1 << SPIF)));
    
    // ? Set CS pin high
    PORTC |= (1 << cs_pin);
}

uint16_t SPI_read(uint8_t cs_pin, uint8_t addr) {
    uint16_t frame = (1 << 14) | ((addr & 0x3F) << 8);  // W0=1 (read)

    PORTC &= ~(1 << cs_pin);
    _delay_us(1);  // CS setup time

    SPDR = frame >> 8;
    while (!(SPSR & (1 << SPIF)));

    uint8_t high = SPDR;

    SPDR = frame & 0xFF;
    while (!(SPSR & (1 << SPIF)));

    uint8_t low = SPDR;

    PORTC |= (1 << cs_pin);
    _delay_us(1);  // CS hold time

    uint16_t response = (high << 8) | low;

    #ifdef DEBUG_SPI
    serial_write('R'); serial_write('[');
    if (addr < 16) serial_write('0');
    serial_write((addr < 10) ? ('0' + addr) : ('A' + addr - 10));
    serial_write(']');
    serial_write('=');
    uint8_t data = response & 0xFF;
    uint8_t status = (response >> 8) & 0xFF;

    serial_write((data >> 4) < 10 ? ('0' + (data >> 4)) : ('A' + (data >> 4) - 10));
    serial_write((data & 0xF) < 10 ? ('0' + (data & 0xF)) : ('A' + (data & 0xF) - 10));
    serial_write(' ');

    serial_write('S'); serial_write('=');
    serial_write((status >> 4) < 10 ? ('0' + (status >> 4)) : ('A' + (status >> 4) - 10));
    serial_write((status & 0xF) < 10 ? ('0' + (status & 0xF)) : ('A' + (status & 0xF) - 10));
    serial_write('\n');
    #endif

    return response;
}


void motor_spi_init() {
    if (MACHINE_TYPE != BAMBOO) return;
    
    _delay_ms(100);
    SPI_init();
    
    for (uint8_t cs = CS1_PIN; cs <= CS2_PIN; cs++) {
        // Write configuration first
        SPI_write(cs, 0x04, 0xCF);
        _delay_ms(1);
        SPI_write(cs, 0x05, 0x06);
        _delay_ms(5);  // Longer delay after write
        
        // Then read back to verify
        SPI_read(cs, 0x04);
        SPI_read(cs, 0x05);
    }

    spi_disable();
}