#include <iostream>
#include <matrix.hh>
#include <string>

using namespace std;

Matrix::Matrix(int nLigne, int nColonne): _nLigne(nLigne), _nColonne(nColonne), _valeur(nLigne, vector<float>(nColonne, 0.0f)) {
    for(unsigned int i = 0; i < nLigne; i++){
        for(unsigned int j = 0; j < nColonne; j++){
            _valeur[i][j] = 0;
        }
    }
}

void Matrix::Remplir(){
    for(int i = 0; i< this->_nLigne; i++){
        for (int j = 0; j< this->_nColonne; j++){
            cin >> this->_valeur[i][j];
        }
    }
}

Matrix Matrix::operator+(const Matrix &matrix) const{

    if (this->_nLigne != matrix._nLigne || this->_nColonne != matrix._nColonne) {
        throw string("Les dimensions des matrices ne correspondent pas");
    }

    Matrix resultat(this->_nLigne, this->_nColonne);

    for(int i = 0; i< this->_nLigne; i++){
        for(int j = 0; j< this->_nColonne; j++){
            resultat._valeur[i][j] = this->_valeur[i][j] + matrix._valeur[i][j];
        }
    }

    return resultat;
}

Matrix Matrix::operator*(const float k) const{
    Matrix resultat(this->_nLigne, this->_nColonne);

    for(int i = 0; i< this->_nLigne; i++){
        for(int j = 0; j < this->_nColonne; j++){
            resultat._valeur[i][j]  = k * (this->_valeur[i][j]);
        }
    }

    return resultat;
}

Matrix Matrix::operator-(const Matrix &matrix) const{

    if (this->_nLigne != matrix._nLigne || this->_nColonne != matrix._nColonne) {
        throw string("Les dimensions des matrices ne correspondent pas");
    }

    Matrix resultat(this->_nLigne, this->_nColonne);

    for(int i = 0; i< this->_nLigne; i++){
        for(int j = 0; j< this->_nColonne; j++){
            resultat._valeur[i][j] = this->_valeur[i][j] - matrix._valeur[i][j];
        }
    }

    return resultat;
}

Matrix Matrix::operator*(const Matrix &matrix) const{

    if(this->_nColonne != matrix._nLigne){
        throw string("On ne peut pas effectuer la multiplication");
    }

    Matrix resultat(this->_nLigne, matrix._nColonne);

    for(int i = 0; i< this->_nLigne; i++){
        for(int j= 0; j<matrix._nColonne; j++){
            for(int k = 0; k< this->_nColonne; k++){
                resultat._valeur[i][j] += this->_valeur[i][k] * matrix._valeur[k][j];
            }
        }
    }
    return resultat;

}

void Matrix::afficher() const {
    for (int i = 0; i < _nLigne; i++) {
        for (int j = 0; j < _nColonne; j++) {
            cout << _valeur[i][j] << " ";
        }
        cout << endl;
    }
}

Matrix Matrix::operator=(const ) const{
    Matrix resultat (this->_nLigne, this->_nColonne);

    for (int i = 0; i < _nLigne; i++) {
        for (int j = 0; j < _nColonne; j++) {
            resultat._valeur[i][j] = this->_valeur[i][j];
        }
    }

    return resultat;
}

bool Matrix::operator==(const Matrix &matrix) const {
    if (this->_nLigne != matrix._nLigne || this->_nColonne != matrix._nColonne) {
        return false;
    }

    for (int i = 0; i < this->_nLigne; i++) {
        for (int j = 0; j < this->_nColonne; j++) {
            if (this->_valeur[i][j] != matrix._valeur[i][j]) {
                return false;
            }
        }
    }

    return true;
}

bool Matrix::operator!=(const Matrix &matrix) const{
    return !(*this == matrix);
}

bool Matrix::operator<(const Matrix &matrix) const {

    if (this->_nLigne != matrix._nLigne || this->_nColonne != matrix._nColonne) {
        return false;
    }
    for(int i = 0; i< this->_nLigne; i++){
        for(int j = 0; j< this->_nColonne; j++){
            bool check = (this->_valeur[i][j] < matrix._valeur[i][j]);
                if(check == 0){
                    return false;
            }
        }
    }

    return true;
}

bool Matrix::operator>(const Matrix &matrix) const {
    if (this->_nLigne != matrix._nLigne || this->_nColonne != matrix._nColonne) {
        return false;
    }
    for(int i = 0; i< this->_nLigne; i++){
        for(int j = 0; j< this->_nColonne; j++){
                bool check = (this->_valeur[i][j] > matrix._valeur[i][j]);
                if(check == 0){
                    return false;
                }
        }
    }

    return true;
}

bool Matrix::operator>=(const Matrix &matrix) const {
    return *this > matrix || *this == matrix;
}

bool Matrix::operator<=(const Matrix &matrix) const {
    return *this < matrix || *this == matrix;
}


int Matrix::ligne() const { return this->_nLigne; }
int Matrix::colonne() const { return this->_nColonne; }

vector<float> Matrix::operator[](const int index) const{
    return this->_valeur[index];
}

void Matrix::modifier(int ligne, int colonne, float valeur) {
    this->_valeur[ligne][colonne] = valeur;
}
