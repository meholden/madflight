/*==============================================================================
Generated on: 2024-01-12 17:04:02.975832
Generated by: betaflight_target_converter.py
Source: https://github.com/betaflight/unified-targets
Board name: FF_FORTINIF4
Manufacturer ID: FFPV

//copy this line to madflight.ino to use this flight controller (or copy/paste the whole file)
#include "boards/betaflight/FFPV-FF_FORTINIF4.h"
==============================================================================*/

#define HW_BOARD_NAME "BETAFLIGHT-FFPV-FF_FORTINIF4"
#define HW_MCU "STM32F405"

//Defines from betaflight. Note: madflight will pick the last sensor defined here, this might not be the sensor that is actually on the board. Comment the offending sensors out.
#define IMU_USE  IMU_USE_SPI_ICM20689
#define IMU_USE  IMU_USE_SPI_MPU6500
#define IMU_USE  IMU_USE_SPI_MPU6000
#define OSD_USE  OSD_USE_MAX7456
#define BB_USE  BB_USE_FLASH
#define BB_FLASH_TYPE  "W25Q128FV"

//Sensor specific setup
#define IMU_ALIGN  IMU_ALIGN_CW180
#define BARO_I2C_ADR  0
#define MAG_I2C_ADR  0

//LED:
const int HW_PIN_LED      = PB5;
const int HW_LED_ON       = 1; //0:low is on, 1:high is on

//IMU SPI: (SPI1)
const int HW_PIN_SPI_MISO = PA6;
const int HW_PIN_SPI_MOSI = PA7;
const int HW_PIN_SPI_SCLK = PA5;
const int HW_PIN_IMU_CS   = PA8;
const int HW_PIN_IMU_EXTI = PC4;

//BARO/MAG I2C: (I2C1)
const int HW_PIN_I2C_SDA  = -1;
const int HW_PIN_I2C_SCL  = -1;

//Outputs:
const int HW_OUT_COUNT    = 4;
const int HW_PIN_OUT[]    = {PB0,PB1,PA3,PA2};

//RC Receiver: (SERIAL1)
const int HW_PIN_RCIN_RX  = PA10;
const int HW_PIN_RCIN_TX  = PA9;
const int HW_PIN_RCIN_INVERTER = -1;

//GPS: (SERIAL3)
const int HW_PIN_GPS_RX   = PB11;
const int HW_PIN_GPS_TX   = PB10;
const int HW_PIN_GPS_INVERTER = PC15;

//Battery ADC voltage and current inputs:
const int HW_PIN_BAT_V    = PC2;
const int HW_PIN_BAT_I    = PC1;

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
#define HW_SERIAL_COUNT 5
#define HW_PIN_SERIAL { {1,PA9,PA10,-1}, {3,PB10,PB11,PC15}, {4,PA0,PA1,-1}, {5,PC12,PD2,-1}, {6,PC6,PC7,-1} } // {INDEX,TX,RX,INVERTER}

//SPI
#define HW_SPI_COUNT 2
#define HW_PIN_SPI { {1,PA5,PA6,PA7}, {3,PC10,PC11,PC12} } // {INDEX,SCK,MISO,MOSI}

//I2C
#define HW_I2C_COUNT 0
#define HW_PIN_I2C {  } // {INDEX,SCL,SDA}

//Motors:
#define HW_MOTOR_COUNT 4
#define HW_MOTOR_OUT {PB0,PB1,PA3,PA2}

//other pins
#define HW_PIN_BEEPER PB4
#define HW_PIN_PPM PB9
#define HW_PIN_LED_STRIP PB7
//#define HW_PIN_LED PB5
#define HW_PIN_LED_2 PB6
#define HW_PIN_ESCSERIAL PB0
#define HW_PIN_CAMERA_CONTROL PB7
#define HW_PIN_ADC_BATT PC2
#define HW_PIN_ADC_CURR PC1
#define HW_PIN_FLASH_CS PB3
#define HW_PIN_OSD_CS PB3
#define HW_PIN_GYRO_EXTI PC4
#define HW_PIN_GYRO_CS PA8

//set statements
#define HW_SET_CURRENT_METER ADC
#define HW_SET_BATTERY_METER ADC
#define HW_SET_BEEPER_INVERSION ON
#define HW_SET_BEEPER_OD OFF
#define HW_SET_SYSTEM_HSE_MHZ 8
#define HW_SET_MAX7456_SPI_BUS 3
#define HW_SET_FLASH_SPI_BUS 3
#define HW_SET_GYRO_1_BUSTYPE SPI
#define HW_SET_GYRO_1_SPIBUS 1
#define HW_SET_GYRO_1_SENSOR_ALIGN CW180
#define HW_SET_GYRO_1_ALIGN_YAW 1800


/*
#==============================================================================
# BetaFlight Source file
#==============================================================================
# Betaflight / STM32F405 (S405) 4.2.0 Feb  2 2020 / 14:39:25 (30bf9e809f) MSP API: 1.43

#define USE_ACC
#define USE_ACC_SPI_ICM20689
#define USE_ACC_SPI_MPU6500
#define USE_ACC_SPI_MPU6000
#define USE_GYRO
#define USE_GYRO_SPI_ICM20689
#define USE_GYRO_SPI_MPU6500
#define USE_GYRO_SPI_MPU6000
#define USE_MAX7456
#define USE_FLASH
#define USE_FLASH_W25Q128FV

board_name FF_FORTINIF4
manufacturer_id FFPV

# resources
resource BEEPER 1 B04
resource MOTOR 1 B00
resource MOTOR 2 B01
resource MOTOR 3 A03
resource MOTOR 4 A02
resource PPM 1 B09
resource LED_STRIP 1 B07
resource SERIAL_TX 1 A09
resource SERIAL_TX 3 B10
resource SERIAL_TX 4 A00
resource SERIAL_TX 5 C12
resource SERIAL_TX 6 C06
resource SERIAL_RX 1 A10
resource SERIAL_RX 3 B11
resource SERIAL_RX 4 A01
resource SERIAL_RX 5 D02
resource SERIAL_RX 6 C07
resource INVERTER 3 C15
resource LED 1 B05
resource LED 2 B06
resource SPI_SCK 1 A05
resource SPI_SCK 3 C10
resource SPI_MISO 1 A06
resource SPI_MISO 3 C11
resource SPI_MOSI 1 A07
resource SPI_MOSI 3 C12
resource ESCSERIAL 1 B00
resource CAMERA_CONTROL 1 B07
resource ADC_BATT 1 C02
resource ADC_CURR 1 C01
resource FLASH_CS 1 B03
resource OSD_CS 1 B03
resource GYRO_EXTI 1 C04
resource GYRO_CS 1 A08

# timer
timer B00 AF2
# pin B00: TIM3 CH3 (AF2)
timer B01 AF2
# pin B01: TIM3 CH4 (AF2)
timer A03 AF1
# pin A03: TIM2 CH4 (AF1)
timer A02 AF1
# pin A02: TIM2 CH3 (AF1)
timer B09 AF2
# pin B09: TIM4 CH4 (AF2)
timer B07 AF2
# pin B07: TIM4 CH2 (AF2)

# dma
dma ADC 1 1
# ADC 1: DMA2 Stream 4 Channel 0
dma pin B00 0
# pin B00: DMA1 Stream 7 Channel 5
dma pin B01 0
# pin B01: DMA1 Stream 2 Channel 5
dma pin A03 1
# pin A03: DMA1 Stream 6 Channel 3
dma pin A02 0
# pin A02: DMA1 Stream 1 Channel 3
dma pin B07 0
# pin B07: DMA1 Stream 3 Channel 2

# feature
feature OSD

# master
set current_meter = ADC
set battery_meter = ADC
set beeper_inversion = ON
set beeper_od = OFF
set system_hse_mhz = 8
set max7456_spi_bus = 3
set flash_spi_bus = 3
set gyro_1_bustype = SPI
set gyro_1_spibus = 1
set gyro_1_sensor_align = CW180
set gyro_1_align_yaw = 1800

*/
