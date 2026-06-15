#include <OneWire.h>
#include <DallasTemperature.h>

#define ONE_WIRE_BUS 15
#define RELAY_PIN 26

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

float setPointLow = 40.0;
float setPointHigh = 45.0;

bool heaterState = false;

void setup()
{
  Serial.begin(115200);

  sensors.begin();

  pinMode(RELAY_PIN, OUTPUT);

  // Relay OFF at startup
  digitalWrite(RELAY_PIN, LOW);

  Serial.println("Temperature Controller Started");
}

void loop()
{
  sensors.requestTemperatures();

  float tempC = sensors.getTempCByIndex(0);

  // Sensor Safety
  if(tempC == DEVICE_DISCONNECTED_C)
  {
    Serial.println("Sensor Error!");
    
    heaterState = false;
    digitalWrite(RELAY_PIN, LOW);

    delay(1000);
    return;
  }

  Serial.print("Temperature: ");
  Serial.print(tempC);
  Serial.println(" C");

  // Hysteresis Control
  if(tempC < setPointLow)
  {
    heaterState = true;
  }

  if(tempC > setPointHigh)
  {
    heaterState = false;
  }

  // Heater Control
  if(heaterState)
  {
    digitalWrite(RELAY_PIN, HIGH);
    Serial.println("HEATER ON");
  }
  else
  {
    digitalWrite(RELAY_PIN, LOW);
    Serial.println("HEATER OFF");
  }

  delay(1000);
}
