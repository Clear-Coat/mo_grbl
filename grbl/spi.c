#include "grbl.h"

#define CS1_PORT PORTC  // Chip Select for Driver 1
#define CS1_PIN  PC0

#define CS2_PORT PORTC  // Chip Select for Driver 2
#define CS2_PIN  PC1

#define SPI_DDR DDRB
#define SPI_PORT PORTB
#define MOSI PB3
#define MISO PB4
#define SCK PB5

void spi_init() {
    SPI_DDR |= (1 << MOSI) | (1 << SCK);  // Set MOSI and SCK as output
    SPI_DDR &= ~(1 << MISO);             // Set MISO as input
    SPCR = (1 << SPE) | (1 << MSTR) | (1 << SPR0); // Enable SPI, Master, Fclk/16
}

void spi_write(uint8_t cs_port, uint8_t cs_pin, uint8_t address, uint8_t data) {
    // Pull CS low to select device
    cs_port &= ~(1 << cs_pin);
    _delay_us(1);  // Short delay for setup

    // Send address
    SPDR = address;
    while (!(SPSR & (1 << SPIF)));  // Wait for transmission to complete

    // Send data
    SPDR = data;
    while (!(SPSR & (1 << SPIF)));  // Wait for transmission to complete

    // Pull CS high to deselect device
    cs_port |= (1 << cs_pin);
    _delay_us(1);  // Short delay for setup
}

/**
 *  CTRL1 Control Register (address = 0x04) [Default = 0Fh]
 *  BIT = 2-0
 *  SETTING = DECAY MODE
 *  111b = Smart tune Ripple Control
 */
void motor_spi_init() {
    if (MACHINE_TYPE == BAMBOO) {
        // Configure Chip Select pins as outputs
        DDRC |= (1 << CS1_PIN) | (1 << CS2_PIN);
        CS1_PORT |= (1 << CS1_PIN);  // Set CS high (deselect)
        CS2_PORT |= (1 << CS2_PIN);  // Set CS high (deselect)

        // Send configuration to Driver 1
        spi_write(CS1_PORT, CS1_PIN, 0x04, 0x07);

        // Send configuration to Driver 2
        spi_write(CS2_PORT, CS2_PIN, 0x04, 0x07);
    }
}