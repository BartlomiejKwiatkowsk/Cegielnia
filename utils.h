#ifndef PROJEKTSO_UTILS_H
#define PROJEKTSO_UTILS_H

#include <sys/sem.h> // Dodane dla semaforów

// Stałe aplikacji - deklaracje extern
extern const int K;    // Maksymalna ilość cegieł na taśmie
extern const int M;    // Maksymalna masa cegieł na taśmie
extern const int C;    // Ładowność ciężarówki
extern const int N;    // Liczba ciężarówek
extern const int TI;   // Czas powrotu ciężarówki (w sekundach)

// Struktura przechowująca dane współdzielone
struct SharedData {
    int masaAktualna;
    int iloscCegiel;
    bool endOfWork;           // Flaga zakończenia pracy
    bool departPartial[3];    // Flagi do sygnalizacji odjazdu z niepełnym ładunkiem
};

// Deklaracja wskaźnika do danych współdzielonych
extern SharedData* sharedData;

// Deklaracja identyfikatorów zasobów współdzielonych
extern int shmID;
extern int semID;

// Funkcje do inicjalizacji i czyszczenia zasobów
bool initSharedResources();
void cleanupSharedResources();

// Funkcje pomocnicze
bool addBrickToBelt(int masa);
bool loadTruck(int truck_id);
void signalTruckDeparture(int truck_id);
void signalEndOfWork();

#endif //PROJEKTSO_UTILS_H
