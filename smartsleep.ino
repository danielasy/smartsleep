#include <DHT.h>
#include <doxygen.h>
#include <ESP8266.h>
#include <stdlib.h>

#define BAUDRATE    115200
#define SSID        "Nome"
#define PASSWORD    "Senha"
#define DHTPIN      7               // Pin 7 is connected to DHT11
#define DHTTYPE     DHT11           // sensor type is DHT11
#define SOUNDPIN    A0
#define LDRPIN      A8
#define DELAYMS     5000
#define NUSP        "10076585"

String  host  = "104.236.90.130";
int     port  = 8000;
String  uri   = "/restAPI/postNewInfo";

ESP8266 wifi(Serial2, (uint32_t) BAUDRATE);
DHT dht(DHTPIN, DHTTYPE);

String postRequestString(String deviceID, String sensorData, String dataType){
  String data, postRequest;
  
  data  = "deviceID=" + deviceID + "&location=labsoft&data=" + sensorData + "&dataType=" + dataType;
  postRequest = "POST " + uri + " HTTP/1.1\r\n" + "Host: " + host +":"+ port + "\r\n" +
  "User-Agent: Arduino MEGA 2560\r\n" + "Content-Type: application/x-www-form-urlencoded\r\n" +
  "Connection: close\r\n" + "Content-Length: " + data.length() + "\r\n" + "\r\n" + data;

  return postRequest;
}

void post(String postRequest){
  char buffer[256] = {0};
  
  if (wifi.createTCP(host, port)) {
    Serial.println("Conexao TCP estabelecida.");
  } else {
    Serial.println("Erro ao criar conexao TCP!");
  }

  Serial.println(postRequest);

  if(wifi.send(postRequest.c_str(), postRequest.length())) {
    Serial.println("Dado enviado a nuvem.");
  } else {
    Serial.println("Erro ao enviar dado a nuvem!");
  }

  if(wifi.recv(buffer, 256)) {
    Serial.println(buffer);
  } else {
    Serial.println("Erro ao receber resposta do servidor!");
  }
}

void setup() {
  Serial2.begin(BAUDRATE);
  Serial.begin(BAUDRATE);

  // ESP8266 setup
  if(wifi.setOprToStationSoftAP()){
    Serial.println("Modo OK.");
  } else {
    Serial.println("Erro na configuracao do modo!");
  }

  if(wifi.joinAP(SSID, PASSWORD)){
    Serial.println("Conexao com wi-fi OK.");
    Serial.print("IP: ");
    Serial.println(wifi.getLocalIP().c_str());
  } else {
    Serial.println("Erro na conexao com wi-fi!");
  }
  
  if(wifi.disableMUX()) {
    Serial.println("Desabilita MUX.\r\n");
  } else {
    Serial.println("Erro ao desabilitar MUX!\r\n");
  }

  // Sensor setup
  dht.begin();
}

void loop() {
  String postRequest;

  char sensor[10] = {0};
  float h = dht.readHumidity(); 
  float t = dht.readTemperature(); // Celsius (default)

  float noise = ((analogRead(SOUNDPIN) - 600.0)/30.0) * 100;

  int light_intensity = analogRead(LDRPIN);

  // Sensor data processing
  if (isnan(h) || isnan(t)) {    
    Serial.println("Failed to read from DHT sensor!");
  }
  
  Serial.print(t, 2);
  Serial.print("\t");
  Serial.print(h, 2);
  Serial.print("\t");
  Serial.print(noise);
  Serial.print("\t");
  Serial.println(light_intensity);
  
  // POST results to cloud
  if (!isnan(t)) {    
    dtostrf(t, 3, 2, sensor);
    postRequest = postRequestString(NUSP, sensor, "temperatura");
    post(postRequest);
  }

  if (!isnan(h)) {    
    dtostrf(h, 3, 2, sensor);
    postRequest = postRequestString(NUSP, sensor, "umidade");
    post(postRequest);
  }

  dtostrf(noise, 4, 2, sensor);
  postRequest = postRequestString(NUSP, sensor, "ruido");
  post(postRequest);

  snprintf(sensor, 10, "%d", light_intensity);
  postRequest = postRequestString(NUSP, sensor, "luminosidade");
  post(postRequest);

  // Interval between sensor readings
  delay(DELAYMS);
}
