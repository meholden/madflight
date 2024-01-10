#define APPNAME "madflight v0.1.2-dev"
//Arduino ESP32 / RP2040 / STM32 Flight Controller
//GPL-3.0 license
//Copyright (c) 2024 https://github.com/qqqlab/madflight
//Copyright (c) 2022 Nicholas Rehm - dRehmFlight
 
/*#########################################################################################################################
How to use this quadcopter demo program 
=======================================

Motor order diagram (Betaflight order)

      front
 CW -->   <-- CCW
     4     2 
      \ ^ /
       |X|
      / - \
     3     1 
CCW -->   <-- CW

Arming: Set throttle low, then flip arm switch from DISARMED to ARMED.
Disarming: Flip arm switch from ARMED to DISARMED, at any throttle position. "Kill switch".

LED Status
==========
OFF - not powered
ON - running setup
blinking long OFF shot ON - running loop() DISARMED
blinking long ON shot OFF - running loop() ARMED
blink interval longer than 1 second - loop() is taking too much time
##########################################################################################################################*/

//========================================================================================================================//
//                                                 BOARD                                                                  //
//========================================================================================================================//

//uncomment and change this to the flight controller you want to use, or leave commented out to use the default from hw_XXXX.h
//#include "boards/madflight/DYST-DYSF4PRO_V2.h"

//========================================================================================================================//
//                                                 INCLUDES                                                               //
//========================================================================================================================//
//include hardware specific code & default board pinout
#if defined ARDUINO_ARCH_ESP32
  #include "hw_ESP32.h"
#elif defined ARDUINO_ARCH_RP2040
  #include "hw_RP2040.h"
#elif defined ARDUINO_ARCH_STM32
  #include "hw_STM32.h"
#else 
  #error "Unknown hardware architecture"
#endif

#include "src/ahrs/ahrs.h"

//========================================================================================================================//
//                                                 USER-SPECIFIED DEFINES                                                 //
//========================================================================================================================//

//-------------------------------------
// APPLICATION SETTINGS
//-------------------------------------
//#define USE_IMU_POLLING //Uncomment poll IMU sensor in loop(), and not via interrupt (keep commented)

//-------------------------------------
// RC RECEIVER
//-------------------------------------
#define RCIN_NUM_CHANNELS 6 //number of receiver channels (minimal 6)
//Uncomment only one USE_RCIN_xxx receiver type
#define USE_RCIN_CRSF
//#define USE_RCIN_PPM
//#define USE_RCIN_PWM 
//#define USE_RCIN_SBUS
//#define USE_RCIN_DSM
#include "src/rcin/rcin.h" //first define USE_RCIN_xxx then include RCIN.h

//-------------------------------------
// IMU SENSOR
//-------------------------------------
#define IMU_GYRO_DPS 2000 //Full scale gyro range in deg/sec. Most IMUs support 250,500,1000,2000. Can use any value here, driver will pick next greater setting.
#define IMU_ACCEL_G 16 //Full scale gyro accelerometer in G's. Most IMUs support 2,4,8,16. Can use any value here, driver will pick next greater setting.
#include "src/imu/imu.h" //first set all #define IMU_xxx then include IMO.h

//-------------------------------------
// GPS
//-------------------------------------
#define GPS_BAUD 115200
#include "src/gps/gps.h" //first set all #define GPS_xxx then include gps.h

//-------------------------------------
// BAROMETER SENSOR
//-------------------------------------
//#define USE_BARO_BMP280
//#define USE_BARO_MS5611
#define USE_BARO_NONE
#include "src/baro/baro.h" //first set all #define BARO_xxx then include baro.h

//-------------------------------------
// EXTERNAL MAGNETOMETER SENSOR
//-------------------------------------
//Uncomment only one USE_MAG_xxx
//#define USE_MAG_QMC5883L
#define USE_MAG_NONE

#define MAG_I2C_ADR 0 //set magnetormeter I2C address, or 0 for default. If unknown, see output of print_i2c_scan()
#include "src/mag/mag.h" //first set all #define MAG_xxx then include mag.h

//-------------------------------------
// BATTERY MONITOR
//-------------------------------------
//BatteryADC calibration
#define BAT_FACTOR_V 8.04/13951 //BatteryADC voltage conversion factor, set this to 1 and enable print_bat(), then enter here: Actual Volt / bat_v ADC reading (for example: 8.04/13951)
#define BAT_FACTOR_I 1.0/847 //BatteryADC current conversion factor, set this to 1 and enable print_bat(), then enter here: Actual Amperes / bat_i ADC reading (for example: 1.0/847)
#include "src/bat/bat.h" //first set all #define BAT_xxx then include bat.h - BatteryADC is used if HW_PIN_BAT_V or HW_PIN_BAT_I is defined

//-------------------------------------
// BLACKBOX LOGGER
//-------------------------------------
//Uncomment only one USE_BB_xxx
#define USE_BB_MEMORY //log to RAM
//#define USE_BB_FLASH //log to W25Qxx SPI flash 
//#define USE_BB_NONE
#include "src/bb/bb.h" //first set all #define BB_xxx then include bb.h

//========================================================================================================================//
//                                               RC RECEIVER CONFIG                                                      //
//========================================================================================================================//

const int rcin_pwm_fs[] = {1000,1500,1500,1500,1000,1000}; //failsafe pwm values

//set channels
const int rcin_cfg_thro_channel  = 1; //low pwm = zero throttle/stick back, high pwm = full throttle/stick forward
const int rcin_cfg_roll_channel  = 2; //low pwm = left, high pwm = right
const int rcin_cfg_pitch_channel = 3; //low pwm = pitch up/stick back, high pwm = pitch down/stick forward
const int rcin_cfg_yaw_channel   = 4; //low pwm = left, high pwm = right
const int rcin_cfg_arm_channel   = 5; //ARM/DISARM switch
const int rcin_cfg_aux_channel   = 6; //Fight mode - 6 position switch

//config throttle pwm values
const int rcin_cfg_thro_min      = 1150;
const int rcin_cfg_thro_low      = 1250; //used to set rcin_thro_is_low flag when pwm is below. Note: your craft won't arm if this is too low
const int rcin_cfg_thro_max      = 1900;

//config roll, pitch, yaw pwm values
const int rcin_cfg_pwm_min       = 1150;
const int rcin_cfg_pwm_center    = 1500;
const int rcin_cfg_pwm_max       = 1900;
const int rcin_cfg_pwm_deadband  = 0; //Amount of deadband around center, center-deadband to center+deadband will be interpreted as central stick. Set to 15 for PPM or 0 for jitter-free serial protocol receivers.

//config pwm range for ARMED on arm channel
const int rcin_cfg_arm_min       = 1600;
const int rcin_cfg_arm_max       = 2200;

//config 6 position switch on aux channel
int rcin_cfg_aux_min = 1115; //lowest switch position
int rcin_cfg_aux_max = 1945; //higest switch position

//========================================================================================================================//
//                                               CALIBRATION PARAMETERS                                                   //
//========================================================================================================================//

//Magnetometer calibration parameters - if using magnetometer, uncomment calibrate_Magnetometer() in void setup() to get these values, else just ignore these
float MagErrorX = 0.0;
float MagErrorY = 0.0; 
float MagErrorZ = 0.0;
float MagScaleX = 1.0;
float MagScaleY = 1.0;
float MagScaleZ = 1.0;

//IMU calibration parameters - calibrate IMU using calibrate_IMU_error() in the void setup() to get these values, then comment out calibrate_IMU_error()
float AccErrorX = 0.0;
float AccErrorY = 0.0;
float AccErrorZ = 0.0;
float GyroErrorX = 0.0;
float GyroErrorY = 0.0;
float GyroErrorZ = 0.0;

//========================================================================================================================//
//                                               USER-SPECIFIED VARIABLES                                                 //                           
//========================================================================================================================//

uint32_t loop_freq = 1000; //The main loop frequency in Hz. imu.h might lower this depending on the sensor used. Do not touch unless you know what you are doing.

//number of motors - out[0..out_MOTOR_COUNT-1] are motors, out[out_MOTOR_COUNT..HW_OUT_COUNT-1] are servos
const int out_MOTOR_COUNT = 4;
//name the outputs, to make code more readable
enum out_enum {MOTOR1,MOTOR2,MOTOR3,MOTOR4,SERVO1,SERVO2,SERVO3,SERVO4,SERVO5,SERVO6,SERVO7,SERVO8,SERVO9,SERVO10,SERVO11,SERVO12}; 

//Low Pass Filter parameters. Do not touch unless you know what you are doing.
float LP_accel = 70;          //Accelerometer lowpass filter cutoff frequency in Hz (default MPU6050: 50Hz, MPU9250: 70Hz)
float LP_gyro = 60;           //Gyro lowpass filter cutoff frequency in Hz (default MPU6050: 35Hz, MPU9250: 60Hz)
float LP_mag = 1e10;          //Magnetometer lowpass filter cutoff frequency in Hz (default 1e10Hz, i.e. no filtering)
float LP_radio = 400;         //Radio input lowpass filter cutoff frequency in Hz (default 400Hz)

//AHRS Parameters
float ahrs_MadgwickB = 0.041;     //ahrs_Madgwick filter parameter
float ahrs_Mahony2KP = (2.0f * 0.5f);		// Mahony: 2 * proportional gain (Kp)
float ahrs_Mahony2KI = (2.0f * 0.0f);		// Mahony: 2 * integral gain (Ki)

//Controller parameters (take note of defaults before modifying!): 
float i_limit = 25.0;         //Integrator saturation level, mostly for safety (default 25.0)
float maxRoll = 30.0;         //Max roll angle in degrees for angle mode (maximum ~70 degrees)
float maxPitch = 30.0;        //Max pitch angle in degrees for angle mode (maximum ~70 degrees)
float maxRollRate = 30.0;     //Max roll rate in deg/sec for rate mode 
float maxPitchRate = 30.0;    //Max pitch rate in deg/sec for rate mode
float maxYawRate = 160.0;     //Max yaw rate in deg/sec for angle and rate mode

float Kp_roll_angle = 0.2;    //Roll P-gain - angle mode 
float Ki_roll_angle = 0.3;    //Roll I-gain - angle mode
float Kd_roll_angle = 0.05;   //Roll D-gain - angle mode (has no effect on control_Angle2)
float B_loop_roll = 0.9;      //Roll damping term for control_Angle2(), lower is more damping (must be between 0 to 1)
float Kp_pitch_angle = 0.2;   //Pitch P-gain - angle mode
float Ki_pitch_angle = 0.3;   //Pitch I-gain - angle mode
float Kd_pitch_angle = 0.05;  //Pitch D-gain - angle mode (has no effect on control_Angle2)
float B_loop_pitch = 0.9;     //Pitch damping term for control_Angle2(), lower is more damping (must be between 0 to 1)

float Kp_roll_rate = 0.15;    //Roll P-gain - rate mode
float Ki_roll_rate = 0.2;     //Roll I-gain - rate mode
float Kd_roll_rate = 0.0002;  //Roll D-gain - rate mode (be careful when increasing too high, motors will begin to overheat!)
float Kp_pitch_rate = 0.15;   //Pitch P-gain - rate mode
float Ki_pitch_rate = 0.2;    //Pitch I-gain - rate mode
float Kd_pitch_rate = 0.0002; //Pitch D-gain - rate mode (be careful when increasing too high, motors will begin to overheat!)

float Kp_yaw = 0.3;           //Yaw P-gain
float Ki_yaw = 0.05;          //Yaw I-gain
float Kd_yaw = 0.00015;       //Yaw D-gain (be careful when increasing too high, motors will begin to overheat!)

//========================================================================================================================//
//                              DECLARE GLOBAL VARIABLES
//========================================================================================================================//

//General stuff
float loop_dt;
uint32_t loop_time; //loop timestamp
uint32_t loop_cnt = 0; //loop counter
uint32_t print_time;
bool print_need_newline;
uint32_t loop_rt, loop_rt_imu; //runtime of loop and imu sensor retrieval

//Radio communication:
int rcin_pwm[RCIN_NUM_CHANNELS]; //filtered raw PWM values
float rcin_thro, rcin_roll, rcin_pitch, rcin_yaw; //rcin_thro 0(cutoff) to 1(full); rcin_roll, rcin_pitch, rcin_yaw -1(left,down) to 1(right,up) with 0 center stick
bool rcin_armed; //status of arm switch, true = armed
bool rcin_thro_is_low; //status of throttle stick, true = throttle low
int rcin_aux; // six position switch connected to aux channel, values 0-5

//IMU:
float AccX, AccY, AccZ, GyroX, GyroY, GyroZ, MagX, MagY, MagZ;
float ahrs_roll, ahrs_pitch, ahrs_yaw;  //ahrs_Madgwick() estimate output in degrees. Positive angles are: roll right, yaw right, pitch up

//External magnetometer:
float mag_x = 0, mag_y = 0, mag_z = 0;

//Controller:
float roll_PID = 0, pitch_PID = 0, yaw_PID = 0;

//Mixer output (motor and servo values are scaled 0.0 to 1.0)
//Outputs:
float out_command[HW_OUT_COUNT] = {0}; //Mixer outputs
PWM out[HW_OUT_COUNT]; //ESC and Servo outputs

//Flight status
bool out_armed = false; //motors will only run if this flag is true

//Conversion
float lowpass_to_beta(float,float); //prototype
const float rad_to_deg = 57.29577951; //radians to degrees conversion constant
float B_accel = lowpass_to_beta(LP_accel, loop_freq);
float B_gyro = lowpass_to_beta(LP_gyro, loop_freq);
float B_mag = lowpass_to_beta(LP_mag, loop_freq);
float B_radio = lowpass_to_beta(LP_radio, loop_freq);

//========================================================================================================================//
//                                                       SETUP()                                                          //
//========================================================================================================================//

void setup() {
  //Set built in LED to turn on to signal startup
  pinMode(HW_PIN_LED, OUTPUT);
  led_SwitchON(true);

  //start console serial
  Serial.begin(115200);

  //3 second startup delay
  for(int i=10;i>0;i--) { 
    Serial.printf(APPNAME " starting %d ...\n",i);
    delay(300);
  }

  //hardware specific setup for spi and Wire (see hw_xxx.h)
  hw_setup();

  Serial.println("HW_BOARD_NAME=" HW_BOARD_NAME);
#ifdef HW_MCU
  Serial.println("HW_MCU=" HW_MCU);
#endif  
#ifdef USE_IMU_POLLING
  Serial.println("USE_IMU_POLLING");
#endif

  //print pin config
  Serial.printf("HW_PIN_LED=%d\n", HW_PIN_LED);
  Serial.printf("HW_PIN_SPI_MOSI=%d MISO=%d SCLK=%d\n", HW_PIN_SPI_MOSI, HW_PIN_SPI_MISO, HW_PIN_SPI_SCLK);
  Serial.printf("HW_PIN_IMU_CS=%d\n",HW_PIN_IMU_CS);  
  Serial.printf("HW_PIN_IMU_EXTI=%d\n",HW_PIN_IMU_EXTI);
  Serial.printf("HW_PIN_I2C_SDA=%d SCL=%d\n", HW_PIN_I2C_SDA, HW_PIN_I2C_SCL);
  Serial.printf("HW_PIN_OUT[%d]=%d", HW_OUT_COUNT, HW_PIN_OUT[0]);  for(int i=1; i<HW_OUT_COUNT; i++) Serial.printf(",%d", HW_PIN_OUT[i]);  Serial.println();
  Serial.printf("HW_PIN_RCIN_RX=%d TX=%d\n", HW_PIN_RCIN_RX, HW_PIN_RCIN_TX);
  Serial.printf("HW_PIN_GPS_RX=%d TX=%d\n", HW_PIN_GPS_RX, HW_PIN_GPS_TX);

  //debug i2c
  print_i2c_scan();
  
  //Initialize radio communication. Set correct USE_RCIN_xxx user specified defines above. Note: rcin_Setup() function is defined in rcin.h, but normally no changes needed there.
  rcin_Setup();

  //Set radio channels to default (safe) values before entering main loop
  for(int i=0;i<RCIN_NUM_CHANNELS;i++) rcin_pwm[i] = rcin_pwm_fs[i];

  //IMU
  while(true) {
    int rv = imu_Setup(); //returns 0 on success, positive on error, negative on warning
    if(rv<=0) break; 
    warn("IMU: init failed rv= " + String(rv) + ". Retrying...\n");
  }

  //Barometer, External Magnetometer, Gps, battery monitor, blackbox
  baro.setup();
  mag_Setup();
  gps_setup();   
  //gps_debug(); //uncomment to debug gps messages
  bat.setup();
  bb.setup();

  //Servos (set servos first just in case motors overwrite frequency of shared timers)
  for(int i=out_MOTOR_COUNT;i<HW_OUT_COUNT;i++) {
    out[i].begin(HW_PIN_OUT[i], 50, 1000, 2000); //Standard servo at 50Hz

    out_command[i] = 0; //keep at 0 if you are using servo outputs for motors
    out[i].writeFactor(out_command[i]); //start the PWM output to the servos
  } 

  //Motors
  for(int i=0;i<out_MOTOR_COUNT;i++) {
    //uncomment one line - sets pin, frequency (Hz), minimum (us), maximum (us)
    out[i].begin(HW_PIN_OUT[i], 400, 950, 2000); //Standard PWM: 400Hz, 950-2000 us
    //out[i].begin(HW_PIN_OUT[i], 2000, 125, 250); //Oneshot125: 2000Hz, 125-250 us

    out_command[i] = 0; //set output to 0 for motors
    out[i].writeFactor(out_command[i]); //start the PWM output to the motors
  }

  //Get IMU error to zero accelerometer and gyro readings, assuming vehicle is level when powered up
  calibrate_IMU_error(); //Calibration parameters printed to serial monitor. Paste these in the user specified variables section, then comment this out. (Or.. keep this active to calibrate on every startup.)

  //If using IMU with magnetometer, uncomment for one-time magnetometer calibration (may need to repeat for new locations)
  //calibrate_Magnetometer(); //Generates magentometer error and scale factors to be pasted in user-specified variables section

  //calibrate_ESCs(); //PROPS OFF. Uncomment this to calibrate your ESCs by setting throttle stick to max, powering on, and lowering throttle to zero after the beeps
  //Code will not proceed past here if this function is uncommented!

  //set quarterion to initial yaw, so that AHRS settles faster
  ahrs_Setup();

  //set times for loop
  loop_RateBegin();

  //start IMU interrupt - do this last in setup(), as the IMU interrupt handler needs all modules configured
  #ifndef USE_IMU_POLLING
    imu_interrupt_setup(); //setup imu interrupt handlers
    delay(100);
    if(loop_cnt==0) die("IMU interrupt not firing.");
  #endif

  //Set built in LED off to signal end of startup
  led_SwitchON(false);
}

//========================================================================================================================//
//                                                            LOOP()                                                      //
//========================================================================================================================//

void loop() {
  //only call imu_loop here if we're not using interrupts
  #ifdef USE_IMU_POLLING
    while ( (micros() - loop_time) < (1000000U / loop_freq) ); //Keeps loop sample rate constant. (Waste time until sample time has passed.)
    imu_loop();
  #endif

  #ifdef USE_IMU_BUS_SPI
    //if IMU uses SPI bus, then read slower i2c sensors here in loop() to keep imu_loop() as fast as possible
    i2c_sensors_update();
  #endif

  //update gps & battery
  gps_loop();
  if(bat.loop()) bb.log_bat(); //log if battery was updated

  //send telemetry
  static uint32_t rcin_telem_ts = 0;
  static uint32_t rcin_telem_cnt = 0;
  if(millis() - rcin_telem_ts > 100) {
    rcin_telem_ts = millis();
    rcin_telem_cnt++;
    rcin_telemetry_flight_mode("madflight"); //only first 14 char get transmitted
    rcin_telemetry_attitude(ahrs_pitch, ahrs_roll, ahrs_yaw);
    if(rcin_telem_cnt % 10 == 0) rcin_telemetry_battery(bat.v, bat.i, bat.mah, 100);
    if(rcin_telem_cnt % 10 == 5) rcin_telemetry_gps(gps.lat, gps.lon, gps.sog/278, gps.cog/1000, (gps.alt<0 ? 0 : gps.alt/1000), gps.sat); // sog/278 is conversion from mm/s to km/h 
  }

  //Debugging - Print data at print_interval microseconds, uncomment line(s) for troubleshooting
  uint32_t print_interval = 100000;
  if (micros() - print_time > print_interval) {
    print_time += print_interval;
    print_need_newline = false;
    //Serial.printf("loop_time:%d\t",loop_time); //print loop time stamp
    //print_overview(); //prints: pwm1, rcin_roll, gyroX, accX, magX, ahrs_roll, pid_roll, motor1, loop_rt
    //print_rcin_RadioPWM();     //Prints radio pwm values (expected: 1000 to 2000)
    //print_rcin_RadioScaled();     //Prints scaled radio values (expected: -1 to 1)    
    //print_imu_GyroData();      //Prints filtered gyro data direct from IMU (expected: -250 to 250, 0 at rest)
    //print_imu_AccData();     //Prints filtered accelerometer data direct from IMU (expected: -2 to 2; x,y 0 when level, z 1 when level)
    //print_imu_MagData();       //Prints filtered magnetometer data direct from IMU (expected: -300 to 300)
    //print_ahrs_RollPitchYaw();  //Prints roll, pitch, and yaw angles in degrees from ahrs_Madgwick filter (expected: degrees, 0 when level)
    //print_control_PIDoutput();     //Prints computed stabilized PID variables from controller and desired setpoint (expected: ~ -1 to 1)
    //print_out_MotorCommands(); //Prints the values being written to the motors (expected: 0 to 1)
    //print_out_ServoCommands(); //Prints the values being written to the servos (expected: 0 to 1)
    //print_loop_Rate();      //Prints the time between loops in microseconds (expected: 1000000 / loop_freq)
    print_bat(); //Prints battery voltage, current, Ah used and Wh used
    //Serial.printf("press:%.1f\ttemp:%.2f\t",baro.press_pa, baro.temp_c); //Prints barometer data
    if(print_need_newline) Serial.println();
    loop_rt = 0; //reset maximum
  }

  while(Serial.available()) {
    char c = Serial.read();
    if(c == 'b') bb.csvDump();
  }
}

void i2c_sensors_update() {
  if(baro.update()) bb.log_baro(); //log if pressure updated
  mag_Read(&mag_x, &mag_y, &mag_z);
}

//========================================================================================================================//
//                                                            IMU_LOOP()                                                      //
//========================================================================================================================//
  /*
   * It's good to operate at a constant loop rate for filters to remain stable and whatnot. Interrupt routines running in the
   * background cause the loop rate to fluctuate. This function basically just waits at the end of every loop iteration until 
   * the correct time has passed since the start of the current loop for the desired loop rate in Hz. 2kHz is a good rate to 
   * be at because the loop nominally will run between 2.8kHz - 4.2kHz. This lets us have a little room to add extra computations
   * and remain above 2kHz, without needing to retune all of our filtering parameters.
   */
   
void imu_loop() {
  //update loop_ variables
  uint32_t now = micros();
  loop_dt = (now - loop_time)/1000000.0;
  loop_time = now;
  loop_cnt++;

  //Blink LED
  loop_Blink();

  //Get vehicle state
  imu_GetData(); //Pulls raw gyro, accelerometer, and magnetometer data from IMU and applies low-pass filters to remove noise
  //ahrs filter method: Madgwick or Mahony - SELECT ONE:
  //ahrs_Madgwick(GyroX, GyroY, GyroZ, AccX, AccY, AccZ, MagX, MagY, MagZ, loop_dt); //Madgwick filter quaternion update
  ahrs_Mahony(GyroX, GyroY, GyroZ, AccX, AccY, AccZ, MagX, MagY, MagZ, loop_dt); //Mahony filter quaternion update
  ahrs_ComputeAngles(&ahrs_roll, &ahrs_pitch, &ahrs_yaw); //get ahrs_roll, ahrs_pitch, and ahrs_yaw angle estimates (degrees) from quaterion

  //Get radio state
  rcin_GetCommands(); //Pulls current available radio commands
  rcin_Failsafe(); //Prevent failures in event of bad receiver connection, defaults to failsafe values assigned in setup
  rcin_Normalize(); //Convert raw commands to normalized values based on saturated control limits

  //Uncomment to debug without remote (and no battery!) - pitch drone up: motors m1,m3 should increase and m2,m4 decrease; bank right: m1,m2 increase; yaw right: m1,m4 increase
  //rcin_thro = 0.5; rcin_thro_is_low = false; rcin_roll = 0; rcin_pitch = 0; rcin_yaw = 0; rcin_armed = true; rcin_aux = 0; out_armed = true;

  //PID Controller - SELECT ONE:
  control_Angle(rcin_thro_is_low); //Stabilize on pitch/roll angle setpoint, stabilize yaw on rate setpoint
  //control_Angle2(rcin_thro_is_low); //Stabilize on pitch/roll setpoint using cascaded method. Rate controller must be tuned well first!
  //control_Rate(rcin_thro_is_low); //Stabilize on rate setpoint

  //Actuator mixing
  control_Mixer(); //Mixes PID outputs to scaled actuator commands -- custom mixing assignments done here

  //Command actuators
  out_KillSwitch(); //Cut all motor outputs if DISARMED.
  out_SetCommands(); //Sends command pulses to motors (only if out_armed=true) and servos

#ifdef USE_IMU_BUS_I2C
  //if IMU uses i2c bus, then get i2c sensor readings in the imu_loop to prevent i2c bus collisions
  i2c_sensors_update();
#endif

  //record max runtime loop
  uint32_t rt = micros() - loop_time;
  if(loop_rt < rt) loop_rt = rt;

  //bb.log_imu(); //full speed black box logging imu data, fills up memory quickly...
}

//========================================================================================================================//
//                                          IMU INTERRUPT HANDLER                                                         //
//========================================================================================================================//
// This runs the IMU updates triggered from pin HW_PIN_IMU_EXTI interrupt. By doing this, unused time can be used in loop() 
// for other functionality. With USE_IMU_POLLING any unused time in loop() is wasted. When using FreeRTOS with 
// HW_USE_FREERTOS the IMU update is not executed directly in the interrupt handler, but a high priority task is used. 
// This prevents FreeRTOS watchdog resets. The delay (latency) from rising edge INT pin to start of imu_loop is approx. 
// 10 us on ESP32 and 50 us on RP2040.
#ifndef USE_IMU_POLLING
  #ifdef HW_USE_FREERTOS
    TaskHandle_t imu_task_handle;

    void imu_interrupt_setup() {
      xTaskCreate(imu_task, "imu_task", 4096, NULL, HW_RTOS_IMUTASK_PRIORITY /*priority 0=lowest*/, &imu_task_handle);
      //vTaskCoreAffinitySet(IsrTaskHandle, 0);
      attachInterrupt(digitalPinToInterrupt(HW_PIN_IMU_EXTI), imu_interrupt_handler, RISING); 
    }

    void imu_task(void*) {
      for(;;) {
        ulTaskNotifyTake( pdTRUE, portMAX_DELAY );
        imu_loop();
      }
    }

    void imu_interrupt_handler() {
      //let imu_task handle the interrupt
      BaseType_t xHigherPriorityTaskWoken = pdFALSE;
      vTaskNotifyGiveFromISR(imu_task_handle, &xHigherPriorityTaskWoken);
      portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
    }
  #else
    volatile bool imu_interrupt_busy = false;
    
    void imu_interrupt_setup() {
      attachInterrupt(digitalPinToInterrupt(HW_PIN_IMU_EXTI), imu_interrupt_handler, RISING);
    }

    void imu_interrupt_handler() {
      if(imu_interrupt_busy) return;
      imu_interrupt_busy = true;
      imu_loop();
      imu_interrupt_busy = false;
    } 
  #endif
#endif

//========================================================================================================================//
//                                          SETUP1() LOOP1() EXECUTING ON SECOND CORE (only ESP32 and RP2040)             //
//========================================================================================================================//
//Uncomment setup1() and/or loop1() to use the second core on ESP32 / RP2040
/*
void setup1() {
  Serial.println("setup1()");
}
void loop1() {
  Serial.println("loop1()"); delay(100);
}
//*/

//========================================================================================================================//
//                      IMU_LOOP() FUNCTIONS - in same order as they are called from imu_loop()                           //
//========================================================================================================================//

void loop_Blink() {
  //Blink LED once per second, if LED blinks slower then the loop takes too much time, use print_loop_Rate() to investigate.
  //DISARMED: long off, short on, ARMED: long on, short off
  if(loop_cnt % loop_freq <= loop_freq / 10)
    led_SwitchON(!out_armed); //short interval
  else
    led_SwitchON(out_armed); //long interval
}

// Reads accelerometer, gyro, and magnetometer data from IMU and stores it as AccX, AccY, AccZ, GyroX, GyroY, GyroZ, MagX, MagY, MagZ. 
// A simple first-order low-pass filter is used to get rid of high frequency noise in these raw signals. 
// Finally, the constant errors found in calibrate_IMU_error() on startup are subtracted from the accelerometer and gyro readings.
void imu_GetData() {
  float ax,ay,az,gx,gy,gz,mx=0,my=0,mz=0;
  uint32_t t1 = micros();
  //imu_Read() returns correctly NED oriented and correctly scaled values in g's, deg/sec, and uT (see file imu.h)
  imu_Read(&ax, &ay, &az, &gx, &gy, &gz, &mx, &my, &mz);
  loop_rt_imu = micros() - t1;

 //Accelerometer
  //Correct the outputs with the calculated error values
  ax = ax - AccErrorX;
  ay = ay - AccErrorY;
  az = az - AccErrorZ;
  //LP filter accelerometer data
  AccX = (1.0 - B_accel) * AccX + B_accel * ax;
  AccY = (1.0 - B_accel) * AccY + B_accel * ay;
  AccZ = (1.0 - B_accel) * AccZ + B_accel * az;

  //Gyro
  //Correct the outputs with the calculated error values
  gx = gx - GyroErrorX;
  gy = gy - GyroErrorY;
  gz = gz - GyroErrorZ;
  //LP filter gyro data
  GyroX = (1.0 - B_gyro) * GyroX + B_gyro * gx;
  GyroY = (1.0 - B_gyro) * GyroY + B_gyro * gy;
  GyroZ = (1.0 - B_gyro) * GyroZ + B_gyro * gz;

  //Magnetometer 
  //use external (if present), then internal mag
  if(!(mag_x == 0 && mag_y == 0 && mag_z == 0)) {
    mx = mag_x;
    my = mag_y;
    mz = mag_z;

  }
  if(mx == 0 && my == 0 && mz == 0) {
    MagX = 0;
    MagY = 0;
    MagZ = 0;
  }else{
    //Correct the outputs with the calculated error values
    mx = (mx - MagErrorX) * MagScaleX;
    my = (my - MagErrorY) * MagScaleY;
    mz = (mz - MagErrorZ) * MagScaleZ;
    //LP filter magnetometer data
    MagX = (1.0 - B_mag) * MagX + B_mag * mx;
    MagY = (1.0 - B_mag) * MagY + B_mag * my;
    MagZ = (1.0 - B_mag) * MagZ + B_mag * mz;
  }
}

void rcin_GetCommands() {
  //DESCRIPTION: Get raw PWM values for every channel from the radio
  /*
   * Updates radio PWM commands in loop based on current available commands. channel_x_pwm is the raw command used in the rest of 
   * the loop. If using a PWM or PPM receiver, the radio commands are retrieved from a function in the readPWM file separate from this one which 
   * is running a bunch of interrupts to continuously update the radio readings. If using an SBUS receiver, the values are pulled from the SBUS library directly.
   * The raw radio commands are filtered with a first order low-pass filter to eliminate any really high frequency noise. 
   */

  int pwm_new[RCIN_NUM_CHANNELS];
  bool got_new_data = rcin_GetPWM(pwm_new);

  //Low-pass the critical commands and update previous values
  for(int i=0; i<RCIN_NUM_CHANNELS; i++) {
    rcin_pwm[i] = lrintf( (1.0 - B_radio)*rcin_pwm[i] + B_radio*pwm_new[i] );
  }

  if(got_new_data) {
    //Serial.print("rcin_GetCommands() "); for(int i=0; i<RCIN_NUM_CHANNELS; i++) Serial.printf("CH%d:%d->%d ",i+1,pwm_new[i],rcin_pwm[i]); Serial.println(); //uncomment for debugging
  }
}

void rcin_Failsafe() {
  //DESCRIPTION: If radio gives garbage values, set all commands to default values
  /*
   * Radio connection failsafe used to check if the getCommands() function is returning acceptable pwm values. If any of 
   * the commands are lower than 800 or higher than 2200, then we can be certain that there is an issue with the radio
   * connection (most likely hardware related). If any of the channels show this failure, then all of the radio commands 
   * channel_x_pwm are set to default failsafe values specified in the setup. Comment out this function when troubleshooting 
   * your radio connection in case any extreme values are triggering this function to overwrite the print_need_newline variables.
   */
  int minVal = 800;
  int maxVal = 2200;
  for(int i=0;i<RCIN_NUM_CHANNELS;i++) {
    if (rcin_pwm[i] > maxVal || rcin_pwm[i] < minVal) {
      //If any failures, set to default failsafe values
      for(int i=0;i<RCIN_NUM_CHANNELS;i++) rcin_pwm[i] = rcin_pwm_fs[i];
      return;
    }
  }  
}

void rcin_Normalize() {
  //DESCRIPTION: Normalizes desired control values to appropriate values
  /*
   * Updates the normalized rcin variables rcin_thro, rcin_roll, rcin_pitch, and rcin_yaw. These are computed by using the raw
   * RC pwm commands and scaling them to be within our limits defined in setup. rcin_thro stays within 0 to 1 range.
   * rcin_roll, rcin_pitch, and rcin_yaw are scaled to -1 to +1.
   */

  //normalized values
  //throttle
  int pwm = rcin_pwm[rcin_cfg_thro_channel-1];
  rcin_thro = constrain( ((float)(pwm - rcin_cfg_thro_min)) / (rcin_cfg_thro_max - rcin_cfg_thro_min), 0.0, 1.0); // output: 0 (throttle cutoff, stick back) to 1 (full throttle, stick forward),
  rcin_thro_is_low = (pwm <= rcin_cfg_thro_low); 

  //roll,pitch,yaw
  rcin_roll = _rcin_ChannelNormalize(rcin_pwm[rcin_cfg_roll_channel-1], rcin_cfg_pwm_min, rcin_cfg_pwm_center, rcin_cfg_pwm_max, rcin_cfg_pwm_deadband); // output: -1 (roll left, stick left) to 1 (roll right, stick right)
  rcin_pitch = - _rcin_ChannelNormalize(rcin_pwm[rcin_cfg_pitch_channel-1], rcin_cfg_pwm_min, rcin_cfg_pwm_center, rcin_cfg_pwm_max, rcin_cfg_pwm_deadband); // output: -1 (pitch down, stick back) to 1 (pitch up, stick forward)
  rcin_yaw = _rcin_ChannelNormalize(rcin_pwm[rcin_cfg_yaw_channel-1], rcin_cfg_pwm_min, rcin_cfg_pwm_center, rcin_cfg_pwm_max, rcin_cfg_pwm_deadband); //  output: -1 (yaw left, stick left) to 1 (yaw right, stick right)

  //arm switch
  pwm = rcin_pwm[rcin_cfg_arm_channel-1];
  rcin_armed = (rcin_cfg_arm_min <= pwm && pwm <= rcin_cfg_arm_max);

  //auc 6 position switch
  int spacing = (rcin_cfg_aux_max - rcin_cfg_aux_min) / 5;
  rcin_aux = ( rcin_pwm[rcin_cfg_aux_channel-1] - rcin_cfg_aux_min + spacing/2) / spacing; //output 0,1,2,3,4,5
}

//helper to nomalize a channel based on min,center,max calibration
float _rcin_ChannelNormalize(int val, int min, int center, int max, int deadband) {
  int rev = 1; //1=normal, -1=reverse channel
  //needs: min < center < max
  if(val<min) return rev * -1.0;
  if(val<center-deadband) return (float)(rev * (val-(center-deadband))) / ((center-deadband)-min); //returns -1 to 0
  if(val<center+deadband) return 0;
  if(val<max) return (float)(rev * (val-(center+deadband))) / (max-(center+deadband)); 
  return rev * 1.0;
}

void control_Angle(bool zero_integrators) {
  //DESCRIPTION: Computes control commands based on state error (angle)
  /*
   * Basic PID control to stablize on angle setpoint based on desired states roll_des, pitch_des, and yaw_des computed in 
   * rcin_Normalize(). Error is simply the desired state minus the actual state (ex. roll_des - ahrs_roll). Two safety features
   * are implimented here regarding the I terms. The I terms are saturated within specified limits on startup to prevent 
   * excessive buildup. This can be seen by holding the vehicle at an angle and seeing the motors ramp up on one side until
   * they've maxed out throttle...saturating I to a specified limit fixes this. The second feature defaults the I terms to 0
   * if the throttle is at the minimum setting. This means the motors will not start spooling up on the ground, and the I 
   * terms will always start from 0 on takeoff. This function updates the variables roll_PID, pitch_PID, and yaw_PID which
   * can be thought of as 1-D stablized signals. They are mixed to the configuration of the vehicle in control_Mixer().
   */ 

  //inputs: roll_des, pitch_des, yawRate_des
  //outputs: roll_PID, pitch_PID, yaw_PID

  //desired values
  float roll_des = rcin_roll * maxRoll; //Between -maxRoll and +maxRoll
  float pitch_des = rcin_pitch * maxPitch; //Between -maxPitch and +maxPitch
  float yawRate_des = rcin_yaw * maxYawRate; //Between -maxYawRate and +maxYawRate

  //Serial.printf("r_des:%+.2f p_des:%+.2f y_des:%+.2f",roll_des,pitch_des,yawRate_des);

  //state vars
  static float integral_roll, integral_pitch, error_yaw_prev, integral_yaw;

  //Zero the integrators (used to don't let integrator build if throttle is too low, or to re-start the controller)
  if(zero_integrators) {
    integral_roll = 0;
    integral_pitch = 0;
    integral_yaw = 0;
  }

  //Roll PID
  float error_roll = roll_des - ahrs_roll;
  integral_roll += error_roll * loop_dt;
  integral_roll = constrain(integral_roll, -i_limit, i_limit); //Saturate integrator to prevent unsafe buildup
  float derivative_roll = GyroX;
  roll_PID = 0.01 * (Kp_roll_angle*error_roll + Ki_roll_angle*integral_roll - Kd_roll_angle*derivative_roll); //Scaled by .01 to bring within -1 to 1 range

  //Pitch PID
  float error_pitch = pitch_des - ahrs_pitch;
  integral_pitch += error_pitch * loop_dt;
  integral_pitch = constrain(integral_pitch, -i_limit, i_limit); //Saturate integrator to prevent unsafe buildup
  float derivative_pitch = GyroY; 
  pitch_PID = 0.01 * (Kp_pitch_angle*error_pitch + Ki_pitch_angle*integral_pitch - Kd_pitch_angle*derivative_pitch); //Scaled by .01 to bring within -1 to 1 range

  //Yaw PID, stablize on rate from GyroZ
  float error_yaw = yawRate_des - GyroZ;
  integral_yaw += error_yaw * loop_dt;
  integral_yaw = constrain(integral_yaw, -i_limit, i_limit); //Saturate integrator to prevent unsafe buildup
  float derivative_yaw = (error_yaw - error_yaw_prev) / loop_dt; 
  yaw_PID = 0.01 * (Kp_yaw*error_yaw + Ki_yaw*integral_yaw + Kd_yaw*derivative_yaw); //Scaled by .01 to bring within -1 to 1 range

  //Update derivative variables
  error_yaw_prev = error_yaw;
}

void control_Angle2(bool zero_integrators) {
  //DESCRIPTION: Computes control commands based on state error (angle) in cascaded scheme
  /*
   * Gives better performance than control_Angle() but requires much more tuning. Not reccommended for first-time setup.
   * See the documentation for tuning this controller.
   */

  //inputs: roll_des, pitch_des, yawRate_des
  //outputs: roll_PID, pitch_PID, yaw_PID

  //desired values
  float roll_des = rcin_roll * maxRoll; //Between -maxRoll and +maxRoll
  float pitch_des = rcin_pitch * maxPitch; //Between -maxPitch and +maxPitch
  float yawRate_des = rcin_yaw * maxYawRate; //Between -maxYawRate and +maxYawRate

  //state vars
  static float integral_roll_ol, integral_roll_il, error_roll_prev, roll_IMU_prev, roll_des_prev;
  static float integral_pitch_ol, integral_pitch_il, error_pitch_prev, pitch_IMU_prev, pitch_des_prev;
  static float integral_yaw, error_yaw_prev;

  //Zero the integrators (used to don't let integrator build if throttle is too low, or to re-start the controller)
  if(zero_integrators) {
    integral_roll_ol = 0;
    integral_roll_il = 0;
    integral_pitch_ol = 0;
    integral_pitch_il = 0;
    integral_yaw = 0;
  }

  //Outer loop - PID on angle for roll & pitch
  //Roll
  float error_roll = roll_des - ahrs_roll;
  integral_roll_ol += error_roll * loop_dt;
  integral_roll_ol = constrain(integral_roll_ol, -i_limit, i_limit); //Saturate integrator to prevent unsafe buildup
  float derivative_roll = (ahrs_roll - roll_IMU_prev) / loop_dt;  
  float roll_des_ol = Kp_roll_angle*error_roll + Ki_roll_angle*integral_roll_ol;// - Kd_roll_angle*derivative_roll;

  //Pitch
  float error_pitch = pitch_des - ahrs_pitch; 
  integral_pitch_ol += error_pitch * loop_dt;
  integral_pitch_ol = constrain(integral_pitch_ol, -i_limit, i_limit); //saturate integrator to prevent unsafe buildup
  float derivative_pitch = (ahrs_pitch - pitch_IMU_prev) / loop_dt; 
  float pitch_des_ol = Kp_pitch_angle*error_pitch + Ki_pitch_angle*integral_pitch_ol;// - Kd_pitch_angle*derivative_pitch;

  //Apply loop gain, constrain, and LP filter for artificial damping
  float Kl = 30.0;
  roll_des_ol = Kl*roll_des_ol;
  pitch_des_ol = Kl*pitch_des_ol;
  roll_des_ol = constrain(roll_des_ol, -240.0, 240.0);
  pitch_des_ol = constrain(pitch_des_ol, -240.0, 240.0);
  roll_des_ol = (1.0 - B_loop_roll)*roll_des_prev + B_loop_roll*roll_des_ol;
  pitch_des_ol = (1.0 - B_loop_pitch)*pitch_des_prev + B_loop_pitch*pitch_des_ol;

  //Inner loop - PID on rate for roll & pitch
  //Roll
  error_roll = roll_des_ol - GyroX;
  integral_roll_il += error_roll * loop_dt;
  integral_roll_il = constrain(integral_roll_il, -i_limit, i_limit); //Saturate integrator to prevent unsafe buildup 
  derivative_roll = (error_roll - error_roll_prev) / loop_dt;
  roll_PID = 0.01 * (Kp_roll_rate*error_roll + Ki_roll_rate*integral_roll_il + Kd_roll_rate*derivative_roll); //Scaled by .01 to bring within -1 to 1 range

  //Pitch
  error_pitch = pitch_des_ol - GyroY;
  integral_pitch_il += error_pitch * loop_dt;
  integral_pitch_il = constrain(integral_pitch_il, -i_limit, i_limit); //Saturate integrator to prevent unsafe buildup
  derivative_pitch = (error_pitch - error_pitch_prev) / loop_dt; 
  pitch_PID = 0.01 * (Kp_pitch_rate*error_pitch + Ki_pitch_rate*integral_pitch_il + Kd_pitch_rate*derivative_pitch); //Scaled by .01 to bring within -1 to 1 range
  
  //Single loop
  //Yaw
  float error_yaw = yawRate_des - GyroZ;
  integral_yaw += error_yaw * loop_dt;    
  integral_yaw = constrain(integral_yaw, -i_limit, i_limit); //Saturate integrator to prevent unsafe buildup
  float derivative_yaw = (error_yaw - error_yaw_prev) / loop_dt;  
  yaw_PID = 0.01 * (Kp_yaw*error_yaw + Ki_yaw*integral_yaw + Kd_yaw*derivative_yaw); //Scaled by .01 to bring within -1 to 1 range
  
  //Update derivative variables
  error_roll_prev = error_roll;
  roll_IMU_prev = ahrs_roll;
  roll_des_prev = roll_des_ol;
  error_pitch_prev = error_pitch;
  pitch_IMU_prev = ahrs_pitch;
  pitch_des_prev = pitch_des_ol;
  error_yaw_prev = error_yaw;
}

void control_Rate(bool zero_integrators) {
  //DESCRIPTION: Computes control commands based on state error (rate)
  /*
   * See explanation for control_Angle(). Everything is the same here except the error is now the desired rate - raw gyro reading.
   */

  //inputs: roll_des, pitch_des, yawRate_des
  //outputs: roll_PID, pitch_PID, yaw_PID

  //desired values
  float rollRate_des = rcin_roll * maxRollRate; //Between -maxRoll and +maxRoll
  float pitchRate_des = rcin_pitch * maxPitchRate; //Between -maxPitch and +maxPitch
  float yawRate_des = rcin_yaw * maxYawRate; //Between -maxYawRate and +maxYawRate 
  
  //state vars
  static float integral_roll, error_roll_prev;
  static float integral_pitch, error_pitch_prev;
  static float integral_yaw, error_yaw_prev;

  //Zero the integrators (used to don't let integrator build if throttle is too low, or to re-start the controller)
  if(zero_integrators) {
    integral_roll = 0;
    integral_pitch = 0;
    integral_yaw = 0;
  }

  //Roll
  float error_roll = rollRate_des - GyroX;
  integral_roll += error_roll * loop_dt;
  integral_roll = constrain(integral_roll, -i_limit, i_limit); //Saturate integrator to prevent unsafe buildup
  float derivative_roll = (error_roll - error_roll_prev) / loop_dt;
  roll_PID = 0.01 * (Kp_roll_rate*error_roll + Ki_roll_rate*integral_roll + Kd_roll_rate*derivative_roll); //Scaled by .01 to bring within -1 to 1 range

  //Pitch
  float error_pitch = pitchRate_des - GyroY;
  integral_pitch += error_pitch * loop_dt;
  integral_pitch = constrain(integral_pitch, -i_limit, i_limit); //Saturate integrator to prevent unsafe buildup
  float derivative_pitch = (error_pitch - error_pitch_prev) / loop_dt;   
  pitch_PID = 0.01 * (Kp_pitch_rate*error_pitch + Ki_pitch_rate*integral_pitch + Kd_pitch_rate*derivative_pitch); //Scaled by .01 to bring within -1 to 1 range

  //Yaw, stablize on rate from GyroZ
  float error_yaw = yawRate_des - GyroZ;
  integral_yaw += error_yaw * loop_dt;
  integral_yaw = constrain(integral_yaw, -i_limit, i_limit); //Saturate integrator to prevent unsafe buildup
  float derivative_yaw = (error_yaw - error_yaw_prev) / loop_dt; 
  yaw_PID = 0.01 * (Kp_yaw*error_yaw + Ki_yaw*integral_yaw + Kd_yaw*derivative_yaw); //Scaled by .01 to bring within -1 to 1 range

  //Update derivative variables
  error_roll_prev = error_roll;
  error_pitch_prev = error_pitch;
  error_yaw_prev = error_yaw;
}

void control_Mixer() {
  //DESCRIPTION: Mixes scaled commands from PID controller to actuator outputs based on vehicle configuration
  /*
   * Takes roll_PID, pitch_PID, and yaw_PID computed from the PID controller and appropriately mixes them for the desired
   * vehicle configuration. For example on a quadcopter, the left two motors should have +roll_PID while the right two motors
   * should have -roll_PID. Front two should have +pitch_PID and the back two should have -pitch_PID etc... every motor has
   * normalized (0 to 1) rcin_thro command for throttle control. Can also apply direct unstabilized commands from the transmitter with 
   * rcin_xxx variables are to be sent to the motor ESCs and servos.
   * 
   *Relevant variables:
   *rcin_thro - direct thottle control
   *roll_PID, pitch_PID, yaw_PID - stabilized axis variables
   *rcin_roll, rcin_pitch, rcin_yaw - direct unstabilized command passthrough
   *rcin_aux - free auxillary channel, can be used to toggle things with an 'if' statement
   */
/*
Motor order diagram (Betaflight order)

      front
 CW -->   <-- CCW
     4     2 
      \ ^ /
       |X|
      / - \
     3     1 
CCW -->   <-- CW

                                        M1234
Pitch up (stick back)   (front+ back-)   -+-+
Roll right              (left+ right-)   --++
Yaw right               (CCW+ CW-)       -++-
*/

  //Quad mixing - EXAMPLE
  out_command[MOTOR1] = rcin_thro - pitch_PID - roll_PID - yaw_PID; //Back Right CW
  out_command[MOTOR2] = rcin_thro + pitch_PID - roll_PID + yaw_PID; //Front Right CCW
  out_command[MOTOR3] = rcin_thro - pitch_PID + roll_PID + yaw_PID; //Back Left CCW
  out_command[MOTOR4] = rcin_thro + pitch_PID + roll_PID - yaw_PID; //Front Left CW

  //0.0 is zero throttle if connecting to ESC for conventional PWM, 1.0 is max throttle
  //0.5 is centered servo, 0.0 and 1.0 are servo at their extreme positions as set with SERVO_MIN and SERVO_MAX
  //out_command[SERVO1] = 0;
  //out_command[SERVO2] = 0; 
}

void out_KillSwitch() {
  //DESCRIPTION: update out_armed status
  /*
      Change to ARMED when throttle is low and radio armed switch was flipped from disamed to armed position
      Change to DISARMED when radio armed switch is in disamed position
  */
  static bool rcin_armed_prev = false; 

  //Set ARMED when throttle is low and rcin_armed was flipped from unarmed to armed
  if (rcin_thro_is_low && rcin_armed && !rcin_armed_prev) {
    out_armed = true;
  }

  //Set DISARMED when rcin_armed is disarmed
   if (!rcin_armed) {
    out_armed = false;
  }

  //If DISARMED set motor outputs to 0
  if(!out_armed) for(int i=0;i<out_MOTOR_COUNT;i++) out_command[i] = 0; 

  rcin_armed_prev = rcin_armed;
}

void out_SetCommands() {
  //DESCRIPTION: Send pulses to motor pins, oneshot125 or PWM protocol and set servos
  for(int i=0;i<HW_OUT_COUNT;i++) out[i].writeFactor( out_command[i] );
}

//========================================================================================================================//
//                                              SETUP() FUNCTIONS                                                         //
//========================================================================================================================//

void loop_RateBegin() {
  loop_time = micros();
  loop_cnt = 0;    
}

void out_ArmMotors() {
  //DESCRIPTION: Sends many command pulses to the motors, to be used to arm motors in the void setup()
  /*  
   *  Loops over the commandMotors() function 50 times with a delay in between, simulating how the commandMotors()
   *  function is used in the main loop. Ensures motors arm within the void setup() where there are some delays
   *  for other processes that sometimes prevent motors from arming.
   */
  for (int i = 0; i <= 50; i++) {
    out_SetCommands();
    delay(2);
  }
}

//set initial quarterion
void ahrs_Setup() 
{
  //estimate yaw based on mag only (assumes vehicle is horizontal)

  //warm up imu by getting 100 samples
  
  for(int i=0;i<100;i++) {
    uint32_t start = micros();
    mag_Read(&mag_x, &mag_y, &mag_z);
    imu_GetData();
    while(micros() - start < 1000000/loop_freq); //wait until next sample time
  }

  //calculate yaw angle
  if(MagX == 0 && MagY == 0 && MagZ == 0) Serial.println("ahrs_Setup() No Magnetometer");
  float yaw = -atan2(MagY, MagX);
  ahrs_yaw = yaw * rad_to_deg;
  ahrs_pitch = 0;
  ahrs_roll = 0;

  //set initial quarterion
  q0 = cos(yaw/2);
  q1 = 0;
  q2 = 0;
  q3 = sin(yaw/2);

  Serial.printf("ahrs_Setup() Estimated yaw:%+.2f\n",ahrs_yaw);  
}

//========================================================================================================================//
//                                          CALIBRATION FUNCTIONS                                                         //
//========================================================================================================================//

//Computes IMU accelerometer and gyro error on startup. Note: vehicle should be powered up on flat surface
void calibrate_IMU_error() {
  Serial.println("Running calibrate_IMU_error() takes a couple of seconds...");

  //Read IMU values, and average the readings
  int cnt = 3000;
  float ax, ay, az, gx, gy, gz, mx, my, mz;
  float axerr=0, ayerr=0, azerr=0, gxerr=0, gyerr=0, gzerr=0;
  for(int i=0; i<cnt; i++) {
    imu_Read(&ax, &ay, &az, &gx, &gy, &gz, &mx, &my, &mz); 
    axerr+=ax; ayerr+=ay; azerr+=az; gxerr+=gx; gyerr+=gy; gzerr+=gz;
    delayMicroseconds(1000000/loop_freq);
  }
  axerr/=cnt; ayerr/=cnt; azerr/=cnt; gxerr/=cnt; gyerr/=cnt; gzerr/=cnt;

  //remove gravitation
  azerr -= 1.0; 

  Serial.printf("float AccErrorX = %+f;\n", axerr);
  Serial.printf("float AccErrorY = %+f;\n", ayerr);
  Serial.printf("float AccErrorZ = %+f;\n", azerr);
  Serial.printf("float GyroErrorX = %+f;\n", gxerr);
  Serial.printf("float GyroErrorY = %+f;\n", gyerr);
  Serial.printf("float GyroErrorZ = %+f;\n", gzerr);
  Serial.println("Paste these values in user specified variables section and comment out calculate_IMU_error() in void setup.");

  //only apply reasonable gyro and acc errors
  float tol = 10;
  if( -tol < gxerr && gxerr < tol  &&  -tol < gyerr && gyerr < tol  &&  -tol < gzerr && gzerr < tol ) {
    GyroErrorX = gxerr; AccErrorY = gyerr; AccErrorZ = gzerr;   
  }
  tol = 0.1;
  if( -tol < axerr && axerr < tol  &&  -tol < ayerr && ayerr < tol  &&  -tol < azerr && azerr < tol ) {
    AccErrorX = axerr; AccErrorY = ayerr; AccErrorZ = azerr;   
  }  
}

void calibrate_ESCs() { //TODO
  //DESCRIPTION: Used in void setup() to allow standard ESC calibration procedure with the radio to take place.
  /*  
   *  Simulates the void loop(), but only for the purpose of providing throttle pass through to the motors, so that you can
   *  power up with throttle at full, let ESCs begin arming sequence, and lower throttle to zero. This function should only be
   *  uncommented when performing an ESC calibration.
   */
  uint32_t ts = micros();
  while (true) {
    while ( (micros() - ts) < (1000000U / loop_freq) ); //Keeps loop sample rate constant. (Waste time until sample time has passed.)
    ts = micros();

    led_SwitchON(true); //LED on to indicate we are not in main loop

    rcin_GetCommands(); //Pulls current available radio commands
    rcin_Failsafe(); //Prevent failures in event of bad receiver connection, defaults to failsafe values assigned in setup
    rcin_Normalize(); //Convert raw commands to normalized values based on saturated control limits
    
    //set all motors
    for(int i=0;i<out_MOTOR_COUNT;i++) out_command[i] = rcin_thro;
  
    //out_KillSwitch(); //Don't update motor outputs to 0 if disarmed
    out_SetCommands(); //Sends command pulses to each motor pin
    
    //printRadioData(); //Radio pwm values (expected: 1000 to 2000)
  }
}

void calibrate_Magnetometer() {
  float bias[3], scale[3];

  Serial.println("Magnetometer calibration. Rotate the IMU about all axes until complete.");
  int rv = _calibrate_Magnetometer(bias, scale);
  if(rv==0) {
    Serial.println("Calibration Successful!");
    Serial.println("Please comment out the calibrateMagnetometer() function and copy these values into the code:");
    Serial.printf("float MagErrorX = %f;\n", bias[0]);
    Serial.printf("float MagErrorY = %f;\n", bias[1]);
    Serial.printf("float MagErrorZ = %f;\n", bias[2]);      
    Serial.printf("float MagScaleX = %f;\n", scale[0]);  
    Serial.printf("float MagScaleY = %f;\n", scale[1]);
    Serial.printf("float MagScaleZ = %f;\n", scale[2]);
    Serial.println(" ");
    Serial.println("If you are having trouble with your attitude estimate at a new flying location, repeat this process as needed.");
  }
  else {
    Serial.printf("Calibration Unsuccessful rv=%d. Please reset the board and try again.\n",rv);
  }

  while(1); //Halt code so it won't enter main loop until this function commented out
}

// finds bias and scale factor calibration for the magnetometer, the sensor should be rotated in a figure 8 motion until complete
// Note: Earth's field ranges between approximately 25 and 65 uT. (Europe & USA: 45-55 uT, inclination 50-70 degrees)
int _calibrate_Magnetometer(float bias[3], float scale[3]) 
{
  const int maxCounts = 1000; //sample for at least 10 seconds @ 100Hz
  const float deltaThresh = 0.3f; //uT
  const float B_coeff = 0.125;

  float ax=0,ay=0,az=0,gx=0,gy=0,gz=0;
  float m[3] = {0};
  int counter;
  float m_filt[3];
  float m_max[3];
  float m_min[3];

  // get a starting set of data
  imu_Read(&ax, &ay, &az, &gx, &gy, &gz, &m[0], &m[1], &m[2]);
  for(int i=0;i<3;i++) {
    m_max[i] = m[i];
    m_min[i] = m[i];
    m_filt[i] = m[i];
  }

  // collect data to find max / min in each channel
  // sample counter times, restart sampling when a min/max changed at least deltaThresh uT
  uint32_t start_time = millis();
  counter = 0;
  while (counter < maxCounts) {
    imu_Read(&ax, &ay, &az, &gx, &gy, &gz, &m[0], &m[1], &m[2]);
    for(int i=0;i<3;i++) {
      m_filt[i] = m_filt[i] * (1 - B_coeff) + m[i] * B_coeff;
      if (m_max[i] < m_filt[i]) {
        float delta =  m_filt[i] - m_max[i];
        if (delta > deltaThresh) counter = 0;
        m_max[i] = m_filt[i];        
      }
      if (m_min[i] > m_filt[i]) {
        float delta = m_min[i] - m_filt[i];
        if (delta > deltaThresh) counter = 0;
        m_min[i] = m_filt[i];
      }
    }
    counter++;
    delay(10); //sample rate = 100Hz

    //print progress
    if(millis() - start_time > 1000) {
      start_time = millis();
      Serial.printf("xmin:%+.2f\txmax:%+.2f\tymin:%+.2f\tymax:%+.2f\tzmin:%+.2f\tzmax:%+.2f\n", m_min[0], m_max[0], m_min[1], m_max[1], m_min[2], m_max[2]);
    }
  }

  // find the magnetometer bias and scale
  float avg_scale = 0;
  for(int i=0;i<3;i++) { 
    bias[i] = (m_max[i] + m_min[i]) / 2;
    scale[i] = (m_max[i] - m_min[i]) / 2;
    avg_scale += scale[i];
  }
  for(int i=0;i<3;i++) {
    scale[i] = (avg_scale / 3) / scale[i];
  }

  return 0;
}

//========================================================================================================================//
//                                                PRINT FUNCTIONS                                                         //
//========================================================================================================================//

void print_overview() {
  Serial.printf("CH%d:%d\t",1,rcin_pwm[0]);  
  Serial.printf("rcin_roll:%+.2f\t",rcin_roll);
  Serial.printf("gx:%+.2f\t",GyroX);
  Serial.printf("ax:%+.2f\t",AccX);
  Serial.printf("mx:%+.2f\t",MagX);
  Serial.printf("ahrs_roll:%+.1f\t",ahrs_roll);
  Serial.printf("roll_PID:%+.3f\t",roll_PID);  
  Serial.printf("m%d%%:%1.0f\t", 1, 100*out_command[0]);
  Serial.printf("sats:%d\t",(int)gps.sat);
  Serial.printf("loop_rt:%d\t",(int)loop_rt);
  Serial.printf("loop_cnt:%d\t",(int)loop_cnt); 
  print_need_newline = true;    
}

void print_rcin_RadioPWM() {
  for(int i=0;i<RCIN_NUM_CHANNELS;i++) Serial.printf("pwm%d:%d\t",i+1,rcin_pwm[i]);
  print_need_newline = true;
}

void print_rcin_RadioScaled() {
  Serial.printf("rcin_thro:%.2f\t",rcin_thro);
  Serial.printf("rcin_roll:%+.2f\t",rcin_roll);
  Serial.printf("rcin_pitch:%+.2f\t",rcin_pitch);
  Serial.printf("rcin_yaw:%+.2f\t",rcin_yaw);
  Serial.printf("rcin_arm:%d\t",rcin_armed);
  Serial.printf("rcin_aux:%d\t",rcin_aux);
  Serial.printf("out_armed:%d\t",out_armed);  
  print_need_newline = true;
}

void print_imu_GyroData() {
  Serial.printf("gx:%+.2f\tgy:%+.2f\tgz:%+.2f\t",GyroX,GyroY,GyroZ);
  print_need_newline = true;
}

void print_imu_AccData() {
  Serial.printf("ax:%+.2f\tay:%+.2f\taz:%+.2f\t",AccX,AccY,AccZ);
  print_need_newline = true;
}

void print_imu_MagData() {
  Serial.printf("mx:%+.2f\tmy:%+.2f\tmz:%+.2f\t",MagX,MagY,MagZ);
  print_need_newline = true;  
}

void print_ahrs_RollPitchYaw() {
  Serial.printf("roll:%+.1f\tpitch:%+.1f\tyaw:%+.1f\t",ahrs_roll,ahrs_pitch,ahrs_yaw);
  Serial.printf("yaw_mag:%+.1f\t",-atan2(MagY, MagX) * rad_to_deg);
  print_need_newline = true;
}

void print_control_PIDoutput() {
  Serial.printf("roll_PID:%+.3f\tpitch_PID:%+.3f\tyaw_PID:%+.3f\t",roll_PID,pitch_PID,yaw_PID);  
  print_need_newline = true;
}

void print_out_MotorCommands() {
  Serial.printf("out_armed:%d", out_armed);  
  for(int i=0;i<out_MOTOR_COUNT;i++) Serial.printf("m%d%%:%1.0f\t", i+1, 100*out_command[i]);
  print_need_newline = true;    
}

void print_out_ServoCommands() {
  for(int i=out_MOTOR_COUNT;i<HW_OUT_COUNT;i++) Serial.printf("s%d%%:%1.0f\t", i-out_MOTOR_COUNT+1, 100*out_command[i]);
  print_need_newline = true;  
}

void print_loop_Rate() {
  static uint32_t loop_cnt_last = 0;
  Serial.printf("loop_dt:%d\t",(int)(loop_dt * 1000000.0));
  Serial.printf("loop_rt:%d\t",(int)loop_rt);
  Serial.printf("loop_rt_imu:%d\t",(int)loop_rt_imu);
  Serial.printf("loop_cnt:%d\t",(int)loop_cnt);  
  Serial.printf("loops:%d\t",(int)(loop_cnt - loop_cnt_last));  
  loop_cnt_last = loop_cnt;
  print_need_newline = true;
}

void print_bat() {
  Serial.printf("bat.v:%.2f\t",bat.v);
  Serial.printf("bat.i:%+.2f\t",bat.i);
  Serial.printf("bat.mah:%+.2f\t",bat.mah);
  Serial.printf("bat.wh:%+.2f\t",bat.wh); 
  print_need_newline = true;  
}

void print_i2c_scan() {
  Serial.printf("I2C: Scanning ...\n");
  byte count = 0;
  i2c->begin();
  for (byte i = 8; i < 120; i++) {
    i2c->beginTransmission(i);          // Begin I2C transmission Address (i)
    if (i2c->endTransmission() == 0) { // Receive 0 = success (ACK response) 
      Serial.printf("I2C: Found address: 0x%02X (%d)\n",i,i);
      count++;
    }
  }
  Serial.printf("I2C: Found %d device(s)\n", count);      
}

//===============================================================================================
// HELPERS
//===============================================================================================

//lowpass frequency to filter beta constant
float lowpass_to_beta(float f0, float fs) {
  return constrain(1 - exp(-2 * PI * f0 / fs), 0.0f, 1.0f);
}

void led_SwitchON(bool set_on) {
  digitalWrite( HW_PIN_LED, (set_on ? HW_LED_ON : !HW_LED_ON) );
}

void warn_or_die(String msg, bool never_return) {
  do{
    Serial.print(msg);
    for(int i=0;i<10;i++) {
      led_SwitchON(true);
      delay(50);
      led_SwitchON(false);
      delay(50);
    }
  } while(never_return);
}
void die(String msg) { warn_or_die(msg, true); }
void warn(String msg) { warn_or_die(msg, false); }
