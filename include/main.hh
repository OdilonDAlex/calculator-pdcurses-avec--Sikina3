#ifndef __MAIN__
#define __MAIN__

void modifyHeader(WINDOW*, string);
void displayHelp(PANEL* , int);
void makeAddition(WINDOW *);
Matrix readMatrix(WINDOW *);
int compute(int, WINDOW*);

/**
 * Affiche le resultat des calculs
 *
 * @param WINDOW* le window sur laquelle le resultat sera afficher
 * @param Vector<Matrix> les vecteurs a affichers
 * @param char l'operateur qui a été utiliser pour faire le calcul
 */
void displayResult(WINDOW*, vector<Matrix>, char);

void printError(WINDOW* , const string &);

#endif
