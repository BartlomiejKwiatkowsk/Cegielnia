#include <iostream>
#include <unistd.h>
#include <signal.h>
#include <sys/sem.h>    // Dodane dla semaforów
#include "ciezarowka.h"
#include "utils.h"

using namespace std;

extern SharedData* sharedData;
extern int semID;        // Dodane

// Zmienna i funkcja zdefiniowane jako static
static volatile sig_atomic_t terminateFlag = 0;

// Obsługa sygnałów dla ciężarówek
static void handle_signal(int signum) {
    terminateFlag = 1;
}

void ciezarowki(int truck_id) {
    // Ignorowanie SIGINT w dzieciach za pomocą sigaction
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

    while (!terminateFlag) {
        if (sharedData->endOfWork) {
            break;
        }

        bool shouldDepartPartial = false;

        // Zablokowanie semafora przed odczytem flagi
        struct sembuf p = {0, -1, 0}; // Operacja P (wait)
        struct sembuf v = {0, 1, 0};  // Operacja V (signal)

        if (semop(semID, &p, 1) == -1) {
            perror("semop P - ciezarowka partial departure");
            break;
        }

        // Sprawdzanie flagi odjazdu niepełnego ładunku
        if (truck_id >= 1 && truck_id <= N && sharedData->departPartial[truck_id - 1]) {
            shouldDepartPartial = true;
            sharedData->departPartial[truck_id - 1] = false; // Reset flagi
        }

        if (semop(semID, &v, 1) == -1) {
            perror("semop V - ciezarowka partial departure");
            break;
        }

        if (shouldDepartPartial) {
            // Blokowanie i załadunek niepełnego ładunku
            struct sembuf p_load = {0, -1, 0}; // Operacja P (lock)
            struct sembuf v_load = {0, 1, 0};  // Operacja V (unlock)

            if (semop(semID, &p_load, 1) == -1) {
                perror("semop P - ciezarowka departurePartial");
                break;
            }

            int currentLoad = sharedData->masaAktualna;
            sharedData->masaAktualna = 0;
            sharedData->iloscCegiel = 0;

            if (semop(semID, &v_load, 1) == -1) {
                perror("semop V - ciezarowka departurePartial");
                break;
            }

            cout << "Ciężarówka " << truck_id << ": Załadowano niepełny ładunek ("
                 << currentLoad << " jednostek). Odjeżdżam." << endl;
            sleep(TI); // Symulacja czasu rozwozu
            cout << "Ciężarówka " << truck_id << ": Rozładowano i wracam." << endl;

            continue; // Sprawdzenie kolejnych flag
        }

        // Próba załadunku pełnego ładunku
        if (loadTruck(truck_id)) {
            cout << "Ciężarówka " << truck_id << ": Załadowano pełny ładunek, odjeżdżam." << endl;
            sleep(TI); // Symulacja czasu rozwozu
            cout << "Ciężarówka " << truck_id << ": Rozładowano i wracam." << endl;
        } else {
            // Taśma nie ma wystarczającej masy, czekanie
            cout << "Ciężarówka " << truck_id << ": Taśma ma niewystarczającą masę, czekam..." << endl;
            sleep(1);
        }
    }

    cout << "Ciężarówka " << truck_id << ": Kończę pracę." << endl;
}