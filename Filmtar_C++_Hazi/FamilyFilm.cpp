#include "FamilyFilm.h"
#include "string5.h"

FamilyFilm::FamilyFilm(const FamilyFilm& other){
    this->length = other.length;
    this->title = other.title;
    this->year = other.year;
    this->age_res = other.age_res;
}

FamilyFilm::~FamilyFilm(){

}

void FamilyFilm::print(std::ostream& os){
    os << this->getTitle() << ", " << this->getLength() << ", " << this->getYear() << ", " << this->getAge_res();
}
