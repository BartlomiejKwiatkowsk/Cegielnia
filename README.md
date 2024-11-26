Projekt: Cegielnia
Autor: Bartłomiej Kwiatkowski
Nr albumu: 152692

Opis Projektu
W cegielni przy taśmie transportowej pracuje trzech pracowników oznaczonych przez P1, P2 i P3. Pracownicy wrzucają na taśmę cegły o masach odpowiednio 1, 2 i 3 jednostki.

Na końcu taśmy stoi ciężarówka o ładowności C jednostek, którą należy zawsze załadować do pełna. Wszyscy pracownicy starają się układać cegły na taśmie najszybciej jak to możliwe. Taśma może przetransportować w danej chwili maksymalnie K sztuk cegieł. Jednocześnie taśma ma ograniczony udźwig M. Niedopuszczalne jest położenie samych najcięższych cegieł (3K > M).

Cegły „zjeżdżające" z taśmy muszą od razu trafić na ciężarówkę dokładnie w takiej kolejności, w jakiej zostały położone na taśmie. Po zapełnieniu ciężarówki na jej miejsce pojawia się nowa ciężarówka o ładowności C. Łączna liczba ciężarówek wynosi N, każda o ładowności C. Ciężarówki rozwożą cegły i po czasie Ti wracają do cegielni.

Na polecenie dyspozytora:

Sygnał 1: Ciężarówka, która w danym momencie stoi przy taśmie, może odjechać z cegielni z niepełnym ładunkiem.
Sygnał 2: Pracownicy kończą pracę.
Ciężarówki kończą pracę po rozwiezieniu wszystkich wyprodukowanych cegieł.

Cel Projektu
Napisanie programów symulujących działanie dyspozytora, pracowników oraz ciężarówek w cegielni zgodnie z powyższym opisem.

Opis Testów
Test 1: Podstawowe działanie systemu
Cel: Sprawdzenie poprawności załadunku cegieł na taśmę i ciężarówki.
Warunki początkowe:

C = 10 jednostek,
K = 5 cegieł,
M = 15 jednostek masy,
N = 3 ciężarówki,
Ti = 2 sekundy.
Oczekiwany wynik: Wszystkie cegły zostają załadowane zgodnie z kolejnością, bez przekroczenia limitów K i M.
Test 2: Przekroczenie udźwigu taśmy
Cel: Weryfikacja reakcji systemu na przekroczenie masy M.
Warunki początkowe:

C = 10, K = 5, M = 5.
Oczekiwany wynik: System odrzuca cegły przekraczające masę M i wyświetla komunikat ostrzegawczy.
Test 3: Sygnał 1 - Odjazd z niepełnym ładunkiem
Cel: Weryfikacja odjazdu ciężarówki na polecenie dyspozytora.
Warunki początkowe: Generowanie sygnału 1 podczas ładowania.
Oczekiwany wynik: Ciężarówka odjeżdża z aktualnym ładunkiem.

Test 4: Sygnał 2 - Zakończenie pracy
Cel: Weryfikacja zakończenia pracy przez pracowników.
Warunki początkowe: Sygnał 2 po określonym czasie.
Oczekiwany wynik: Pracownicy kończą pracę, a system kończy symulację.

Repozytorium GitHub
Link do repozytorium
