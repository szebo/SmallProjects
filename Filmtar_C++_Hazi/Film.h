#ifndef FILM_H
#define FILM_H

#include "string5.h"

class Film
{
    protected:
        int length;
        String title;
        int year;

    public:
        Film(): length(0), title(String("")), year(0){}
        Film(const int len, const String& title, const int year): length(len), title(title), year(year){}
        Film(const Film& other): length(other.length), title(other.title), year(other.year){}
        virtual ~Film();

/**
*Getters and Setters
*/
        inline int getLength() const { return length; }
        inline void setLength(const int val) { length = val; }
        inline String& getTitle() { return title; }
        inline void setTitle(const String& val){ this->title = val; }
        inline int getYear() { return year; }
        inline void setYear(const int val) { year = val; }

        virtual inline String& getDescription(){return title;}
        virtual inline int getAge_res(){return 0;}
        virtual inline void setDescription(const String& val){}
        virtual inline void setAge_res(const int& val){}

        virtual void print(std::ostream& os);



};
#endif // FILM_H
