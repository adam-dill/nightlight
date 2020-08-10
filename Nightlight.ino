const int rgbPot = A0;
const int dimPot = A1;
const int durPot = A2;
const int motionPin = 11;
const int lightPin = 12;

const int r = 3;
const int g = 5;
const int b = 6;

const int pwmMin = 0;
const int pwmMax = 255;

int on = 0;
float fade = 0;
float fadeMod = 0.0;

unsigned long lastTime = 0.0;
unsigned long timePassed = 0.0;

void setup() {
  Serial.begin(9600);

  pinMode(rgbPot, INPUT);
  pinMode(dimPot, INPUT);
  pinMode(durPot, INPUT);
  pinMode(motionPin, INPUT);
  pinMode(lightPin, INPUT);
  
  pinMode(r, OUTPUT);
  pinMode(g, OUTPUT);
  pinMode(b, OUTPUT);
}

void loop() {
  int rgbValue = analogRead(rgbPot);
  int dimValue = analogRead(dimPot);
  int durValue = analogRead(durPot);
  int motionValue = digitalRead(motionPin);
  int lightValue = !digitalRead(lightPin);

  unsigned long currentTime = millis();
  unsigned long deltaTime = currentTime - lastTime;
  lastTime = currentTime;
  
  int timeRange = map(durValue, 0, 1023, 0, 100);
  double delayTime = 300000.0 * (timeRange / 100.0);
  if (motionValue && (!lightValue || fade > 0)) {
    on = 1;
    timePassed = 0.0;
  }

  if (on) {
    timePassed += deltaTime;
  }
  
  if (timePassed > delayTime) {
    on = 0;
    timePassed = 0.0;
  }
  
  if (!on && fade > 0) {
    fadeMod = -0.00001;
  } else if (on && fade < 1) {
    fadeMod = 0.0005;
  } else {
    fadeMod = 0.0;
  }
  fade += fadeMod;

  int rValue = 1;
  int gValue = 1;
  int bValue = 1;
  
  int range = map(rgbValue, 0, 1023, pwmMin, pwmMax);
  int rgbRange = map(range, pwmMin, pwmMax, 0, 6); //allows for pot to trigger the different cases
  int dimRange = map(dimValue, 0, 1023, 0, 100);
  float dimPercent = dimRange / 100.0;

  int gUp = map(range, 0, 42, pwmMin, pwmMax); //0 - green value full range rise mapped to 0-42 on pot
  int rDown = map(range, 43, 84, pwmMax, pwmMin); //1 - red value full range rise mapped to 43-84 on pot
  int bUp = map(range, 85, 127, pwmMin, pwmMax); //2 - blue value full range rise mapped to 85-127 on pot
  int gDown = map(range, 128, 169, pwmMax, pwmMin); //3 - green value full range fall mapped to 128-169 on pot
  int rUp = map(range, 170, 212, pwmMin, pwmMax); //4 - red value full range fall mapped to 170-212 on pot
  int bDown = map(range, 213, 255, pwmMax, pwmMin); //5 - blue value full range fall mapped to 213-255 on pot
  switch (rgbRange) {
    case 0:
      analogWrite(g, gUp * dimPercent * fade);
      analogWrite(r, pwmMax * dimPercent * fade); //for full low &/or high just using digitalWrite rather than storing analog value
      digitalWrite(b, LOW);
      break;
    case 1:
      analogWrite(r, rDown * dimPercent * fade);
      analogWrite(g, pwmMax * dimPercent * fade);
      digitalWrite(b, LOW);
      break;
    case 2:
      analogWrite(b, bUp * dimPercent * fade);
      analogWrite(g, pwmMax * dimPercent * fade);
      digitalWrite(r, LOW);
      break;
    case 3:
      analogWrite(g, gDown * dimPercent * fade);
      analogWrite(b, pwmMax * dimPercent * fade);
      digitalWrite(r, LOW);
      break;
    case 4:
      analogWrite(r, rUp * dimPercent * fade);
      analogWrite(b, pwmMax * dimPercent * fade);
      digitalWrite(g, LOW);
      break;
    case 5:
      analogWrite(b, bDown * dimPercent * fade);
      analogWrite(r, pwmMax * dimPercent * fade);
      digitalWrite(g, LOW);
      break;
  }
}
