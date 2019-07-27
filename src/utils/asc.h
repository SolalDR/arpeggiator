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
 * Calcule et ajoute à une passe les notes de la variation 1 ascendante
 */
void addPassNoteVar23ASC(Pass * pass, Melody * melody) {
  int inputRanks[4];

  // Index à partir du quel on compte les 4 rang composant la pass
  int startAt = (melody->inputLength < 5 || pass->rank % 2 == 0) ? 0 : melody->inputLength - 4;

  // On attributs les rangs dans le bon ordre
  for (int i=0; i < 4; i++) {
    inputRanks[i] = startAt + i % melody->inputLength;
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
 * Rempli les paramètre d'une passe en se basant sur son précédent niveau et l'état de la mélodie
 */
void hydratePassASC(Pass * pass, Pass * previousPass, Melody * melody) {
  int rankMax = melody->octaveLength;
  if ((melody->variation == 2 || melody->variation == 3) && melody->inputLength > 4) {
      rankMax *= 2;
  }



  if(
      previousPass->direction == pass->direction
      && previousPass->variation == pass->variation
  ) {
      pass->rank = (previousPass->rank + 1) % rankMax;
  } else {
      pass->rank = 0;
  }


  switch(pass->variation) {
      case 1: addPassNoteVar1ASC(pass, melody); break;
      case 2: addPassNoteVar23ASC(pass, melody); break;
      case 3: addPassNoteVar23ASC(pass, melody); break;
      default: break;
  }
}