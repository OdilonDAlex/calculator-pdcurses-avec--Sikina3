#include <iostream>
#include <tools.hh>
#include <sstream>

using namespace std;

char* stringToChar(string value) {
    char* result = new char[value.size()+1];

    for(unsigned int i=0; i < value.size(); i++ ){ result[i] = value[i] ; }

    result[value.size()] = '\0';

    return result;
}

template <typename type_>
string toString(type_ number){
    ostringstream str_;
    str_ << number;

    return str_.str();
}
