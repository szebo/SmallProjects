#ifndef DOCUMENTFILM_H
#define DOCUMENTFILM_H

#include "Film.h"
#include "string5.h"

class DocumentFilm : public Film
{
    public:
        DocumentFilm(const int len, const int year, const String& title, const String& desc): Film(len, title, year), description(desc){}
        DocumentFilm(const DocumentFilm& other);
        ~DocumentFilm();

        inline String& getDescription() {return this->description;}
        inline void setDescription(const String& val){this->description = val;}

        void print(std::ostream& os);

    private:
        String description;
};


#endif // DOCUMENTFILM_H
