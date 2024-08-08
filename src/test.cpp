#include <iostream>
#include <matrix.hh>
#include <test.hh>
#include <string>
#include <test-item.hh>


using namespace std;

vector<TestItem> runTest() {


    string pass = "Reussi";
    string fail = "Echec";
    Matrix A = Matrix(2, 2);
    Matrix B = Matrix(2, 2);
    Matrix D = Matrix(3, 2);

    Matrix C = Matrix(2, 2);

    vector<TestItem> tests;
    /**
    * Remplissage de la matrice A pour le test
    */
    A.modifier(0, 0, 1);
    A.modifier(0, 1, 1);
    A.modifier(1, 0, 1);
    A.modifier(1, 1, 1);

    /**
    * Remplissage de la matrice B pour le test
    */
    B.modifier(0, 0, 1);
    B.modifier(0, 1, 1);
    B.modifier(1, 0, 1);
    B.modifier(1, 1, 1);


    /**
     * ADDITION
     */

    C.modifier(0, 0, 2);
    C.modifier(0, 1, 2);
    C.modifier(1, 0, 2);
    C.modifier(1, 1, 2);

    tests.push_back(TestItem("Test addition: ", assertEqual(A + B, C) ? pass : fail ));

    tests.push_back(TestItem("Test addition resultat faux: ", assertNotEqual(B + C, A) ? pass : fail ));

    tests.push_back(TestItem("Test addition dimension incompatible: ", thisShouldThrowMatrixShapeError(A, D, '+') ? pass : fail ));


    /**
     * SOUSTRACTION
     */

    tests.push_back(TestItem("Test soustraction: ", assertEqual(C - A, B) ? pass : fail ));

    tests.push_back(TestItem("Test soustraction resultat faux: ", assertNotEqual(A - B, C) ? pass : fail ));

    tests.push_back(TestItem("Test soustraction dimension incompatible: ", thisShouldThrowMatrixShapeError(A, D, '-') ? pass : fail ));

     /**
      * MULTIPLICATION
      */

    tests.push_back(TestItem("Test multiplication: ", assertEqual(A * B, C) ? pass : fail ));

    tests.push_back(TestItem("Test multiplication resultat faux: ", assertNotEqual(A * C, B) ? pass : fail ));

    tests.push_back(TestItem("Test multiplication dimension incompatible: ", thisShouldThrowMatrixShapeError(A, D, '*') ? pass : fail ));

     /**
      * COMPARAISON
      */

    tests.push_back(TestItem("Test comparasion strictement superieur: ", assertFalse(C > C) ? pass : fail ));
    tests.push_back(TestItem("Test comparasion strictement inferieur: ", assertTrue(B < C) ? pass : fail ));
    tests.push_back(TestItem("Test comparasion egale: ", assertTrue(B == B) ? pass : fail ));
    tests.push_back(TestItem("Test comparasion differents: ", assertTrue(C != B) ? pass : fail ));
    tests.push_back(TestItem("Test comparasion superieur ou egale: ", assertTrue(B >= B) ? pass : fail ));
    tests.push_back(TestItem("Test comparasion inferieur ou egale: ", assertTrue(B <= C) ? pass : fail ));
    tests.push_back(TestItem("Test comparasion inferieur ou egale faux: ", assertFalse(C <= B) ? pass : fail ));
    tests.push_back(TestItem("Test comparasion inferieur ou egale faux: ", assertFalse(C <= B) ? pass : fail ));

    return tests;
}

bool thisShouldThrowMatrixShapeError(Matrix A, Matrix B, char operator_) {

    try {

        if(operator_ == '+'){
            A + B;
        }
        else if(operator_ == '-'){
            A - B;
        }
        else if(operator_ == '*'){
            A * B;
        }
        return false;
    }
    catch(const string &error){
        return true;
    }
}

template <typename T>
bool assertTrue(T conditions){
    if(conditions){
        return true;
    }
    return false;
}

template <typename T>
bool assertFalse(T conditions){
    if(conditions){
        return false;
    }
    return true;
}

template <typename T>
bool assertEqual(T a, T b){
    if(a == b){
        return true;
    }
    return false;
}

template <typename T>
bool assertNotEqual(T a, T b){
    return !assertEqual(a, b);
}
