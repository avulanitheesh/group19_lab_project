# group19_lab_project
# Signal Generator using TIVA Microcontroller and MCP4725 DAC

This repository contains the code for developing a signal generator with variable frequency, using a TIVA C Series Launchpad (TM4C123G) and an MCP4725 Digital-to-Analog Converter (DAC). The project enables the generation of four types of waveforms at varying frequencies, all of which can be controlled via a console interface in Code Composer Studio (CCS).

## Project Overview

The program outputs different waveforms (sine, square, sawtooth, and triangle) at adjustable frequencies. The waveform data is stored in a lookup table (LUT), with each waveform represented by samples. By sending these samples to the MCP4725 DAC via the I2C protocol, the program generates analog waveforms from digital values.

### Prerequisites

- Hardware:
  - TIVA C Series Launchpad (TM4C123G)
  - MCP4725 DAC (I2C-based DAC)
  - Oscilloscope or suitable device to observe the generated waveform

- Software:
  - Code Composer Studio (CCS) for TIVA development
  - Git to upload the work 

### Functions

Four Waveform Types  : Sine, Square, Sawtooth, and Triangle
Frequency Control    : Variable frequency, adjustable from the console interface
Console Interface    : Control waveform type and frequency directly from the CCS console, removing the need for physical switches

### How It Works
1. UART Console Control:
   - Upon startup, the program initializes UART for console-based communication, allowing users to set the waveform type and frequency directly from CCS's console interface.
   - Users can specify the waveform type (sine, square, sawtooth, triangle) and frequency in Hz.

2. I2C Communication with MCP4725:
   - The MCP4725 DAC is connected via I2C. Each sample from the selected waveform’s LUT is sent to the DAC to produce the corresponding voltage output.
   
3. Waveform Generation:
   - A delay loop of 32 µs between samples is used to control the frequency of the generated waveform.
   - The frequency is adjusted by controlling the delay between samples in the loop.

### Console Commands
Once the code is running, enter the following commands in the console:

1. Set Frequency:
   - The program for the frequency in Hz.
   - Enter a numeric value (e.g., 50) and press Enter.

2. Set Waveform Type:
   - The program for the waveform type:
     1 for Sine
     2 for Square
     3 for Sawtooth
     4 for Triangle
   - Enter the corresponding number and press Enter.

### Example Console Usage
Select waveform: 1. Sine, 2. Sawtooth, 3. Square, 4. Triangle
> 1
Enter frequency (Hz):
> 100
Generating waveform...
