#ifndef MATRIX_H_INCLUDED
#define MATRIX_H_INCLUDED

#include <vector>

using namespace std;

class Matrix {
    public:
        Matrix(){

        }
        Matrix(int, int);

        void Remplir();
        void afficher() const;

        Matrix operator+(const Matrix &) const; //operation
        Matrix operator*(const float) const; //multiplication par scalaire
        Matrix operator-(const Matrix &) const; //soustraction
        Matrix operator*(const Matrix &) const; //multiplication entre matrice
        Matrix operator= (const ) const; //affectation d'une matrice

        //comparaison de deux matrices
        bool operator==(const Matrix &) const;
        bool operator!=(const Matrix &) const;

        bool operator> (const Matrix &) const;
        bool operator< (const Matrix &) const;
        bool operator>=(const Matrix &) const;
        bool operator<=(const Matrix &) const;
        int ligne() const;
        int colonne() const;
        vector<float> operator[](const int index) const;
        void modifier(int, int, float);


    private :
        int _nLigne, _nColonne;
        vector<vector<float> > _valeur;

};

#endif // MATRIX_H_INCLUDED
