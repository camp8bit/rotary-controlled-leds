int val;
 int encoder0Click = 5;
 int encoder0PinA = 3;
 int encoder0PinB = 4;

//int encoder0Click = 3;
//int encoder0PinA = 5;
//int encoder0PinB = 4;

int encoder0Pos = 0;
int encoder0PinALast = LOW;
int n = LOW;

void setup() {
  pinMode (encoder0Click, INPUT);
  pinMode (encoder0PinA, INPUT);
  pinMode (encoder0PinB, INPUT);
  digitalWrite(encoder0Click, HIGH);  
  Serial.begin (9600);
}

void loop() {
  if (!digitalRead(encoder0Click)) {
    Serial.print ("Click\n");

     while (!digitalRead(encoder0Click)) {
       true;
     }
  }
  
  n = digitalRead(encoder0PinA);
  if ((encoder0PinALast == LOW) && (n == HIGH)) {
    if (digitalRead(encoder0PinB) == LOW) {
      encoder0Pos--;
    } else {
      encoder0Pos++;
    }
    Serial.print (encoder0Pos);
    Serial.print ("\n");
  }
  encoder0PinALast = n;
}

