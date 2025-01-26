#include <iostream>
#include <unistd.h>
#include <signal.h>
#include <sys/sem.h>
#include "ciezarowka.h"
#include "utils.h"

using namespace std;

extern SharedData* sharedData;
extern int semID;
extern bool isMainProcess;

static volatile sig_atomic_t terminateFlag = 0;

// Funkcja obsługująca sygnały dla ciężarówek
static void handle_signal(int signum) {
    terminateFlag = 1;
}

void ciezarowki(int truck_id) {
    // Ignorowanie SIGINT wewnątrz potomnych procesów
    struct sigaction sa_ignore;
    sa_ignore.sa_handler = SIG_IGN;
    sigemptyset(&sa_ignore.sa_mask);
    sa_ignore.sa_flags = 0;
    if (sigaction(SIGINT, &sa_ignore, nullptr) == -1) {
        perror("sigaction - ignore SIGINT");
        exit(1);
    }

    // Ustawianie obsługi sygnałów (SIGTERM)
    struct sigaction sa;
    sa.sa_handler = handle_signal;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    if (sigaction(SIGTERM, &sa, nullptr) == -1) {
        perror("sigaction - ciężarówka");
        exit(1);
    }

    while (!terminateFlag && !sharedData->endOfWork) {
        // Próba załadunku pełnego ładunku
        if (loadTruck(truck_id)) {
            cout << "Ciężarówka " << truck_id << ": Załadowano pełny ładunek, odjeżdżam." << endl;
            sleep(TI); // Symulacja czasu rozładunku
            cout << "Ciężarówka " << truck_id << ": Rozładowano i wracam." << endl;
        }
        else {
            // Taśma nie ma wystarczającej masy do załadunku, czekanie
            cout << "Ciężarówka " << truck_id << ": Czekam na pełny ładunek..." << endl;
            sleep(1); // Czekanie na kolejną szansę załadunku
        }

        // Obsługa odjazdu z niepełnym ładunkiem (jeśli ustalone przez dyspozytora)
        bool shouldDepartPartial = false;
        struct sembuf p = {0, -1, 0}; // Operacja P (wait)
        struct sembuf v = {0, 1, 0};  // Operacja V (signal)

        if(semop(semID, &p, 1) == -1) {
            perror("semop P - ciezarowka partial departure");
            break;
        }

        // Sprawdzanie flagi częściowego odjazdu
        if (truck_id >= 1 && truck_id <= N && sharedData->departPartial[truck_id - 1]) {
            shouldDepartPartial = true;
            sharedData->departPartial[truck_id - 1] = false; // Reset flagi
        }

        if(semop(semID, &v, 1) == -1) {
            perror("semop V - ciezarowka partial departure");
            break;
        }

        if (shouldDepartPartial) {
            cout << "Ciężarówka " << truck_id << ": Sygnalizowany odjazd z niepełnym ładunkiem. Odjeżdżam." << endl;
            sleep(TI); // Symulacja czasu rozładunku
            cout << "Ciężarówka " << truck_id << ": Rozładowano i wracam." << endl;
        }
    }

    cout << "Ciężarówka " << truck_id << ": Kończę pracę." << endl;

    // Odłączenie się od pamięci dzielonej bez usuwania zasobów
    cleanupSharedResources();
    exit(0);
}