
#include "pracownik.h"
#include "utils.h"

Pracownik::Pracownik(int id, int masaCegly)
        : id(id), masaCegly(masaCegly) {}
using namespace std;
void Pracownik::pracuj() {
    log("Pracownik " + to_string(id) + " pracuje.");

}
