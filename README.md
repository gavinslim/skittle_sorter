# Skittle Sorter

STM32F4-based skittle sorting machine.  

Components:
- TCS3472 Colour Sensor ([Adafruit](https://www.adafruit.com/product/1334))
- NEMA-17 stepper motor ([Amazon](https://www.amazon.ca/gp/product/B081RCK8QK/ref=ppx_yo_dt_b_search_asin_title?ie=UTF8&psc=1)) 
- A4988 motor driver ([Amazon](https://www.amazon.ca/UEETEK-Compatible-Stepper-Driver-Printer/dp/B07B5ZQXQ9/ref=pd_rhf_ee_s_rp_c_2_6/147-1935409-9490261?pd_rd_w=mK4Wy&pf_rd_p=4a1f7675-9d78-4d55-adb2-96dfabf27919&pf_rd_r=Y8G2PRBBY7YHCVYAS36N&pd_rd_r=b6e4b725-a241-4596-a51e-aa7b62eb8220&pd_rd_wg=3JzLd&pd_rd_i=B07B5ZQXQ9&psc=1))
- Nucleo STM32F410 ([Digikey](https://www.digikey.ca/en/products/detail/stmicroelectronics/NUCLEO-F410RB/5428806))

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
