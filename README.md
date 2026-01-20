# 📊 PC-Based Data Acquisition System

An embedded data acquisition system using **I2C and SPI protocols** to collect, store, and display sensor data in real-time.

---

## 📋 Project Description

This project implements a **Data Acquisition System (DAQ)** that interfaces with multiple sensors and storage devices using industry-standard communication protocols. The system demonstrates how industrial monitoring and logging systems work.

### What I Built:
- **I2C Interface**: Communication with EEPROM for data logging and RTC for timestamping
- **SPI Interface**: High-speed data acquisition from external ADC
- **LCD Display**: Real-time visualization of acquired data
- **Data Logger**: Stores sensor readings with timestamps in non-volatile memory

### Real-World Application:
Data acquisition systems are fundamental in industrial automation, IoT, environmental monitoring, and test & measurement equipment. This project demonstrates the core concepts used in professional DAQ systems.

---

## 🛠️ Technologies Used

| Category | Technology |
|----------|------------|
| **Microcontroller** | LPC2129 (ARM7TDMI-S) |
| **Protocols** | I2C, SPI |
| **Storage** | I2C EEPROM (24Cxx series) |
| **Timekeeping** | I2C RTC Module |
| **ADC** | SPI-based External ADC |
| **IDE** | Keil µVision |
| **Language** | Embedded C |
| **Display** | 16x2 LCD (4-bit mode) |

---

## 📊 System Architecture

```
    ┌───────────────────────────────────────────────┐
    │                    LPC2129 MCU                │
    │                                               │
    │   ┌─────────┐    ┌─────────┐    ┌─────────┐   │
    │   │  I2C    │    │   SPI   │    │  GPIO   │   │
    │   │ Master  │    │ Master  │    │  Pins   │   │
    │   └────┬────┘    └────┬────┘    └────┬────┘   │
    └────────┼──────────────┼──────────────┼────────┘
             │              │              │
        ┌────┴────┐    ┌────┴────┐    ┌────┴────┐
        │ EEPROM  │    │   ADC   │    │   LCD   │
        │   RTC   │    │         │    │ Display │
        └─────────┘    └─────────┘    └─────────┘
```

---

## 🔑 Key Skills Demonstrated

### Serial Communication Protocols

**I2C (Inter-Integrated Circuit)**
- Master-slave communication implementation
- START/STOP condition generation
- ACK/NACK handling
- Multi-device addressing on shared bus
- Clock stretching awareness

**SPI (Serial Peripheral Interface)**
- Full-duplex communication
- Mode configuration (CPOL, CPHA)
- Chip select management
- High-speed data transfer

### Memory & Storage Operations
- EEPROM page write with timing constraints
- Sequential and random read operations
- Wear leveling concepts for flash longevity
- Data integrity verification

### Real-Time Clock Integration
- BCD to decimal conversion
- Time/date formatting
- Alarm and interrupt handling

### Analog Data Acquisition
- ADC sampling and conversion
- Signal conditioning concepts
- Sampling rate considerations
- Resolution and accuracy

### Embedded Software Design
- Modular driver architecture
- Hardware abstraction layer (HAL)
- Polling vs interrupt-driven approaches
- Resource management

---

## 📁 Project Structure

| File | Purpose |
|------|---------|
| `data_acquisation.c` | Main application with I2C, SPI, LCD integration |
| `lcd_fourbit.h` | LCD driver for 4-bit mode operation |

---

## 🎯 What I Learned

- Implementing multiple serial protocols on a single MCU
- Timing-critical operations in embedded systems
- Non-volatile data storage techniques
- Building modular, reusable driver code
