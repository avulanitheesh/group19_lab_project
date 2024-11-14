
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
