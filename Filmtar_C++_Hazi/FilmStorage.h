#ifndef FILMLIST_H_INCLUDED
#define FILMLIST_H_INCLUDED

#include <exception>
#include <sstream>
#include <cstring>
#include "Film.h"
#include "FamilyFilm.h"
#include "DocumentFilm.h"
#include "string5.h"
#include "memtrace.h"

class not_found: public std::exception{
    String msg;
public:
    explicit not_found(const String& s): msg(s){}
};


class FilmStorage{
    Film* film;

public:
    FilmStorage *next, *prev;

    FilmStorage(Film* film = NULL): film(film), next(NULL), prev(NULL){}

    inline Film* getFilm(){ return film; }
    inline void setFilm(Film* val){this->film = val;}

    ~FilmStorage();

};

class FilmList{

    FilmStorage* first;
    size_t size;
    String lastSortAttrib;

public:
    FilmList(): first(NULL), size(0), lastSortAttrib(String("title")){}

    void add(Film* film);

    void get(std::ostream& os, const String& attrib, int value);
    void get(std::ostream& os, const String& attrib, String value);
    void get(std::ostream& os);
    void get(std::ostream& os, size_t i);

    void remove(const String& title);

    void sort(const String& attrib);

    void mod(const String& title, const String& attrib, int value);
    void mod(const String& title, const String& attrib, String value);

    inline size_t getSize() const {return this->size;}

    void swap(FilmStorage* fs1, FilmStorage* fs2);

    ~FilmList(){
        FilmStorage* temp = first;
        while(temp->next != NULL){
            temp = temp->next;
        }
        FilmStorage* temp2 = NULL;
        while(temp != NULL){
            temp2 = temp;
            temp = temp->prev;
            delete temp2;
        }
    }
};

#endif // FILMLIST_H_INCLUDED
