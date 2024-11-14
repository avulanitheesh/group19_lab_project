
#include <stdint.h>
#include <stdlib.h>

#include "tm4c123gh6pm.h"
#include "Waveforms.h" // Lookup tables for different waveforms

void DisableInterrupts(void);
void EnableInterrupts(void);
void UART_Init(void);
void UART_OutString(char *pt);
void UART_InString(char *buf, uint16_t max);
void I2C0_Init(void);
void I2C0_Write(uint8_t device_addr, uint16_t data);
int getFrequencyFromUser(void);
int getWaveformTypeFromUser(void);

extern int wt[4][120]; // Lookup tables for waveforms

void delay_32us();

// MCP4725 I2C address
#define MCP4725_ADDR 0x60

// Function to output the selected waveform
void OutputWaveform(int waveformType, int frequency) {
    int i, k = 0;
    int delayCycles = (int)(100000 / (96 * frequency)); // Delay adjustment based on frequency

    while (1) {
        // Send the next sample of the waveform to the DAC
        I2C0_Write(MCP4725_ADDR, wt[waveformType - 1][k]);

        // Apply delay for the selected frequency
        for (i = 0; i < delayCycles; i++) {
            delay_32us();
        }

        // Increment sample index
        k = (k + 1) % 120;

        // Check if the user wants to change the waveform or frequency
        if ((UART0_FR_R & 0x10) == 0) {  // Non-blocking check if there is input in UART buffer
            break;  // Exit loop if user input is detected
        }
    }
}
int main(void) {
    int waveformType = 1;
    int frequency = 10;

    // Initialize UART for console input
    UART_Init();
    I2C0_Init(); // Initialize I2C for DAC communication
    EnableInterrupts(); // Enable global interrupts

    while (1) {
        // Get user input for waveform type and frequency
        waveformType = getWaveformTypeFromUser();
        if (waveformType < 1 || waveformType > 4) {
            UART_OutString("Invalid waveform selection. Please select between 1 and 4.\n");
            continue;
        }

        frequency = getFrequencyFromUser();
        if (frequency <= 0) {
            UART_OutString("Invalid frequency. Please enter a positive integer.\n");
            continue;
        }

        UART_OutString("Generating waveform...\n");

        // Output the waveform based on user selection
        OutputWaveform(waveformType, frequency);
    }
}
// Initialize UART0 for console input
void UART_Init(void) {
    SYSCTL_RCGCUART_R |= 0x01;   // Activate UART0
    SYSCTL_RCGCGPIO_R |= 0x01;   // Activate Port A
    UART0_CTL_R &= ~0x01;        // Disable UART0
    UART0_IBRD_R = 104;          // 9600 baud rate, assuming 16 MHz system clock
    UART0_FBRD_R = 11;
    UART0_LCRH_R = 0x0070;       // 8-bit word length, enable FIFO
    UART0_CTL_R = 0x301;         // Enable UART0
    GPIO_PORTA_AFSEL_R |= 0x03;  // Enable alt function on PA1-0
    GPIO_PORTA_DEN_R |= 0x03;    // Enable digital I/O on PA1-0
}

