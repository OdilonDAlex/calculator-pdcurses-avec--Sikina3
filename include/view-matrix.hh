#ifndef __VIEW_MATRIX__
#define __VIEW_MATRIX__
#include <iostream>
#include <vector>
#include <curses.h>
#include <panel.h>
#include <matrix.hh>

using std::vector;

class ViewMatrix {
    public:
        ViewMatrix(){

        }
        ViewMatrix(int, int);
        ViewMatrix(Matrix &);
        void display(WINDOW* , int, int, string);
        int getTotalHeight(int);
        int getTotalWidth(int);
        void displaySimpleView();
        int getRowSpacing(int, int);
        int getColSpacing(int, int);
        Matrix getMatrix() const;
    private:
        Matrix _data;
        /**
         * apres construction de l'objet 
         * si hasValue == true ( 1 ), alors c'est une affichage 
         * sinon c'est pour une lecture de matrice
        */  
        int _hasValue, _lines, _cols;
};

#endif