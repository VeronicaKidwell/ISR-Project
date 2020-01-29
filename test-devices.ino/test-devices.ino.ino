// loop counter for debug monitor
int loopCnt;
//
// -- Servo connected p pulse-width-modulated single (longer width = brighter)
int ServoPin = 11;
//
// -- force resistor on pin 0
int fsrAnalogPin0 = 0;
int fsrAnalogPin1 = 1;
int fsrAnalogPin2 = 2;
int fsrAnalogPin3 = 3;
int fsrAnalogPin4 = 4;
//
// -- servo min and max values
int forceMin = 0;
int forceMax = 1023;
//
// -- servo min and max values
int serverMin = 40;
int serverMax = 240;
//
// -- noise filter depth
int filterWeight = 5;
int filteredInput = 0;
//
// -- Arduino calls this setup method once when the device initializes
void setup() {
  //
  // -- initialize the serial port to display data on the computer monitor
  Serial.begin(9600);
  while (!Serial) {
    ; // -- wait for serial port to connect. Needed for native USB port only
  }
  //
  // -- send message to debug monitor
  Serial.println("setup");
  //
  // -- setup the LED pin as an output pin  
  pinMode(ServoPin, OUTPUT);
}
//
// -- Arduino calls loop repeatedly
void loop() {
  //
  // -- begin a pass through the loop
  // Serial.print(", loop:" + String( loopCnt ));
  //
  // -- read in analog value from force resisters
  int forceInput0 = analogRead(fsrAnalogPin0);
  int forceInput1 = analogRead(fsrAnalogPin1);
  int forceInput2 = analogRead(fsrAnalogPin2);
  int forceInput3 = analogRead(fsrAnalogPin3);
  int forceInput4 = analogRead(fsrAnalogPin4);
  //
  //
  // Averaging out the input while ignoring values under 50
  int sum = 0;
  int snsrNumber = 0;
  if(forceInput0 < 900)
  {
    sum = forceInput0;
    snsrNumber++;
  }
  if(forceInput1 < 900)
  {
    sum += forceInput1;
    snsrNumber++;
  }
  if(forceInput2 < 900)
  {
    sum += forceInput2;
    snsrNumber++;
  }
  if(forceInput3 < 900)
  {
    sum += forceInput3;
    snsrNumber++;
  }
  if(forceInput4 < 900)
  {
    sum += forceInput4;
    snsrNumber++;
  }
  int averageForce = 1023;
  if(snsrNumber > 0)
    averageForce = sum/snsrNumber;
  //
  // Printing out the averaged value
  Serial.print(forceInput0);
  Serial.print(", ");
  Serial.print(averageForce);
  //
  // -- noise filter
  filteredInput = ((filteredInput * filterWeight) + averageForce)/(filterWeight+1);
  Serial.print(",");
  Serial.print(filteredInput);
  //
  // -- weight the input
  //long filteredInput = ( (long)filteredInput * (long)filteredInput ) / (long)1048576;
  //
  // -- convert reading 0-1023 to output 0-255, and write servo position
  int ServoPosition;
  ServoPosition = map((int)filteredInput, forceMin, forceMax, serverMin, serverMax);
  ServoPosition = constrain( ServoPosition, serverMin, serverMax );
  
  Serial.print(",");
  Serial.print(ServoPosition);
  //
  // LED gets brighter the harder you press
  analogWrite(ServoPin, ServoPosition);
  //
  // -- count the loop pass
  loopCnt += 1;
  //
  // -- monitor new line
  Serial.println("");
}
