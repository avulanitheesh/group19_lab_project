
# Embedded Waveform Generator

This project generates various waveforms (Sine, Triangular, Sawtooth, and Square) using a microcontroller (Tiva C series) and stores them in an array. The project aims to provide a clear understanding of how different waveforms are represented and utilized in embedded systems for signal generation.

## Project Structure

The main file for this project is `Waveforms.h`. This file contains the definitions and values of different waveforms stored as arrays. The file defines four types of waveforms:

- **Sine Wave**
- **Triangular Wave**
- **Sawtooth Wave**
- **Square Wave**

Each waveform is represented by an array of integers, where each integer corresponds to a sample of the waveform.

## Waveforms Explained

### 1. **Sine Wave**

A sine wave is a smooth, periodic oscillation that is commonly used in signal processing and audio applications. It is mathematically defined by the following formula:

\[
y(t) = A \cdot \sin(2 \pi f t + \phi)
\]

Where:
- \( A \) is the amplitude (peak value),
- \( f \) is the frequency (number of cycles per second),
- \( t \) is time,
- \( \phi \) is the phase.

**Pictorial Representation:**

![Sine Wave](path_to_sine_wave_image.png)

The sine wave array in the code (`wt[0]`) contains sampled values representing one cycle of the sine wave.

### 2. **Triangular Wave**

A triangular wave is a non-sinusoidal waveform that has a shape like a triangle. It rises and falls linearly, making it different from the sinusoidal waveform.

The triangular wave can be defined by:

\[
y(t) = \frac{2A}{T} \cdot |t - nT| \quad \text{for} \quad t \in [0, T]
\]

Where:
- \( A \) is the amplitude,
- \( T \) is the period (time for one complete cycle),
- \( t \) is time.

**Pictorial Representation:**

![Triangular Wave](path_to_triangular_wave_image.png)

The triangular wave values are stored in the second waveform array (`wt[1]`).

### 3. **Sawtooth Wave**

A sawtooth wave is another type of waveform where the signal rises steadily and drops sharply. It is widely used in music synthesis and signal processing.

The sawtooth wave is mathematically defined as:

\[
y(t) = \frac{2A}{T} (t \, \text{mod} \, T)
\]

Where:
- \( A \) is the amplitude,
- \( T \) is the period,
- \( t \) is time.

**Pictorial Representation:**

![Sawtooth Wave](path_to_sawtooth_wave_image.png)

The sawtooth wave values are contained in the third array (`wt[2]`).

### 4. **Square Wave**

A square wave alternates between two levels (high and low) and is used in digital systems for clock generation.

The square wave can be described by:

\[
y(t) = 
\begin{cases} 
A & \text{if } 0 \leq t < \frac{T}{2} \\
0 & \text{if } \frac{T}{2} \leq t < T
\end{cases}
\]

Where:
- \( A \) is the amplitude,
- \( T \) is the period,
- \( t \) is time.

**Pictorial Representation:**

![Square Wave](path_to_square_wave_image.png)

The square wave values are represented in the fourth array (`wt[3]`).

## How to Use the Code

1. **Clone the repository:**

   ```bash
   git clone https://github.com/your-username/your-repo-name.git
