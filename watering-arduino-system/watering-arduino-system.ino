#include <EEPROM.h>
#include <iarduino_RTC.h>
iarduino_RTC time(RTC_DS1302,10,11,12);

int LIGHT_PIN = 6;
int WATER_PIN = 7;
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

String endTimeForWater = "";
String endTimeForLight = "";


String extraLight = "";
String extraWater = "";

String currentTimes = "";

//data for usage
int iterator = 0;
int endIteration = 0;
int stepForReasing = 8;

int EEPROMAddres = 0;

int dataReadedFromEEPROM = 0;

// Flags for denounce
boolean waterTurnedOn = false;
boolean lightTurnedOn = false;

void setup() {
  Serial.begin(9600);
  time.begin();
  Serial.println("Current time: " + getCurrentTime());

  if (dataReadedFromEEPROM < 1) {
    dataReadedFromEEPROM = dataReadedFromEEPROM + 1;
    
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

    calculateEndTime();

    pinMode(LED, OUTPUT);
    pinMode(LIGHT_PIN, OUTPUT);
    pinMode(WATER_PIN, OUTPUT);
  }

}

void loop() {

//  Serial.println(getCurrentTime());
  CheckingNeadedProcessing();

  if (Serial.available()) {
    char c = Serial.read();
    delay(1);
    r = c;

 
    if (r.equalsIgnoreCase(";")) {
      Serial.println("Nachal rabotat s " + command);
      // validate
      command = validator(command);
      Serial.println("Validated comment is: " + command);
      iterator = 0;
      endIteration = 0;
      stepForReasing = 8;

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

      // current Time
      endIteration += stepForReasing;
      currentTimes = parceCommand(command, iterator, endIteration);
      Serial.println("currentTimes: " + currentTimes);
      iterator = endIteration;

      // exra commands
      endIteration += 1;
      extraWater = parceCommand(command, iterator, endIteration);
      iterator = endIteration;
      
      endIteration += 1;
      extraLight = parceCommand(command, iterator, endIteration);
      iterator = endIteration;

      Serial.println("extraLight: " + extraLight);
      Serial.println("extraWater: " + extraWater);

      // Parse extra water & Light command
      parseExtraCommand();

      // setup current time
      setUpClock(getHours(currentTimes), getMinutes(currentTimes), getSeconds(currentTimes));

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

      calculateEndTime();
     
      endIteration = 0;
      command = "";
    }
    else {
      Serial.println("Device command is " + command);
      command += c;
    }
  }
}

boolean checkExistingTime(String minTime, String maxTime, String currentTime) {
  boolean result = false;
  String minTimeParsed = getHours(minTime) + getMinutes(minTime) + getSeconds(minTime);
  String maxTimeParsed = getHours(maxTime) + getMinutes(maxTime) + getSeconds(maxTime);
  String currentTimeParsed = getHours(currentTime) + getMinutes(currentTime) + getSeconds(currentTime);
  // Serial.println("" + currentTimeParsed + ">" + minTimeParsed + " && " + currentTimeParsed + "<" + maxTimeParsed);
  
  return currentTimeParsed.toInt() > minTimeParsed.toInt() && currentTimeParsed.toInt() < maxTimeParsed.toInt();
}

void CheckingNeadedProcessingWater(String currentTime) {
  boolean neadTurnOnWater = checkExistingTime(startTimeForWater, endTimeForWater, currentTime);

  if (neadTurnOnWater) {
    //Serial.println("neadTurnOnWater: " + extraWater);
    if (extraWater.equalsIgnoreCase("0")) {
      turnOffWater();
      return;
    } else {
      extraWater = "_";
      turnOnWater();
      return;
    }
  } else {
    //Serial.println("!neadTurnOnWater: " + extraWater);
    if (extraWater.equalsIgnoreCase("1")) {
      turnOnWater();
      return;
    }

    if (extraWater.equalsIgnoreCase("0")) {
      turnOffWater();
      extraWater = "_";
      return;
    }

    if (extraWater.equalsIgnoreCase("_")) {
      turnOffWater();
      return;
    }
  }  
}

void CheckingNeadedProcessingLight(String currentTime ) {
  boolean neadTurnOnLight = checkExistingTime(startTimeForLight, endTimeForLight, currentTime);
  
  if (neadTurnOnLight) {
    //Serial.println("neadTurnOnWater: " + extraWater);
    if (extraLight.equalsIgnoreCase("0")) {
      turnOffLight();
      return;
    } else {
      extraLight = "_";
      turnOnLight();
      return;
    }
  } else {
    if (extraLight.equalsIgnoreCase("1")) {
      turnOnLight();
      return;
    }

    if (extraLight.equalsIgnoreCase("0")) {
      turnOffLight();
      extraLight = "_";
      return;
    }

    if (extraLight.equalsIgnoreCase("_")) {
      turnOffLight();
      return;
    }
  }
}

void CheckingNeadedProcessing() {
  String currentTime = getCurrentTime();

  CheckingNeadedProcessingWater(currentTime);
  CheckingNeadedProcessingLight(currentTime);
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
  int result = 0;

  for (int i = 0; i < 8; i = i + 2) {
    result = data.substring(i, i+2).toInt();
    EEPROM.put(EEPROMAddres, result);
    EEPROMAddres = EEPROMAddres + 1;
    i = i + 1;
  }
}

String addMissedSigns(String readed) {
  if (readed.length() < 2) {
    readed = "0" + readed;
  }
  return readed;
}

String readData(int EEPROMAddres) {
  String result = "";
  String readed = "";

  for (int i = 0; i < 8; i = i + 2) {
    readed = EEPROM.read(EEPROMAddres);
    readed = addMissedSigns(readed);
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
  time.settime(hour.toInt(),minutes.toInt(),hour.toInt(),11,3,18,6);
}

String getCurrentTime() {
  String result = "";
  result = result + time.gettime("H:i:s");
  return result;
}

void turnOnWater() {
  if (waterTurnedOn == false) {
    Serial.println("Turn on Water");
    waterTurnedOn = !waterTurnedOn;
    digitalWrite(WATER_PIN, waterTurnedOn);
  }
}

void turnOffWater() {
  if (waterTurnedOn == true) {
    Serial.println("Turn off Water");
    waterTurnedOn = !waterTurnedOn;
    digitalWrite(WATER_PIN, waterTurnedOn);
  }
}

void turnOnLight() {
  if (lightTurnedOn == false) {
    Serial.println("Turn on Light");
    lightTurnedOn = !lightTurnedOn;
    digitalWrite(LIGHT_PIN, lightTurnedOn);
  }
}

void turnOffLight() {
  if (lightTurnedOn == true) {
    Serial.println("Turn off Light");
    lightTurnedOn = !lightTurnedOn;
    digitalWrite(LIGHT_PIN, lightTurnedOn);
  }
}

String isBiggestSixty(String input) {
  if (input.toInt() >= 60) {
    return addMissedSigns(String(input.toInt() - 60));
  } else {
    return input;
  }
}

int prevBiggestOne(String input) {
  int result = 0;
  if (input.toInt() >= 60) {
    result = result + 1;
  }
  return result;
}

String validateHours(String input) {
  if (input.toInt() >= 24) {
    input = String(input.toInt() - 24);
  }
  return input;
}

void calculateEndTime() {
  delay(500);
  String hours = "";
  String minutes = "";
  String seconds = "";
  seconds = String(getSeconds(startTimeForWater).toInt() + getSeconds(durationForWater).toInt());
  minutes = String(getMinutes(startTimeForWater).toInt() + getMinutes(durationForWater).toInt() + prevBiggestOne(seconds));
  hours = String(getHours(startTimeForWater).toInt() + getHours(durationForWater).toInt() + prevBiggestOne(minutes));

  endTimeForWater = addMissedSigns(validateHours(hours)) + ":" + addMissedSigns(isBiggestSixty(minutes)) + ":" + addMissedSigns(isBiggestSixty(seconds));

  seconds = String(getSeconds(startTimeForLight).toInt() + getSeconds(durationForLight).toInt());
  minutes = String(getMinutes(startTimeForLight).toInt() + getMinutes(durationForLight).toInt() + prevBiggestOne(seconds));
  hours = String(getHours(startTimeForLight).toInt() + getHours(durationForLight).toInt() + prevBiggestOne(minutes));

  endTimeForLight = addMissedSigns(validateHours(hours)) + ":" + addMissedSigns(isBiggestSixty(minutes)) + ":" + addMissedSigns(isBiggestSixty(seconds));

  Serial.println("End time for water: " + endTimeForWater);
  Serial.println("End time for light: " + endTimeForLight);
}

void parseExtraCommand() {
  Serial.println("parseExtraCommand:start");
  if (extraLight.equalsIgnoreCase("1")) {  
    turnOnLight();
  } else {
    turnOffLight();
  }
  if (extraWater.equalsIgnoreCase("1")) {
    turnOnWater();
  } else {
    turnOffWater();
  }
  Serial.println("parseExtraCommand:end");
}
