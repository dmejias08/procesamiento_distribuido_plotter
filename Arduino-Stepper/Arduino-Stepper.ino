const int IN1z = 13;
const int IN2z = 12;
const int IN3z = 11;
const int IN4z = 10;

const int IN1y = 9;
const int IN2y = 8;
const int IN3y = 7;
const int IN4y = 6;

const int IN1x = 5;
const int IN2x = 4;
const int IN3x = 3;
const int IN4x = 2;

//analog mapping for Leonardo
// const int calibrated = 18;
// const int right      = 19;
// const int up         = 20;
// const int left       = 21;
// const int down       = 22;

//analog mapping for UNO
const int calibrated = 14;
const int right      = 15;
const int up         = 16;
const int left       = 17;
const int down       = 18;


const int stepsPerRotation = 512;
const int stepperdelay = 1000;


const int width = stepsPerRotation*3.5;
const int height = (stepsPerRotation*3.5)*0.8;

int prevCalibrated = 0;

const int bufferSize = 6;
float buffer[bufferSize];
String inputString = "";

int prevReady = 0;
int ready     = 0;
int x = 0;
int y = height;
int z = 0;

void write(int a, int b, int c, int d, int coord){
  if (coord == 0){
    digitalWrite(IN1x,a);
    digitalWrite(IN2x,b);
    digitalWrite(IN3x,c);
    digitalWrite(IN4x,d);
  } else if (coord == 1) {
    digitalWrite(IN1y,a);
    digitalWrite(IN2y,b);
    digitalWrite(IN3y,c);
    digitalWrite(IN4y,d);
  }
  else if (coord == 2) {
    digitalWrite(IN1z,a);
    digitalWrite(IN2z,b);
    digitalWrite(IN3z,c);
    digitalWrite(IN4z,d);
  }
  
}

//CW step
void onestep(int coord){
  write(1,0,0,0, coord);
  delayMicroseconds(stepperdelay);
  write(1,1,0,0, coord);
  delayMicroseconds(stepperdelay);
  write(0,1,0,0, coord);
  delayMicroseconds(stepperdelay);
  write(0,1,1,0, coord);
  delayMicroseconds(stepperdelay);
  write(0,0,1,0, coord);
  delayMicroseconds(stepperdelay);
  write(0,0,1,1, coord);
  delayMicroseconds(stepperdelay);
  write(0,0,0,1, coord);
  delayMicroseconds(stepperdelay);
  write(1,0,0,1, coord);
  delayMicroseconds(stepperdelay);
  write(0,0,0,0, coord);
  delayMicroseconds(stepperdelay);
}

//ACW step
void negonestep(int coord){
  write(0,0,0,1, coord);
  delayMicroseconds(stepperdelay);
  write(0,0,1,1, coord);
  delayMicroseconds(stepperdelay);
  write(0,0,1,0, coord);
  delayMicroseconds(stepperdelay);
  write(0,1,1,0, coord);
  delayMicroseconds(stepperdelay);
  write(0,1,0,0, coord);
  delayMicroseconds(stepperdelay);
  write(1,1,0,0, coord);
  delayMicroseconds(stepperdelay);
  write(1,0,0,0, coord);
  delayMicroseconds(stepperdelay);
  write(1,0,0,1, coord);
  delayMicroseconds(stepperdelay);
  write(0,0,0,0, coord);
  delayMicroseconds(stepperdelay);
}

//CW turn
void step(int number_of_steps, int coord){
  int i;
  i=0;
  while(i<number_of_steps){
    onestep(coord);
    i++;
  }
}

//ACW turn
void negstep(int number_of_steps, int coord){
  int i;
  i=0;
  while(i<number_of_steps){
    negonestep(coord);
    i++;
    if(!digitalRead(calibrated)){
      break;
    }
  }
}

//
void moveUp(int n){
  for (int i =0; i<n;i++){
    if (y>=height & digitalRead(calibrated)){
      return;
    }
    step(1,1);
    y++;
    
    if(!digitalRead(calibrated)){
      break;
    }
  }
  
}

void moveDown(int n){
  for (int i =0; i<n;i++){
    if(y<=0 & digitalRead(calibrated)){
      return;
      
    }
    negstep(1,1);
    y--;
    
    if(!digitalRead(calibrated)){
      break;
    }
  }
  
}

void moveRight(int n){
  for (int i =0; i<n;i++){
    if(x>=width & digitalRead(calibrated)){
      return;
    }
    step(1,0);
    x++;
    
    if(!digitalRead(calibrated)){
      break;
    }
  }
  
}

void moveLeft(int n){
  for (int i =0; i<n;i++){
    if(x<=0 & digitalRead(calibrated)){
      return;
    }
    negstep(1,0);
    x--;
    
    if(!digitalRead(calibrated)){
      break;
    }
  }

  
}

void moveLift(int n){
  for (int i =0; i<n;i++){
    negstep(1,2);
    if(!digitalRead(calibrated)){
      break;
    }
  }
}

void moveLower(int n){
  for (int i =0; i<n;i++){
    step(1,2);
    if(!digitalRead(calibrated)){
      break;
    }
  }
}
void setup(){
  pinMode(IN1x, OUTPUT);
  pinMode(IN2x, OUTPUT);
  pinMode(IN3x, OUTPUT);
  pinMode(IN4x, OUTPUT);

  pinMode(IN1y, OUTPUT);
  pinMode(IN2y, OUTPUT);
  pinMode(IN3y, OUTPUT);
  pinMode(IN4y, OUTPUT);

  pinMode(IN1z, OUTPUT);
  pinMode(IN2z, OUTPUT);
  pinMode(IN3z, OUTPUT);
  pinMode(IN4z, OUTPUT);
  
  pinMode(calibrated, OUTPUT);
  pinMode(right, OUTPUT);
  pinMode(left, OUTPUT);
  pinMode(up, OUTPUT);
  pinMode(down, OUTPUT);

  
  Serial.begin(9600);
  while(digitalRead(calibrated)){
    Serial.print(".");
    delay(500);
  }
  for(int i=0;i<bufferSize;i++){
    buffer[i] = 0.0;
  }
  
  
  Serial.println("Calibration in progress...");
  delay(500);
}

void loop(){
  if(Serial.available()>0){
    //DATA RECEIVING MODE
    //buffer fill and shift
    char inChar = (char)Serial.read();

    inputString += inChar;

    if(inChar == ','){
      float num = inputString.substring(0,inputString.length()-1).toFloat();
      inputString = "";

      for(int i = bufferSize -1; i > 0; i--){
        buffer[i] = buffer[i-1];
      }

      buffer[0] = num;

      Serial.print("Buffer: ");
      for(int i = 0; i < bufferSize; i++) {
        Serial.print(buffer[i]);
        Serial.print(" ");
      }
      Serial.println();
    }

  }
  if (prevCalibrated != digitalRead(calibrated)){
    //CALIBRATION MODE CHANGE
    ready = 0;
    if(digitalRead(calibrated)){
      Serial.println("Calibrated!");
      x = 0;
      y = height;
      moveLift(stepsPerRotation/10);
 
      z = stepsPerRotation/10;
    } else {
      Serial.println("Calibration in progress...");
    }
    prevCalibrated = digitalRead(calibrated);
  }
  else if(digitalRead(calibrated)){
    //PLOTTING MODE
    if(buffer[0] == 255.0){
      ready = 0;
      Serial.println("Plotting!");
      moveDown(height);
      moveLower(z);
      for (int i = 0; i < 5; i++){
        int y_displacement = abs(buffer[i+1]*height - y);
        if(y<buffer[i+1]*height){
          moveUp(y_displacement);
        } else {
          
          moveDown(y_displacement);
        }
        moveRight(width/5);
        
        
      }
      moveDown(height);
      moveLift(z);
      moveUp(abs(y-height));
      moveLeft(x);
      moveLower(z);
      for(int i=0;i<bufferSize;i++){
        buffer[i] = 0.0;
      }
    } else {
      ready = 1;
      if (prevReady != ready){
        Serial.println("Ready to plot!");
      }
    }
    prevReady = ready;
  } else {
    //CALIBRATION MODE
    ready = 0;
    digitalWrite(IN1z,0);
    digitalWrite(IN2z,0);
    digitalWrite(IN3z,0);
    digitalWrite(IN4z,0);
    if(digitalRead(up)){
      moveUp(1);
    } else if(digitalRead(down)) {
      moveDown(1);
    } if(digitalRead(right)) {
      moveRight(1);
    } else if(digitalRead(left)) {
      moveLeft(1);
    }
  }
  
}
