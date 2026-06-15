#include <WiFi.h>
#include <WebServer.h>
#include <OneWire.h>
#include <DallasTemperature.h>

// ---------------- WIFI ----------------

const char* ssid = "Nayanjyoti_";
const char* password = "password2024";

// ---------------- PINS ----------------

#define ONE_WIRE_BUS 15
#define RELAY_PIN 26

// ---------------- SENSOR ----------------

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

// ---------------- WEB SERVER ----------------

WebServer server(80);

// ---------------- VARIABLES ----------------

float currentTemp = 0.0;

float setPoint = 45.0;

bool heaterState = false;
bool emergencyStop = false;

// ---------------- HTML PAGE ----------------

String webpage = R"rawliteral(
<!DOCTYPE html>
<html>

<head>

<meta name="viewport" content="width=device-width, initial-scale=1">

<title>ESP32 Temperature Controller</title>

<style>

body{
font-family:Arial;
background:#121212;
color:white;
text-align:center;
padding:20px;
}

.card{
background:#1e1e1e;
padding:20px;
border-radius:15px;
max-width:500px;
margin:auto;
}

.temp{
font-size:50px;
color:#00ff99;
}

.status{
font-size:25px;
margin:10px;
}

input[type=range]{
width:80%;
}

button{
padding:10px 20px;
font-size:18px;
border:none;
border-radius:10px;
cursor:pointer;
}

</style>

</head>

<body>

<div class="card">

<h1>ESP32 Temperature Controller</h1>

<div class="temp" id="temp">--</div>

<div class="status" id="heater">--</div>

<h3>Setpoint</h3>

<input type="range"
min="20"
max="80"
value="45"
id="slider"
onchange="updateSetpoint(this.value)">

<p id="sp">45 °C</p>

<button onclick="toggleStop()">
Emergency Stop
</button>

</div>

<script>

setInterval(loadData,1000);

function loadData(){

fetch('/data')

.then(response => response.json())

.then(data => {

document.getElementById('temp').innerHTML =
data.temp + " °C";

document.getElementById('heater').innerHTML =
"Heater : " + data.heater;

document.getElementById('sp').innerHTML =
data.setpoint + " °C";

});

}

function updateSetpoint(val){

fetch('/setpoint?value='+val);

}

function toggleStop(){

fetch('/stop');

}

</script>

</body>

</html>
)rawliteral";

// ---------------- ROUTES ----------------

void handleRoot()
{
  server.send(200, "text/html", webpage);
}

void handleData()
{
  String json = "{";

  json += "\"temp\":";
  json += String(currentTemp);

  json += ",\"heater\":\"";
  json += heaterState ? "ON" : "OFF";
  json += "\"";

  json += ",\"setpoint\":";
  json += String(setPoint);

  json += "}";

  server.send(200, "application/json", json);
}

void handleSetpoint()
{
  if(server.hasArg("value"))
  {
    setPoint = server.arg("value").toFloat();
  }

  server.send(200, "text/plain", "OK");
}

void handleStop()
{
  emergencyStop = !emergencyStop;

  if(emergencyStop)
  {
    heaterState = false;
    digitalWrite(RELAY_PIN, LOW);
  }

  server.send(200, "text/plain", "OK");
}

// ---------------- SETUP ----------------

void setup()
{
  Serial.begin(115200);

  pinMode(RELAY_PIN, OUTPUT);

  digitalWrite(RELAY_PIN, LOW);

  sensors.begin();

  WiFi.begin(ssid, password);

  Serial.print("Connecting");

  while(WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("WiFi Connected");

  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  server.on("/", handleRoot);

  server.on("/data", handleData);

  server.on("/setpoint", handleSetpoint);

  server.on("/stop", handleStop);

  server.begin();

  Serial.println("Web Server Started");
}

// ---------------- LOOP ----------------

void loop()
{
  server.handleClient();

  sensors.requestTemperatures();

  currentTemp = sensors.getTempCByIndex(0);

  if(currentTemp == DEVICE_DISCONNECTED_C)
  {
    heaterState = false;
    digitalWrite(RELAY_PIN, LOW);
    return;
  }

  if(currentTemp > 70)
  {
    heaterState = false;
  }

  if(!emergencyStop)
  {
    if(currentTemp < (setPoint - 2))
    {
      heaterState = true;
    }

    if(currentTemp > setPoint)
    {
      heaterState = false;
    }
  }

  digitalWrite(RELAY_PIN,
               heaterState ? HIGH : LOW);

  delay(1000);
}
