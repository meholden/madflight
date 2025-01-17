import re
import os
import datetime 

DEBUG = False
#DEBUG = True

source_dirname = "betaflight_source"
destination_dirname = "betaflight"

ignore_defines = ["USE_GYRO", "USE_ACC.*", "USE_MAG", "USE_BARO", "USE_FLASH"]

ignore_pins = ["LED","IMU_EXTI","IMU_CS","BAT_V","BAT_I"]



def main() :
    for filename in os.listdir(source_dirname) :
        convert(filename)
        if DEBUG: return

def convert(filename) :
    def toint(s) :
        try:
            return int(s)
        except ValueError:
            return 0
    
    #in and output files
    infile = source_dirname + "/" + filename
    strippedfilename = re.sub(r"(.config$)", r"", filename)
    outfile = destination_dirname + "/" + strippedfilename + ".h"

    #read lines from infile
    f = open(infile,"r")
    lines = f.readlines()
    f.close()

    #don't parse files that are too short
    if len(lines)<10 : return

    #parse lines to topics
    defines = []
    board_name = ""
    manufacturer_id = ""
    resources = {}
    sets = {}
    motors = []
    serials = [ {} for i in range( 20 ) ]
    spis = [ {} for i in range( 20 ) ]
    i2cs = [ {} for i in range( 20 ) ]
    pins = {}


    for line in lines:
        s = line.strip();
        s = re.sub(" +", " ", s) #replace multiple spaces with one space
        s = s + "   " #add dummy spaces to fill at least p[4]
        p = s.split(" ")

        if p[0] == "#define" :
            found = False;
            for ignore in ignore_defines :
                if re.fullmatch(ignore, p[1]) :
                    found = True
                    break
            if not found :
                nme = p[1]
                val = p[2]
                if nme.startswith("USE_GYRO_") :
                    val = "IMU_USE_" + nme.replace("USE_GYRO_", "") #remove prefix
                    nme = "IMU_USE"
                if nme.startswith("USE_BARO_") :
                    val = "BARO_USE_" + nme.replace("USE_BARO_", "") #remove prefix
                    nme = "BARO_USE"
                if nme.startswith("USE_FLASH_") :
                    defines.append("BB_USE  BB_USE_FLASH");
                    val = "\"" + nme.replace("USE_FLASH_", "") + "\"" #remove prefix
                    nme = "BB_FLASH_TYPE"
                if nme == "USE_SDCARD" :
                    val = "BB_USE_SDCARD"
                    nme = "BB_USE"
                if nme == "USE_MAX7456" :
                    val = "OSD_USE_MAX7456"
                    nme = "OSD_USE"
                defines.append( (nme + "  " + val).strip() )

        if p[0] == "board_name" :
            board_name = p[1]

        if p[0] == "manufacturer_id" :
            manufacturer_id = p[1]

        if p[0] == "resource" :
            r = p[1]
            i = toint(p[2])
            pin = "P" + re.sub(r"0([0-9])", r"\1", p[3]) #convert pin name from C01 to PC1
            resources[ r + ":" + str(i) ] = pin
            if   r == "SERIAL_TX" : serials[i]["TX"] = pin
            elif r == "SERIAL_RX" : serials[i]["RX"] = pin
            elif r == "INVERTER"  : serials[i]["INVERTER"] = pin
            elif r == "SPI_SCK"   : spis[i]["SCK"] = pin
            elif r == "SPI_MISO"  : spis[i]["MISO"] = pin
            elif r == "SPI_MOSI"  : spis[i]["MOSI"] = pin
            elif r == "I2C_SCL"   : i2cs[i]["SCL"] = pin
            elif r == "I2C_SDA"   : i2cs[i]["SDA"] = pin
            elif r == "MOTOR"     : motors.append( pin )
            else :
                name = r
                if i!=1 : name = name + "_" + str(i)
                pins[name] = pin

        if p[0] == "set" :
            sets[ p[1]] = p[3]

    #assign rcin = 1st serial and gps = 2nd serial
    rcin_serial = ""
    gps_serial = ""
    for i in range(len(serials)) :
        b = serials[i]
        if b.get("RX") and b.get("TX") :
            if rcin_serial == "" : rcin_serial = str(i)
            elif gps_serial == "" : gps_serial = str(i)

    #debug print topics
    # print( "defines:", defines )
    # print( "board_name:", board_name )
    # print( "manufacturer_id:", manufacturer_id )
    # print( "resources:", resources )
    # print( "sets:", sets )
    # print( "motors:", motors )
    # print( "serials:", serials )

    #output madflight target header file
    f = open(outfile,"w")

    def fprint(txt) :
        if DEBUG : print( txt )
        else : f.write(txt + "\n")

    fprint( "/*==============================================================================" )
    fprint( "Generated on: " + str(datetime.datetime.now()) )
    fprint( "Generated by: betaflight_target_converter.py" )
    fprint( "Source: https://github.com/betaflight/unified-targets" )
    fprint( "Board name: " + board_name )
    fprint( "Manufacturer ID: " + manufacturer_id )
    fprint( "" )
    fprint( "//copy this line to madflight.ino to use this flight controller (or copy/paste the whole file)" )
    fprint( "#include \"boards/" + outfile + "\"" )
    fprint( "==============================================================================*/" )

    fprint( "" )
    fprint( "#define HW_BOARD_NAME \"BETAFLIGHT-" + strippedfilename + "\"" )
    mcu_re = re.search(r"\bSTM32\w+", lines[0] + " " + lines[1]);
    if mcu_re : fprint( "#define HW_MCU \"" + mcu_re.group() + "\"" )

    fprint( "" )
    fprint( "//Defines from betaflight. Note: madflight will pick the last sensor defined here, this might not be the sensor that is actually on the board. Comment the offending sensors out." )
    for define in defines:
        fprint( "#define " + define )

    fprint( "" )
    fprint( "//Sensor specific setup" )
    fprint( "#define IMU_ALIGN  IMU_ALIGN_" + sets.get("gyro_1_sensor_align","CW0") )
    fprint( "#define BARO_I2C_ADR  " + sets.get("baro_i2c_address","0") )
    fprint( "#define MAG_I2C_ADR  " + sets.get("mag_i2c_address","0") )

    fprint( "" )
    fprint( "//LED:" )
    fprint( "const int HW_PIN_LED      = " + resources.get("LED:1","-1") + ";" )
    fprint( "const int HW_LED_ON       = 1; //0:low is on, 1:high is on" )
 
    spi = sets.get("gyro_1_spibus","1")
    fprint( "" )
    fprint( "//IMU SPI: (SPI"+spi+")" )
    fprint( "const int HW_PIN_SPI_MISO = " + resources.get("SPI_MISO:" + spi,"-1") + ";" )
    fprint( "const int HW_PIN_SPI_MOSI = " + resources.get("SPI_MOSI:" + spi,"-1") + ";" )
    fprint( "const int HW_PIN_SPI_SCLK = " + resources.get("SPI_SCK:" + spi,"-1") + ";" )
    fprint( "const int HW_PIN_IMU_CS   = " + resources.get("GYRO_CS:1","-1") + ";" )
    fprint( "const int HW_PIN_IMU_EXTI = " + resources.get("GYRO_EXTI:1","-1") + ";" )

    i2c = sets.get("mag_i2c_device")
    i2c = sets.get("baro_i2c_device",i2c)
    if i2c is None: i2c = "1"
    fprint( "" )
    fprint( "//BARO/MAG I2C: (I2C"+i2c+")" )
    fprint( "const int HW_PIN_I2C_SDA  = " + resources.get("I2C_SCL:" + i2c,"-1") + ";" )
    fprint( "const int HW_PIN_I2C_SCL  = " + resources.get("I2C_SDA:" + i2c,"-1") + ";" )

    fprint( "" )
    fprint( "//Outputs:" )
    fprint( "const int HW_OUT_COUNT    = " + str(len(motors)) + ";" )
    fprint( "const int HW_PIN_OUT[]    = {" + ",".join(motors) + "};" )

    fprint( "" )
    fprint( "//RC Receiver: (SERIAL"+rcin_serial+")" )
    fprint( "const int HW_PIN_RCIN_RX  = " + resources.get("SERIAL_RX:"+rcin_serial,"-1") + ";" )
    fprint( "const int HW_PIN_RCIN_TX  = " + resources.get("SERIAL_TX:"+rcin_serial,"-1") + ";" )
    fprint( "const int HW_PIN_RCIN_INVERTER = " + resources.get("INVERTER:"+rcin_serial,"-1") + ";" )

    fprint( "" )
    fprint( "//GPS: (SERIAL"+gps_serial+")" )
    fprint( "const int HW_PIN_GPS_RX   = " + resources.get("SERIAL_RX:"+gps_serial,"-1") + ";" )
    fprint( "const int HW_PIN_GPS_TX   = " + resources.get("SERIAL_TX:"+gps_serial,"-1") + ";" )
    fprint( "const int HW_PIN_GPS_INVERTER = " + resources.get("INVERTER:"+gps_serial,"-1") + ";" )

    fprint( "" )
    fprint( "//Battery ADC voltage and current inputs:" )
    fprint( "const int HW_PIN_BAT_V    = " + resources.get("ADC_BATT:1","-1") + ";" )
    fprint( "const int HW_PIN_BAT_I    = " + resources.get("ADC_CURR:1","-1") + ";" )

    fprint( "" )
    fprint( "//Include Libraries" )
    fprint( "#include <Wire.h>                      //I2C communication" )
    fprint( "#include <SPI.h>                       //SPI communication" )
    fprint( "#include \"src/hw_STM32/STM32_PWM.h\"    //Servo and oneshot" )

    fprint( "" )
    fprint( "//Bus Setup" )
    fprint( "HardwareSerial *rcin_Serial = new HardwareSerial(HW_PIN_RCIN_RX, HW_PIN_RCIN_TX);" )
    fprint( "HardwareSerial gps_Serial(HW_PIN_GPS_RX, HW_PIN_GPS_TX);" )
    fprint( "typedef TwoWire HW_WIRETYPE; //define the class to use for I2C" )
    fprint( "HW_WIRETYPE *i2c = &Wire; //&Wire or &Wire1" )
    fprint( "SPIClass *spi = &SPI;" )

    fprint( "" )
    fprint( "//Serial" )
    p = []
    for i in range(len(serials)) :
        b = serials[i]
        if b.get("RX") or b.get("TX") :
            p.append( "{" + str(i) + "," + b.get("TX","-1") + "," + b.get("RX","-1") + "," + b.get("INVERTER","-1") + "}" )
    fprint( "#define HW_SERIAL_COUNT " + str(len(p)) )
    fprint( "#define HW_PIN_SERIAL { " + ", ".join(p) + " } // {INDEX,TX,RX,INVERTER}" )

    fprint( "" )
    fprint( "//SPI" )
    p = []
    for i in range(len(spis)) :
        b = spis[i]
        if b.get("SCK") or b.get("MISO") or b.get("MOSI"):
            p.append( "{" + str(i) + "," + b.get("SCK","-1") + "," + b.get("MISO","-1") + "," + b.get("MOSI","-1") + "}" )
    fprint( "#define HW_SPI_COUNT " + str(len(p)) )
    fprint( "#define HW_PIN_SPI { " + ", ".join(p) + " } // {INDEX,SCK,MISO,MOSI}" )

    fprint( "" )
    fprint( "//I2C" )
    p = []
    for i in range(len(i2cs)) :
        b = i2cs[i]
        if b.get("SCL") or b.get("SDA") :
            p.append( "{" + str(i) + "," + b.get("SCL","-1") + "," + b.get("SDA","-1") + "}" )
    fprint( "#define HW_I2C_COUNT " + str(len(p)) )
    fprint( "#define HW_PIN_I2C { " + ", ".join(p) + " } // {INDEX,SCL,SDA}" )


    fprint( "" )
    fprint( "//Motors:" )
    fprint( "#define HW_MOTOR_COUNT " + str(len(motors)) )
    fprint( "#define HW_MOTOR_OUT {" + ",".join(motors) + "}" )

    fprint( "" )
    fprint( "//other pins" )
    for name in pins.keys() :
        pre = ""
        if name in ignore_pins : pre = "//"
        fprint( pre + "#define HW_PIN_" + name + " " + pins[name] )

    fprint( "" )
    fprint( "//set statements" )
    for name in sets.keys() :
       fprint( "#define HW_SET_" + name.upper() + " " + sets[name] )


    fprint( "" )
    fprint( "" )
    fprint( "/*" )
    fprint( "#==============================================================================" )
    fprint( "# BetaFlight Source file" )
    fprint( "#==============================================================================" )
    fprint( "".join(lines) )
    fprint( "*/" )

    f.close()

main()