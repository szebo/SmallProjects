#include "DocumentFilm.h"

DocumentFilm::DocumentFilm(const DocumentFilm& other){
    this->length = other.length;
    this->title = other.title;
    this->year = other.year;
    this->description = other.description;
}

DocumentFilm::~DocumentFilm(){}

void DocumentFilm::print(std::ostream& os){
    os << this->getTitle() << ", " << this->getLength() << ", " << this->getYear() << ", " << this->getDescription();
}
