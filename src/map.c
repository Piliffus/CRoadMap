#include "map.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/**
 * @brief Struktura przechowująca informacje o mieście.
 */
struct City
{
    /**
     * @brief Nazwa miasta.
     */
    char *name;

    /**
     * @brief Lista łączona zawierająca odcinki drogowe incydentne do tego miasta.
     */
    RoadList *roads;
};

struct CityList
{
    City *this;
    CityList *next;
};

struct Road
{
    City *cityA;
    City *cityB;
    unsigned length;
    int year;
};

/**
 *
 */
struct RoadList
{
    Road *this;
    RoadList *next;
};

/**
 * @brief Struktura drogi krajowej.
 */
struct Route
{
    /**
     * @brief Numer drogi krajowej. Musi zawierać się w przedziale [0, HOW_MANY_ROUTES].
     */
    unsigned routeID;

    /**
     * @brief Tekstowy opis drogi.
     */
    char howTheWayGoes[];
};

/**
 * @brief Główna struktura zawierająca wskaźnik do listy miast i tablicy dróg krajowych.
 * Inicjalizację i usuwanie struktury realizują odpowiednio funkcje newMap() i deleteMap(Map *).
 */
struct Map
{
    /**
     * @brief Lista łączona zawierająca miasta.
     */
    CityList *cities;

    /**
     * @brief Tablica dróg krajowych o stałym rozmiarze (HOW_MANY_ROUTES).
     */
    Route *routes[HOW_MANY_ROUTES];
};

/**
 * @brief Struktura pomocnicza
 */
struct Solution
{
    unsigned totalLength;
    int totalAge;
    char howTheWayGoes[];
};
typedef struct Solution Solution;

/**
 * @brief Znajduje na liście miast węzeł, do którego można dołączyć kolejny.
 * @param map Mapa.
 * @return Ostatni węzeł mapy, lub NULL jeśli mapa jest pusta.
 */
CityList *findEmptySpotForCityList(Map *map);

/**
 * @brief Dodaje nowy węzeł odcinka drogowego do listy odcinków w podanym mieście.
 * @param whereRoad Miasto.
 * @param addedRoad Węzeł listy odcinków drogowych.
 */
void addToRoadList(City *whereRoad, RoadList *addedRoad);

/**
 * @brief Wyszukuje
 * @param map Mapa miast.
 * @param city1 Miasto "z".
 * @param city2 Miasto "do".
 * @return Znaleziony odcinek drogi; NULL, jeśli nie istnieje.
 */
Road *findRoadBetweenCities(Map *map, const char *city1, const char *city2);

void recursionFindWay(City *start, City *finish, unsigned totalLength, int totalAge,
                      char goes[], Solution *solution);

/**
 * Funkcja pomocnicza. Sprawdza, czy odcinek drogi znajduje się w podanej tablicy wskaźników.
 * @param pRoad Poszukiwany odcinek drogowy.
 * @param remove Tablica odcinków drogowych.
 * @param ofRemove Rozmiar tablicy remove.
 * @return true, jeśli odcinek znajduje się na liście, w przeciwnym wypadku false.
 */
bool isOnTheArray(Road *pRoad, Road *remove[], int ofRemove);

Map *newMap(void)
{
    Map *newMap = malloc(sizeof(Map));

    if (newMap != NULL)
    {
        newMap->cities = NULL;
        int i = 0;
        for (; i < HOW_MANY_ROUTES; i++)
        {
            newMap->routes[i] = NULL;
        }
    }

    return newMap;
}

void deleteMap(Map *map)
{
    Road **roadsToRemove = malloc(sizeof(Road*) * 0);
    int sizeOfRemove = 0;

    if (map != NULL)
    {
        int i = 0;
        for (; i < HOW_MANY_ROUTES; i++)
        {
            free(map->routes[i]); // remove Route
        }

        CityList *act = map->cities;
        while (act != NULL)
        {
            RoadList *actRoad = act->this->roads;
            while (actRoad != NULL)
            {
                if (!isOnTheArray(actRoad->this, roadsToRemove, sizeOfRemove))
                {
                    // remember to remove this road later
                    sizeOfRemove++;
                    roadsToRemove = realloc(roadsToRemove,
                            sizeOfRemove * (sizeof(Road *)));
                    roadsToRemove[sizeOfRemove - 1] = actRoad->this;
                }
                RoadList *remove = actRoad;
                actRoad = actRoad->next;
                free(remove); // remove RoadList
            }
            free(act->this); // remove City
            CityList *removeAct = act;
            act = act->next;
            free(removeAct); // remove CityList
        }

        int j = 0;
        for (; j < sizeOfRemove; j++)
        {
            free(roadsToRemove[j]); // remove road
        }

        free(map);
    }
}

bool isOnTheArray(Road *pRoad, Road *remove[], int ofRemove) // helper
{
    int i = 0;
    for(; i < ofRemove; i++)
    {
        if (remove[i] == pRoad)
        {
            return true;
        }
    }
    return false;
}

City *findCity(Map *map, const char *cityName)
{
    CityList *current = map->cities;

    while (current != NULL)
    {
        // if current is not null then it must have `this`

        printf("%s %s\n", cityName, current->this->name);
        if (strcmp(current->this->name, cityName) == 0)
        {
            return current->this;
        }

        current = current->next;
    }

    return NULL;
}

bool isCorrectName(const char *name)  // helper
{
    int i = 0;

    if (name[i] == '\0')
    {
        return false;
    }

    while (name[i] != '\0')
    {
        // We check if the name is positive, because Unicode characters outside
        // the 0..127 range are encoded as two to four bytes with most
        // significant bit on. This causes them to be negative and to fail the
        // name[i] <= 31 check.
        if ((name[i] >= 0 && name[i] <= 31) || name[i] == ';')
        {
            return false;
        }
        else
        {
            i++;
        }
    }

    return true;
}

void addToRoadList(City *whereRoad, RoadList *addedRoad)
{
    RoadList *current = whereRoad->roads;

    if (current == NULL)
    {
        whereRoad->roads = addedRoad;
    }

    else
    {
        RoadList *previous = current;
        while (current != NULL)
        {
            previous = current;
            current = current->next;
        }

        previous->next = addedRoad;
        addedRoad->next = NULL;
    }
}

/**
 * @brief Tworzy nowy odcinek drogowy między podanymi miastami i dodaje go do list odcinków obydwu miast.
 * @param cityA Pierwsze miasto.
 * @param cityB Drugie miasto.
 * @param length Długość odcinka.
 * @param builtYear Rok wybudowania.
 * @return false, jeśli nie udało się zaalokować pamięci; w przeciwnym wypadku true
 */
bool makeNewRoad(City *cityA, City *cityB, unsigned length,
                 int builtYear) // helper, true = ok, false error
{
    RoadList *newNodeA = malloc(sizeof(RoadList));
    RoadList *newNodeB = malloc(sizeof(RoadList));
    Road *road;
    if (newNodeA != NULL && newNodeB != NULL)
    {
        newNodeA->next = newNodeB->next = NULL;
        road = newNodeA->this = newNodeB->this = malloc(sizeof(Road));
        if (newNodeA->this != NULL)
        {
            road->cityA = cityA;
            road->cityB = cityB;
            road->length = length;
            road->year = builtYear;
            addToRoadList(cityA, newNodeA);
            addToRoadList(cityB, newNodeB);
            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        return false;
    }
}

/**
 * Tworzy miasto o zadanej nazwie i dodaje
 * @param map
 * @param name
 * @return
 */
City *makeNewCity(Map *map, const char *name) // helper
{
    CityList *node, *where;
    if (map->cities == NULL)
    {
        node = map->cities = malloc(sizeof(CityList));
    }
    else
    {
        where = findEmptySpotForCityList(map);
        node = where->next = malloc(sizeof(CityList));
    }

    if (node != NULL)
    {
        node->this = malloc(sizeof(City));
        node->next = NULL;
        if (node->this != NULL)
        {
            node->this->name = malloc(sizeof(char) * 0);
            strcpy(node->this->name, name);
            node->this->roads = NULL;

            return node->this; // first node has no this
        }
        else
        {
            return NULL;
        }
    }
    else
    {
        return NULL;
    }
}

CityList *findEmptySpotForCityList(Map *map) // helper
{
    CityList *whereCity = map->cities;
    CityList *act = map->cities;

    while (act != NULL)
    {
        whereCity = act;
        act = act->next;
    }

    return whereCity;
}

bool addRoad(Map *map, const char *city1, const char *city2,
             unsigned length, int builtYear)
{
    City *cityA = NULL;
    City *cityB = NULL;

    if (strcmp(city1, city2) != 0)
    {
        // different names, so it`s ok
        cityA = findCity(map, city1);
        cityB = findCity(map, city2);
    }
    else
    {
        return false;
    }

    if (builtYear == 0)
    {
        return false;
    }

    if (length < 0)
    {
        /* this statement will probably never be true, but could be
        if type of length variable is ever changed */
        return false;
    }

    if (!isCorrectName(city1) || !isCorrectName(city2))
    {
        return false;
    }

    if (findRoadBetweenCities(map, city1, city2) != NULL)
    {
        return false;
    }

    if (cityA == NULL)
    {
        cityA = makeNewCity(map, city1);
        if (cityA == NULL)
        {
            return false;
        }
    }

    if (cityB == NULL)
    {
        cityB = makeNewCity(map, city2);
        if (cityB == NULL)
        {
            return false;
        }
    }

    bool success = makeNewRoad(cityA, cityB, length, builtYear);

    return success;
}

Road *findRoadBetweenCities(Map *map, const char *city1, const char *city2)
{
    City *first = findCity(map, city1);
    City *second = findCity(map, city2);

    if (first == NULL || second == NULL)
    {
        return NULL;
    }

    RoadList *act = first->roads;

    while (act != NULL)
    {
        if (act->this->cityA == second || act->this->cityB == second)
        {
            return act->this;
        }
        else
        {
            act = act->next;
        }
    }

    return NULL;
}

bool repairRoad(Map *map, const char *city1, const char *city2, int repairYear)
{
    if (!isCorrectName(city1) || !isCorrectName(city2))
    {
        return false;
    }

    Road *repairedRoad = findRoadBetweenCities(map, city1, city2);

    if (repairedRoad != NULL)
    {
        if (repairYear < repairedRoad->year)
        {
            return false;
        }
        repairedRoad->year = repairYear;
        return true;
    }
    else
    {
        return false;
    }
}

void recursionFindWay(City *start, City *finish, unsigned totalLength, int totalAge,
                      char goes[], Solution *solution)
{
    strcat(goes, start->name);

    if (start != finish)
    {
        RoadList *act = start->roads;

        while (act != NULL)
        {
            char split[] = "placeholder";
            strcpy(split, goes);
            strcat(split, act->this->length);
            strcat(split, act->this->year);
            recursionFindWay(act->this->cityA == start ?
                             act->this->cityB : act->this->cityA, finish,
                             totalLength + act->this->length,
                             totalAge + act->this->year, split, solution);
        }
    }
    else // we found it
    {
        if (solution == NULL)
        {
            solution = malloc(sizeof(struct Solution));
            if (solution != NULL)
            {
                solution->totalAge = totalAge;
                solution->totalLength = totalLength;
                strcpy(solution->howTheWayGoes, goes);
            }
            else
            {
                return;
            }
        }
        else // we will decide which solution is better
        {
            if (solution->totalLength < totalLength)
            {
                return;
            }
            else if (solution->totalLength == totalLength)
            {
                if (solution->totalAge > totalAge)
                {
                    free(solution);
                    solution = malloc(sizeof(struct Solution));
                    if (solution != NULL)
                    {
                        solution->totalAge = totalAge;
                        solution->totalLength = totalLength;
                        strcpy(solution->howTheWayGoes, goes);
                    }
                    else
                    {
                        return;
                    }
                }
                else
                {
                    return;
                }
            }
            else // solution->totalLength > totalLength
            {
                free(solution);
                solution = malloc(sizeof(struct Solution));
                if (solution != NULL)
                {
                    solution->totalAge = totalAge;
                    solution->totalLength = totalLength;
                    strcpy(solution->howTheWayGoes, goes);
                }
                else
                {
                    return;
                }
            }
        }
    }
}

bool newRoute(Map *map, unsigned routeId, const char *city1, const char *city2)
{
    if (routeId > 999 || routeId < 1)
    {
        return false;
    }

    City *cityA = findCity(map, city1);
    City *cityB = findCity(map, city2);

    if (cityA == NULL || cityB == NULL)
    {
        return false;
    }

    if (map->routes[routeId] == NULL)
    {
        map->routes[routeId] = malloc(sizeof(Route));
        if (map->routes[routeId] != NULL)
        {
            map->routes[routeId]->routeID = routeId;
                int totalLength = 0;
                int totalAge = 0;
                char howRouteGoes[] = "";
                Solution *solution = NULL;
                recursionFindWay(cityA, cityB, totalLength, totalAge, howRouteGoes, solution);
                if (solution != NULL)
                {
                    strcpy(solution->howTheWayGoes, map->routes[routeId]->howTheWayGoes);
                }
                else //there is no way there
                {
                    free(map->routes[routeId]);
                    map->routes[routeId] = NULL;
                    return false;
                }

        }
        else
        {
            return false;
        }
    }
    else
    {
        return false;
    }
}