#include <EEPROM.h>                 // Импортируем бмблиотеку
int LED = 13;
int length = 4;
String command = "";
String test = "";
String r = "";
//sizeof(defaultVal) стандартная длинна ячейкми памяти для такого обьема равна 6
String startTimeForLight = "11:11:11"; // 0-6
String durationForLight = "11:11:11"; // 7 - 13
String brakeTimeForLight = "11:11:11"; // 14 - 20

String startTimeForWater = "11:11:11"; // 21 - 27
String durationForWater = "11:11:11"; // 28 - 34
String brakeTimeForWater = "11:11:11"; // 35 - 42

String extraLight = "";
String extraWater = "";
String readedStrng = "";

//data for usage
int iterator = 0;
int endIteration = 0;
int stepForReasing = 8;
int stepForEEPROM = 8;

int EEPROMAddres = 0;

int dataReadedFromEEPROM = 0;

void setup() {
  Serial.begin(9600);

  if (dataReadedFromEEPROM < 1) {
    dataReadedFromEEPROM = dataReadedFromEEPROM + 1;
    readDataFromEEPROM();
    pinMode(LED, OUTPUT);
  }

}

void loop() {

  if (Serial.available()) {
    char c = Serial.read();
    r = c;
 
    if (r.equalsIgnoreCase(";")) {
      Serial.println("Nachal rabotat s " + command);

      //light config
      endIteration += stepForReasing;
      startTimeForWater = parceCommand(command, iterator, endIteration);
      Serial.println("startTimeForWater: " + startTimeForWater);
      endIteration += 1;
      iterator = endIteration;
      
      endIteration += stepForReasing;
      durationForWater = parceCommand(command, iterator, endIteration);
      Serial.println("durationForWater: " + durationForWater);
      endIteration += 1;
      iterator = endIteration;

      endIteration += stepForReasing;
      brakeTimeForWater = parceCommand(command, iterator, endIteration);
      Serial.println("brakeTimeForWater: " + brakeTimeForWater);
      endIteration += 1;
      iterator = endIteration;

      //water config
      endIteration += stepForReasing;
      startTimeForLight = parceCommand(command, iterator, endIteration);
      Serial.println("startTimeForLight: " + startTimeForLight);
      endIteration += 1;
      iterator = endIteration;
      
      endIteration += stepForReasing;
      durationForLight = parceCommand(command, iterator, endIteration);
      Serial.println("durationForLight: " + durationForLight);
      endIteration += 1;
      iterator = endIteration;
      
      endIteration += stepForReasing;
      brakeTimeForLight = parceCommand(command, iterator, endIteration);
      Serial.println("brakeTimeForLight: " + brakeTimeForLight);
      endIteration += 1;
      iterator = endIteration;

      // exra commands
      endIteration += 1;
      extraLight = parceCommand(command, iterator, endIteration);
      endIteration += 1;
      iterator = endIteration;
      
      endIteration += 1;
      extraWater = parceCommand(command, iterator, endIteration);
      endIteration += 1;
      iterator = endIteration;

      String hours = getHours(startTimeForWater);
      String minutes = getMinutes(startTimeForWater);
      String seconds = getSeconds(startTimeForWater);
//      Serial.println("hours: " + hours + " ,Minutes " + minutes + " ,Seconds" + seconds);
      

      Serial.println("extraLight: " + extraLight);
      Serial.println("extraWater: " + extraWater);
      
      Serial.println("Size of startTimeForWater :" + sizeof(startTimeForWater));
      // Write data to EEPROM
      Serial.println("Write data to EEPROM");
      writeDataFromEEPROM();

     
      endIteration = 0;
      stepForReasing = 8;
      command = "";
    }
    else {
      Serial.println("Device command is " + command);
      command += c;
    }
  }
}

String parceCommand(String str, int start, int endRead) {
  return str.substring(start, endRead);
}

String getHours(String str) {
  return str.substring(0, 2);
}
String getMinutes(String str) {
  return str.substring(3, 5);
}
String getSeconds(String str) {
  return str.substring(6, 8);
}

void readDataFromEEPROM() {
  iterator = 0;
  EEPROM.get(iterator, startTimeForLight); // 0-6
  Serial.println("EEPROM startTimeForLight: " + startTimeForLight + ", readed with step " + iterator);
  iterator = iterator + stepForEEPROM;
  EEPROM.get(iterator, durationForLight); // 7 - 13
  Serial.println("EEPROM durationForLight: " + durationForLight + ", readed with step " + iterator);
  iterator = iterator + stepForEEPROM;
  EEPROM.get(iterator, brakeTimeForLight); // 14 - 20
  Serial.println("EEPROM brakeTimeForLight: " + brakeTimeForLight + ", readed with step " + iterator);
  iterator = iterator + stepForEEPROM;
  
  EEPROM.get(iterator, startTimeForWater); // 21 - 27
  Serial.println("EEPROM startTimeForWater: " + startTimeForWater + ", readed with step " + iterator);
  iterator = iterator + stepForEEPROM;
  EEPROM.get(iterator, durationForWater); // 28 - 34
  Serial.println("EEPROM durationForWater: " + durationForWater + ", readed with step " + iterator);
  iterator = iterator + stepForEEPROM;
  EEPROM.get(iterator, brakeTimeForWater); // 35 - 42
  Serial.println("EEPROM brakeTimeForWater: " + brakeTimeForWater + ", readed with step " + iterator);
  iterator = 0;

//  Serial.println("Readed data from EEPROM");
//  Serial.println("startTimeForLight: " + startTimeForLight);
//  Serial.println("brakeTimeForLight: " + brakeTimeForLight);
//  Serial.println("startTimeForWater: " + startTimeForWater);
//  Serial.println("durationForWater: " + durationForWater);
//  Serial.println("brakeTimeForWater: " + brakeTimeForWater);
}
 
void writeDataFromEEPROM() {
  iterator = 0;
  if( startTimeForLight.length() > 7) {
    Serial.println("Write data to EEPROM size: " + startTimeForLight);
    EEPROM.put(iterator, startTimeForLight); // 0-6
    iterator = iterator + stepForEEPROM;
    EEPROM.put(iterator, durationForLight); // 7 - 13
    iterator = iterator + stepForEEPROM;
    EEPROM.put(iterator, brakeTimeForLight); // 14 - 20
    iterator = iterator + stepForEEPROM;
    
    EEPROM.put(iterator, startTimeForWater); // 21 - 27
    iterator = iterator + stepForEEPROM;
    EEPROM.put(iterator, durationForWater); // 28 - 34
    iterator = iterator + stepForEEPROM;
    EEPROM.put(iterator, brakeTimeForWater); // 35 - 42
    
    iterator = 0;
  }
}

void saveData(String data) {
  String result = "";
  EEPROMAddres = 0;

  for (int i = 0; i < 8; i = i + 2) {
    result = data.substring(i, i+2);
    Serial.println(result);
    EEPROM.put(EEPROMAddres, result);
    EEPROMAddres = EEPROMAddres + 1;
    i = i + 1;
  }
}

String readData() {
  String result = "";
  EEPROMAddres = 0;

  for (int i = 0; i < 8; i = i + 2) {
    result = result + EEPROM.read(EEPROMAddres) + ":";
    Serial.println(result);
    EEPROMAddres = EEPROMAddres + 1;
    i = i + 1;
  }

  return result.substring(0, 8);
}

