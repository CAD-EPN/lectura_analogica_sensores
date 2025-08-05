# Proyecto IoT con ESP32 – Lectura de Temperatura, Humedad y Luz

Este proyecto utiliza un ESP32 con sensores para medir la temperatura, humedad y luz, y enviar alertas por Telegram cuando se detectan valores anormales.

# ¿Cómo funciona?

1. El ESP32 se conecta a WiFi con las credenciales que pongas en el código.
2. Lee los valores de los sensores:
   - Sensor DHT11 para temperatura y humedad
   - Sensor LDR para luz ambiente
3. Si algún valor supera un umbral (ej. temperatura > 30°C), se envía una alerta por Telegram.
4. Registra los datos en una base de datos llamada InfluxDB para monitoreo.

# Para usar este código:

1. Configura tu red WiFi: cambia los valores de `WIFI_SSID` y `WIFI_PASSWORD`.
2. Configura Telegram: crea un bot de Telegram y usa el token que te da BotFather, luego pon tu `chatID`.
3. Configura InfluxDB: necesitas tener una cuenta en InfluxDB para poder almacenar los datos.
4. Carga el código en tu ESP32 y conéctalo a los sensores.

# Librerías necesarias:

- `WiFi.h` – para la conexión WiFi
- `HTTPClient.h` – para enviar mensajes a Telegram
- `InfluxDbClient.h` – para guardar los datos en InfluxDB
- `DHT.h` – para leer el sensor de temperatura y humedad

# Autor

Este proyecto fue realizado por Mateo Rosero y Jefferson Quispe, estudiante de la Escuela Politecnica Nacional
