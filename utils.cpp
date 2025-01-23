#include <iostream>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <cstring>
#include <cstdlib>
#include <errno.h>
#include "utils.h"

using namespace std;

// Definicje globalne - powinny być zdefiniowane tylko raz
const int K = 10;    // Maksymalna ilość cegieł na taśmie
const int M = 20;    // Maksymalna masa cegieł na taśmie
const int C = 20;    // Ładowność ciężarówki
const int N = 2;     // Liczba ciężarówek
const int TI = 5;    // Czas powrotu ciężarówki (w sekundach)

// Definicja wskaźników i identyfikatorów
SharedData* sharedData;
int shmID;
int semID;

// Funkcja inicjalizująca semafory w trybie mutex
bool initSemaphores() {
    union semun {
        int val;
        struct semid_ds* buf;
        unsigned short* array;
    } sem_union;

    // Inicjalizacja semaforu nr 0 jako mutex
    sem_union.val = 1;
    if (semctl(semID, 0, SETVAL, sem_union) == -1) {
        perror("semctl - inicjalizacja semaforu 0");
        return false;
    }
    return true;
}

bool initSharedResources() {
    key_t key = ftok("ceglarnia", 65);
    if (key == -1) {
        perror("ftok");
        return false;
    }

    // Tworzenie segmentu pamięci dzielonej z minimalnymi uprawnieniami
    shmID = shmget(key, sizeof(SharedData), 0600 | IPC_CREAT);
    if (shmID < 0) {
        perror("shmget");
        return false;
    }

    sharedData = (SharedData*)shmat(shmID, nullptr, 0);
    if (sharedData == (void*)-1) {
        perror("shmat");
        return false;
    }

    // Inicjalizacja danych współdzielonych
    sharedData->masaAktualna = 0;
    sharedData->iloscCegiel = 0;
    sharedData->endOfWork = false;
    for(int i = 0; i < N; ++i) {
        sharedData->departPartial[i] = false;
    }

    // Tworzenie i inicjalizacja semaforów z minimalnymi uprawnieniami
    semID = semget(key, 1, 0600 | IPC_CREAT);
    if (semID < 0) {
        perror("semget");
        return false;
    }

    if (!initSemaphores()) {
        return false;
    }

    return true;
}

void cleanupSharedResources() {
    // Odłączenie pamięci dzielonej
    if (shmdt(sharedData) == -1) {
        perror("shmdt");
    }

    // Usunięcie segmentu pamięci dzielonej
    if (shmctl(shmID, IPC_RMID, nullptr) == -1) {
        perror("shmctl");
    }

    // Usunięcie semaforów
    if (semctl(semID, 0, IPC_RMID) == -1) {
        perror("semctl - usunięcie semaforów");
    }
}

bool addBrickToBelt(int masa) {
    struct sembuf p = {0, -1, 0}; // Operacja P (wait)
    struct sembuf v = {0, 1, 0};  // Operacja V (signal)

    if (semop(semID, &p, 1) == -1) {
        perror("semop P - addBrickToBelt");
        return false;
    }

    bool success = false;
    if (sharedData->iloscCegiel < K && sharedData->masaAktualna + masa <= M) {
        sharedData->iloscCegiel++;
        sharedData->masaAktualna += masa;
        success = true;
        cout << "Pracownik dodał cegłę. Masa taśmy: " << sharedData->masaAktualna
             << ", Ilość cegieł: " << sharedData->iloscCegiel << endl;
    }

    if (semop(semID, &v, 1) == -1) {
        perror("semop V - addBrickToBelt");
        return false;
    }

    return success;
}

bool loadTruck(int truck_id) {
    struct sembuf p = {0, -1, 0}; // Operacja P (wait)
    struct sembuf v = {0, 1, 0};  // Operacja V (signal)

    bool success = false;
    const int MIN_LOAD = 10; // Minimalna masa do częściowego załadunku

    if (semop(semID, &p, 1) == -1) {
        perror("semop P - loadTruck");
        return false;
    }

    if (sharedData->masaAktualna >= C) {
        sharedData->masaAktualna -= C;
        sharedData->iloscCegiel = 0; // Załadowane cegły zdjęte z taśmy
        success = true;
    } else if (sharedData->masaAktualna >= MIN_LOAD) {
        sharedData->masaAktualna = 0;
        sharedData->iloscCegiel = 0;  // Rozładuj całkowicie wszystkie cegły
        success = true;
    }

    if (semop(semID, &v, 1) == -1) {
        perror("semop V - loadTruck");
        return false;
    }

    return success;
}

void signalTruckDeparture(int truck_id) {
    if(truck_id >=1 && truck_id <= N){
        struct sembuf p = {0, -1, 0}; // Operacja P (wait)
        struct sembuf v = {0, 1, 0};  // Operacja V (signal)

        if (semop(semID, &p, 1) == -1) {
            perror("semop P - signalTruckDeparture");
            return;
        }

        sharedData->departPartial[truck_id - 1] = true;

        if (semop(semID, &v, 1) == -1) {
            perror("semop V - signalTruckDeparture");
        }

        cout << "Dyspozytor: Sygnalizuję Ciężarówce " << truck_id
             << " odjazd z niepełnym ładunkiem." << endl;
    } else {
        cout << "Dyspozytor: Nieprawidłowy identyfikator ciężarówki." << endl;
    }
}

void signalEndOfWork() {
    cout << "Dyspozytor: Kończymy pracę." << endl;
    sharedData->endOfWork = true;
}