/** @file
 * Implementacja klasy odpowiadającej za interakcję z uzytkownikiem
 *
 * @author Filip Bieńkowski 407686
 * @copyright Uniwersytet Warszawski
 */

#include "map_userInterface.h"
#include "map.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h>

#define CHAR_BUFFER 4096

/**
 * @brief Zamienia podany string na odpowiadającą mu wartość int. Funkcja pomocnicza
 * Funkcja zamienia string naint za pomocą funkcji strtol. Funkcja szuka na końcu
 * znaku '\0' aby upewnic sie ze dostala poprawna wartosc numeryczna.
 * Odczytana wartość jest przypisana parametrowi "numerical"
 * @param string [in,out]
 * @param isLastArgument [in]
 * @param numerical [out]
 * @return wartość @p true, jeśli się udało, wartość @p false jeśli wystąpił błąd:
 * wartość za duża, za mała, lub nieoczekiwane znaki.
 */
static bool parseStringToInt(char* string, int *numerical)
{
    errno = 0;
    char *lengthIntLastChar;
    long castCheck = strtol(string, &lengthIntLastChar, 10);

    if (castCheck > INT_MAX || castCheck < INT_MIN) return false;

    if (*lengthIntLastChar != '\0') return false;

    if (errno == ERANGE || errno == EINVAL) return false;

    *numerical = castCheck;
    return true;
}

/**
 * @brief Zamienia podany string na odpowiadającą mu wartość unsigned. Funkcja pomocnicza
 * Funkcja zamienia string na unsigned int za pomocą funkcji strtol. Funkcja
 * szuka na końcu znaku '\0' aby upewnic sie ze dostala poprawna wartosc numeryczna.
 * Odczytana wartość jest przypisana parametrowi "numerical"
 * @param string [in,out]
 * @param isLastArgument [in]
 * @param numerical [out]
 * @return wartość @p true, jeśli się udało, wartość @p false jeśli wystąpił błąd:
 * wartość za duża, za mała, lub nieoczekiwane znaki.
 */
static bool parseStringToUnsigned(char *string, unsigned *numerical)
{
    errno = 0;
    char *lengthIntLastChar;
    long castCheck = strtol(string, &lengthIntLastChar, 10);

    if (castCheck > UINT_MAX || castCheck < 0) return false;

    if (*lengthIntLastChar != '\0') return false;

    if (errno == ERANGE || errno == EINVAL) return false;

    *numerical = castCheck;
    return true;
}

/**
 * @brief Wypisuje komunikat o błędzie. Funkcja pomocnicza
 * Wypisuje komunikat o błędzie na standardowe wyjście diagnostyczne, w formacie
 * "ERROR x", gdzie x to numer linii w której wpisano błędne polecenie.
 * Funkcja pomocnicza używana przez userReadInput
 * @param lineNumber[in]        -Numer linii, w której wpisano błędne polecenie
 * */
static void printErrorMessage(int lineNumber)
{
    fprintf(stderr, "ERROR %d\n", lineNumber);
}

/**
 * @brief Resetuje string. Funkcja pomocnicza
 * Ustawia pierwszy znak w zadanym stringu na NULL, aby uniknąć błędów przy
 * późniejszym odczycie i zapisie.
 * Funkcja pomocnicza używana przez userReadInput
 * @param string[in,out]        -Resetowany string
 * */
static inline void resetString(char *string)
{
    string[0] = '\0';
}

/**
 * @brief Sprawdza czy udało się wczytać całą linię tekstu. Funkcja pomocnicza
 * Funkcja szuka w zadanym stringu znaku końca linii. Jego brak oznacza że
 * nie wszystkie znaki w linii zostały poprawnie wczytane.
 * Funkcja pomocnicza używana przez userReadInput
 * @param string[in]            -Sprawdzany string
 * @return wartość @p true, jeśli znaleziono znak konca linii, wartość @p false
 * w przeciwnym wypadku.
 * */
static inline bool entireLineRead(char *string)
{
    char *success = strchr(string, '\n');
    if (success == NULL)
    {
        return false;
    }
    else
    {
        return true;
    }
}

/**
 * @brief Analizuje string i wywołuje odpowiednią funkcję. Funkcja pomocnicza
 * Funkcja sprawdza czy skłądnia stringa odpowiada któremuś z poprawnych poleceń.
 * Jeżeli tak, to wywoluje je, jeżeli nie to zwraca wartość false będącą
 * informacją o błędzie. String zostanie pofragmentowany.
 * Funkcja pomocnicza używana przez userReadInput
 * @param map[in]                    - Wskaźnik na strukturę zawierającą mapę dróg krajowych
 * @param command[in,out]            - Sprawdzany string
 * @return wartość @p true, jeśli string jest poprawnym poleceniem, ktore zostało
 * poprawnie wykonane. Wartość @p false w przeciwnym wypadku.
 * */
static bool analyzeString(Map *map, char *command)
{
    // line without '\n'
    if (!entireLineRead(command))
    {
        return false;
    }

    // lines starting with '#' and empty lines are ignored
    if (command[0] == '#' || command[0] == '\n')
    {
        return true;
    }

    ADD_ROAD
    REPAIR_ROAD
    GET_ROUTE_DESCRIPTION
    REMOVE_ROAD
    REMOVE_ROUTE
    NEW_AUTO_ROUTE
    EXTEND_ROUTE
    DELIMITER

    char *whichCommand = strtok(command, delimiter);

    if (strcmp(whichCommand, addRoad) == 0) // addRoad
    {
        if (!userAddRoad(map)) return false;
    }
    else if (strcmp(whichCommand, repairRoad) == 0) // repairRoad
    {
        if (!userRepairRoad(map)) return false;
    }
    else if (strcmp(whichCommand, getRouteDescription) == 0) // getRtDescription
    {
        if (!userGetRouteDescription(map)) return false;
    }
    else if (strcmp(whichCommand, removeRoad) == 0) // removeRoad
    {
        if (!userRemoveRoad(map)) return false;
    }
    else if (strcmp(whichCommand, removeRoute) == 0) // removeRoute
    {
        if (!userRemoveRoute(map)) return false;
    }
    else if (strcmp(whichCommand, newAutoRoute) == 0) // userNewAutoRoute
    {
        if (!userNewAutoRoute(map)) return false;
    }
    else if (strcmp(whichCommand, extendRoute) == 0) // userExtendRoute
    {
        if (!userExtendRoute(map)) return false;
    }
    else // makeRoute
    {
        if (!userMakeRoute(map, whichCommand)) return false;
    }

    return true;
}

/**
 * @brief Sprawdza czy otrzymaliśmy za dużo argumentów. Funkcja pomocnicza
 * Funkcja sprawdza czy na końcu podanego stringu, który z założeniem jest
 * ostatnim wyrazem znalezionym w linii wpisanej przez użytkownika, znajduje
 * się znak końca linii. Jeżeli nie, oznacza to że albo wpisano za dużo argumentów,
 * albo ostatni wyraz nie kończy się poprawnym znakiem. Znak konca linii zostaje
 * potem usuniety, poniewaz nie bedzie juz potrzebny
 * Funkcja pomocnicza używana przez wiele funkcji
 * @param argument[in]            - Sprawdzany string, ostatni wyraz polecenia
 * @return wartość @p true, jeśli otrzymaliśmy za dużo argumentów.
 * Wartość @p false w przeciwnym wypadku.
 * */
static bool tooManyArguments(char *argument)
{
    unsigned length = strlen(argument);

    if (*(argument + (length - 1)) != '\n')
    {
        return true;
    }

    else
    {
        *(argument + (length - 1)) = '\0';
        return false;
    }
}

bool userAddRoad(Map *map)
{
    DELIMITER

    char *city1 = strtok(NULL, delimiter);
    char *city2 = strtok(NULL, delimiter);
    char *length = strtok(NULL, delimiter);
    char *year = strtok(NULL, delimiter);

    if (city1 == NULL || city2 == NULL || length == NULL || year == NULL)
    {
        // not enough arguments
        return false;
    }
    if (tooManyArguments(year))
    {
        return false;
    }

    unsigned lengthInt;
    if (!parseStringToUnsigned(length, &lengthInt)) return false;

    int yearInt;
    if (!parseStringToInt(year, &yearInt)) return false;

    if (!addRoad(map, city1, city2, lengthInt, yearInt)) return false;

    return true;
}

bool userGetRouteDescription(Map *map)
{
    DELIMITER

    char *routeId = strtok(NULL, delimiter);

    if (routeId == NULL)
    {
        // not enough arguments
        return false;
    }
    if (tooManyArguments(routeId))
    {
        return false;
    }

    int routeIdInt;
    if (!parseStringToInt(routeId, &routeIdInt)) return false;

    const char *string = getRouteDescription(map, routeIdInt);
    if (string == NULL) return false;

    printf("%s\n", string);
    free((void *)string);

    return true;
}

bool userRepairRoad(Map *map)
{
    DELIMITER

    char *city1 = strtok(NULL, delimiter);
    char *city2 = strtok(NULL, delimiter);
    char *year = strtok(NULL, delimiter);

    if (city1 == NULL || city2 == NULL || year == NULL)
    {
        // not enough arguments
        return false;
    }
    if (tooManyArguments(year))
    {
        return false;
    }

    int yearInt;
    if (!parseStringToInt(year, &yearInt)) return false;

    if (!repairRoad(map, city1, city2, yearInt)) return false;

    return true;
}

bool userMakeRoute(Map *map, char *command)
{
    DELIMITER

    char *routeId = command;

    if (routeId == NULL) return false;

    int routeIdInt;
    if (!parseStringToInt(routeId, &routeIdInt)) return false;

    char *startCity = strtok(NULL, delimiter);
    if (startCity == NULL) return false;

    Route *newRoute = newCustomRoute(map, routeIdInt, startCity);
    if (newRoute == NULL)
    {
        return false;
    }

    bool allRead = false;
    while (!allRead)
    {
        char *length = strtok(NULL, delimiter);
        char *year = strtok(NULL, delimiter);
        char *destination = strtok(NULL, delimiter);

        if (length == NULL || year == NULL || destination == NULL)
        {
            // means syntax error
            return false;
        }

        if (destination[strlen(destination)-1] == '\n')
        {
            // it was last argument
            destination[strlen(destination)-1] = '\0';
            allRead = true;
        }

        unsigned lengthInt;
        if (!parseStringToUnsigned(length, &lengthInt)) return false;

        int yearInt;
        if (!parseStringToInt(year, &yearInt)) return false;

        if (!extendCustomRoute(map, routeIdInt, lengthInt, yearInt, destination))
        {
            return false;
        }
    }

    return true;
}

void userReadInput(Map *map)
{
    bool madeOwnMap = false;

    // if we weren`t given any map, then we make a new one
    if (map == NULL)
    {
        madeOwnMap = true;
        map = newMap();
    }

    int lineNumber = 0;

    char *buffer = malloc(sizeof(char) * CHAR_BUFFER);
    int bufferSize = CHAR_BUFFER;
    resetString(buffer);

    char *command = malloc(sizeof(char) * CHAR_BUFFER);
    int commandSize = CHAR_BUFFER;
    int lastWriteToBufferPosition = 0;
    resetString(command);

    while (fgets(buffer, bufferSize, stdin) != NULL)
    {
        ++lineNumber;
        strcpy(command, buffer);

        while (!entireLineRead(command))
        {
            // command doesn`t fit, so we must increase size
            commandSize += CHAR_BUFFER;
            command = realloc(command, ((sizeof(char)) * commandSize));
            // now we get next part of the line
            resetString(buffer);
            // if we entered this loop, then in worst case the only character
            // not read is '\n'. If we get a null here, that means there is no
            // '\n' in this line, so the file ends abruptly and that is an error
            if (fgets(buffer, bufferSize, stdin) == NULL)
            {
                printErrorMessage(lineNumber);
                free(buffer);
                free(command);
                if (madeOwnMap)
                {
                    deleteMap(map);
                }
                return;
            }
            // we want to overwrite NULL, so -1
            lastWriteToBufferPosition += CHAR_BUFFER - 1;
            strcpy(command + (lastWriteToBufferPosition), buffer);
        }

        if (!analyzeString(map, command))
        {
            printErrorMessage(lineNumber);
        }

        resetString(command);
        resetString(buffer);
        lastWriteToBufferPosition = 0;
    }

    free(buffer);
    free(command);
    // if we had to make our own map, we must delete it as we won`t pass it outside
    if (madeOwnMap)
    {
        deleteMap(map);
    }
}

bool userRemoveRoad(Map *map)
{
    DELIMITER

    char *city1 = strtok(NULL, delimiter);
    char *city2 = strtok(NULL, delimiter);

    if (city1 == NULL || city2 == NULL)
    {
        // not enough arguments
        return false;
    }
    if (tooManyArguments(city2))
    {
        return false;
    }

    if (!removeRoad(map, city1, city2)) return false;

    return true;
}

bool userRemoveRoute(Map *map)
{
    DELIMITER

    char *routeId = strtok(NULL, delimiter);

    if (routeId == NULL)
    {
        // not enough arguments
        return false;
    }
    if (tooManyArguments(routeId))
    {
        return false;
    }

    int routeIdInt;
    if (!parseStringToInt(routeId, &routeIdInt)) return false;

    if (!removeRoute(map, routeIdInt)) return false;

    return true;
}

bool userNewAutoRoute(Map *map)
{
    DELIMITER

    char *routeId = strtok(NULL, delimiter);
    char *city1 = strtok(NULL, delimiter);
    char *city2 = strtok(NULL, delimiter);

    if (routeId == NULL || city1 == NULL || city2 == NULL)
    {
        // not enough arguments
        return false;
    }
    if (tooManyArguments(city2))
    {
        return false;
    }

    int routeIdInt;
    if (!parseStringToInt(routeId, &routeIdInt)) return false;

    if (!newRoute(map, routeIdInt, city1, city2)) return false;

    return true;
}

bool userExtendRoute(Map *map)
{
    DELIMITER

    char *routeId = strtok(NULL, delimiter);
    char *city = strtok(NULL, delimiter);

    if (routeId == NULL || city == NULL)
    {
        // not enough arguments
        return false;
    }
    if (tooManyArguments(city))
    {
        return false;
    }

    int routeIdInt;
    if (!parseStringToInt(routeId, &routeIdInt)) return false;

    if (!extendRoute(map, routeIdInt, city)) return false;

    return true;
}