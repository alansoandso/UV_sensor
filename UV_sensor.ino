/*
#######################################################################

HARDWARE NOTE: Anodes driven direct from pins 7, 6, 5 .....
               Cathodes through a resistor (220R) to analogue inputs A0, A1, ....
               Analogue inputs can be reprogrammed to be digital outputs

#######################################################################
*/

// LED
byte anodePin   = 12; // D12 
byte cathodePin = 14; // A0

void setup() { 
  //start serial with baud rate 38400
  Serial.begin(9600);          
  
  _SFR_IO8(0x35) |= 0x10;   // global disable pull up resistors

  setupLED(anodePin, cathodePin);
}

void loop () {
  int lightLevel;
  
  lightLevel = readLED(anodePin, cathodePin);
  Serial.println(lightLevel, DEC);
}

void setupLED(byte anodePin, byte cathodePin) {
  digitalWrite(anodePin, LOW); // ensure pins go low immediatly after initilising them to outputs
  pinMode(anodePin, OUTPUT);   // declare the anode pins as outputs
  pinMode(cathodePin, OUTPUT);    // Enable cathode pins as outputs
  digitalWrite(cathodePin, LOW);  // Turn ON LED
}

int readLED(byte anodePin, byte cathodePin) {
  // 256+ - UV detected
  // 516+ - 
  int result;
 
  // LEDs on
  digitalWrite(anodePin, HIGH);   // 1 - LED On
   
  // charge up LED as a capacitor. cathode = HIGH, anode = LOW
  digitalWrite(anodePin, LOW);    // 2
  digitalWrite(cathodePin, HIGH); // 3

  // Put cathode pins into measuring state (analogue input)
  pinMode(cathodePin, INPUT);

  // Take a reading of the voltage level on the inputs to get a referance level before discharge
  result = analogRead(0);  // 4 - Store the reference level
  delay(40);  // LED discharge over time or by photon intergration

  // Read the sensors after discharge to measure the incedent light
  result -= analogRead(0);     // 5 - subtract current reading from the reference to give the drop

  // by discharging the LED immediately the charge on the A/D input is removed
  pinMode(cathodePin, OUTPUT);   
  digitalWrite(cathodePin, LOW); // 6 - discharge

  // Ignore false positives
  if(result < 255) result = 0;
  return(result);
}
