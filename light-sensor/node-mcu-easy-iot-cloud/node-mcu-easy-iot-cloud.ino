#include <ESP8266WiFi.h>
#include <EIoTCloudRestApiConfig.h>
#include <EIoTCloudRestApiV1.0.h>

#define DEBUG

#define WIFI_SSID "nereusiphone"
#define WIFI_PASS "ajit1234"
#define TOKEN "5SpmyUqPgNWGBEMS3bGI5J6rXDwAtW4FRCRZvkcN" //replace with https://easyiot-cloud.com/ token
EIoTCloudRestApi eiotcloud;

uint existingModuleId = 6;//replace with moduleId, if set to 0 a new module will be created in default module group at https://easyiot-cloud.com/.

String parameterId = "";
int sensorValue = 0;


#define REPORT_INTERVAL 1

#define MIN_VALUE 25
#define MAX_VALUE 800

#define SENSOR_PIN A0    // select the input pin for the

void setup()
{
  Serial.begin(115200);
  /*
    //following is an example to connection to any wifi, and get the mac address of node-mcu, and verify the ip assigned.
    WiFi.mode(WIFI_STA);
    WiFi.begin("nereusiphone", "ajit1234");

    Serial.print("Connecting");
    while (WiFi.status() != WL_CONNECTED)
    {
    delay(500);
    Serial.print("y");
    }
    Serial.println();

    Serial.print("Connected, IP address: ");
    Serial.println(WiFi.localIP());
    Serial.print("MAC: ");
    Serial.println(WiFi.macAddress());

    //arduino's wifi lib provided this impl to connect to any external service and later print messages to it, this is just an example.
    WiFiClient client;
    while(!client.connect("cloud.iot-playground.com", 40404)) {
    Serial.println("connection failed..!!!");
    Serial.println(EIOT_CLOUD_ADDRESS);
    Serial.println(EIOT_CLOUD_PORT);
    Serial.println(WiFi.localIP());
    delay(2000);
    }*/

  ////--------------
  eiotcloud.begin(WIFI_SSID, WIFI_PASS, TOKEN);
  Serial.println("token set........");

  //----------------create ne module if not existing---
  if (existingModuleId == 0)
  {

    Serial.println("creating new module");

    // add new module and configure it
    //creating a new module with curl
    //example curl -X POST   http://cloud.iot-playground.com:40404/RestApi/v1.0/Module/New   -H 'Cache-Control: no-cache'   -H 'EIOT-AuthToken: 5SpmyUqPgNWGBEMS3bGI5J6rXDwAtW4FRCRZvkcN'   -H 'Postman-Token: 1ac0ad32-e8cf-4394-b085-55264531642e' -d ""

    String moduleId = eiotcloud.ModuleNew();
    Serial.print("ModuleId: ");
    Serial.println(moduleId);
    existingModuleId = moduleId.toInt();

    // stop if module ID is not valid
    if (existingModuleId == 0)
    {
      Serial.println("Check Account limits -> module limit.");
    }
    else
    {
      // set module type
      bool modtyperet = eiotcloud.SetModulType(moduleId, "MT_GENERIC");
      Serial.print("SetModulType: ");
      Serial.println(modtyperet);

      // set module name
      bool modname = eiotcloud.SetModulName(moduleId, "LUX meter");
      Serial.print("SetModulName: ");
      Serial.println(modname);

      // add image settings parameter
      String parameterImgId = eiotcloud.NewModuleParameter(moduleId, "Settings.Icon1");
      Serial.print("parameterImgId: ");
      Serial.println(parameterImgId);

      // set module image
      bool valueRet1 = eiotcloud.SetParameterValue(parameterImgId, "radiation.png");
      Serial.print("SetParameterValue: ");
      Serial.println(valueRet1);

      // now add parameter to value
      parameterId = eiotcloud.NewModuleParameter(moduleId, "Sensor.Parameter1");
      Serial.print("ParameterId: ");
      Serial.println(parameterId);

      //set parameter description
      bool valueRet2 = eiotcloud.SetParameterDescription(parameterId, "LUX");
      Serial.print("SetParameterDescription: ");
      Serial.println(valueRet2);

      //set unit
      bool valueRet3 = eiotcloud.SetParameterUnit(parameterId, "%");
      Serial.print("SetParameterUnit: ");
      Serial.println(valueRet3);

      //Set parameter LogToDatabase
      bool valueRet4 = eiotcloud.SetParameterLogToDatabase(parameterId, true);
      Serial.print("SetLogToDatabase: ");
      Serial.println(valueRet4);

      //SetAvreageInterval
      bool valueRet5 = eiotcloud.SetParameterAverageInterval(parameterId, "10");
      Serial.print("SetAvreageInterval: ");
      Serial.println(valueRet5);
    }
  }
  else
  {
    parameterId = eiotcloud.GetModuleParameterByName(String(existingModuleId), "Sensor.Parameter1");
    Serial.println("parameterId:  ");
    Serial.println(parameterId);
  }
}

void loop() {
  //read the value from lux sensor and push to easy io cloud rest end point
  sensorValue = analogRead(SENSOR_PIN);
  float level = 100 - ((sensorValue - MIN_VALUE) * 100 / (MAX_VALUE - MIN_VALUE));

  sensorValue = level;
  Serial.println("LUX = ");
  Serial.println(sensorValue);

  bool valueRet = eiotcloud.SetParameterValue(parameterId, String(sensorValue));
  Serial.println("SetParameterValue: ");
  Serial.println(valueRet);

  delay(1000 * 10 * REPORT_INTERVAL);
}
