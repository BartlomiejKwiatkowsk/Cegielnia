
#ifndef PROJEKTSO_CIEZAROWKA_H
#define PROJEKTSO_CIEZAROWKA_H

class Ciezarowka {
public:
    Ciezarowka(int ladownosc);
    void zaladuj(int masa);
    void odjedz();
    bool jestPelna() const;
private:
    int ladownosc;
    int aktualnaMasa;
};
#endif //PROJEKTSO_CIEZAROWKA_H
