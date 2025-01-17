#include <iostream>
#include "pracownik.h"
#include "ciezarowka.h"
#include "dyspozytor.h"
#include "utils.h"

int main() {
    int ladownoscCiezarowki = 10;
    Ciezarowka ciezarowka(ladownoscCiezarowki);
    Dyspozytor dyspozytor;
    Pracownik pracownik1(1, 1);
    Pracownik pracownik2(2, 2);
    Pracownik pracownik3(3, 3);


    pracownik1.pracuj();
    pracownik2.pracuj();
    pracownik3.pracuj();

    ciezarowka.zaladuj(5);// Testowa ładowność
    if (ciezarowka.jestPelna()) {
        ciezarowka.odjedz();
    } else {
        dyspozytor.wyslijSygnal1();
    }

    dyspozytor.wyslijSygnal2();

    return 0;
}
