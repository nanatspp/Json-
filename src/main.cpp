
#include "BluetoothSerial.h"
#include "Wire.h"
#include "LiquidCrystal_I2C.h" 
#include <ArduinoJson.h>

LiquidCrystal_I2C lcd(0x27, 16, 4); 
BluetoothSerial SerialBT;

uint8_t endMaster[] = {0x14, 0x33, 0x5C, 0x52, 0x3C, 0xD8};

String g_umidade = "---";
String g_tempC   = "---";
String g_tempF   = "---";
String g_status = "Iniciando";

unsigned long tempoAnterior = 0;
const long intervalo = 2000;

void processarDados(String dados) {
  dados.trim(); 

  StaticJsonDocument<192> doc; 

  DeserializationError error = deserializeJson(doc, dados);

  if (error) {
    Serial.print("deserializeJson() falhou: ");
    Serial.println(error.c_str());
    g_status = "JSON Err";
    return;
  }

  g_umidade = String(doc["umid"].as<float>(), 1);
  g_tempC   = String(doc["tempC"].as<float>(), 1);
  g_tempF   = String(doc["tempF"].as<float>(), 1);
  
  Serial.print("JSON Recebido: U:"); Serial.print(g_umidade);
  Serial.print(" C:"); Serial.print(g_tempC);
  Serial.print(" F:"); Serial.println(g_tempF);
}

void atualizarLCD() {
  lcd.clear();

  lcd.setCursor(0, 0); 
  lcd.print("Umid: " + g_umidade + "%");
  
  lcd.setCursor(0, 1); 
  lcd.print("Temp C: " + g_tempC + "C");

  lcd.setCursor(0, 2); 
  lcd.print("Temp F: " + g_tempF + "F");

  lcd.setCursor(0, 3); 
  lcd.print("Status: " + g_status);
}

void setup() {
  Serial.begin(9600); 

  lcd.init();
  lcd.backlight();
  lcd.print("Iniciando JSON...");

  SerialBT.begin("ESP32_CLIENTE", true); 
  
  Serial.println("Iniciando modo Cliente (Master)...");
  Serial.println("Conectando por Endereco MAC...");
  
  if (SerialBT.connect(endMaster)) {
    Serial.println("Conectado ao servidor!");
    g_status = "OK";
    lcd.clear();
    lcd.print("Conectado!");
  } else {
    Serial.println("Falha ao conectar.");
    g_status = "Erro";
    lcd.clear();
    lcd.print("Falha conexao");
  }

  tempoAnterior = millis(); 
}

void loop() {
  
  if (!SerialBT.connected()) {
    g_status = "X"; 
    Serial.println("Desconectado. Tentando reconectar por MAC...");
    
    if (SerialBT.connect(endMaster)) {
      Serial.println("Reconectado!");
      g_status = "OK"; 
    } else {
      Serial.println("Falha na reconexao.");
    
      delay(3000); 
    }
  }

  if (SerialBT.available()) {
    String dadosRecebidos = SerialBT.readStringUntil('\n');
    processarDados(dadosRecebidos); 
  }

  unsigned long tempoAtual = millis();
  if (tempoAtual - tempoAnterior >= intervalo) {
    tempoAnterior = tempoAtual;
    atualizarLCD(); 
  }
}
    // coisado do COISO AAAAAAAAAAAAAAAA

//     // Lógica de Reconexão (Tenta reconectar se a conexão cair)
//     if (!SerialBT.connected()) {
//         g_status = "X";
//         Serial.println("Desconectado. Tentando reconectar por MAC...");
        
//         if (SerialBT.connect(endMaster)) { 
//             Serial.println("Reconectado!");
//             g_status = "OK"; 
//         } else {
//             Serial.println("Falha na reconexao.");
//             delay(5000); // Espera antes de tentar novamente
//         }
//     }

    
//     // 3. Leitura e Processamento dos dados
//     if (SerialBT.available()) {
        
//         // Lê a string completa até a quebra de linha ('\n')
//         String dadosRecebidos = SerialBT.readStringUntil('\n');
//         processarDados(dadosRecebidos);
//     }

//     // 4. Atualiza o LCD a cada 2 segundos
//     unsigned long tempoAtual = millis();
//     if (tempoAtual - tempoAnterior >= intervalo) {
//         tempoAnterior = tempoAtual;
//         atualizarLCD(); 
//     }
// }

// #include "BluetoothSerial.h"
// #include "Wire.h"
// #include "LiquidCrystal_I2C.h"
// #include <ArduinoJson.h> 

// LiquidCrystal_I2C lcd(0x27, 16, 4);
// BluetoothSerial SerialBT;
// uint8_t endMaster[] = {0xF4, 0x65, 0x0B, 0x54, 0x58, 0xAE}; 


// String g_umidade = "---";
// String g_tempC   = "---";
// String g_tempF   = "---";
// String g_status = "Iniciando";

// unsigned long tempoAnterior = 0;
// const long intervalo = 2000;
// void processarDados(String jsonDados) {
  
//     const size_t CAPACITY = JSON_OBJECT_SIZE(3);
//     StaticJsonDocument<CAPACITY> doc;

//     DeserializationError error = deserializeJson(doc, jsonDados);

//     if (error) {
//         Serial.print("Erro de Desserializacao JSON: ");
//         Serial.println(error.c_str());
//         g_status = "JSON Erro";
//         return;
//     }

//     float umidade = doc["umidade"].as<float>();
//     float tempC = doc["tempC"].as<float>();
//     float tempF = doc["tempF"].as<float>();

//     g_umidade = String(umidade, 1); 
//     g_tempC   = String(tempC, 1);
//     g_tempF   = String(tempF, 1);
//     g_status = "OK"; 

//     Serial.print("JSON Recebido e Desmembrado. U:");
//     Serial.print(g_umidade);
//     Serial.print(" C:");
//     Serial.print(g_tempC);
//     Serial.print(" F:");
//     Serial.println(g_tempF);
// }


// void atualizarLCD() {
//     lcd.clear();

//     lcd.setCursor(0, 0);
//     lcd.print("Umid: " + g_umidade + "%");

//     lcd.setCursor(0, 1);
//     lcd.print("Temp C: " + g_tempC + "C");

//     lcd.setCursor(0, 2);
//     lcd.print("Temp F: " + g_tempF + "F");

//     lcd.setCursor(0, 3);
//     lcd.print("Status: " + g_status);
// }

// void setup() {
//     Serial.begin(9600);

//     lcd.init();
//     lcd.backlight();
//     lcd.print("Iniciando...");

    
//     SerialBT.begin("ESP32_CLIENTE", true);

//     Serial.println("Iniciando modo Cliente...");

//     Serial.println("Conectando por Endereco MAC...");

//     if (SerialBT.connect(endMaster)) { 
//         Serial.println("Conectado ao servidor!");
//         g_status = "OK";
//         lcd.clear();
//         lcd.print("Conectado!");
//     } else {
//         Serial.println("Falha ao conectar.");
//         g_status = "Erro";
//         lcd.clear();
//         lcd.print("Falha conexao");
//     }

//     tempoAnterior = millis();

// }

// void loop() {

//     if (!SerialBT.connected()) {
//         g_status = "X";
//         Serial.println("Desconectado. Tentando reconectar por MAC...");

//         if (SerialBT.connect(endMaster)) {
//             Serial.println("Reconectado!");
//             g_status = "OK";
//         } else {
//             Serial.println("Falha na reconexao.");
//         }
//     }

//     if (SerialBT.available()) {
//         String dadosRecebidos = SerialBT.readStringUntil('\n');
  
//         processarDados(dadosRecebidos);
//     }

//     unsigned long tempoAtual = millis();
//     if (tempoAtual - tempoAnterior >= intervalo) {
//         tempoAnterior = tempoAtual;
//         atualizarLCD();
//     }
// }