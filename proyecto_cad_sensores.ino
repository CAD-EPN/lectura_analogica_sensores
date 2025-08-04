#include <WiFi.h>
#include <HTTPClient.h>
#include <InfluxDbClient.h>
#include <InfluxDbCloud.h>
#include "DHT.h"

#define WIFI_SSID "nombre de tu red"
#define WIFI_PASSWORD "contraseña de tu red"

const char* telegramToken = "es el token unico que genera chat bot";
const char* chatID = "tu chat id de telegram";

#define INFLUXDB_URL "https:api de tu influx"
#define INFLUXDB_TOKEN "token de tu influx"
#define INFLUXDB_ORG "org unico de influx"
#define INFLUXDB_BUCKET "bucket de influx"
#define TZ_INFO "tu zona de horario"

InfluxDBClient client(INFLUXDB_URL, INFLUXDB_ORG, INFLUXDB_BUCKET, INFLUXDB_TOKEN, InfluxDbCloud2CACert);
Point sensor("PCDA");

#define DHTPIN 15
#define DHTTYPE DHT11
#define LDR_PIN 33

DHT dht(DHTPIN, DHTTYPE);

unsigned long tiempoActual = 0;
unsigned long tiempoAnterior = 0;
const unsigned long intervalo = 10000; 

void setup() {
  Serial.begin(115200);
  dht.begin();
  analogReadResolution(12); 

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }

  timeSync(TZ_INFO, "pool.ntp.org", "time.nis.gov");

  sensor.addTag("device", "ESP32");
  sensor.addTag("SSID", WiFi.SSID());
}

void loop() {
  tiempoActual = millis();
  if (tiempoActual - tiempoAnterior >= intervalo) {
    tiempoAnterior = tiempoActual;

    float temp = dht.readTemperature();
    float hum = dht.readHumidity();
    int ldrValue = analogRead(LDR_PIN);
    float luzPorcentaje = 100.0 - (ldrValue / 4095.0 * 100.0);

    if (temp >= 30.0) {
      String msg = " Alerta de alta temperatura\nTemperatura: " + String(temp, 1) + " °C";
      enviarTelegram(msg);
    }

    if (hum > 50.0) {
      String msg = " Alerta de alta humedad\nHumedad: " + String(hum, 1) + " %";
      enviarTelegram(msg);
    }

    if (luzPorcentaje <= 0.0) {
      String msg = " Alerta de poca luz\nLuz: 0%";
      enviarTelegram(msg);
    }

    sensor.clearFields();
    sensor.addField("Temperatura", temp);
    sensor.addField("Humedad", hum);
    sensor.addField("Luz", luzPorcentaje);
    sensor.addField("rssi", WiFi.RSSI());
    client.writePoint(sensor);
  }
}

void enviarTelegram(String mensaje) {
  if (WiFi.status() != WL_CONNECTED) return;

  HTTPClient http;
  String url = "https://api.telegram.org/bot" + String(telegramToken) +
               "/sendMessage?chat_id=" + String(chatID) +
               "&text=" + urlencode(mensaje) +
               "&parse_mode=Markdown";
  http.begin(url);
  http.GET();
  http.end();
}

String urlencode(String str) {
  String encoded = "";
  char c;
  char code0, code1;
  for (int i = 0; i < str.length(); i++) {
    c = str.charAt(i);
    if (isalnum(c)) {
      encoded += c;
    } else {
      code1 = (c & 0xf) + '0';
      if ((c & 0xf) > 9) code1 += 7;
      c = (c >> 4) & 0xf;
      code0 = c + '0';
      if (c > 9) code0 += 7;
      encoded += '%';
      encoded += code0;
      encoded += code1;
    }
  }
  return encoded;
}