#include "Film.h"
#include <string.h>

Film::~Film(){}

void Film::print(std::ostream& os){
    os << this->getTitle() << ", " << this->getLength() << ", " << this->getYear();
}
