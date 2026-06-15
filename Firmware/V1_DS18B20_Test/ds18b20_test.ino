#include <OneWire.h>
#include <DallasTemperature.h>

#define ONE_WIRE_BUS 15

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

void setup()
{
  Serial.begin(115200);
  sensors.begin();

  Serial.println("DS18B20 Test Started");
}

void loop()
{
  sensors.requestTemperatures();

  float tempC = sensors.getTempCByIndex(0);

  Serial.print("Temperature = ");
  Serial.print(tempC);
  Serial.println(" C");

  delay(1000);
}
