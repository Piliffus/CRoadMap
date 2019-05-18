#ifndef DROGI_MAP_USERINTERFACE_H
#define DROGI_MAP_USERINTERFACE_H

#include <stdbool.h>
#include "map.h"

/**
 * @brief Przyjmuje polecenie od użytkownika
 * Funkcja przyjmuje polecenie ze standardowego wyjścia. Funkcja ignoruje
 * wiersze zaczynające się znakiem '#'. Wypisuje informację o błędzie,
 * jeśli otrzymała niepoprawny parametr lub wywołanie funkcji zakońćzyło się błędem.
 * Komunikat jest wypisywany na standardowe wyjscie diagnostyczne, w formacie
 * "ERROR x", gdzie x to numer linii w której wpisano błędne polecenie. Jeżeli
 * funkcja nie dostanie gotowej mapy, to stworzy własną, pustą mapę.
 * @param map[in,out]       - Wskaźnik na strukturę zawierającą mapę dróg krajowych
 * */
void userReadInput(Map *map);

/**
 * @brief Pozwala użytkownikowi stworzyć nową drogę krajową
 * Funkcja tworzy drogę krajową zgodnie z poleceniem zadanym przez użytkownika,
 * zawartym w podanym stringu.
 * @param map[in]           - Wskaźnik na strukturę zawierającą mapę dróg krajowych
 * @param command[in]       - String zawierający polecenie
 * @return Prawda jeśli wykonanie zakończyło się sukcesem, fałsz w przeciwnym
 * wypadku
 * */
bool userMakeRoute(Map *map, char *command);

/**
 * @brief Pozwala użytkownikowi stworzyć nowy odcinek drogi
 * Funkcja tworzy odcinek drogi zgodnie z poleceniem zadanym przez użytkownika,
 * zawartym w podanym stringu.
 * @param map[in]           - Wskaźnik na strukturę zawierającą mapę dróg krajowych
 * @param command[in]       - String zawierający polecenie
 * @return Prawda jeśli wykonanie zakończyło się sukcesem, fałsz w przeciwnym
 * wypadku
 * */
bool userAddRoad(Map *map, char *command);

/**
 * @brief Pozwala użytkownikowi wyremontować odcinek drogi
 * Funkcja remontuje odcinek drogi zgodnie z poleceniem zadanym przez użytkownika,
 * zawartym w podanym stringu.
 * @param map[in]           - Wskaźnik na strukturę zawierającą mapę dróg krajowych
 * @param command[in]       - String zawierający polecenie
 * @return Prawda jeśli wykonanie zakończyło się sukcesem, fałsz w przeciwnym
 * wypadku
 * */
bool userRepairRoad(Map *map, char *command);

/**
 * @brief Pozwala użytkownikowi uzyskać informację o danym odcinku drogi
 * Funkcja wypisuje informację o drodze krajowej zgodnie z poleceniem zadanym
 * przez użytkownika, zawartym w podanym string.
 * @param map[in]           - Wskaźnik na strukturę zawierającą mapę dróg krajowych
 * @param command[in]       - String zawierający polecenie
 * @return Prawda jeśli wykonanie zakończyło się sukcesem, fałsz w przeciwnym
 * wypadku
 * */
bool userGetRouteDescription(Map *map, char *command);

#endif //DROGI_MAP_USERINTERFACE_H
