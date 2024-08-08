#ifndef TEST_HH_INCLUDED
#define TEST_HH_INCLUDED
#include <test-item.hh>

vector<TestItem> runTest();
bool thisShouldThrowMatrixShapeError(Matrix, Matrix, char);

template <typename T>
bool assertTrue(T);

template <typename T>
bool assertFalse(T);

template <typename T>
bool assertEqual(T, T);


template <typename T>
bool assertNotEqual(T, T);

#endif // TEST_HH_INCLUDED
