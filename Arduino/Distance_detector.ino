#include <LiquidCrystal.h>
LiquidCrystal lcd( 8, 9, 4, 5, 6, 7 );



#define MAX_RANGE  1000 //max range of ultrasonic sensor

const int echoPin = 2;
const int trigPin = 3;
const int buzzPin = 12;
int redPin = A1;
int greenPin = A2;
int range = 80; // distance that triggers buzzer, input value in serial monitor or edit code manually
int red;
int green;
int x;
float freq;
float duration, distance;

void setup(){
  Serial.begin(9600);
  lcd.begin(16, 2); // set up the LCD's number of columns and rows
  pinMode(trigPin, OUTPUT); 
  pinMode(echoPin, INPUT); 
  pinMode(buzzPin, OUTPUT);  
  pinMode(redPin, OUTPUT);              
  pinMode(greenPin, OUTPUT);
  Serial.println("Enter range:"); // prints prompt to enter range value
}

void loop()
{
  x = analogRead(0);
    if(x == 132){
      range++;

    }
    if(x == 309){
      range--;
    }
    
  // measure distance from ultrasonic sensor
  distance = measureDistance(); 

    // changes LED from green to red as distance gets closer to 20cm
    if((distance < range) && (distance > 20)){ 
      //red= map(distance, range, 20 , 0, 255);
      //green = map(distance, range, 20, 255, 0);
      setColour(255, 165);
      tone(buzzPin, 150); 
      lcd.setCursor(0,0); //set cursor to the start of the first line
      lcd.print("Back up         ");
      
    }
    //when distance is less then 20cm set LED red
      else if(distance < 20){ 
        setColour(255, 0);
        tone(buzzPin, 200); 
        lcd.setCursor(0,0); //set cursor to the start of the first line
        lcd.print("TOO CLOSE!      ");
        
      }
    else{
    noTone(buzzPin);
    setColour(0, 255);
    lcd.setCursor(0,0); //set cursor to the start of the first line
    lcd.print("Current range:");
    delay(100);
    lcd.print(range);
  }

 delay(200);
} 


float measureDistance(){
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  return duration / 58.0;
}

void setColour(int redValue, int greenValue){
  analogWrite(redPin, redValue);
  analogWrite(greenPin,  greenValue);
}
