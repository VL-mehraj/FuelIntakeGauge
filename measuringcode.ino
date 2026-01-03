/*Design and Fabrication of fuel intake gauge for fuel storage in vehicles
  Arduino Uno program*/
// LiquidCrystal - Version: Latest
#include <LiquidCrystal.h>
//Pin declaration for each component
unsigned char flowsensor = 2;
unsigned char buttonPin = 7;
int potPin = A3;
LiquidCrystal lcd(12, 11, 5, 4, 3, 9);
//Variables
volatile int flow_frequency;
/*flow_frequency stores the number of pulses received from flowsensor*/
float volume = 0.0, tube_volume = 0.2;
/*Volume of tube taken as 200mL*/
float litre_p_min; //flow rate
//Component state variables
int potVal = 0; //potentiometer
int buttonState = 0; //limit switch
//Time variables
unsigned long currentTime;
unsigned long cloopTime;
// Function to add number of pulses received
void flow() {
  flow_frequency++;
}
// Setup function called on every use
void setup() {
  pinMode(flowsensor, INPUT); // declaring flowsensor as input device
  pinMode(buttonPin, INPUT);  // declaring limit switch as input device
  Serial.begin(9600);
  lcd.begin(16, 2);
  attachInterrupt(digitalPinToInterrupt(flowsensor), flow, RISING);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Fuel Flow Meter");
  lcd.setCursor(0, 1);
  lcd.print("Start");
  currentTime = millis();
  cloopTime = currentTime;
}
void loop() {
  potVal = analogRead(potPin);
  /*analogRead() returns a number between 0 and 1023
    potVal store the resistance value of the potentiometer attached to flowsensor*/
  buttonState = digitalRead(buttonPin); // stores the state of the limit switch
  /*if the limit switch is open ,the loop starts*/
  if (buttonState == LOW) {
    currentTime = millis();
    /*Goes into if-loop after every second if the float valve is open
      The maximum valve of readable from the potentiometer is 1023*/
    if (currentTime >= (cloopTime + 1000) && potVal < 1020) {
      cloopTime = currentTime;
      // considers the loop if any input is received from the flow sensor during the last second
      if (flow_frequency != 0) {
        // calculates the flow rate per minute from the number of pulses from flowsensor
        litre_p_min = (flow_frequency * 60) / 550.0;
        // Shows the instantaneous flow rate on LCD screen
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Rate: ");
        lcd.print(litre_p_min);
        lcd.print("L/min");
        /*Calculates the volume from the number of pulses received
          1 Litre = 550 pulse
          1 pulse = (1/550)L */
        volume = volume + (flow_frequency / 550);
        lcd.print("Vol: ");
        lcd.print(volume); // prints the volume of fuel received
        lcd.print(" L");
        flow_frequency = 0;
        /*prints the flow rate on the serial monitor of Arduino IDEfor debugging purposes*/
        Serial.print(litre_p_min, DEC);
        Serial.println("L/min");
      }
      // considers the else loop if no input is received from the flow sensor
      else {
        // considers the loop while float valve is open
        if (potVal < 1020 ) {
          litre_p_min = 0;
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Rate: ");
          lcd.print(litre_p_min);
          lcd.print("L/min");
          lcd.setCursor(0, 1);
          lcd.print("Vol: ");
          lcd.print(volume);  // prints the volume of fuel received
          lcd.print(" L");
        }
      }
    }
    // considers the loop only if float valve is closed
    if (potVal > 1020) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Full Tank Filling");
      lcd.setCursor(0, 1);
      delay(10000); //delays the final result for 10 sec for complete filling of fuel tank
      // prints the volume of fuel received combining the volume of tube upto the fuel dispenser nozzle
      volume = volume + tube_volume;
      lcd.print("Vol: ");
      lcd.print(volume);
      lcd.print(" L");
    }
  }
}





