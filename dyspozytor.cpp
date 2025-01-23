#include <iostream>
#include <signal.h>
#include <unistd.h>
#include "dyspozytor.h"
#include "utils.h"

using namespace std;

// Prototypy funkcji
extern void signalTruckDeparture(int truck_id);
extern void signalEndOfWork();

void dyspozytor() {
    while (true) {
        cout << "\nDyspozytor: Podaj komendę:\n";
        cout << "1: Wypuść ciężarówkę z niepełnym ładunkiem\n";
        cout << "2: Zakończ pracę\n";
        cout << "Wybór: ";

        char command;
        cin >> command;

        if (command == '1') {
            // Wybór ciężarówki do odjazdu z niepełnym ładunkiem
            cout << "Dyspozytor: Podaj numer ciężarówki (1-" << N << "): ";
            int truck_id;
            cin >> truck_id;

            if(truck_id >= 1 && truck_id <= N){
                signalTruckDeparture(truck_id);
            } else {
                cout << "Dyspozytor: Nieprawidłowy numer ciężarówki." << endl;
            }
        }
        else if (command == '2') {
            // Sygnalizacja zakończenia pracy wszystkim procesom
            signalEndOfWork();
            break;
        }
        else {
            cout << "Dyspozytor: Nieznana komenda. Spróbuj ponownie." << endl;
        }
    }
}