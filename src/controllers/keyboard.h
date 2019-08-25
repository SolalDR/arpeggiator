#include <CapacitiveSensor.h>

#define THRESHOLD 34

bool inputs[] = {false, false, false, false, false, false, false};
/**
 * Mesure la capacité électrique présente sur une broches digitale
 *
 * Principe : Place la broche au 0v, puis compte le nombre de cycles requis avant que la broche ne commute.
 * Ce système tire parti du temps de charge d'un condensateur au travers d'une résistance de forte valeur (>1M ohms).
 *
 * @param measurePin Broche sur laquelle effectuer la mesure
 * @return Valeur comprise entre 0 (= pas de capacitance) et 255 (= "grosse" capacitance)
 *
 * Remarque : une résistance >1M ohms entre la broche et +VCC est obligatoire !
 */
uint8_t touch_measure(uint8_t measurePin){
 
  /* Registre bas-niveau, DDR = port de configuration, PORT = port de sortie, PIN = port d'entrée */
  uint8_t xport = digitalPinToPort(measurePin);
  volatile uint8_t *ddr = portModeRegister(xport);
  volatile uint8_t *port = portOutputRegister(xport);
  volatile uint8_t *pin = portInputRegister(xport);
 
  /* Résultat de la mesure, Bitmask de la broche à mesurer */
  uint8_t cycles, bitmask = digitalPinToBitMask(measurePin);
 
  /* Décharge la capacité en contact avec la broche */
  *port &= ~(bitmask);     // Place la broche à LOW
  *ddr |= bitmask;         // Place la broche en sortie
  delayMicroseconds(1000); // Attend pour être sur que la capacité est déchargé
 
  /* Place la broche en entrée, sans résistance de pull-up ! */
  /* (La résistance de >1M ohms externe servira de résistance de pull-up) */
  *ddr &= ~(bitmask);
 
  /* Mesure le nombre de cycles CPU requis avant que la broche ne commute */
  for(cycles = 0; cycles < 256; ++cycles){
    if (*pin & bitmask) break; // Si la broche a commuter on quitte la boucle
  }
 
  /* Re-décharge la capacité en contact avec la broche
   * afin d'éviter tout parasitages d'une autre mesure sur une autre broche.
   * Dans le cas contraire il serait impossible de manipuler plus d'une touche "tactile" */
  *port &= ~(bitmask);
  *ddr |= bitmask;
 
  /* Retourne le résultat */
  return cycles;
}

void setupKeyboard() {
  pinMode(4, OUTPUT);
  digitalWrite(4, HIGH);
}

/* loop() */
void loopKeyboard() {
  /* Pour chaque broches de D2 à D11 (10 touches) */
  for (uint8_t i = 2; i < 3; ++i){
 
    Serial.println(touch_measure(i));
    /* Test si la "touche" a été appuyé ou non */
    if(touch_measure(i) > THRESHOLD) {
      if (inputs[0] == false) {
        Serial.println("Click");
        digitalWrite(4, LOW);
        inputs[0] = true;
      }
    } else {
      if (inputs[0] == true) {
        Serial.println("Release");
        digitalWrite(4, HIGH);
        inputs[0] = false;
      }
    }
   
    /* Sépare chaque bits */
  }
  /* Sépare chaque lignes */
  // Serial.println();
  /* Délai no-flood */
  // delay(100);
}


