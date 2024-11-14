
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
