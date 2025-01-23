#include <iostream>
#include <unistd.h>
#include <signal.h>
#include "pracownik.h"
#include "utils.h"

using namespace std;

extern SharedData* sharedData;

// Zmienna i funkcja zdefiniowane jako static
static volatile sig_atomic_t terminateFlag = 0;

// Obsługa sygnałów dla pracowników
static void handle_signal(int signum) {
    terminateFlag = 1;
}

void pracownik(int id) {
    int masa = id; // P1 = 1, P2 = 2, P3 = 3 jednostki masy

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
        perror("sigaction - pracownik");
        exit(1);
    }

    while (!terminateFlag) {
        sleep(1); // Symulacja pracy

        if (sharedData->endOfWork) {
            break;
        }

        if (!addBrickToBelt(masa)) {
            cout << "Pracownik " << id << ": Taśma pełna, czekam..." << endl;
        } else {
            cout << "Pracownik " << id << ": Dodałem cegłę o masie " << masa
                 << ". Masa taśmy: " << sharedData->masaAktualna
                 << ", Ilość cegieł: " << sharedData->iloscCegiel << endl;
        }
    }

    cout << "Pracownik " << id << ": Kończę pracę." << endl;
}