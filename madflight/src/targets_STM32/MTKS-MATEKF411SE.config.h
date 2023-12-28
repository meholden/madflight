//==============================================================================
// Generated on: 2023-12-28 01:17:04.481604
// Generated by: _convert.py in this directory
// Source: https://github.com/betaflight/unified-targets
// Board name: MATEKF411SE
// Manufacturer ID: MTKS
//==============================================================================
#define USE_IMU_SPI_MPU6000
#define USE_BARO_BMP280
#define USE_MAX7456

//LED:
const int HW_PIN_LED      = C13;
#define LED_ON 0 //low = on

//Battery voltage divider:
const int HW_PIN_BAT_ADC  = B00;
const int HW_PIN_BAT_CURR = B01;

//GPS: (SERIAL1)
const int HW_PIN_GPS_RX   = B03;
const int HW_PIN_GPS_TX   = A15;
HardwareSerial gps_Serial(HW_PIN_GPS_RX, HW_PIN_GPS_TX);

//RC Receiver: (SERIAL3)
const int HW_PIN_RCIN_RX  = -1;
const int HW_PIN_RCIN_TX  = -1;
HardwareSerial *rcin_Serial = new HardwareSerial(HW_PIN_RCIN_RX, HW_PIN_RCIN_TX);

//IMU:
const int HW_PIN_IMU_INT  = A14;

//I2C: (I2C1)
const int HW_PIN_I2C_SDA  = B06;
const int HW_PIN_I2C_SCL  = B07;
typedef TwoWire HW_WIRETYPE; //define the class to use for I2C
HW_WIRETYPE *i2c = &Wire; //&Wire or &Wire1

//SPI: (SPI1)
const int HW_PIN_SPI_MISO = A06;
const int HW_PIN_SPI_CS   = A04;
const int HW_PIN_SPI_SCLK = A05;
const int HW_PIN_SPI_MOSI = A07;
SPIClass *spi = &SPI;

//Outputs:
#define HW_OUT_COUNT 6
const int8_t HW_PIN_OUT[HW_OUT_COUNT] = {PB4,PB5,PA8,PA9,PA10,PB8};


/*
#==============================================================================
# BetaFlight Source file
#==============================================================================
# Betaflight / STM32F411 (S411) 4.1.0 Jun 25 2019 / 10:27:57 (2a6e94d03) MSP API: 1.42

#mcu STM32F411

#define USE_ACC
#define USE_ACC_SPI_MPU6000
#define USE_GYRO
#define USE_GYRO_SPI_MPU6000
#define USE_BARO
#define USE_BARO_BMP280
#define USE_MAX7456

board_name MATEKF411SE
manufacturer_id MTKS

# resources
resource BEEPER 1 B02
resource MOTOR 1 B04
resource MOTOR 2 B05
resource MOTOR 3 A08
resource MOTOR 4 A09
resource MOTOR 5 A10
resource MOTOR 6 B08
resource PPM 1 A03
resource LED_STRIP 1 B10
resource SERIAL_TX 1 A15
resource SERIAL_TX 2 A02
resource SERIAL_TX 11 B09
resource SERIAL_TX 12 A02
resource SERIAL_RX 1 B03
resource SERIAL_RX 2 A03
resource I2C_SCL 1 B06
resource I2C_SDA 1 B07
resource LED 1 C13
resource LED 2 C14
resource SPI_SCK 1 A05
resource SPI_SCK 2 B13
resource SPI_MISO 1 A06
resource SPI_MISO 2 B14
resource SPI_MOSI 1 A07
resource SPI_MOSI 2 B15
resource CAMERA_CONTROL 1 A01
resource ADC_BATT 1 B00
resource ADC_RSSI 1 A00
resource ADC_CURR 1 B01
resource PINIO 1 A13
resource OSD_CS 1 B12
resource GYRO_EXTI 1 A14
resource GYRO_CS 1 A04
resource USB_DETECT 1 C15

# timer list
timer B04 AF2
# pin B04: TIM3 CH1 (AF2)
timer B05 AF2
# pin B05: TIM3 CH2 (AF2)
timer A08 AF1
# pin A08: TIM1 CH1 (AF1)
timer A09 AF1
# pin A09: TIM1 CH2 (AF1)
timer A10 AF1
# pin A10: TIM1 CH3 (AF1)
timer B08 AF2
# pin B08: TIM4 CH3 (AF2)
timer B10 AF1
# pin B10: TIM2 CH3 (AF1)
timer B09 AF3
# pin B09: TIM11 CH1 (AF3)
timer A02 AF3
# pin A02: TIM9 CH1 (AF3)
timer A03 AF3
# pin A03: TIM9 CH2 (AF3)
timer A01 AF2
# pin A01: TIM5 CH2 (AF2)

# dma
dma ADC 1 1
# ADC 1: DMA2 Stream 4 Channel 0
dma pin B04 0
# pin B04: DMA1 Stream 4 Channel 5
dma pin B05 0
# pin B05: DMA1 Stream 5 Channel 5
dma pin A08 1
# pin A08: DMA2 Stream 1 Channel 6
dma pin A09 1
# pin A09: DMA2 Stream 2 Channel 6
dma pin A10 1
# pin A10: DMA2 Stream 6 Channel 6
dma pin B08 0
# pin B08: DMA1 Stream 7 Channel 2
dma pin B10 0
# pin B10: DMA1 Stream 1 Channel 3
dma pin A01 0
# pin A01: DMA1 Stream 4 Channel 6

# feature
feature RX_SERIAL
feature SOFTSERIAL
feature TELEMETRY
feature OSD
feature AIRMODE

# serial
serial 1 64 115200 57600 0 115200

# master
set serialrx_provider = SBUS
set mag_bustype = I2C
set mag_i2c_device = 1
set mag_hardware = NONE
set baro_bustype = I2C
set baro_i2c_device = 1
set current_meter = ADC
set battery_meter = ADC
set vbat_detect_cell_voltage = 300
set beeper_inversion = ON
set beeper_od = OFF
set system_hse_mhz = 8
set max7456_spi_bus = 2
set pinio_box = 40,255,255,255
set gyro_1_bustype = SPI
set gyro_1_spibus = 1
set gyro_1_i2cBus = 0
set gyro_1_i2c_address = 0
set gyro_1_sensor_align = CW180

*/