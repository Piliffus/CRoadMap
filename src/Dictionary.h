/** @file
 * Interfejs klasy Dictionary
 *
 * @author Filip Bieńkowski 407686
 * @copyright Uniwersytet Warszawski
 */

#ifndef DROGI_DICTIONARY_H
#define DROGI_DICTIONARY_H

#include "map.h"
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define HASHSIZE 1009

/**
 * @brief Typ przechowywany w słowniku. Domyślnie wskaźnik na City.
 */
typedef struct City* Value;

/**
 * @brief Typ używany jako klucz. Domyślnie String, uzyskiwany z Value->Name
 */
typedef const char* Key;

/**
 * @brief Węzeł wewnętrzny słownika
 */
struct dNode
{
    struct dNode *next;
    Value this;
};
typedef struct dNode dNode;

/**
 * @brief Główna struktura
 */
struct Dictionary
{
    struct dNode *nodes[HASHSIZE];
};
typedef struct Dictionary Dictionary;

/**
 * @brief Stwórz nowy słownik
 * @return Wskaźnik na nowy słownik, lub NULL jeśli nie udało się stworzyć
 */
Dictionary* newDictionary();

/**
 * @brief Usuń dany słownik
 * @param dictionary
 */
void removeDictionary(Dictionary *dictionary);

/**
 * @brief Uzyskaj element dopasowany do danego klucza
 * @param dictionary - Wskaźnik na zadany słownik
 * @param key - Klucz zadanego elementu
 * @return Element jeśli jest, NULL jeśli nie ma takiego elementu
 */
Value get(Dictionary *dictionary, Key key);

/**
 * @brief Włóż dany element do słownika.
 * @param dictionary - Wskaźnik na słownik
 * @param value - Element wkładany. Jego pole "name" pełni rolę klucza
 * @return wartość @p true jeśli udało się włożyć, @p false jeśli nie.
 */
bool put(Dictionary *dictionary, Value value);

#endif //DROGI_DICTIONARY_H
