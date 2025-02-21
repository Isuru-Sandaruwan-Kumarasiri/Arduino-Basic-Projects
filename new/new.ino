int trigPin = 2;      
int echoPin = 4;

const int echo_2 = A0;
const int trig_2 = A1;

const int echo_3 = A2;
const int trig_3 = A3;


const int relayPin=11;

int gap=20;






const int enl = 10;
const int lfmotor = 8;   // Forward motion of Left motor
const int lrmotor = 9;   // Reverse motion of Left motor

const int enr = 5;
const int rfmotor = 6;  // Forward motion of Right motor
const int rrmotor = 7;  // Reverse motion of Right motor

const int bluetoothStatePin = 12;

long  distance_1,distance_2,distance_3;
bool on=false;
bool auto_mode=false;



int maxSpeed = 140;
int turn = 1;

char command;

void setup() {
  pinMode(bluetoothStatePin, INPUT);

  delay(random(500, 2000));  // delay for random time

  Serial.begin(9600);

  pinMode(lfmotor, OUTPUT);  // set Motor pins as output
  pinMode(lrmotor, OUTPUT);
  pinMode(rfmotor, OUTPUT);
  pinMode(rrmotor, OUTPUT);

  pinMode(trigPin, OUTPUT);  // set trig pin as output
  pinMode(echoPin, INPUT);   // set echo pin as input to capture reflected waves
  pinMode(trig_2, OUTPUT);  // Setup for second sensor
  pinMode(echo_2, INPUT);
  pinMode(trig_3, OUTPUT);  // Setup for third sensor
  pinMode(echo_3, INPUT);


  pinMode(relayPin,OUTPUT);   
}

void loop() {

  if (Serial.available() > 0) { 
    command = Serial.read(); 
    Serial.println(command);
    stop();  // Stop the robot before processing any new command

    if(on==true){
       digitalWrite(relayPin,HIGH);
    }

    switch (command) {
      case '1':  // Move forward
        forward();
        break;

      case '2':  // Move backward
        backward();
        break;

      case '3':  // (Optional) Move left - no implementation yet
         left();
        break;

      case '4':  // Turn right
        right();
        break;

      case '5':  // Use ultrasonic sensor to avoid obstacles
        stop();
        break;

      case '6':
        auto_mode=true;
        ultrasonicObstacleAvoidance();
        break;
      case '8':
        on=true;
        break;
      case '7':
        on=false;
      case '9':
        auto_mode=false;
        break;
      default:  // If no valid command, stop the robot
        stop();
        break;
    }
    Serial.print(on);
  }
}

void ultrasonicObstacleAvoidance() {
  while(auto_mode){
      
    distance_1 = returnDistance(echoPin, trigPin);  // Original sensor
    distance_2 = returnDistance(echo_2, trig_2);    // Second sensor
    distance_3 = returnDistance(echo_3, trig_3);
    Serial.print("distance_1=" + String(distance_1) + "\t");
    Serial.print("distance_2=" + String(distance_2) + "\t");
    Serial.println("distance_3=" + String(distance_3));

    delay(50);

   if((distance_1 <gap && distance_2 > gap && distance_3 > gap)){
     stop();
     backward();
     delay(1000);
   }else if( (distance_1 >gap && distance_2 < gap && distance_3 > gap)){
     stop();
     backward();
     left();
   }else if( (distance_1 >gap && distance_2 > gap && distance_3 < gap)){
    stop();
    backward();
    right();
   }else if( (distance_1 <gap && distance_2 <gap && distance_3 > gap)){
    stop();
    backward();
    left();
   }else if( (distance_1 <gap && distance_2 > gap && distance_3 < gap)){

    stop();
    backward();
    right();
   }else if( (distance_1 >gap && distance_2 < gap && distance_3 < gap) || (distance_1 <gap && distance_2 < gap && distance_3 < gap)){

    stop();
    backward();
    backward();
    if((distance_2 >gap ) ){
      left();
    }else{
      right();
    }
    
   }

   forward();
   



      
  }
}




int returnDistance(int echo,int trig){

  digitalWrite(trig, LOW);
  delayMicroseconds(2);
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);
  
  long duration=pulseIn(echo, HIGH);  // receive reflected waves
  int  distance = duration / 58.2;
  return distance;
}



void stop() {
  digitalWrite(rrmotor, LOW);  // Stop right motor
  digitalWrite(rfmotor, LOW);

  digitalWrite(lrmotor, LOW);   // Stop left motor
  digitalWrite(lfmotor, LOW);

  analogWrite(enr, 0);  // Stop speed control for right motor
  analogWrite(enl, 0);  // Stop speed control for left motor

  delay(500);  // Short delay for stabilization
}

void forward() {
  digitalWrite(rrmotor, LOW);   // move forward: right motor
  digitalWrite(rfmotor, HIGH);

  digitalWrite(lrmotor, LOW);    // move forward: left motor
  digitalWrite(lfmotor, HIGH);

  analogWrite(enr, maxSpeed);     // set speed for right motor
  analogWrite(enl, maxSpeed); // set slightly reduced speed for left motor to adjust alignment
}

void backward() {
  digitalWrite(rrmotor, HIGH);  // move backward: right motor
  digitalWrite(rfmotor, LOW);

  digitalWrite(lrmotor, HIGH);   // move backward: left motor
  digitalWrite(lfmotor, LOW);

  analogWrite(enr, maxSpeed);     // set speed for backward motion
  analogWrite(enl, maxSpeed);
  delay(500);
}

void left() {
  digitalWrite(rrmotor, LOW);   // right motor forward
  digitalWrite(rfmotor, HIGH);

  digitalWrite(lrmotor, LOW);    // stop left motor for right turn
  digitalWrite(lfmotor, LOW);

  analogWrite(enr, maxSpeed);     // set speed for right motor
  analogWrite(enl, 0);            // stop left motor for turning
  delay(1500);  // delay for turning time, adjust as necessary
  stop();
}

void right() {
  digitalWrite(rrmotor, LOW);   // right motor forward
  digitalWrite(rfmotor, LOW);

  digitalWrite(lrmotor, LOW);    // stop left motor for right turn
  digitalWrite(lfmotor, HIGH);

  analogWrite(enr,0 );     // set speed for right motor
  analogWrite(enl,maxSpeed);            // stop left motor for turning
  delay(1500);  // delay for turning time, adjust as necessary
  stop();
}
