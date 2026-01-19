# PC-based Data Acquisition System

An embedded data acquisition system for LPC2129 microcontroller using I2C and SPI interfaces to collect, store, and display sensor data.

## Features

- **I2C Communication**: Interface with EEPROM for data storage and RTC for timestamping
- **SPI Interface**: Read analog sensor data via external ADC
- **LCD Display**: 4-bit mode LCD for real-time data visualization
- **Data Logging**: Store acquired data in EEPROM with timestamps

## Hardware Requirements

- LPC2129 microcontroller development board
- 16x2 LCD display
- I2C EEPROM (e.g., 24C02)
- RTC module (I2C-based)
- SPI ADC (for analog sensor input)
- Sensors for data acquisition

## Software Prerequisites

- Keil µVision IDE
- LPC21xx header files
- Flash Magic or similar programmer

## Building the Project

1. Open the project in Keil µVision
2. Configure target settings for LPC2129
3. Build the project (F7)
4. Flash the hex file to the microcontroller

## File Structure

| File | Description |
|------|-------------|
| `data_acquisation.c` | Main application with I2C, SPI, and LCD functions |
| `lcd_fourbit.h` | LCD driver header for 4-bit mode operation |

## Interfaces

### I2C Functions
- `i2c_init()` - Initialize I2C peripheral
- `eeprom_write()` / `eeprom_read()` - EEPROM data storage
- `rtc()` - Real-time clock interface

### SPI Functions
- SPI configuration for ADC communication
- Data read operations for sensor values

### LCD Functions
- 4-bit mode LCD initialization and control
- String and data display routines

## How It Works

1. System initializes I2C, SPI, and LCD peripherals
2. Sensor data is acquired via SPI ADC
3. Data is timestamped using RTC
4. Values are displayed on LCD and optionally stored in EEPROM
5. Continuous monitoring loop for real-time acquisition
