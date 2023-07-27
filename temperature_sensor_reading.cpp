#include <WaspWIFI_PRO_V3.h>
#include <WaspFrame.h>
#include <WaspSensorSW.h>

//choose socket (SELECT USER'S SOCKET)
const uint8_t SOCKET_NUMBER = SOCKET0;

//choose HTTP server settings
const char HTTP_TYPE[] = "http";
const char SERVER_HOST[] = "82.78.81.178";
const uint16_t SERVER_PORT = 80;

uint8_t error;
uint8_t status;

//define the Waspmote ID
const char MOTE_ID[] = "DinuAlexandru";

const int SENSOR_READ_DELAY = 2000;
const int WIFI_CONNECT_DELAY = 10000;

float temperatureValue;

//Calibration values
#define cal_point_10 1.985
#define cal_point_7 2.070
#define cal_point_4 2.227
//Temperature at which calibration was carried out
#define cal_temp 23.7
//Offset obtained from sensor calibration
#define calibration_offset 0.0

//Value 1 used to calibrate the sensor
#define point1_cond 10500
//Value 2 used to calibrate the sensor
#define point2_cond 40000
//Point 1 of the calibration 
#define point1_cal 197.00
//Point 2 of the calibration 
#define point2_cal 150.00


pt1000Class TemperatureSensor;

void setup() 
{
  USB.ON();
  USB.println(F("Frame Utility Example for Smart Water"));
  USB.println(F("******************************************************"));
  USB.println(F("WARNING: This example is valid only for Waspmote v15"));
  USB.println(F("If you use a Waspmote v12, you MUST use the correct "));
  USB.println(F("sensor field definitions"));
  USB.println(F("******************************************************"));

  //Set the Waspmote ID
  frame.setID(MOTE_ID);
}

void loop() 
{
  //1. Turn on the board
  Water.ON();
  delay(SENSOR_READ_DELAY);

  //2. Read sensors

  //Read the temperature sensor
  temperatureValue = TemperatureSensor.readTemperature();

  //3. Turn off the sensors
  Water.OFF();

  //4. Create ASCII frame

  //Create new frame (ASCII)
  frame.createFrame(ASCII);

  //Add temperature
  frame.addSensor(SENSOR_WATER_WT, temperatureValue);

  //Show the frame
  frame.showFrame();

  //Wait for 2 seconds
  delay(SENSOR_READ_DELAY);

  //5. Send Frame to Meshlium via WiFi
  if (WIFI_PRO_V3.ON(SOCKET_NUMBER) == 0) 
  {
    USB.println(F("WiFi switched ON"));

    if (WIFI_PRO_V3.isConnected()) 
    {
      if (WIFI_PRO_V3.sendFrameToMeshlium(HTTP_TYPE, SERVER_HOST, SERVER_PORT, frame.buffer, frame.length) == 0) 
      {
        USB.println(F("Send frame to Meshlium done"));
      } 
      else 
      {
        USB.println(F("Error sending frame"));
        if (WIFI_PRO_V3._httpResponseStatus) 
        {
          USB.print(F("HTTP response status: "));
          USB.println(WIFI_PRO_V3._httpResponseStatus);
        }
      }
    } 
    else 
    {
      USB.println(F("WiFi is not connected"));
    }

    WIFI_PRO_V3.OFF(SOCKET_NUMBER);
    USB.println(F("WiFi switched OFF\n\n"));
  } 
  else 
  {
    USB.println(F("WiFi did not initialize correctly"));
  }

  //Delay for 10 seconds before next iteration
  delay(WIFI_CONNECT_DELAY);
}
