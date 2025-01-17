/*==============================================================================
Generated on: 2024-01-12 17:04:03.209230
Generated by: betaflight_target_converter.py
Source: https://github.com/betaflight/unified-targets
Board name: REVONANO
Manufacturer ID: OPEN

//copy this line to madflight.ino to use this flight controller (or copy/paste the whole file)
#include "boards/betaflight/OPEN-REVONANO.h"
==============================================================================*/

#define HW_BOARD_NAME "BETAFLIGHT-OPEN-REVONANO"
#define HW_MCU "STM32F411"

//Defines from betaflight. Note: madflight will pick the last sensor defined here, this might not be the sensor that is actually on the board. Comment the offending sensors out.
#define IMU_USE  IMU_USE_SPI_MPU9250

//Sensor specific setup
#define IMU_ALIGN  IMU_ALIGN_CW270
#define BARO_I2C_ADR  0
#define MAG_I2C_ADR  0

//LED:
const int HW_PIN_LED      = PC14;
const int HW_LED_ON       = 1; //0:low is on, 1:high is on

//IMU SPI: (SPI2)
const int HW_PIN_SPI_MISO = PB14;
const int HW_PIN_SPI_MOSI = PB15;
const int HW_PIN_SPI_SCLK = PB13;
const int HW_PIN_IMU_CS   = PB12;
const int HW_PIN_IMU_EXTI = PA15;

//BARO/MAG I2C: (I2C3)
const int HW_PIN_I2C_SDA  = PA8;
const int HW_PIN_I2C_SCL  = PB4;

//Outputs:
const int HW_OUT_COUNT    = 6;
const int HW_PIN_OUT[]    = {PA10,PB3,PB8,PB9,PA0,PA1};

//RC Receiver: (SERIAL1)
const int HW_PIN_RCIN_RX  = PB7;
const int HW_PIN_RCIN_TX  = PB6;
const int HW_PIN_RCIN_INVERTER = -1;

//GPS: (SERIAL2)
const int HW_PIN_GPS_RX   = PA3;
const int HW_PIN_GPS_TX   = PA2;
const int HW_PIN_GPS_INVERTER = PC15;

//Battery ADC voltage and current inputs:
const int HW_PIN_BAT_V    = PA6;
const int HW_PIN_BAT_I    = PA7;

//Include Libraries
#include <Wire.h>                      //I2C communication
#include <SPI.h>                       //SPI communication
#include "src/hw_STM32/STM32_PWM.h"    //Servo and oneshot

//Bus Setup
HardwareSerial *rcin_Serial = new HardwareSerial(HW_PIN_RCIN_RX, HW_PIN_RCIN_TX);
HardwareSerial gps_Serial(HW_PIN_GPS_RX, HW_PIN_GPS_TX);
typedef TwoWire HW_WIRETYPE; //define the class to use for I2C
HW_WIRETYPE *i2c = &Wire; //&Wire or &Wire1
SPIClass *spi = &SPI;

//Serial
#define HW_SERIAL_COUNT 2
#define HW_PIN_SERIAL { {1,PB6,PB7,-1}, {2,PA2,PA3,PC15} } // {INDEX,TX,RX,INVERTER}

//SPI
#define HW_SPI_COUNT 1
#define HW_PIN_SPI { {2,PB13,PB14,PB15} } // {INDEX,SCK,MISO,MOSI}

//I2C
#define HW_I2C_COUNT 1
#define HW_PIN_I2C { {3,PA8,PB4} } // {INDEX,SCL,SDA}

//Motors:
#define HW_MOTOR_COUNT 6
#define HW_MOTOR_OUT {PA10,PB3,PB8,PB9,PA0,PA1}

//other pins
#define HW_PIN_BEEPER PC13
#define HW_PIN_PPM PB10
#define HW_PIN_PWM PB10
#define HW_PIN_PWM_2 PB1
#define HW_PIN_PWM_3 PB0
#define HW_PIN_PWM_4 PA7
#define HW_PIN_PWM_5 PA6
#define HW_PIN_PWM_6 PA5
//#define HW_PIN_LED PC14
#define HW_PIN_LED_2 PC13
#define HW_PIN_ESCSERIAL PB10
#define HW_PIN_ADC_BATT PA6
#define HW_PIN_ADC_RSSI PA5
#define HW_PIN_ADC_CURR PA7
#define HW_PIN_PINIO PB10
#define HW_PIN_GYRO_EXTI PA15
#define HW_PIN_GYRO_CS PB12

//set statements
#define HW_SET_BARO_BUSTYPE I2C
#define HW_SET_BARO_I2C_DEVICE 3
#define HW_SET_SYSTEM_HSE_MHZ 8
#define HW_SET_GYRO_1_BUSTYPE SPI
#define HW_SET_GYRO_1_SPIBUS 2
#define HW_SET_GYRO_1_SENSOR_ALIGN CW270
#define HW_SET_GYRO_1_ALIGN_YAW 2700


/*
#==============================================================================
# BetaFlight Source file
#==============================================================================
# Betaflight / STM32F411 (S411) 4.2.0 Apr 18 2020 / 18:30:25 (fde6c680b3) MSP API: 1.43

#define USE_GYRO
#define USE_GYRO_SPI_MPU9250
#define USE_ACC
#define USE_ACC_SPI_MPU9250

board_name REVONANO
manufacturer_id OPEN

# resources
resource BEEPER 1 C13
resource MOTOR 1 A10
resource MOTOR 2 B03
resource MOTOR 3 B08
resource MOTOR 4 B09
resource MOTOR 5 A00
resource MOTOR 6 A01
resource PPM 1 B10
resource PWM 1 B10
resource PWM 2 B01
resource PWM 3 B00
resource PWM 4 A07
resource PWM 5 A06
resource PWM 6 A05
resource SERIAL_TX 1 B06
resource SERIAL_TX 2 A02
resource SERIAL_RX 1 B07
resource SERIAL_RX 2 A03
resource INVERTER 2 C15
resource I2C_SCL 3 A08
resource I2C_SDA 3 B04
resource LED 1 C14
resource LED 2 C13
resource SPI_SCK 2 B13
resource SPI_MISO 2 B14
resource SPI_MOSI 2 B15
resource ESCSERIAL 1 B10
resource ADC_BATT 1 A06
resource ADC_RSSI 1 A05
resource ADC_CURR 1 A07
resource PINIO 1 B10
resource GYRO_EXTI 1 A15
resource GYRO_CS 1 B12

# timer
timer B10 AF1
# pin B10: TIM2 CH3 (AF1)
timer B01 AF2
# pin B01: TIM3 CH4 (AF2)
timer B00 AF2
# pin B00: TIM3 CH3 (AF2)
timer A07 AF2
# pin A07: TIM3 CH2 (AF2)
timer A06 AF2
# pin A06: TIM3 CH1 (AF2)
timer A05 AF1
# pin A05: TIM2 CH1 (AF1)
timer A10 AF1
# pin A10: TIM1 CH3 (AF1)
timer B03 AF1
# pin B03: TIM2 CH2 (AF1)
timer B08 AF2
# pin B08: TIM4 CH3 (AF2)
timer B09 AF2
# pin B09: TIM4 CH4 (AF2)
timer A00 AF2
# pin A00: TIM5 CH1 (AF2)
timer A01 AF2
# pin A01: TIM5 CH2 (AF2)

# dma
dma ADC 1 1
# ADC 1: DMA2 Stream 4 Channel 0
dma pin B10 0
# pin B10: DMA1 Stream 1 Channel 3
dma pin B01 0
# pin B01: DMA1 Stream 2 Channel 5
dma pin B00 0
# pin B00: DMA1 Stream 7 Channel 5
dma pin A07 0
# pin A07: DMA1 Stream 5 Channel 5
dma pin A06 0
# pin A06: DMA1 Stream 4 Channel 5
dma pin A05 0
# pin A05: DMA1 Stream 5 Channel 3
dma pin A10 0
# pin A10: DMA2 Stream 6 Channel 0
dma pin B03 0
# pin B03: DMA1 Stream 6 Channel 3
dma pin B08 0
# pin B08: DMA1 Stream 7 Channel 2
dma pin A00 0
# pin A00: DMA1 Stream 2 Channel 6
dma pin A01 0
# pin A01: DMA1 Stream 4 Channel 6

# master
set baro_bustype = I2C
set baro_i2c_device = 3
set system_hse_mhz = 8
set gyro_1_bustype = SPI
set gyro_1_spibus = 2
set gyro_1_sensor_align = CW270
set gyro_1_align_yaw = 2700

*/
