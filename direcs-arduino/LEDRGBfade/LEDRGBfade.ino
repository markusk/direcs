// the pins
int ledR1 =  2;
int ledG1 =  3;
int ledB1 =  4;

int ledR2 =  5;
int ledG2 =  6;
int ledB2 =  7;

int ledR3 =  8;
int ledG3 =  9;
int ledB3 = 10;


int pause = 750; // ms

void setup()
{
  // declare pin to be an output:
  pinMode(ledR1, OUTPUT);
  pinMode(ledR2, OUTPUT);
  pinMode(ledR3, OUTPUT);

  pinMode(ledG1, OUTPUT);
  pinMode(ledG2, OUTPUT);
  pinMode(ledG3, OUTPUT);

  pinMode(ledB1, OUTPUT);
  pinMode(ledB2, OUTPUT);
  pinMode(ledB3, OUTPUT);
}


void loop()
{
  digitalWrite(ledR1, HIGH);
  digitalWrite(ledR2, HIGH);
  digitalWrite(ledR3, HIGH);

  digitalWrite(ledG1,  LOW);
  digitalWrite(ledG2,  LOW);
  digitalWrite(ledG3,  LOW);

  digitalWrite(ledB1,  LOW);
  digitalWrite(ledB2,  LOW);
  digitalWrite(ledB3,  LOW);
  delay(pause);


  digitalWrite(ledR1,  LOW);
  digitalWrite(ledR2,  LOW);
  digitalWrite(ledR3,  LOW);

  digitalWrite(ledG1, HIGH);
  digitalWrite(ledG2, HIGH);
  digitalWrite(ledG3, HIGH);

  digitalWrite(ledB1,  LOW);
  digitalWrite(ledB2,  LOW);
  digitalWrite(ledB3,  LOW);
  delay(pause);


  digitalWrite(ledR1,  LOW);
  digitalWrite(ledR2,  LOW);
  digitalWrite(ledR3,  LOW);

  digitalWrite(ledG1,  LOW);
  digitalWrite(ledG2,  LOW);
  digitalWrite(ledG3,  LOW);

  digitalWrite(ledB1, HIGH);
  digitalWrite(ledB2, HIGH);
  digitalWrite(ledB3, HIGH);
  delay(pause);


  digitalWrite(ledR1, HIGH);
  digitalWrite(ledR2, HIGH);
  digitalWrite(ledR3, HIGH);

  digitalWrite(ledG1, HIGH);
  digitalWrite(ledG2, HIGH);
  digitalWrite(ledG3, HIGH);

  digitalWrite(ledB1, HIGH);
  digitalWrite(ledB2, HIGH);
  digitalWrite(ledB3, HIGH);
  delay(pause);
}

