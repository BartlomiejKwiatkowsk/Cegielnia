#include <iostream>
#include <sys/wait.h>
#include <unistd.h>
#include <vector>
#include <signal.h>
#include "pracownik.h"
#include "ciezarowka.h"
#include "dyspozytor.h"
#include "utils.h"

using namespace std;

// Lista identyfikatorów procesów dzieci
vector<pid_t> child_pids;

// Funkcja czyszcząca zasoby i kończąca wszystkie procesy potomne
void cleanup(int signum) {
    cout << "\nPrzerwanie sygnałem " << signum << ". Czyszczenie zasobów..." << endl;
    // Zabijanie wszystkich procesów potomnych
    for (pid_t pid : child_pids) {
        kill(pid, SIGTERM);
    }
    // Oczekiwanie na zakończenie procesów potomnych
    for (pid_t pid : child_pids) {
        waitpid(pid, nullptr, 0);
    }
    // Czyszczenie pamięci dzielonej i semaforów
    cleanupSharedResources();
    exit(0);
}

int main() {
    // Ustawienie flagi głównego procesu
    isMainProcess = true;

    // Tworzenie plików dla ftok, jeśli nie istnieją
    // Jest to konieczne, ponieważ ftok wymaga istniejących plików
    system("touch ceglarnia_shm");
    system("touch ceglarnia_sem");

    // Ustawianie obsługi sygnałów (tylko dla procesu rodzica)
    struct sigaction sa;
    sa.sa_handler = cleanup;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;

    if (sigaction(SIGINT, &sa, nullptr) == -1) {
        perror("sigaction - SIGINT");
        return 1;
    }
    if (sigaction(SIGTERM, &sa, nullptr) == -1) {
        perror("sigaction - SIGTERM");
        return 1;
    }
    if (sigaction(SIGTSTP, &sa, nullptr) == -1) { // Obsługa Ctrl+Z
        perror("sigaction - SIGTSTP");
        return 1;
    }

    // Inicjalizacja pamięci dzielonej i semaforów
    if (!initSharedResources()) {
        cerr << "Nie udało się zainicjalizować zasobów współdzielonych." << endl;
        return 1;
    }

    // Uruchamianie procesów pracowników
    for (int i = 1; i <= 3; ++i) {
        pid_t pid = fork();
        if (pid < 0) {
            perror("fork - pracownik");
            cleanup(SIGINT);
        }
        if (pid == 0) {
            // Proces pracownika
            isMainProcess = false;
            pracownik(i);
            exit(0);
        }
        child_pids.push_back(pid);
    }

    // Uruchamianie procesów ciężarówek
    for (int i = 1; i <= N; ++i) {
        pid_t pid = fork();
        if (pid < 0) {
            perror("fork - ciężarówka");
            cleanup(SIGINT);
        }
        if (pid == 0) {
            // Proces ciężarówki
            isMainProcess = false;
            ciezarowki(i);
            exit(0);
        }
        child_pids.push_back(pid);
    }

    // Uruchamianie procesu dyspozytora
    pid_t pidDispatcher = fork();
    if (pidDispatcher < 0) {
        perror("fork - dyspozytor");
        cleanup(SIGINT);
    }
    if (pidDispatcher == 0) {
        // Proces dyspozytora
        isMainProcess = false;
        dyspozytor();
        exit(0);
    }
    child_pids.push_back(pidDispatcher);

    // Czekanie na zakończenie wszystkich procesów potomnych
    for (pid_t pid : child_pids) {
        waitpid(pid, nullptr, 0);
    }

    // Czyszczenie pamięci dzielonej i semaforów
    cleanupSharedResources();

    cout << "Program zakończony pomyślnie." << endl;
    return 0;
}