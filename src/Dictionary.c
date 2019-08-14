/** @file
 * Implementacja klasy Dictionary oparta o hashtablicę
 *
 * @author Filip Bieńkowski 407686
 * @copyright Uniwersytet Warszawski
 */


#include "Dictionary.h"

/**
 * @brief Funkcja pomocnicza do usuwania podlist słownika
 * @param this - Węzeł do usunięcia, ktorego następniki też zostaną usunięte
 */
static void removeSubList(struct dNode *this)
{
    if (this != NULL)
    {
        removeSubList(this->next);
        free(this);
    }
}

/**
 * @brief Funkcja znajduje hashvalue dla zadanego klucza będącego stringiem
 * @param key - string będący kluczem
 * @return Hashvalue dla zadanego stringa
 */
static inline unsigned hashFor(Key key)
{
    unsigned hash = 0;

    for (; *key != '\0'; key++)
    {
        hash = *key + 31 * hash;
    }

    return hash % HASHSIZE;
}

/**
 * @brief Inicjalizuje pola w nowo utworzonym węźle
 * @param newNode - wskaźnik na nowy węzeł
 * @param value - wartość w węźle
 */
static inline void setFields(dNode *newNode, Value value)
{
    newNode->next = NULL;
    newNode->this = value;
}

Dictionary* newDictionary()
{
    Dictionary *dictionary = malloc(sizeof(Dictionary));
    
    if (dictionary == NULL) return NULL;
    
    for (int i = 0; i < HASHSIZE; ++i)
    {
        dictionary->nodes[i] = NULL;
    }

    return dictionary;
}

void removeDictionary(Dictionary *dictionary)
{
    for (int i = 0; i < HASHSIZE; ++i)
    {
        // Remove all nodes in given sublist
        removeSubList(dictionary->nodes[i]);
    }

    free(dictionary);
}

Value get(Dictionary *dictionary, Key key)
{
    dNode *current = dictionary->nodes[hashFor(key)];

    while (current != NULL)
    {
        if (strcmp(current->this->name, key) == 0)
        {
            return current->this;
        }
        else
        {
            current = current->next;
        }
    }

    // Nothing found for this key
    return NULL;
}

bool put(Dictionary *dictionary, Value value)
{
    Key key = value->name;
    unsigned hashValue = hashFor(key);

    if (dictionary->nodes[hashValue] == NULL)
    {
        // First element
        dictionary->nodes[hashValue] = malloc(sizeof(dNode));
        // Not enough memory
        if (dictionary->nodes[hashValue] == NULL) return false;
        // Set fields
        setFields(dictionary->nodes[hashValue], value);
    }
    else
    {
        // At least second element
        dNode *current = dictionary->nodes[hashValue];
        // Go to the end of the list
        while (current->next != NULL)
        {
            current = current->next;
        }
        // Current->next == NULL
        current->next = malloc(sizeof(dNode));
        // Memory fail
        if (current->next == NULL) return false;
        // Set fields
        setFields(current->next, value);
    }

    return true;
}

