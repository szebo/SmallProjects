#ifndef FAMILYFILM_H
#define FAMILYFILM_H

#include "Film.h"
#include "string5.h"


class FamilyFilm : public Film
{
    int age_res;

    public:
        FamilyFilm(const int len, const int year, const String& title, const int age): Film(len, title, year), age_res(age){}
        FamilyFilm(const FamilyFilm& other);
        ~FamilyFilm();

        inline int getAge_res() { return age_res; }
        inline void setAge_res(const int& val) { age_res = val; }

        void print(std::ostream& os);
};

#endif // FAMILYFILM_H
