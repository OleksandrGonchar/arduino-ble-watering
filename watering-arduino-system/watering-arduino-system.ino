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

int EEPROMAddres = 0;

int dataReadedFromEEPROM = 0;

void setup() {
  Serial.begin(9600);

  if (dataReadedFromEEPROM < 1) {
    dataReadedFromEEPROM = dataReadedFromEEPROM + 1;
    Serial.println("Read Data From EEPROM");
    readData(0);
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
      writeData(startTimeForWater);

     
      endIteration = 0;
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

void writeData(String data) {
  Serial.println("Start writing: " + data);
  int result = 0;
  EEPROMAddres = 0;

  for (int i = 0; i < 8; i = i + 2) {
    result = data.substring(i, i+2).toInt();
    Serial.println(result);
    EEPROM.put(EEPROMAddres, result);
    EEPROMAddres = EEPROMAddres + 1;
    i = i + 1;
  }
}

String readData(int startAdress) {
  String result = "";
  EEPROMAddres = 0;

  for (int i = 0; i < 8; i = i + 2) {
    result = result + EEPROM.read(EEPROMAddres) + ":";
    EEPROMAddres = EEPROMAddres + 1;
    i = i + 1;
  }
  Serial.println("I am read from rpm: " + result);

  return result.substring(startAdress, 8);
}

