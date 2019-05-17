#ifndef DROGI_MAP_USERINTERFACE_H
#define DROGI_MAP_USERINTERFACE_H

#include <stdbool.h>

/**
 * @brief Przyjmuje polecenie od użytkownika
 * Funkcja przyjmuje polecenie ze standardowego wyjścia. Funkcja ignoruje
 * wiersze zaczynające się znakiem '#'. Wypisuje informację o błędzie,
 * jeśli otrzymała niepoprawny parametr lub wywołanie funkcji zakońćzyło się błędem
 * */
void userReadInput();

/**
 * @brief Pozwala użytkownikowi stworzyć nową drogę krajową
 * Funkcja tworzy drogę krajową zgodnie z poleceniem zadanym przez użytkownika,
 * zawartym w podanym string. String zostanie podzielony przez funkcję na częsci.
 * @param[in,out] command       - String zawierający polecenie
 * @return Prawda jeśli wykonanie zakończyło się sukcesem, fałsz w przeciwnym
 * wypadku
 * */
bool userMakeRoute(char *command);

/**
 * @brief Pozwala użytkownikowi stworzyć nowy odcinek drogi
 * Funkcja tworzy odcinek drogi zgodnie z poleceniem zadanym przez użytkownika,
 * zawartym w podanym string. String zostanie podzielony przez funkcję na częsci.
 * @param[in,out] command       - String zawierający polecenie
 * @return Prawda jeśli wykonanie zakończyło się sukcesem, fałsz w przeciwnym
 * wypadku
 * */
bool userAddRoad(char *command);

/**
 * @brief Pozwala użytkownikowi wyremontować odcinek drogi
 * Funkcja remontuje odcinek drogi zgodnie z poleceniem zadanym przez użytkownika,
 * zawartym w podanym string. String zostanie podzielony przez funkcję na częsci.
 * @param[in,out] command       - String zawierający polecenie
 * @return Prawda jeśli wykonanie zakończyło się sukcesem, fałsz w przeciwnym
 * wypadku
 * */
bool userRepairRoad(char *command);

/**
 * @brief Pozwala użytkownikowi uzyskać informację o danym odcinku drogi
 * Funkcja wypisuje informację o drodze krajowej zgodnie z poleceniem zadanym
 * przez użytkownika,
 * zawartym w podanym string. String zostanie podzielony przez funkcję na częsci.
 * @param[in,out] command       - String zawierający polecenie
 * @return Prawda jeśli wykonanie zakończyło się sukcesem, fałsz w przeciwnym
 * wypadku
 * */
bool userGetRouteDescription(char *command);

#endif //DROGI_MAP_USERINTERFACE_H
