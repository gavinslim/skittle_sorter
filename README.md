# color_sorter

TCS3472
- 3x4 photodiode array and four ADC
- I2C interface, 400kHz
- Active low interrupt signal output (triggered when user-defined thresholds exceeded)
- Pin out:
  - VIN: Supply voltage (3V3 - 5V)
  - GND: Ground
  - 3V3: I2C Voltage
  - SDA: I2C Data
  - SCL: I2C Clock
  - INT: Interrupt - open drain (active low)
  - LED: LED Enable (active high, 3V3)

State Machine (Reference: AMS TCS3472 Datasheet)
![image](https://user-images.githubusercontent.com/13181638/118351334-45cb7d00-b510-11eb-9e18-839ee6d91bb3.png)

1. On boot up, device is placed in low-power SLEEP state
2. Send start condition on I2C bus to initialize device into IDLE state
3. In IDLE state, 
   - If PON == 1b AND WEN == 1b AND AEN == 1b, transition to WAIT state
   - If PON == 1b AND WEN == 0b AND AEN == 1b, transition to RGBC state
   - Note: PON = Enable Register (0x00), WEN = Power management, AEN = RGBC Enable
4. In WAIT state, transition to RGBC state
5. In RGBC state, transition to IDLE state

RGBC Operation:
- RGBC integration time (ATIME) affects resolution and sensitivity of RGBC reading. Range from 2.4ms to 614ms
- RGBC gain control (AGAIN) affects the magnitude of the reading. Options: 1x, 4x, 16x, 50x


Inline-style: 
![Orderly Demo](demo/orderly.gif)
