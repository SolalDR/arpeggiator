bool buttons[] = {false, false, false, false, false, false, false};

// Connecter le cable rouge au ground et les 7 autres aux pins 2 à 8

void setup() {
  // put your setup codehere, to run once:
  Serial.begin(9600);
  pinMode(2, INPUT_PULLUP);
  pinMode(3, INPUT_PULLUP);
  pinMode(4, INPUT_PULLUP);
  pinMode(5, INPUT_PULLUP);
  pinMode(6, INPUT_PULLUP);
  pinMode(7, INPUT_PULLUP);
  pinMode(8, INPUT_PULLUP);
}

void updateButtonValue(int pin, int rank) {
   int value = digitalRead(pin);
   if(!value != buttons[rank]) {
    buttons[rank] = !value;
    if(value == HIGH) {
      Serial.print("Release button "); 
    } else {
      Serial.print("Click button "); 
    } 
    Serial.println(rank);
   }
}

void loop() {
  updateButtonValue(2, 0);
  updateButtonValue(3, 1);
  updateButtonValue(4, 2);  
  updateButtonValue(5, 3);
  updateButtonValue(6, 4);
  updateButtonValue(7, 5);
  updateButtonValue(8, 6);
  delay(200);
}
