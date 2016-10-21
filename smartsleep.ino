#include <DHT.h>
#include <doxygen.h>
#include <ESP8266.h>

#define SSID "neon junkyard"
#define PASSWORD "monomania"

#define DHTPIN 7          //define as DHTPIN the Pin 3 used to connect the Sensor
#define DHTTYPE DHT11     //define the sensor used(DHT11)
DHT dht(DHTPIN, DHTTYPE); //create an instance of DHT

String server = "104.236.90.130:8000";
String uri = "/postNewInfo";

ESP8266 wifi(Serial2, (uint32_t) 115200);

void setup() {
  Serial2.begin(115200);
  Serial.begin(115200);

  // Modo de operação do ESP8266
  if(wifi.setOprToStationSoftAP()){
    Serial.print("Modo OK\r\n");
  } else {
    Serial.print("Erro na configuracao do modo\r\n");
  }

  // Conecta a rede wi-fi
  if(wifi.joinAP(SSID, PASSWORD)){
    Serial.print("Conexao OK\r\n");
    Serial.print("IP: ");
    Serial.println(wifi.getLocalIP().c_str());
  } else {
    Serial.print("Erro na conexao\r\n");
  }

  // Desabilita multiplas conexoes
  if(wifi.disableMUX()) {
    Serial.print("Desabilita MUX\r\n");
  } else {
    Serial.print("Erro ao desabilitar MUX\r\n");
  }
//
//  // Inicializa sensor de temperatura e umidade
//  dht.begin();
}

void loop() {
while (Serial2.available()) {
Serial.write(Serial2.read());
}
while (Serial.available()) {
Serial2.write(Serial.read());
}
//  char buffer[1024] = {0};
//  String data, postRequest;
//
//  char sensor[10] = {0};
//  float h = dht.readHumidity(); 
//  float t = dht.readTemperature(); // Celsius (default)
//
//  // Checar se leituras foram realizadas
//  if (isnan(h) || isnan(t)) {    
//    Serial.println("Failed to read from DHT sensor!");
//    return;
//  }
//  Serial.print(t, 2);    //print the temperature
//  Serial.print("\t");
//  Serial.println(h, 2);  //print the humidity
//  
//  snprintf(sensor, 10, "%f", t);
//  data  = "deviceID=DTH11&location=labsoft&data=10&dataType=temperatura";
//  postRequest = "POST " + uri + " HTTP/1.1\r\n" + "Host: " + server + "\r\n" +
//  "User-Agent: Arduino\r\n" + "Content-Type: application/x-www-form-urlencoded\r\n" +
//  "Connection: close\r\n" + "Content-Length: " + data.length() + "\r\n" + "\r\n" + data;
//
//  // Cria conexao TCP
//  if (wifi.createTCP("104.236.90.130", 80)) {
//    Serial.print("Conexao TCP OK\r\n");
//  } else {
//    Serial.print("Erro ao criar conexao TCP\r\n");
//  }
//
//  // Loga request no terminal serial
//  Serial.println(postRequest);
//
//  // Envia POST
//  if(wifi.send(postRequest.c_str(), postRequest.length())) {
//    Serial.print("Dado enviado\r\n");
//  } else {
//    Serial.print("Erro ao enviar dado\r\n");
//  }
//
//  // Recebe resposta do servidor
//  if(wifi.recv(buffer, 256)) {
//    Serial.println(buffer);
//  } else {
//    Serial.print("Erro ao receber dado\r\n");
//  }
//
//  delay(5000);
}
