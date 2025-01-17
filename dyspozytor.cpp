#include "dyspozytor.h"
#include "utils.h"

Dyspozytor::Dyspozytor() {}

void Dyspozytor::wyslijSygnal1() {
    log("Sygnal 1: Ciezarowka moze odjechac z niepelnym ladunkiem.");
}

void Dyspozytor::wyslijSygnal2() {
    log("Sygnal 2: Pracownicy koncza prace.");
}