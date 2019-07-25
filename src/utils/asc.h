/**
 * Calcule et ajoute à une passe les notes de la variation 1 ascendante
 */
void addPassNoteVar1ASC(Pass * pass, Melody * melody) {
  InputNode * input = 0;
  for (int i=0; i < melody->inputLength; i++) {

    input = melody->getInputAt(i);
    int degree = input->degree;
    pass->addNote( degree, pass->rank + 1 );
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
  }
}