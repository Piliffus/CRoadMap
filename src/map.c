/** @file
 * Implementacja klasy przechowującej mapę dróg krajowych
 *
 * @author Filip Bieńkowski 407686
 * @copyright Uniwersytet Warszawski
 */

#include "map.h"
#include "map_operations.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

Map *newMap(void)
{
    Map *newMap = malloc(sizeof(Map));

    if (newMap != NULL)
    {
        newMap->cities = NULL;

        for (int i = 0; i < ROUTES_AMOUNT; i++)
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

        for (int i = 0; i < ROUTES_AMOUNT; i++) // remove routes
        {
            if (map->routes[i] != NULL)
            {
                free(map->routes[i]->howTheWayGoes);
                free(map->routes[i]);
            }
        }
        free(map);
    }
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

    if (length <= 0)
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
        unsigned length = road->length;
        int year = road->year; // in case we put it back
        removeFromRoadList(road, cityA);
        removeFromRoadList(road, cityB);
        free(road);

        if (!checkRoutesAfterRoadRemoval(map, cityA, cityB))
        {
            addRoad(map, city1, city2, length, year);
            return false;
        }

        return true;
    }
}

bool repairRoad(Map *map, const char *city1, const char *city2, int repairYear)
{
    if (!isCorrectName(city1) || !isCorrectName(city2))
    {
        return false;
    }

    if (repairYear == 0)
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
    if (routeId >= ROUTES_AMOUNT || routeId < 1)
    {
        return false;
    }

    if (map->routes[routeId] != NULL)
    {
        return false;
    }

    City *start = findCity(map, city1);
    City *finish = findCity(map, city2);

    if (start == NULL || finish == NULL)
    {
        return false;
    }

    Route *newRoute = dkstra(map, routeId, start, finish);

    if (newRoute == NULL)
    {
        return false;
    }
    else
    {
        map->routes[routeId] = newRoute;
        return true;
    }
}

bool extendRoute(Map *map, unsigned routeId, const char *city)
{
    if (routeId >= ROUTES_AMOUNT || routeId < 1)
    {
        return false;
    }

    Route *oldRoute = map->routes[routeId];
    if (oldRoute == NULL) return false;

    City *newFinish = findCity(map, city);
    if (newFinish == NULL) return false;

    if (findCityIndex(map->routes[routeId], newFinish) != INFINITY) return false;

    Route *newPart = dkstra(map, routeId,
                            oldRoute->howTheWayGoes[oldRoute->length - 1],
                            newFinish);
    if (newPart == NULL) return false;

    City **failInsurance = oldRoute->howTheWayGoes;
    oldRoute->howTheWayGoes = realloc(oldRoute->howTheWayGoes, sizeof(City *) *
                                                               (oldRoute->length +
                                                                newPart->length));
    if (oldRoute->howTheWayGoes == NULL)
    {
        oldRoute->howTheWayGoes = failInsurance;
        free(newPart->howTheWayGoes);
        free(newPart);
        return false;
    }

    unsigned oldLength = oldRoute->length;
    oldRoute->length += newPart->length;
    oldRoute->length--; // length is 1 more than last index, so if we add two
    // lengths to each other then the result will be 2 bigger, so we compensate

    for (unsigned i = 0; i < newPart->length; i++)
    {
        oldRoute->howTheWayGoes[oldLength-1 + i] = newPart->howTheWayGoes[i];
    }

    free(newPart->howTheWayGoes);
    free(newPart);
    return true;
}

char const *getRouteDescription(Map *map, unsigned routeId)
{
    char* fail = malloc(sizeof(char)*1);
    if (fail == NULL) return fail;
    fail[0] = '\0';

    if (routeId >= ROUTES_AMOUNT || routeId < 1) return fail;

    if (map->routes[routeId] == NULL) return fail;

    char *returnedString = malloc(sizeof(char) * CHAR_BUFFER);
    int neededLength = snprintf(NULL, 0, "%u;", routeId);
    int actualLength = CHAR_BUFFER;

    if (neededLength >= actualLength)
    {
        char *failInsurance = returnedString;
        returnedString = realloc(returnedString, sizeof(char) * neededLength+1);
        if (returnedString == NULL)
        {
            free(failInsurance);
            return fail;
        }
    }

    int lastChar = sprintf(returnedString, "%u;", routeId);
    if (lastChar < 0)
    {
        free(returnedString);
        return fail;
    }

    unsigned i = 0; // 'i' declared outside because will be needed later
    for (; i < map->routes[routeId]->length - 1; i++)
    {
        neededLength += snprintf(NULL, 0, "%s;",
                                map->routes[routeId]->howTheWayGoes[i]->name);
        if (neededLength >= actualLength)
        {
            char *failInsurance = returnedString;
            returnedString = realloc(returnedString, sizeof(char) * neededLength+1);
            if (returnedString == NULL)
            {
                free(failInsurance);
                return fail;
            }
            actualLength = neededLength + 1;
        }
        int success = sprintf(returnedString + lastChar, "%s;",
                              map->routes[routeId]->howTheWayGoes[i]->name);
        if (success < 0)
        {
            free(returnedString);
            return fail;
        }
        lastChar += success;

        City *destination = map->routes[routeId]->howTheWayGoes[i + 1];
        Road *road = findRoadBetween(map->routes[routeId]->howTheWayGoes[i],
                                     destination);
        neededLength += snprintf(NULL, 0, "%u;%d;", road->length,
                                 road->year);
        if (neededLength >= actualLength)
        {
            char *failInsurance = returnedString;
            returnedString = realloc(returnedString, sizeof(char) * neededLength+1);
            if (returnedString == NULL)
            {
                free(failInsurance);
                return fail;
            }
            actualLength = neededLength + 1;
        }
        success = sprintf(returnedString + lastChar, "%u;%d;", road->length,
                          road->year);
        if (success < 0)
        {
            free(returnedString);
            return fail;
        }
        lastChar += success;

    }

    // we do this outside of loop because we don`t want ';'
    // nor want to overflow with howTheWayGoes[i + 1]

    neededLength += snprintf(NULL, 0, "%s",
                             map->routes[routeId]->howTheWayGoes[i]->name);
    if (neededLength >= actualLength)
    {
        char *failInsurance = returnedString;
        returnedString = realloc(returnedString, sizeof(char) * neededLength+1);
        if (returnedString == NULL)
        {
            free(failInsurance);
            return fail;
        }
    }

    int success = sprintf(returnedString + lastChar, "%s",
                          map->routes[routeId]->howTheWayGoes[i]->name);
    if (success < 0)
    {
        free(returnedString);
        return fail;
    }

    free(fail);
    return returnedString;
}

Route* newCustomRoute(Map *map, unsigned routeId, const char *startCity)
{
    if (routeId < 1 || routeId >= ROUTES_AMOUNT)
    {
        return NULL;
    }
    if (map->routes[routeId] != NULL)
    {
        return NULL;
    }
    if (!isCorrectName(startCity))
    {
        return NULL;
    }

    City *startCityPtr = findCity(map, startCity);
    if (startCityPtr == NULL)
    {
        startCityPtr = makeNewCity(map, startCity);
    }

    Route *newRoute = malloc(sizeof(Route));
    if (newRoute == NULL) return NULL;

    newRoute->length = 1;
    newRoute->howTheWayGoes = malloc(sizeof(City*) * newRoute->length);
    newRoute->howTheWayGoes[0] = startCityPtr;
    map->routes[routeId] = newRoute;

    return newRoute;
}

bool extendCustomRoute(Map *map, unsigned routeId, unsigned length, int year,
                       const char *destinationName)
{
    // if wrong arguments don`t waste time doing rest, routeId assumed correct
    if (length <= 0 || year == 0) return false;

    City *start = map->routes[routeId]->howTheWayGoes[
            map->routes[routeId]->length - 1];
    City *destination = findCity(map, destinationName);
    Road *road = (destination == NULL ? NULL : findRoadBetween(start,
                                                               destination));
    // if we don`t have a city, make one.
    if (destination == NULL)
    {
        if (!isCorrectName(destinationName)) return false;
        destination = makeNewCity(map, destinationName);
    }
    // failed alloc
    if (destination == NULL) return false;

    // if we already have a road, update it`s year
    if (road != NULL)
    {
        if (road->length != length || road->year > year) return false;
        else road->year = year;
    }
    else // if road == NULL, make one
    {
        if (!makeNewRoad(start, destination, length, year)) return false;
    }

    // now add new part to the route
    map->routes[routeId]->length++;
    City **failInsurance = map->routes[routeId]->howTheWayGoes;

    map->routes[routeId]->howTheWayGoes = realloc(
            map->routes[routeId]->howTheWayGoes,
            sizeof(City *) * map->routes[routeId]->length);
    if (map->routes[routeId]->howTheWayGoes == NULL)
    {
        map->routes[routeId]->howTheWayGoes = failInsurance;
        return false;
    }

    map->routes[routeId]->howTheWayGoes[map->routes[routeId]->length -
                                        1] = destination;
    return true;
}

bool removeRoute(Map *map, unsigned routeId)
{
    if (routeId >= ROUTES_AMOUNT || routeId <= 0)
    {
        return false;
    }

    if (map->routes[routeId] == NULL)
    {
        return false;
    }

    else
    {
        free(map->routes[routeId]->howTheWayGoes);
        free(map->routes[routeId]);
        map->routes[routeId] = NULL;
        return true;
    }
}