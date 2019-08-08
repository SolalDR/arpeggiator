/**
 * Calcule et ajoute à une passe les notes de la variation 1 ascendante
 */
void addPassNoteVar1ASC(Pass * pass, Melody * melody) {
  for (int i=0; i < melody->inputLength; i++) {
    int degree = melody->getInputAt(i)->degree;
    pass->addNote( degree, pass->rank + 1 );
  }
}

/**
 * Calcule et ajoute à une passe les notes des variation 2 et 3 ascendante
 */
void addPassNoteVar23ASC(Pass * pass, Melody * melody) {
  int inputRanks[4];

  // Index à partir du quel on compte les 4 rang composant la pass
  int startAt = (melody->inputLength < 5 || pass->rank % 2 == 0) ? 0 : melody->inputLength - 4;

  // On attributs les rangs dans le bon ordre
  for (int i=0; i < 4; i++) {
    inputRanks[i] = melody->getInputAt(startAt + i % melody->inputLength)->degree;
  }

    // Calcule de l'octave courant
  int octave = (melody->inputLength <= 4)
    ? pass->rank + 1 
    : (pass->rank / 2) + 1;

  // Quel rangs on inverse avec le rang 0
  int switchRank = pass->variation == 2 ? 1 : 2;

  for (int j=0; j<4; j++) {
    switch (j) {
      case 0: pass->addNote( inputRanks[switchRank], octave ); break;
      case 1: pass->addNote( inputRanks[0], octave ); break;
      default: pass->addNote( inputRanks[j], octave ); break;
    }
  }
}

/**
 * Calcule et ajoute à une passe les notes de la variation 4 ascendante
 */
void addPassNoteVar4ASC(Pass * pass, Melody * melody) {
  int inputRanks[3];
  int octaves[3];

  // Premiere note, calculé à partir du rang et du nombre de note
  inputRanks[0] = (pass->rank * 2) % melody->inputLength;
  octaves[0] = (pass->rank * 2) / melody->inputLength + 1;

  // Deuxième note, rajoute 2, gestion de l'octave supplémentaire
  inputRanks[1] = (inputRanks[0] + 2) % melody->inputLength;
  octaves[1] = inputRanks[0] + 2 >= melody->inputLength 
    ? octaves[0] + 1
    : octaves[0];
  
  // Deuxième note, enlève 1, gestion de l'octave supprimé
  inputRanks[2] = inputRanks[1] - 1;
  octaves[2] = octaves[1];
  if (inputRanks[2] < 0) {
    inputRanks[2] = melody->inputLength + inputRanks[2];
    octaves[2] = octaves[1] - 1;
  }

  // Cas particulier si seulement 1 input (les octaves peuvent passer de 2 en 2)
  if(melody->inputLength == 1) {
    inputRanks[0] = 0;
    inputRanks[1] = 0;
    inputRanks[2] = 0;
    octaves[0] = 1;
    octaves[1] = octaves[0] + 2 % (melody->octaveLength);
    octaves[2] = octaves[1] - 1;
    if(octaves[2] == 0) {
      octaves[2] = melody->octaveLength;
    }
  }

  // Cas particulier si seulement 1 octave et 2 notes (override)
  if(melody->octaveLength == 1) {
    octaves[0] = 1;
    octaves[1] = 1;
    octaves[2] = 1;
  }

  for (int i = 0; i<3; i++) {
    pass->addNote(melody->getInputAt(inputRanks[i])->degree, octaves[i]);
  }
}

/**
 * Compte le nombre de pass maximal
 * @todo duplicate content desc.h
 */
int getVar4ASCRankMax(int inputLength, int octaveLength) {
  int head[] = {0, 0};
  int rank = 0;
  int countPass = 0;
  // Serial.println("---");
  // Serial.println(String("Head : ") + head[0] + String(" ") + head[1]);
  while(head[1] < octaveLength) {
    switch(rank) {
      case 0: head[0] += 2; break;
      case 1: head[0] -= 1; break;
      case 2: head[0] += 1; break;
    }

    if (head[0] >= inputLength) {     
      head[0] = head[0] % inputLength;
      head[1] += 1;
    }
    if( head[0] < 0) {
      head[0] = inputLength - head[0];  
      head[1] -= 1;
    }

    // Serial.println(String("Head : ") + head[0] + String(" ") + head[1]);
    if(rank == 2) {
      countPass++;
    }
    rank = (rank + 1) % 3;
  }



  return countPass == 0 ? 1 : countPass;
}

/**
 * Rempli les paramètre d'une passe en se basant sur son précédent niveau et l'état de la mélodie
 */
void hydratePassASC(Pass * pass, Pass * previousPass, Melody * melody) {
  int rankMax = melody->octaveLength;
  if ((melody->variation == 2 || melody->variation == 3) && melody->inputLength > 4) {
    rankMax *= 2;
  }

  if (melody->variation == 4) {
    rankMax = getVar4ASCRankMax(melody->inputLength, melody->octaveLength);
  }

  if (
    previousPass->direction == pass->direction
    && previousPass->variation == pass->variation
  ) {
    pass->rank = (previousPass->rank + 1) % rankMax;
  } else {
    pass->rank = 0;
  }

  switch (pass->variation) {
    case 1: addPassNoteVar1ASC(pass, melody); break;
    case 2: addPassNoteVar23ASC(pass, melody); break;
    case 3: addPassNoteVar23ASC(pass, melody); break;
    case 4: addPassNoteVar4ASC(pass, melody); break;
    default: break;
  }
}
