/** @file
 * Interfejs klasy odpowiedzialnej za interakcję z użytkownikiem
 *
 * @author Filip Bieńkowski 407686
 * @copyright Uniwersytet Warszawski
 */

#ifndef DROGI_MAP_USERINTERFACE_H
#define DROGI_MAP_USERINTERFACE_H

#include <stdbool.h>
#include "map.h"

#define ADD_ROAD const char *addRoad = "addRoad";
#define REPAIR_ROAD const char *repairRoad = "repairRoad";
#define GET_ROUTE_DESCRIPTION const char *getRouteDescription = "getRouteDescription";
#define REMOVE_ROAD const char *removeRoad = "removeRoad";
#define DELIMITER const char *delimiter = ";";

/**
 * @brief Przyjmuje polecenie od użytkownika
 * Funkcja przyjmuje polecenie ze standardowego wyjścia. Funkcja ignoruje
 * wiersze zaczynające się znakiem '#'. Wypisuje informację o błędzie,
 * jeśli otrzymała niepoprawny parametr lub wywołanie funkcji zakońćzyło się błędem.
 * Komunikat jest wypisywany na standardowe wyjscie diagnostyczne, w formacie
 * "ERROR x", gdzie x to numer linii w której wpisano błędne polecenie. Jeżeli
 * funkcja nie dostanie gotowej mapy (dostanie NULL) to stworzy własną, pustą
 * mapę, którą na koniec działania usunie.
 * @param map[in,out]       - Wskaźnik na strukturę zawierającą mapę dróg krajowych
 * */
void userReadInput(Map *map);

/**
 * @brief Pozwala użytkownikowi stworzyć nową drogę krajową
 * Funkcja tworzy drogę krajową zgodnie z poleceniem zadanym przez użytkownika,
 * zawartym w podanym stringu. Funkcja dodaje po kolei kolejne miasta i drogi
 * do drogi krajowej, do momentu gdy natrafi na błąd lub koniec polecenia.
 * @param map[in]           - Wskaźnik na strukturę zawierającą mapę dróg krajowych
 * @param command[in]       - String zawierający polecenie
 * @return wartość @p true jeśli wykonanie zakończyło się sukcesem, wartość @p
 * false w przeciwnym wypadku - niepoprawna składnia polecenia lub argumenty
 * */
bool userMakeRoute(Map *map, char *command);

/**
 * @brief Pozwala użytkownikowi stworzyć nowy odcinek drogi
 * Funkcja tworzy odcinek drogi zgodnie z poleceniem zadanym przez użytkownika,
 * zawartym tokenach uzyskanych za pomocą strtok.
 * @param map[in]           - Wskaźnik na strukturę zawierającą mapę dróg krajowych
 * @return wartość @p true jeśli wykonanie zakończyło się sukcesem, wartość @p
 * false w przeciwnym wypadku - niepoprawna składnia polecenia lub argumenty
 * */
bool userAddRoad(Map *map);

/**
 * @brief Pozwala użytkownikowi wyremontować odcinek drogi
 * Funkcja remontuje odcinek drogi zgodnie z poleceniem zadanym przez użytkownika,
 * zawartym w tokenach uzyskanych za pomocą strtok
 * @param map[in]           - Wskaźnik na strukturę zawierającą mapę dróg krajowych
 * @return wartość @p true jeśli wykonanie zakończyło się sukcesem, wartość @p
 * false w przeciwnym wypadku - niepoprawna składnia polecenia lub argumenty
 * */
bool userRepairRoad(Map *map);

/**
 * @brief Pozwala użytkownikowi uzyskać informację o danym odcinku drogi
 * Funkcja wypisuje informację o drodze krajowej zgodnie z poleceniem zadanym
 * przez użytkownika, zawartym w tokenach uzyskanych za pomocą strtok
 * @param map[in]           - Wskaźnik na strukturę zawierającą mapę dróg krajowych
 * @return wartość @p true jeśli wykonanie zakończyło się sukcesem, wartość @p
 * false w przeciwnym wypadku  - niepoprawna składnia polecenia lub argumenty
 * */
bool userGetRouteDescription(Map *map);

/**
 * @brief Pozwala użytkownikowi usunąć drogę pomiędzy dwoma miastami
 * Funkcja usuwa odcinek drogi pomiędzy dwoma miastami zgodnie z poleceniem zadanym
 * przez użytkownika, zawartym w tokenach uzyskanych za pomocą strtok
 * @param map[in]           - Wskaźnik na strukturę zawierającą mapę dróg krajowych
 * @return wartość @p true jeśli wykonanie zakończyło się sukcesem, wartość @p
 * false w przeciwnym wypadku  - niepoprawna składnia polecenia lub argumenty
 * */
bool userRemoveRoad(Map *map);

#endif //DROGI_MAP_USERINTERFACE_H
