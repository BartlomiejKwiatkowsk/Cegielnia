
#include "ciezarowka.h"
#include "utils.h"

Ciezarowka::Ciezarowka(int ladownosc)
        : ladownosc(ladownosc), aktualnaMasa(0) {}

void Ciezarowka::zaladuj(int masa) {
    if (aktualnaMasa + masa <= ladownosc) {
        aktualnaMasa += masa;
        log("Zaladowano cegle o masie: " + std::to_string(masa));
    } else {
        log("Nie mozna zaladowac cegly - przekroczona ladownosc.");
    }
}

void Ciezarowka::odjedz() {
    log("Ciezarowka odjezdza z masa: " + std::to_string(aktualnaMasa));
    aktualnaMasa = 0; // Resetowanie ładowności po odjeździe
}

bool Ciezarowka::jestPelna() const {
    return aktualnaMasa == ladownosc;
}