#include <EEPROM.h>
#include <iarduino_RTC.h>
iarduino_RTC time(RTC_DS1302,10,13,12);

int LED = 13;
int length = 4;
String command = "";
String test = "";
String r = "";

String startTimeForLight = "";
String durationForLight = "";
String brakeTimeForLight = "";

String startTimeForWater = "";
String durationForWater = "";
String brakeTimeForWater = "";

String extraLight = "";
String extraWater = "";

//data for usage
int iterator = 0;
int endIteration = 0;
int stepForReasing = 8;

int EEPROMAddres = 0;

int dataReadedFromEEPROM = 0;


void setup() {
  Serial.begin(9600);
//  setUpClock("0", "51", "21"); // Uncomment for setup data

  if (dataReadedFromEEPROM < 1) {
    dataReadedFromEEPROM = dataReadedFromEEPROM + 1;
    Serial.println("Read Data From EEPROM");
    
    startTimeForWater = readData(0);
    durationForWater = readData(3);
    brakeTimeForWater = readData(6);
    Serial.println("startTimeForWater : " + startTimeForWater);
    Serial.println("durationForWater : " + durationForWater);
    Serial.println("brakeTimeForWater : " + brakeTimeForWater);

    startTimeForLight = readData(9);
    durationForLight = readData(12);
    brakeTimeForLight = readData(15);
    Serial.println("startTimeForLight : " + startTimeForLight);
    Serial.println("durationForLight : " + durationForLight);
    Serial.println("brakeTimeForLight : " + brakeTimeForLight);

    pinMode(LED, OUTPUT);
  }

}

void loop() {

  Serial.println(getCurrentTime());
  if (Serial.available()) {
    char c = Serial.read();
    r = c;

 
    if (r.equalsIgnoreCase(";")) {
      Serial.println("Nachal rabotat s " + command);
      // validate
      command = validator(command);
      Serial.println("Validated comment is: " + command);

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

      Serial.println("extraLight: " + extraLight);
      Serial.println("extraWater: " + extraWater);

      // Write data to EEPROM
      Serial.println("Write data to EEPROM");
      // Water
      writeData(startTimeForWater, 0);
      writeData(durationForWater, 3);
      writeData(brakeTimeForWater, 6);
      // Light
      writeData(startTimeForLight, 9);
      writeData(durationForLight, 12);
      writeData(brakeTimeForLight, 15);

     
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

void writeData(String data, int EEPROMAddres) {
  Serial.println("Start writing: " + data);
  int result = 0;

  for (int i = 0; i < 8; i = i + 2) {
    result = data.substring(i, i+2).toInt();
    Serial.println(result);
    EEPROM.put(EEPROMAddres, result);
    EEPROMAddres = EEPROMAddres + 1;
    i = i + 1;
    Serial.println("Last addres eerpm: " + EEPROMAddres); 
  }
}

String readData(int EEPROMAddres) {
  String result = "";
  String readed = "";

  for (int i = 0; i < 8; i = i + 2) {
    readed = EEPROM.read(EEPROMAddres);
    if (readed.length() < 2) {
      readed = "0" + readed;
    }
    result = result + readed + ":";
    EEPROMAddres = EEPROMAddres + 1;
    i = i + 1;
  }
  result = result.substring(0, 8);

  return result;
}

String validator(String inputData) {
  int len = inputData.length();

  for (int i = 0; i < len; i++) {
    if (!isdigit(inputData[i])) {
       inputData = inputData.substring(1, len); 
       i = i - 1;
    } else {
      i = len + 1;
    }
  }

  return inputData;
}

void setUpClock(String hour, String minutes, String seconds) { 
  time.begin();
  time.settime(hour.toInt(),minutes.toInt(),hour.toInt(),11,3,18,6);  // 0  сек, 51 мин, 21 час, 27, октября, 2015 года, вторник
}

String getCurrentTime() {
  String result = "";
  result = result + time.gettime("d-m-Y, H:i:s, D");
  return result;
}

