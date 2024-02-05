#include <iostream>             // Kiíratáshoz
#include <cstring>              // Sztringműveletekhez

#include "string5.h"

///Az 5. laboron készített string osztály definiciói

String::String(const char c){
    len = 1;
    pData = new char[2];
    pData[0] = c;
    pData[1] = '\0';
}

String::String(const char* s){
    len = strlen(s);
    pData = new char[len+1];
    strcpy(pData, s);
    pData[len] = '\0';
}

String::String(const String& s){
    this->len = s.len;
    this->pData = new char[this->len+1];
    strcpy(this->pData, s.pData);
    this->pData[len] = '\0';
}

String::~String(){
    delete[] pData;
}

String& String::operator=(const String& s){
    if(this == &s){}
    else{
        this->len = s.len;
        delete[] this->pData;
        this->pData = new char[len+2];
        strcpy(this->pData, s.pData);
    }

    return *this;
}

char& String::operator[](size_t i){

    if(i >= this->len || i < 0) throw "VNQPVS";
    return this->pData[i];
}

String String::operator+(const String& s) const{
    String temp;
    temp.len = this->len + s.len;
    delete[] temp.pData;
    temp.pData = new char[temp.len+1];
    strcpy(temp.pData, this->pData);
    strcat(temp.pData, s.pData);
    return temp;

}

String String::operator+(const char c) const{
    String temp;
    temp.len = this->len+1;
    delete[] temp.pData;
    temp.pData = new char[temp.len+1];
    strcpy(temp.pData, this->pData);
    temp.pData[temp.len-1] = c;
    temp.pData[temp.len] = '\0';
    return temp;
}

bool String::operator==(const String& s) const{
    return strcmp(this->pData, s.pData) == 0 ? true : false;
}

bool String::operator==(const char* s) const{
    return strcmp(this->pData, s) == 0 ? true : false;
}

bool String::operator<(const String& s) const{
    return strcmp(this->pData, s.pData) < 0 ? true : false;
}

bool String::operator>(const String& s) const{
    return strcmp(this->pData, s.pData) > 0 ? true : false;
}

bool String::operator!=(const char* s) const{
    return strcmp(this->pData, s) != 0 ? true : false;
}

String operator+(const char c, const String& s){
    String temp(c);
    return temp + s;
}

std::ostream& operator<<(std::ostream& os, const String& s){
    os << s.c_str();
    return os;
}
