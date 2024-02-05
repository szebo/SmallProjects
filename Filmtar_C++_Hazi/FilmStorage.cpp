#include "FilmStorage.h"
#include "Film.h"
#include "string5.h"
#include <exception>
#include "memtrace.h"

void FilmList::swap(FilmStorage* fs1, FilmStorage* fs2){
    Film* temp = fs1->getFilm();
    fs1->setFilm(fs2->getFilm());
    fs2->setFilm(temp);
}

void FilmList::add(Film* film){
    FilmStorage* fs = new FilmStorage();
    fs->setFilm(film);
    this->size++;

    if(first == NULL)
        first = fs;
    else{
        FilmStorage* i = first;
        while(i->next != NULL){
            i = i->next;
        }
        i->next = fs;
        fs->prev = i;
    }
    this->sort(lastSortAttrib);
}

void FilmList::get(std::ostream& os, const String& attrib, int value){
    for(FilmStorage* i = this->first; i != NULL; i = i->next){
        if(attrib == "length"){
            if(i->getFilm()->getLength() == value){
                i->getFilm()->print(os);
            }
        }
        else if(attrib == "year"){
            if(i->getFilm()->getYear() == value){
                i->getFilm()->print(os);
            }
        }
        else{
            throw not_found(String("Nincs ilyen elem."));
        }
    }
}
void FilmList::get(std::ostream& os, const String& attrib, String value){
    for(FilmStorage* i = this->first; i != NULL; i = i->next){
        if(attrib == "title"){
            if(i->getFilm()->getTitle() == value){
                i->getFilm()->print(os);
            }
        }
        else{
            throw not_found(String("Nincs ilyen elem."));
        }
    }
}

void FilmList::get(std::ostream& os){
    for(FilmStorage* i = first; i != NULL; i = i->next){
        i->getFilm()->print(os);
        os << std::endl;
    }
}

void FilmList::get(std::ostream& os, size_t i){
    if(i >= this->getSize()) throw std::out_of_range("Nincs ennyi elem!");
    FilmStorage* temp = this->first;
    for(size_t k = 0; k < i; k++){
        temp = temp->next;
    }
    temp->getFilm()->print(os);
}

void FilmList::remove(const String& title){
    FilmStorage* temp = NULL;
    for(FilmStorage* i = this->first; i != NULL; i = i->next){
        if(i->getFilm()->getTitle() == title){
            if(i == first){
                first = i->next;
                first->prev = NULL;
                first->next->prev = first;
            }
            else if(i->next == NULL){
                temp->next = NULL;
            }
            else{
                temp->next = i->next;
                temp->next->prev = temp;
            }
            delete i;
            size--;
            return;
        }
        temp = i;
    }
    throw not_found(String("Nincs ilyen elem."));
}

void FilmList::sort(const String& attrib){
    FilmStorage* temp = first;
    lastSortAttrib = attrib;
    for(int i = size-1; i > 0; i--){
        for(int j = 0; j < i; j++){
            if(attrib == "title"){
                if(temp->next->getFilm()->getTitle() < temp->getFilm()->getTitle()){
                    swap(temp, temp->next);
                }
            }
            else if(attrib == "length"){
                if(temp->next->getFilm()->getLength() < temp->getFilm()->getLength()){
                    swap(temp, temp->next);
                }
            }
            else if(attrib == "year"){
                if(temp->next->getFilm()->getYear() < temp->getFilm()->getYear()){
                    swap(temp, temp->next);
                }
            }
            temp = temp->next;
        }
        temp = first;
    }
}

void FilmList::mod(const String& title, const String& attrib, int value){
    for(FilmStorage* i = first; i != NULL; i = i->next){
        if(i->getFilm()->getTitle() == title){
            if(attrib == "length"){
                i->getFilm()->setLength(value);
                break;
            }
            if(attrib == "year"){
                i->getFilm()->setYear(value);
                break;
            }
            if(attrib == "age"){
                i->getFilm()->setAge_res(value);
                break;
            }
        }
    }
    this->sort(attrib);
}

void FilmList::mod(const String& title, const String& attrib, String value){
    for(FilmStorage* i = first; i != NULL; i = i->next){
        if(i->getFilm()->getTitle() == title){
            if(attrib == "title"){
                i->getFilm()->setTitle(value);
                break;
            }
            if(attrib == "desc"){
                i->getFilm()->setDescription(value);
                break;
            }
        }
    }
    this->sort(attrib);
}

FilmStorage::~FilmStorage(){
}
