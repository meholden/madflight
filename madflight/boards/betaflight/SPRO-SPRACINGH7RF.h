/*==============================================================================
Generated on: 2024-01-12 17:04:03.274318
Generated by: betaflight_target_converter.py
Source: https://github.com/betaflight/unified-targets
Board name: SPRACINGH7RF
Manufacturer ID: SPRO

//copy this line to madflight.ino to use this flight controller (or copy/paste the whole file)
#include "boards/betaflight/SPRO-SPRACINGH7RF.h"
==============================================================================*/

#define HW_BOARD_NAME "BETAFLIGHT-SPRO-SPRACINGH7RF"
#define HW_MCU "STM32H730"

//Defines from betaflight. Note: madflight will pick the last sensor defined here, this might not be the sensor that is actually on the board. Comment the offending sensors out.
#define TARGET_BOARD_IDENTIFIER  "SP7R"
#define USBD_PRODUCT_STRING  "SPRacingH7RF"
#define FC_VMA_ADDRESS  0x90100000
#define EEPROM_SIZE  8192
#define USE_SPRACING_PERSISTENT_RTC_WORKAROUND
#define USE_BUTTONS
#define BUTTON_A_PIN  PC14
#define BUTTON_A_PIN_INVERTED
#define BUTTON_B_PIN  PC14
#define BUTTON_B_PIN_INVERTED
#define USE_OCTOSPI
#define USE_OCTOSPI_DEVICE_1
#define USE_SPI
#define USE_SPI_DEVICE_2
#define SPI2_SCK_PIN  PD3
#define SPI2_MISO_PIN  PB14
#define SPI2_MOSI_PIN  PB15
#define SPI2_NSS_PIN  PB12
#define USE_SPI_DEVICE_6
#define SPI6_SCK_PIN  PB3
#define SPI6_MISO_PIN  PB4
#define SPI6_MOSI_PIN  PB5
#define SPI6_NSS_PIN  PA15
#define SX1280_BUSY_PIN  PC7
#define SX1280_DIO1_PIN  PC6
#define SX1280_DIO2_PIN  PD4
#define SX1280_DIO3_PIN  NONE
#define BB_USE  BB_USE_FLASH
#define BB_FLASH_TYPE  "MEMORY_MAPPED"
#define FLASH_OCTOSPI_INSTANCE  OCTOSPI1
#define CONFIG_IN_MEMORY_MAPPED_FLASH
#define USE_FIRMWARE_PARTITION
#define SDCARD_DETECT_PIN  PC13
#define SDCARD_DETECT_INVERTED
#define SDIO_DEVICE  SDIODEV_1
#define SDIO_USE_4BIT  1
#define SDIO_CK_PIN  PC12
#define SDIO_CMD_PIN  PD2
#define SDIO_D0_PIN  PC8
#define SDIO_D1_PIN  PC9
#define SDIO_D2_PIN  PC10
#define SDIO_D3_PIN  PC11
#define TARGET_IO_PORTA  0xffff
#define TARGET_IO_PORTB  (0xffff
#define TARGET_IO_PORTC  0xffff
#define TARGET_IO_PORTD  (0xffff
#define TARGET_IO_PORTE  (0xffff
#define TARGET_IO_PORTF  0xffff
#define TARGET_IO_PORTG  0xffff
#define TARGET_IO_PORTH  0xffff
#define USE_I2C
#define USE_I2C_DEVICE_1
#define I2C1_SCL_PIN  PB8
#define I2C1_SDA_PIN  PB9
#define USE_I2C_DEVICE_2
#define I2C2_SCL_PIN  PB10
#define I2C2_SDA_PIN  PB11
#define MAG_I2C_INSTANCE  (I2CDEV_1)
#define BARO_I2C_INSTANCE  (I2CDEV_2)
#define USE_MPU_DATA_READY_SIGNAL
#define ENSURE_MPU_DATA_READY_IS_LOW
#define SX1280_BUSY_PIN  PC7
#define SX1280_DIO1_PIN  PC6
#define SX1280_DIO2_PIN  PD4
#define SX1280_DIO3_PIN  NONE
#define SX1280_NRESET_PIN  PD10
#define USE_RX_SPI
#define USE_RX_EXPRESSLRS
#define USE_RX_SX1280
#define RX_SPI_INSTANCE  SPI2
#define RX_NSS_PIN  SPI2_NSS_PIN
#define RX_SPI_EXTI_PIN  SX1280_DIO1_PIN
#define RX_EXPRESSLRS_SPI_RESET_PIN  SX1280_NRESET_PIN
#define RX_EXPRESSLRS_SPI_BUSY_PIN  SX1280_BUSY_PIN
#define RX_EXPRESSLRS_TIMER_INSTANCE  TIM6
#define DEFAULT_RX_FEATURE  FEATURE_RX_SPI
#define RX_SPI_DEFAULT_PROTOCOL  RX_SPI_EXPRESSLRS
#define ADC3_DMA_OPT  10
#define ADC_INSTANCE  ADC3
#define ADC1_INSTANCE  ADC1
#define ADC3_INSTANCE  ADC3
#define CURRENT_METER_2_ADC_PIN  PC0
#define CURRENT_METER_2_ADC_INSTANCE  ADC3
#define CURRENT_METER_1_ADC_PIN  PC1
#define CURRENT_METER_1_ADC_INSTANCE  ADC3
#define EXTERNAL1_ADC_PIN  PC2
#define EXTERNAL1_ADC_INSTANCE  ADC3
#define VBAT_ADC_PIN  PC3
#define VBAT_ADC_INSTANCE  ADC3
#define VIDEO_IN_ADC_PIN  PC5
#define VIDEO_OUT_ADC_PIN  PC4
#define RSSI_ADC_PIN  CURRENT_METER_2_ADC_PIN
#define RSSI_ADC_INSTANCE  CURRENT_METER_2_ADC_INSTANCE
#define CURRENT_METER_ADC_PIN  CURRENT_METER_1_ADC_PIN
#define CURRENT_METER_ADC_INSTANCE  CURRENT_METER_1_ADC_INSTANCE
#define DEFAULT_VOLTAGE_METER_SOURCE  VOLTAGE_METER_ADC
#define DEFAULT_CURRENT_METER_SOURCE  CURRENT_METER_ADC
#define VTX_ENABLE_PIN  PC15
#define PINIO1_PIN  VTX_ENABLE_PIN
#define IMU_USE  IMU_USE_SPI_ICM42605
#define IMU_USE  IMU_USE_SPI_ICM42688P
#define BARO_USE  BARO_USE_BMP388
#define USE_MAG_HMC5883
#define USE_MAG_QMC5883
#define BB_USE  BB_USE_FLASH
#define BB_FLASH_TYPE  "W25Q128FV"
#define BB_USE  BB_USE_SDCARD
#define USE_OSD
#define SPRACING_PIXEL_OSD_BLACK_PIN  PE12
#define SPRACING_PIXEL_OSD_WHITE_PIN  PE13
#define SPRACING_PIXEL_OSD_MASK_ENABLE_PIN  PE14
#define SPRACING_PIXEL_OSD_WHITE_SOURCE_SELECT_PIN  PE15
#define SPRACING_PIXEL_OSD_SYNC_IN_PIN  PE11
#define SPRACING_PIXEL_OSD_SYNC_OUT_PIN  PA8
#define SPRACING_PIXEL_OSD_WHITE_SOURCE_PIN  PA4
#define SPRACING_PIXEL_OSD_VIDEO_THRESHOLD_DEBUG_PIN  PA5
#define SPRACING_PIXEL_OSD_PIXEL_DEBUG_1_PIN  PE5
#define SPRACING_PIXEL_OSD_PIXEL_DEBUG_2_PIN  PE6
#define SPRACING_PIXEL_OSD_PIXEL_GATING_DEBUG_PIN  PB0
#define SPRACING_PIXEL_OSD_PIXEL_BLANKING_DEBUG_PIN  PB1

//Sensor specific setup
#define IMU_ALIGN  IMU_ALIGN_CW270FLIP
#define BARO_I2C_ADR  0
#define MAG_I2C_ADR  0

//LED:
const int HW_PIN_LED      = PE5;
const int HW_LED_ON       = 1; //0:low is on, 1:high is on

//IMU SPI: (SPI6)
const int HW_PIN_SPI_MISO = -1;
const int HW_PIN_SPI_MOSI = -1;
const int HW_PIN_SPI_SCLK = -1;
const int HW_PIN_IMU_CS   = PA15;
const int HW_PIN_IMU_EXTI = PD15;

//BARO/MAG I2C: (I2C2)
const int HW_PIN_I2C_SDA  = PB10;
const int HW_PIN_I2C_SCL  = PB11;

//Outputs:
const int HW_OUT_COUNT    = 8;
const int HW_PIN_OUT[]    = {PB0,PB1,PA6,PA7,PA0,PA1,PA2,PA3};

//RC Receiver: (SERIAL1)
const int HW_PIN_RCIN_RX  = PNONE;
const int HW_PIN_RCIN_TX  = PNONE;
const int HW_PIN_RCIN_INVERTER = -1;

//GPS: (SERIAL2)
const int HW_PIN_GPS_RX   = PD6;
const int HW_PIN_GPS_TX   = PD5;
const int HW_PIN_GPS_INVERTER = -1;

//Battery ADC voltage and current inputs:
const int HW_PIN_BAT_V    = PC3;
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
#define HW_SERIAL_COUNT 10
#define HW_PIN_SERIAL { {1,PNONE,PNONE,-1}, {2,PD5,PD6,-1}, {3,PD8,PD9,-1}, {4,PD1,PD0,-1}, {5,PNONE,PB13,-1}, {6,PNONE,PNONE,-1}, {7,PNONE,PNONE,-1}, {8,PE1,PE0,-1}, {9,PNONE,PNONE,-1}, {10,PNONE,PNONE,-1} } // {INDEX,TX,RX,INVERTER}

//SPI
#define HW_SPI_COUNT 0
#define HW_PIN_SPI {  } // {INDEX,SCK,MISO,MOSI}

//I2C
#define HW_I2C_COUNT 4
#define HW_PIN_I2C { {1,PB8,PB9}, {2,PB10,PB11}, {3,PNONE,PNONE}, {4,PNONE,PNONE} } // {INDEX,SCL,SDA}

//Motors:
#define HW_MOTOR_COUNT 8
#define HW_MOTOR_OUT {PB0,PB1,PA6,PA7,PA0,PA1,PA2,PA3}

//other pins
#define HW_PIN_BEEPER PE4
#define HW_PIN_LED_STRIP PB7
//#define HW_PIN_LED PE5
#define HW_PIN_LED_2 PE6
#define HW_PIN_LED_3 PNONE
#define HW_PIN_RX_BIND PNONE
#define HW_PIN_RX_BIND_PLUG PNONE
#define HW_PIN_ADC_BATT PC3
#define HW_PIN_ADC_RSSI PC0
#define HW_PIN_ADC_CURR PC1
#define HW_PIN_ADC_EXT PC2
#define HW_PIN_SDCARD_DETECT PC13
#define HW_PIN_SDIO_CK PC12
#define HW_PIN_SDIO_CMD PD2
#define HW_PIN_SDIO_D0 PC8
#define HW_PIN_SDIO_D1 PC9
#define HW_PIN_SDIO_D2 PC10
#define HW_PIN_SDIO_D3 PC11
#define HW_PIN_PINIO PC15
#define HW_PIN_RX_SPI_CS PB12
#define HW_PIN_RX_SPI_EXTI PC6
#define HW_PIN_RX_SPI_BIND PNONE
#define HW_PIN_RX_SPI_LED PNONE
#define HW_PIN_RX_SPI_EXPRESSLRS_RESET PD10
#define HW_PIN_RX_SPI_EXPRESSLRS_BUSY PC7
#define HW_PIN_GYRO_EXTI PD15
#define HW_PIN_GYRO_EXTI_2 PNONE
#define HW_PIN_GYRO_CS PA15
#define HW_PIN_GYRO_CS_2 PNONE

//set statements
#define HW_SET_BEEPER_INVERSION ON
#define HW_SET_BEEPER_OD OFF
#define HW_SET_MAG_BUSTYPE I2C
#define HW_SET_MAG_I2C_DEVICE 1
#define HW_SET_BARO_BUSTYPE I2C
#define HW_SET_BARO_I2C_DEVICE 2
#define HW_SET_ADC_DEVICE 3
#define HW_SET_RX_SPI_PROTOCOL EXPRESSLRS
#define HW_SET_RX_SPI_BUS 2
#define HW_SET_PINIO_CONFIG 1,1,1,1
#define HW_SET_PINIO_BOX 40,255,255,255
#define HW_SET_BLACKBOX_DEVICE SDCARD
#define HW_SET_CURRENT_METER ADC
#define HW_SET_BATTERY_METER ADC
#define HW_SET_SDCARD_DETECT_INVERTED ON
#define HW_SET_SDCARD_MODE SDIO
#define HW_SET_SDIO_USE_4BIT_WIDTH ON
#define HW_SET_SDIO_DEVICE 1
#define HW_SET_GYRO_1_BUSTYPE SPI
#define HW_SET_GYRO_1_SPIBUS 6
#define HW_SET_GYRO_1_SENSOR_ALIGN CW270FLIP


/*
#==============================================================================
# BetaFlight Source file
#==============================================================================
# Betaflight / STM32H730 (SP7R) 4.4.0 Jan 28 2023 / 21:17:30 (d78f0c9183) MSP API: 1.45

#define TARGET_BOARD_IDENTIFIER "SP7R"
#define USBD_PRODUCT_STRING "SPRacingH7RF"

#
# Settings required to boot
#
# Note: hardware for all possible config storage subsystems needs to be known and initialised BEFORE loading
# a config from it can work. (SPI/OCTOSPI/SDCARD).
#

#define FC_VMA_ADDRESS    0x90100000

#define EEPROM_SIZE 8192

#define USE_SPRACING_PERSISTENT_RTC_WORKAROUND

#define USE_BUTTONS
#define BUTTON_A_PIN            PC14
#define BUTTON_A_PIN_INVERTED
#define BUTTON_B_PIN            PC14
#define BUTTON_B_PIN_INVERTED

#define USE_OCTOSPI
#define USE_OCTOSPI_DEVICE_1

# Bootloader will have configured the OCTOSPI and OCTOSPIM peripherals and pins, firmware needs no-knowledge of the configuration.
# So the pins below are not required to compile but are included here for reference only
# #define OCTOSPIM_P1_SCK_PIN PB2
# #define OCTOSPIM_P1_CS_PIN PB6

# Using IO0:3
# #define OCTOSPIM_P1_IO0_PIN PD11
# #define OCTOSPIM_P1_IO1_PIN PD12
# #define OCTOSPIM_P1_IO2_PIN PE2
# #define OCTOSPIM_P1_IO3_PIN PD13

# Not using IO4:7
# #define OCTOSPIM_P1_IO4_PIN NONE
# #define OCTOSPIM_P1_IO5_PIN NONE
# #define OCTOSPIM_P1_IO6_PIN NONE
# #define OCTOSPIM_P1_IO7_PIN NONE

# #define OCTOSPIM_P1_MODE OCTOSPIM_P1_MODE_IO03_ONLY
# #define OCTOSPIM_P1_CS_FLAGS (OCTOSPIM_P1_CS_HARDWARE)

#define USE_SPI

#define USE_SPI_DEVICE_2
#define SPI2_SCK_PIN            PD3
#define SPI2_MISO_PIN           PB14
#define SPI2_MOSI_PIN           PB15
#define SPI2_NSS_PIN            PB12

# SPI3 and SPI6 use the same pins however SPI6 supports independent clock configuration, thus SPI6 used to allow separation between SX1280 and Gyro SPI clocks.
# SPI3 uses DMA1, SPI6 also uses DMA2
#define USE_SPI_DEVICE_6
#define SPI6_SCK_PIN            PB3
#define SPI6_MISO_PIN           PB4
#define SPI6_MOSI_PIN           PB5
#define SPI6_NSS_PIN            PA15

#define SX1280_BUSY_PIN         PC7
#define SX1280_DIO1_PIN         PC6
#define SX1280_DIO2_PIN         PD4
#define SX1280_DIO3_PIN         NONE

# FC has an a OctoSPI flash chip, for code/data storage on OCTOSPIM_P1 (Memory mapped mode)
# Config is to be stored on the flash chip used for code/data storage, this requires that the
# config load/save routines must run from RAM and a method to enable/disable memory mapped mode is needed.

#define USE_FLASH
#define USE_FLASH_MEMORY_MAPPED
#define FLASH_OCTOSPI_INSTANCE  OCTOSPI1
#define CONFIG_IN_MEMORY_MAPPED_FLASH
#define USE_FIRMWARE_PARTITION

#define SDCARD_DETECT_PIN PC13
#define SDCARD_DETECT_INVERTED
#define SDIO_DEVICE             SDIODEV_1
#define SDIO_USE_4BIT           1
#define SDIO_CK_PIN             PC12
#define SDIO_CMD_PIN            PD2
#define SDIO_D0_PIN             PC8
#define SDIO_D1_PIN             PC9
#define SDIO_D2_PIN             PC10
#define SDIO_D3_PIN             PC11

# Disable OCTOSPI pins PB2/CLK, PB6/NCS, PD11/IO0, PD12/IO1, PD13/IO3, PE2/IO2
# PE7/IO4, PE8/IO5, PE9/IO6, PE10/IO7
#define TARGET_IO_PORTA 0xffff
#define TARGET_IO_PORTB (0xffff & ~(BIT(2)|BIT(6)))
#define TARGET_IO_PORTC 0xffff
#define TARGET_IO_PORTD (0xffff & ~(BIT(11)|BIT(12)|BIT(13)))
#define TARGET_IO_PORTE (0xffff & ~(BIT(2)|BIT(7)|BIT(8)|BIT(9)|BIT(10)))
#define TARGET_IO_PORTF 0xffff
#define TARGET_IO_PORTG 0xffff
#define TARGET_IO_PORTH 0xffff

#
# Settings to enable/configure remaining hardware
#

#define USE_I2C
# I2C1 for external MAG connection on J8, no pull-ups, external pull-ups REQUIRED.
#define USE_I2C_DEVICE_1
#define I2C1_SCL_PIN            PB8
#define I2C1_SDA_PIN            PB9

# I2C2 has BMP388 internally connected, 10k pull-ups
#define USE_I2C_DEVICE_2
#define I2C2_SCL_PIN            PB10
#define I2C2_SDA_PIN            PB11

#define MAG_I2C_INSTANCE        (I2CDEV_1)
#define BARO_I2C_INSTANCE       (I2CDEV_2)

#define USE_MPU_DATA_READY_SIGNAL
#define ENSURE_MPU_DATA_READY_IS_LOW

#define SX1280_BUSY_PIN         PC7
#define SX1280_DIO1_PIN         PC6
#define SX1280_DIO2_PIN         PD4
#define SX1280_DIO3_PIN         NONE

# RevA has 0ohm jumper on R26.  Delete schematic connection and support if SX1280 doesn't need NRESET to free a pin.
# The idea is that NRESET should not be needed at all.  Production boards probably won't have jumper installed.
#define SX1280_NRESET_PIN       PD10

#define USE_RX_SPI
#define USE_RX_EXPRESSLRS
#define USE_RX_SX1280
#define RX_SPI_INSTANCE                                 SPI2
#define RX_NSS_PIN                                      SPI2_NSS_PIN
#define RX_SPI_EXTI_PIN                                 SX1280_DIO1_PIN
#define RX_EXPRESSLRS_SPI_RESET_PIN                     SX1280_NRESET_PIN
#define RX_EXPRESSLRS_SPI_BUSY_PIN                      SX1280_BUSY_PIN
#define RX_EXPRESSLRS_TIMER_INSTANCE                    TIM6

#define DEFAULT_RX_FEATURE      FEATURE_RX_SPI
#define RX_SPI_DEFAULT_PROTOCOL RX_SPI_EXPRESSLRS

# On the H7RF ADC 1 is reserved for VIDEO, ADC2 is not used. all other ADC functions MUST use ADC3
#define ADC3_DMA_OPT 10
#define ADC_INSTANCE                  ADC3
#define ADC1_INSTANCE                 ADC1
#define ADC3_INSTANCE                 ADC3
# 2 Current meter ADC inputs, one on each 4in1ESC connector. NO RSSI input due to on-board SX1280 RF chip.
#define CURRENT_METER_2_ADC_PIN       PC0
#define CURRENT_METER_2_ADC_INSTANCE  ADC3
#define CURRENT_METER_1_ADC_PIN       PC1
#define CURRENT_METER_1_ADC_INSTANCE  ADC3
#define EXTERNAL1_ADC_PIN             PC2
#define EXTERNAL1_ADC_INSTANCE        ADC3
#define VBAT_ADC_PIN                  PC3
#define VBAT_ADC_INSTANCE             ADC3
# Video ADC settings (Reserved)
#define VIDEO_IN_ADC_PIN              PC5
#define VIDEO_OUT_ADC_PIN             PC4
# ADC mapping from actual intended use to BF inputs
#define RSSI_ADC_PIN                  CURRENT_METER_2_ADC_PIN
#define RSSI_ADC_INSTANCE             CURRENT_METER_2_ADC_INSTANCE
#define CURRENT_METER_ADC_PIN         CURRENT_METER_1_ADC_PIN
#define CURRENT_METER_ADC_INSTANCE    CURRENT_METER_1_ADC_INSTANCE
#define DEFAULT_VOLTAGE_METER_SOURCE VOLTAGE_METER_ADC
#define DEFAULT_CURRENT_METER_SOURCE CURRENT_METER_ADC

#define VTX_ENABLE_PIN          PC15
#define PINIO1_PIN              VTX_ENABLE_PIN

#
# Settings that are currently defined in target/common_pre.h for non-cloud builds that probably shouldn't be.
# There are here to illustrate that they SHOULD be included in THIS target when they are removed by default.
#

#define USE_ACC
#define USE_ACC_SPI_ICM42605
#define USE_ACC_SPI_ICM42688P
#define USE_GYRO
#define USE_GYRO_SPI_ICM42605
#define USE_GYRO_SPI_ICM42688P
#define USE_BARO
#define USE_BARO_BMP388
#define USE_MAG
#define USE_MAG_HMC5883
#define USE_MAG_QMC5883
#define USE_FLASH_W25Q128FV
#define USE_SDCARD

#
# SPRacing Pixel OSD settings
#

#define USE_OSD
#define SPRACING_PIXEL_OSD_BLACK_PIN                    PE12
#define SPRACING_PIXEL_OSD_WHITE_PIN                    PE13
#define SPRACING_PIXEL_OSD_MASK_ENABLE_PIN              PE14
#define SPRACING_PIXEL_OSD_WHITE_SOURCE_SELECT_PIN      PE15

# COMP2_INP
#define SPRACING_PIXEL_OSD_SYNC_IN_PIN                  PE11

# TIM1_CH1 
#define SPRACING_PIXEL_OSD_SYNC_OUT_PIN                 PA8

# DAC1_OUT1
#define SPRACING_PIXEL_OSD_WHITE_SOURCE_PIN             PA4

# DAC1_OUT2
#define SPRACING_PIXEL_OSD_VIDEO_THRESHOLD_DEBUG_PIN    PA5

# TIM15_CH1 - For DMA updates
#define SPRACING_PIXEL_OSD_PIXEL_DEBUG_1_PIN            PE5

# TIM15_CH2 - Spare
#define SPRACING_PIXEL_OSD_PIXEL_DEBUG_2_PIN            PE6

# TIM1_CH2N - actual gating is on CH4
#define SPRACING_PIXEL_OSD_PIXEL_GATING_DEBUG_PIN       PB0

# TIM1_CH3N - actual blanking is on CH5
#define SPRACING_PIXEL_OSD_PIXEL_BLANKING_DEBUG_PIN     PB1


board_name SPRACINGH7RF
manufacturer_id SPRO

resource BEEPER 1 E04
resource MOTOR 1 B00
resource MOTOR 2 B01
resource MOTOR 3 A06
resource MOTOR 4 A07
resource MOTOR 5 A00
resource MOTOR 6 A01
resource MOTOR 7 A02
resource MOTOR 8 A03
resource LED_STRIP 1 B07

resource SERIAL_TX 1 NONE
resource SERIAL_TX 2 D05
resource SERIAL_TX 3 D08
resource SERIAL_TX 4 D01
resource SERIAL_TX 5 NONE
resource SERIAL_TX 6 NONE
resource SERIAL_TX 7 NONE
resource SERIAL_TX 8 E01
resource SERIAL_TX 9 NONE
resource SERIAL_TX 10 NONE
resource SERIAL_RX 1 NONE
resource SERIAL_RX 2 D06
resource SERIAL_RX 3 D09
resource SERIAL_RX 4 D00
resource SERIAL_RX 5 B13
resource SERIAL_RX 6 NONE
resource SERIAL_RX 7 NONE
resource SERIAL_RX 8 E00
resource SERIAL_RX 9 NONE
resource SERIAL_RX 10 NONE

resource I2C_SCL 1 B08
resource I2C_SCL 2 B10
resource I2C_SCL 3 NONE
resource I2C_SCL 4 NONE
resource I2C_SDA 1 B09
resource I2C_SDA 2 B11
resource I2C_SDA 3 NONE
resource I2C_SDA 4 NONE

resource LED 1 E05
resource LED 2 E06
resource LED 3 NONE
resource RX_BIND 1 NONE
resource RX_BIND_PLUG 1 NONE

resource ADC_BATT 1 C03
resource ADC_RSSI 1 C00
resource ADC_CURR 1 C01
resource ADC_EXT 1 C02

resource SDCARD_DETECT 1 C13
resource SDIO_CK 1 C12
resource SDIO_CMD 1 D02
resource SDIO_D0 1 C08
resource SDIO_D1 1 C09
resource SDIO_D2 1 C10
resource SDIO_D3 1 C11

resource PINIO 1 C15

resource RX_SPI_CS 1 B12
resource RX_SPI_EXTI 1 C06
resource RX_SPI_BIND 1 NONE
resource RX_SPI_LED 1 NONE
resource RX_SPI_EXPRESSLRS_RESET 1 D10
resource RX_SPI_EXPRESSLRS_BUSY 1 C07

resource GYRO_EXTI 1 D15
resource GYRO_EXTI 2 NONE
resource GYRO_CS 1 A15
resource GYRO_CS 2 NONE

dma ADC 3 10

feature LED_STRIP
feature TELEMETRY
feature RX_SPI

set beeper_inversion = ON
set beeper_od = OFF

set mag_bustype = I2C
set mag_i2c_device = 1

set baro_bustype = I2C
set baro_i2c_device = 2

set adc_device = 3

set rx_spi_protocol = EXPRESSLRS
set rx_spi_bus = 2

set pinio_config = 1,1,1,1
set pinio_box = 40,255,255,255

set blackbox_device = SDCARD

set current_meter = ADC
set battery_meter = ADC

set sdcard_detect_inverted = ON
set sdcard_mode = SDIO
set sdio_use_4bit_width = ON
set sdio_device = 1

set gyro_1_bustype = SPI
set gyro_1_spibus = 6
set gyro_1_sensor_align = CW270FLIP

*/
