#include <iostream>
#include <view-matrix.hh>
#include <matrix.hh>
#include <tools.hh>

ViewMatrix::ViewMatrix(int lines, int cols){
    this->_lines = lines;
    this->_cols = cols;
    this->_data = Matrix(lines, cols);
    this->_hasValue = 0;
};

ViewMatrix::ViewMatrix(Matrix &data) {
    this->_data = data;
    this->_lines = data.ligne();
    this->_cols = data.colonne();
    this->_hasValue = 1;
}

void ViewMatrix::display(WINDOW* win, int winWidth, int winHeight, string help){

    /**
     * @var int rowSpacing: espacement entre les lignes
     *
     * @var int colSpacing: espacement entre les colonnes
     *
    */

    int rowSpacing = 2;
    int colSpacing = 5;

    wclear(win);

    int totalHeight = this->getTotalHeight(rowSpacing);
    int totalWidth = this->getTotalWidth(colSpacing);

    if(totalHeight > winHeight || totalWidth > winWidth){
        return this->displaySimpleView();
    }

    int marginTop = (unsigned int)(winHeight - totalHeight) / 2;
    int marginLeft = (unsigned int)(winWidth - totalWidth) / 2;

    mvwprintw(win, 2, (winWidth / 2) - (string(help).size() / 2), stringToChar(help));
    if(this->_hasValue){
        for(int i=0; i < this->_lines; i++){
            for(int j=0;j < this->_cols; j++){
                mvwprintw(win, marginTop + rowSpacing*i + i, marginLeft + colSpacing*j + 5*j, "%.1f", this->_data[i][j]);
            }
        }
    }

    else{
        int x, y;
        float tmp;
        for(int i=0; i < this->_lines; i++){
            for(int j=0;j < this->_cols ; j++){
                mvwprintw(win, marginTop  + rowSpacing*i + i, marginLeft + 5*j + colSpacing*j, "_");

                getyx(win, y, x);

                mvcur(y, x, marginTop + i + rowSpacing * i, marginLeft + 5*j + colSpacing*j);

                mvwscanw(win, marginTop + i + rowSpacing * i, marginLeft + 5*j + colSpacing*j, "%f", &tmp);

                this->_data.modifier(i, j, tmp);

                wnoutrefresh(win);

                doupdate();

            }
        }

        this->_hasValue = 1;
    }

    wrefresh(win);
    doupdate();

}

int ViewMatrix::getTotalHeight(int rowSpacing){
    return this->_lines + rowSpacing*(this->_lines - 1);
}

int ViewMatrix::getTotalWidth(int colSpacing){
    return this->_cols*5 + colSpacing * (this->_cols - 1);
}

void ViewMatrix::displaySimpleView(){
    this->getMatrix().afficher();
}

/**
 * Determine l'espacement entre les lignes
 *
 * @param int ch taille du valeur qui sera entrer par l'utilisateur ( en terme d'espace, pas de limite )
 *
 * @param int winWidth largeur du window
 * @param int winHeight longueur du window
 *
 * @return int espace entre les lignes
*/
int ViewMatrix::getRowSpacing(int ch, int winHeight){
    int leltHeight = winHeight - this->_lines*ch;
    return leltHeight / ( ( this->_lines - 1 ) + 2 );
}

/**
 * Determine l'espacement entre les colonnes
*/
int ViewMatrix::getColSpacing(int ch, int winWidth){
    int leftWidth = winWidth - this->_cols*ch;
    return leftWidth / ( ( this->_cols - 1 ) + 2 );
}

/**
 * @return Matrix la matrice li� au ViewMatrix
*/
Matrix ViewMatrix::getMatrix() const {
    return this->_data;
}
