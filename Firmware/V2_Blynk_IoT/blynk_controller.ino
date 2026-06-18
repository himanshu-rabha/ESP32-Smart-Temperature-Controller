

#define BLYNK_TEMPLATE_ID "Your_template_id"
#define BLYNK_TEMPLATE_NAME "ESP32 Temperature Controller V2"
#define BLYNK_AUTH_TOKEN "Your_auth_token"

#include <WiFi.h>
#include <BlynkSimpleEsp32.h>

#include <OneWire.h>
#include <DallasTemperature.h>

char ssid[] = "Your_wifi_password";
char pass[] = "Your_password";

#define ONE_WIRE_BUS 15
#define RELAY_PIN 26

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

float setPoint = 45.0;

bool heaterState = false;
bool emergencyStop = false;

BlynkTimer timer;

BLYNK_WRITE(V2)
{
  setPoint = param.asFloat();
}

BLYNK_WRITE(V3)
{
  emergencyStop = param.asInt();

  if(emergencyStop)
  {
    heaterState = false;
    digitalWrite(RELAY_PIN, LOW);
  }
}

void sendTemperature()
{
  sensors.requestTemperatures();

  float tempC = sensors.getTempCByIndex(0);

  if(tempC == -127.0)
  {
    Serial.println("Sensor Error");

    digitalWrite(RELAY_PIN, LOW);

    Blynk.virtualWrite(V1, 0);

    return;
  }

  if(tempC > 70)
  {
    heaterState = false;
  }

   if(!emergencyStop)
  {
    if(tempC < (setPoint - 2))
    {
      heaterState = true;
    }

    if(tempC > setPoint)
    {
      heaterState = false;
    }
  }


  digitalWrite(RELAY_PIN, heaterState ? HIGH : LOW);

  Blynk.virtualWrite(V0, tempC);
  Blynk.virtualWrite(V1, heaterState);

  Serial.print("Temp: ");
  Serial.print(tempC);
  Serial.print(" Setpoint: ");
  Serial.print(setPoint);
  Serial.print(" Heater: ");
  Serial.println(heaterState ? "ON" : "OFF");
}

void setup()
{
  Serial.begin(115200);

  pinMode(RELAY_PIN, OUTPUT);

  digitalWrite(RELAY_PIN, LOW);

  sensors.begin();

  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

  timer.setInterval(1000L, sendTemperature);
}

void loop()
{
  Blynk.run();
  timer.run();
}
