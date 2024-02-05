#ifndef STRING_H
#define STRING_H

#include <iostream>

///Az 5.laboron készített string osztály deklarációi, és rövid függvény definiciói

class String {
    char *pData;
    size_t len;
public:

    void printDbg(const char *txt = "") const {
        std::cout << txt << "[" << len << "], "
                  << (pData ? pData : "(NULL)") << '|' << std::endl;
    }


    String() :pData(0), len(0) {
        pData = new char[1];
        pData[0] = '\0';
        len = 0;
    }

    String(const char c);
    String(const char* s);
    String(const String& s);

    ~String();


    size_t size() const { return len; }


    const char* c_str() const { return pData;}


    String& operator=(const String& s);

    String operator+(const String& s) const;
    String operator+(const char c) const;

    bool operator==(const String& s) const;
    bool operator<(const String& s) const;
    bool operator>(const String& s) const;
    bool operator==(const char* s) const;
    bool operator!=(const char* s) const;
    //bool operator<(const char* s) const;
    //bool operator>(const char* s) const;

    char& operator[](size_t i);

};

String operator+(const char c, const String& s);

std::ostream& operator<<(std::ostream& os, const String& s);



#endif
