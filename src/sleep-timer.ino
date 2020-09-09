/*
 * Project sleep-timer
 * Description: Device code for sleep timer project
 * Author: Jayd Lawrence
 * Date: 9th September 2020
 * OS version: 1.5.2+
 */


int led1 = D0; // Done pin
int led2 = D1; // 75% pin
int led3 = D2; // 50% pin
int led4 = D3; // 25% pin
int led5 = D4; // 0% pin
int startTime = 0;
int endTime = 0;

// reset timer
void  resetTimer() {
  startTime = 0;
  endTime = 0;
}

// turn off all LEDs
void turnAllOff() {
  digitalWrite(led1, LOW);
  digitalWrite(led2, LOW);
  digitalWrite(led3, LOW);
  digitalWrite(led4, LOW);
  digitalWrite(led5, LOW);
}

// reset state
int resetAll(String command) {
  resetTimer();
  turnAllOff();
  return 1;
}

//set time end to now plus passed seconds
int setTimerPeriod(String periodString) {
  int period = periodString.toInt();
  if (period > 0) {
    startTime = Time.now();
    endTime = startTime + period;
    turnAllOff();
    return checkTimeUpdateLights();
  } else {
    return -1;
  }
}

//set time end to passed timestamp
int setTimerEndTimestamp(String timestampString) {
  int timestamp = timestampString.toInt();
  int start = Time.now();
  if (timestamp > start) {
    startTime = start;
    endTime = timestamp;
    turnAllOff();
    return checkTimeUpdateLights();
  } else {
    return -1;
  }
}

// get the current time vs start and end time as a percentage of completion
int getCurrentPercentage () {
  if (startTime == 0 && endTime == 0) return 0;
  int currentTime = Time.now();
  // usually gets caught before as showing the 100% currently clears timer
  if (currentTime > endTime) return 100;
  int currentPercentage = (int)((((double)currentTime - (double)startTime) / ((double)endTime - (double)startTime)) * 100.00);
  return currentPercentage;
}

// check the current timer state and update lights accordingly
int checkTimeUpdateLights() {
  int currentPercentage = getCurrentPercentage();
  turnAllOff();
  if (currentPercentage >= 100) {
    digitalWrite(led1, HIGH);
    resetTimer();
    // TODO publish event that we have completed
    return 1;
  }
  if (currentPercentage > 75) {
    digitalWrite(led2, HIGH);
  }
  if (currentPercentage > 50) {
    digitalWrite(led3, HIGH);
  }
  if (currentPercentage > 25) {
    digitalWrite(led4, HIGH);
  }
  digitalWrite(led5, HIGH);
  return 1;
}

int getTimeRemaining() {
  if ( endTime == 0 ) return -1;
  int currentTime = Time.now();
  return endTime - currentTime;
}

String getState() {
  if (endTime > 0) return "timing";
  return "idle";
}


void setup() {
  // pin modes
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
  pinMode(led4, OUTPUT);
  pinMode(led5, OUTPUT);

  // cloud functions
  Particle.function("setEnd", setTimerEndTimestamp);
  Particle.function("setPeriod", setTimerPeriod);
  Particle.function("reset", resetAll);
  // TODO, can we somehow get all of these to return in a single value?
  Particle.variable("timeRemaining", getTimeRemaining);
  Particle.variable("percentage", getCurrentPercentage);
  Particle.variable("state", getState);
}


void loop() {
  if(startTime > 0 && endTime > 0) {
    checkTimeUpdateLights();
  }
  delay(1000);
}