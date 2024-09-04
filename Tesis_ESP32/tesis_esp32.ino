#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h> // Asegúrate de tener esta librería instalada

// Configura tus credenciales WiFi
const char* ssid = "Celerity_lara";
const char* password = "vicflara09";

// Configura la dirección del broker MQTT y el puerto
const char* mqtt_server = "192.168.3.10";
const int mqtt_port = 1883;

// Crear un cliente WiFi y MQTT
WiFiClient espClient;
PubSubClient client(espClient);

// Definiciones del sensor de flujo de agua
#define FLOW_SENSOR_PIN 4
float calibrationFactor = 2.2;
volatile byte pulseCount;
float flowRate; // En Litros por minuto (L/min)
float flowRate_m3s; // En metros cúbicos por segundo (m³/s)
unsigned long flowMilliLitres;
float totalLitres;

// Definiciones del sensor de nivel de combustible
#define FUEL_LEVEL_SENSOR_PIN 32

// Definiciones de los sensores de presión
#define PRESSURE_SENSOR_IN_PIN 34 // Pin analógico para la presión de entrada
#define PRESSURE_SENSOR_OUT_PIN 35 // Pin analógico para la presión de salida
float sensorMinVoltage = 0.0; // Voltaje mínimo de salida del sensor (V)
float sensorMaxVoltage = 4.75; // Voltaje máximo de salida del sensor (V)

// Offset para asegurar que las lecturas sin presión den cero
const float pressureOffset = 0.0; // Ajusta este valor según la lectura sin presión

// Valores constantes de voltaje y corriente
const float voltage = 110.0; // Voltaje en voltios
const float current = 1.5;  // Corriente en amperios

// Variables para almacenar los últimos valores válidos de presión
float lastPressureIn = -1.0;
float lastPressureOut = -1.0;

// Función de interrupción para el contador de pulsos
void IRAM_ATTR pulseCounter() {
  pulseCount++;
}

void setup() {
  Serial.begin(115200);
  
  // Conectar a la red WiFi
  setup_wifi();
  
  // Configurar el servidor MQTT
  client.setServer(mqtt_server, mqtt_port);
  
  // Configurar el sensor de flujo de agua
  pinMode(FLOW_SENSOR_PIN, INPUT_PULLUP);
  pulseCount = 0;
  flowRate = 0.0;
  flowMilliLitres = 0;
  totalLitres = 0;
  
  // Configurar el sensor de nivel de combustible
  pinMode(FUEL_LEVEL_SENSOR_PIN, INPUT);
  
  // Configurar los sensores de presión
  pinMode(PRESSURE_SENSOR_IN_PIN, INPUT);
  pinMode(PRESSURE_SENSOR_OUT_PIN, INPUT);
  
  // Configurar la interrupción para el sensor de flujo
  attachInterrupt(digitalPinToInterrupt(FLOW_SENSOR_PIN), pulseCounter, FALLING);
}

void setup_wifi() {
  delay(10);
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect("ESP32Client")) {
      Serial.println("connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

float readPressure(int pin) {
    int sensorValue = analogRead(pin);
    float sensorVoltage = (sensorValue / 4095.0) * 5.25;

    Serial.print("Sensor Voltage (Pin ");
    Serial.print(pin);
    Serial.print("): ");
    Serial.println(sensorVoltage);

    // Verificar si el voltaje está dentro del rango deseado
    if (sensorVoltage >= sensorMinVoltage && sensorVoltage <= sensorMaxVoltage) {
        // Calibrar la presión basada en el voltaje usando interpolación lineal
        float pressure = (sensorVoltage - sensorMinVoltage) * (20.0 / (sensorMaxVoltage - sensorMinVoltage));
        pressure -= pressureOffset;

        // Asegurar que la presión no sea negativa
        if (pressure < 0.0) {
            return -1.0; // Indicador de lectura inválida
        }

        Serial.print("Pressure: ");
        Serial.println(pressure);

        return pressure;
    } else {
        // Si el voltaje no está dentro del rango, retornar -1 como indicador de valor no válido
        return -1.0; // Indicador de lectura inválida
    }
}

void loop() {
  // Asegurarse de que el cliente MQTT está conectado
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  
  unsigned long currentMillis = millis();
  static unsigned long previousMillis = 0;
  int interval = 1000; // Intervalo de 1 segundo
  
  if (currentMillis - previousMillis > interval) {
    byte pulse1Sec = pulseCount;
    pulseCount = 0;
    
    // Verificar el número de pulsos contados
    Serial.print("Pulses in 1 second: ");
    Serial.println(pulse1Sec);
    
    flowRate = ((1000.0 / (currentMillis - previousMillis)) * pulse1Sec) / calibrationFactor;
    previousMillis = currentMillis;
    
    flowMilliLitres = (flowRate / 60.0) * 1000.0;
    totalLitres += (flowRate / 60.0);
    
    // Calcular el flujo en metros cúbicos por segundo (m³/s)
    flowRate_m3s = flowRate / 1000.0 / 60.0;
    
    // Publicar datos de flujo en formato JSON
    StaticJsonDocument<200> flowDoc;
    flowDoc["flowRate"] = flowRate; // Usando el flujo en L/min
    flowDoc["totalLitres"] = totalLitres;
    String flowPayload;
    serializeJson(flowDoc, flowPayload);
    client.publish("sensor/waterflow", flowPayload.c_str());
    
    Serial.println(flowPayload);
  }
  
  // Leer y publicar el nivel de combustible en formato JSON
  int levelSensorValue = analogRead(FUEL_LEVEL_SENSOR_PIN);
  float altitudeFuel = (levelSensorValue / 4095.0) * 10.5;
  float fuelLevel = (altitudeFuel / 10.5) * 0.25;

  StaticJsonDocument<200> levelDoc;
  levelDoc["fuelLevel"] = fuelLevel;
  String levelPayload;
  serializeJson(levelDoc, levelPayload);
  client.publish("sensor/fuellevel", levelPayload.c_str());
  Serial.println(levelPayload);
  
  // Leer la presión de entrada y salida
  float pressureIn = readPressure(PRESSURE_SENSOR_IN_PIN);
  float pressureOut = readPressure(PRESSURE_SENSOR_OUT_PIN);

  // Mantener el último valor válido si la lectura es menor que cero
  if (pressureIn >= 0) {
    lastPressureIn = pressureIn;
  }
  if (pressureOut >= 0) {
    lastPressureOut = pressureOut;
  }

  // Asegurar que la presión de entrada no sea mayor que la presión de salida
  if (lastPressureIn > lastPressureOut) {
    lastPressureIn = pressureIn;
  }

  // Crear y publicar el JSON con ambos valores de presión
  StaticJsonDocument<200> pressureDoc;
  
  // Solo incluir valores válidos en el JSON si son mayores que cero
  if (lastPressureIn > 0) {
    pressureDoc["pressure_in"] = lastPressureIn;
  }
  if (lastPressureOut > 0) {
    pressureDoc["pressure_out"] = lastPressureOut;
  }
  
  // Calcular la diferencia de presión en Pascales
  float pressureDifferencePa = (lastPressureOut - lastPressureIn) * 6894.76; // Conversión de psi a Pascales
  
  // Solo publicar el JSON si contiene datos válidos
  if (!pressureDoc.isNull()) {
    String pressurePayload;
    serializeJson(pressureDoc, pressurePayload);
    client.publish("sensor/pressure", pressurePayload.c_str());
    Serial.println(pressurePayload);
  }
  
  // Calcular la potencia hidráulica en vatios
  float powerHydraulicW = pressureDifferencePa * flowRate_m3s; // Potencia en vatios usando Pa y m³/s
  
  // Calcular la potencia eléctrica en vatios
  float powerElectricW = voltage * current; // Potencia eléctrica en vatios
  
  // Publicar la potencia hidráulica y eléctrica en vatios en formato JSON
  StaticJsonDocument<200> powerDoc;
  powerDoc["powerHydraulic_W"] = powerHydraulicW;
  powerDoc["powerElectric_W"] = powerElectricW;
  
  String powerPayload;
  serializeJson(powerDoc, powerPayload);
  client.publish("sensor/power", powerPayload.c_str());
  Serial.println(powerPayload);
  
  // Calcular la eficiencia de la bomba en porcentaje
  float efficiency = (powerHydraulicW / powerElectricW) * 100.0;
  
  // Publicar la eficiencia de la bomba en formato JSON
  StaticJsonDocument<200> efficiencyDoc;
  efficiencyDoc["efficiency"] = efficiency;
  
  String efficiencyPayload;
  serializeJson(efficiencyDoc, efficiencyPayload);
  client.publish("sensor/efficiency", efficiencyPayload.c_str());
  Serial.println(efficiencyPayload);
  
  delay(1000);
}

