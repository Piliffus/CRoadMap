#include "map.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define UNUSED(v) ((void)(v))

/**
   * Struktura przechowująca informacje o mieście.
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

/**
 * Węzeł łączonej listy miast.
 */
struct CityList
{
    /**
     * Miasto, na które wskazuje aktualny węzeł.
     */
    City *this;

    /**
     * Następny węzeł na liście.
     */
    CityList *next;
};

/**
   * Struktura przechowująca informacje o drodze.
   */
struct Road
{
    /**
     * @brief Pierwszy koniec odcinka.
     */
    City *cityA;

    /**
     * @brief Drugi koniec odcinka.
     */
    City *cityB;

    /**
     * @brief Długość odcinka.
     */
    unsigned length;

    /**
     * @brief Rok budowy lub ostatniego remontu.
     */
    int year;
};

/**
 * Węzeł listy odcinków drogowych dla pojedynczego miasta.
 */
struct RoadList
{
    /**
     * Wskazywany odcinek.
     */
    Road *this;

    /**
     * Następny węzeł.
     */
    RoadList *next;
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
};

/**
 * @brief Znajduje na liście miast węzeł, do którego można dołączyć kolejny.
 * @param map -- struktura mapy
 * @return Ostatni węzeł mapy, lub NULL jeśli mapa jest pusta.
 */
CityList *findEmptySpotForCityList(Map *map);

/**
 * @brief Dodaje nowy węzeł odcinka drogowego do listy odcinków w podanym mieście.
 * @param whereRoad -- miasto
 * @param addedRoad -- węzeł listy odcinków drogowych
 */
void addToRoadList(City *whereRoad, RoadList *addedRoad);

/**
 * @brief Wyszukuje odcinek drogi między dwoma podanymi miastami.
 * @param map -- struktura mapy
 * @param city1 -- miasto "z"
 * @param city2 -- miasto "do"
 * @return Znaleziony odcinek drogi; NULL, jeśli nie istnieje.
 */
Road *findRoadBetweenCities(Map *map, const char *city1, const char *city2);

/**
 * Funkcja pomocnicza. Sprawdza, czy odcinek drogi znajduje się w podanej tablicy wskaźników.
 * @param pRoad -- poszukiwany odcinek drogowy
 * @param remove -- tablica odcinków drogowych
 * @param ofRemove -- rozmiar tablicy remove
 * @return true, jeśli odcinek znajduje się na liście, w przeciwnym wypadku false.
 */
bool isOnTheArray(Road *pRoad, Road *remove[], int ofRemove);

/**
 * @brief Usuwa odcinek drogowy z listy odcinków danego miasta.
 * @param pRoad -- wskaznik na usuwany odcinek drogi
 * @param pCity -- wskaznik na miasto z którego listy usuwamy odcinek drogi
 */
void removeFromRoadList(Road *pRoad, City *pCity);

Map *newMap(void)
{
    Map *newMap = malloc(sizeof(Map));

    if (newMap != NULL)
    {
        newMap->cities = NULL;
    }

    return newMap;
}

void deleteMap(Map *map)
{
    Road **roadsToRemove = malloc(sizeof(Road*) * 0);
    int sizeOfRemove = 0;

    if (map != NULL)
    {
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
            free(act->this->name);
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
        free(roadsToRemove);

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
                 int builtYear);

/**
 * @brief Tworzy miasto o zadanej nazwie i dodaje je do mapy.
 * Funkcja pomocnicza.
 * @param map -- wskaźnik na mapę
 * @param name -- łańcuch znaków zawierający nazwę miasta
 * @return Wskaźnik do utworzonej struktury miasta.
 */
City *makeNewCity(Map *map, const char *name);

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

bool removeRoad(Map *map, const char *city1, const char *city2)
{
    Road *road = findRoadBetweenCities(map, city1, city2);
    if (road == NULL)
    {
        return false;
    }
    else
    {
        City *cityA = road->cityA;
        City *cityB = road->cityB;
        removeFromRoadList(road, cityA);
        removeFromRoadList(road, cityB);
        free(road);
        return true;
    }
}

void removeFromRoadList(Road *pRoad, City *pCity)
{
    if (pCity->roads->this == pRoad)
    {
        RoadList *removed = pCity->roads;
        pCity->roads = pCity->roads->next;
        free(removed);
    }
    else
    {
        RoadList *act = pCity->roads->next;
        RoadList *previous = pCity->roads;
        while (act->this != pRoad)
        {
            previous = act;
            act = act->next;
        }
        previous->next = act->next;
        free(act);
    }
}

City *makeNewCity(Map *map, const char *name)
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
            node->this->name = malloc(sizeof(char) * (strlen(name) + 1));
            if (node->this->name == NULL)
            {
                free(node->this);
                return NULL;
            }
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

bool makeNewRoad(City *cityA, City *cityB, unsigned length, int builtYear) // helper, true = ok, false error
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

bool newRoute(Map *map, unsigned routeId, const char *city1, const char *city2)
{
    UNUSED(map);
    UNUSED(routeId);
    UNUSED(city1);
    UNUSED(city2);

    return false;
}

bool extendRoute(Map *map, unsigned routeId, const char *city)
{
    UNUSED(map);
    UNUSED(routeId);
    UNUSED(city);

    return false;
}

char const* getRouteDescription(Map *map, unsigned routeId)
{
    UNUSED(map);
    UNUSED(routeId);

    return false;
}