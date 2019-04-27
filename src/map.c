#include "map.h"

#include <stdlib.h>
#include <string.h>

struct City
{
    char *name;
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

struct RoadList
{
    Road *this;
    RoadList *next;
};

struct Map
{
    CityList *cities;
};

CityList *findEmptySpotForCityList(Map *map);

bool addToRoadList(City *whereRoad, RoadList *whatRoad);

Road *findRoadBetweenCities(Map *map, const char *city1, const char *city2);

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
    if (map != NULL)
    {
        // TODO: fix memory leak here, as soon as lists are added
        free(map);
    }
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

bool addToRoadList(City *whereRoad, RoadList *addedRoad)
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
    }
}

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
            node->this->name = name; // TODO: will it even work?
            node->this->roads = NULL;

            return node->this; // TODO: first node has no this
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
