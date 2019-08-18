void addPassNoteVar1ASCDESC(Pass * pass, Melody * melody, int rankMax) {
  if(pass->rank < rankMax / 2) {
    addPassNoteVar1ASC(pass, melody);
  } else {
    int tmpPassRank = pass->rank;
    pass->rank -= rankMax / 2;
    addPassNoteVar1DESC(pass, melody);
    pass->rank = tmpPassRank;
  }
}


/**
 * Rempli les paramètre d'une passe en se basant sur son précédent niveau et l'état de la mélodie
 */
void hydratePassASC_DESC(Pass * pass, Pass * previousPass, Melody * melody) {
  int rankMax = melody->octaveLength * 2;

  if (
    previousPass->direction == pass->direction
    && previousPass->variation == pass->variation
  ) {
    pass->rank = (previousPass->rank + 1) % rankMax;
  } else {
    pass->rank = 0;
  }

  switch (pass->variation) {
    case 1: addPassNoteVar1ASCDESC(pass, melody, rankMax); break;
    case 2: addPassNoteVar23ASC(pass, melody); break;
    case 3: addPassNoteVar23ASC(pass, melody); break;
    case 4: addPassNoteVar4ASC(pass, melody); break;
    default: break;
  }
}