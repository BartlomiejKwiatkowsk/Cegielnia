
#ifndef PROJEKTSO_PRACOWNIK_H
#define PROJEKTSO_PRACOWNIK_H

class Pracownik {
public:
    Pracownik(int id, int masaCegly);
    void pracuj();
private:
    int id;
    int masaCegly;
};
#endif //PROJEKTSO_PRACOWNIK_H
