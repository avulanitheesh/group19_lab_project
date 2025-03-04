
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
// Output string to UART
void UART_OutString(char *pt) {
    while (*pt) {
        while ((UART0_FR_R & 0x20) != 0);      // Wait until TXFF is empty
        UART0_DR_R = *pt;                      // Output character
        pt++;
    }
}

// Input string from UART
void UART_InString(char *buf, uint16_t max) {
    char c;
    int i = 0;
    while (i < max - 1) {
        while ((UART0_FR_R & 0x10) != 0); // Wait until RXFE is empty
        c = UART0_DR_R;
        if (c == '\r') break;  // Stop at carriage return
        buf[i] = c;
        i++;
    }
    buf[i] = '\0'; // Null-terminate the string
}
// Get frequency from the user
int getFrequencyFromUser(void) {
    char buf[10];
    int frequency;

    UART_OutString("Enter frequency (Hz): ");
    UART_InString(buf, 10);
    frequency = atoi(buf);  // Convert string to integer
    return frequency;
}

// Get waveform type from the user
int getWaveformTypeFromUser(void) {
    char buf[10];
    int waveformType;

    UART_OutString("Select waveform: 1. Sine, 2. Triangle, 3. Sawtooth, 4. Square\n");
    UART_InString(buf, 10);
    waveformType = atoi(buf);  // Convert string to integer
    return waveformType;
}
// Initialize I2C0 for DAC communication
void I2C0_Init(void) {
    SYSCTL_RCGCI2C_R |= 0x01;    // Activate I2C0
    SYSCTL_RCGCGPIO_R |= 0x02;   // Activate Port B
    GPIO_PORTB_AFSEL_R |= 0x0C;  // Enable alt function on PB2-3
    GPIO_PORTB_ODR_R |= 0x08;    // Enable open drain on PB3
    GPIO_PORTB_DEN_R |= 0x0C;    // Enable digital I/O on PB2-3
    I2C0_MCR_R = 0x10;           // Initialize I2C Master
    I2C0_MTPR_R = 24;            // Set SCL clock speed
}

// Write 12-bit data to MCP4725 DAC
void I2C0_Write(uint8_t device_addr, uint16_t data) {
    I2C0_MSA_R = (device_addr << 1); // Set slave address and write
    I2C0_MDR_R = data >> 8;          // Transmit high byte first
    I2C0_MCS_R = 0x03;               // Start and run
    while (I2C0_MCS_R & 0x01);       // Wait for transmission
    I2C0_MDR_R = data & 0xFF;        // Transmit low byte
    I2C0_MCS_R = 0x05;               // Run and stop
    while (I2C0_MCS_R & 0x01);       // Wait for transmission
}
// 32 us delay function
void delay_32us() {
    SYSCTL_RCGCTIMER_R |= 2;         // Enable clock to Timer 1
    TIMER1_CTL_R = 0;                // Disable Timer 1
    TIMER1_CFG_R = 0x04;             // 16-bit timer
    TIMER1_TAMR_R = 0x02;            // Periodic mode
    TIMER1_TAILR_R = 512;            // Set interval for 32 us
    TIMER1_ICR_R = 0x01;             // Clear timeout flag
    TIMER1_CTL_R |= 0x01;            // Enable Timer 1
    while ((TIMER1_RIS_R & 0x01) == 0); // Wait for timeout
}
// Disable global interrupts
void DisableInterrupts(void) {
    __asm(" CPSID I");  // Disable interrupts
}

// Enable global interrupts
void EnableInterrupts(void) {
    __asm(" CPSIE I");  // Enable interrupts
}
