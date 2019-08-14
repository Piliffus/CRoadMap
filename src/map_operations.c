/** @file
 * Implementacja operacji pomocniczych dla głównego modułu
 *
 * @author Filip Bieńkowski 407686
 * @copyright Uniwersytet Warszawski
 */

#include "map_operations.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static void markUnvisited(dNode *node)
{
    if (node != NULL)
    {
        node->this->distance = INFINITY;
        node->this->worstAge = YEAR_INFINTY;
        node->this->visited = false;
        node->this->previous = NULL;
        markUnvisited(node->next);
    }
}

static void markAllUnvisited(Dictionary *dictionary)
{
    for (unsigned i = 0; i < HASHSIZE; ++i)
    {
        markUnvisited(dictionary->nodes[i]);
    }
}

static int min(int arg1, int arg2)
{
    return arg1 < arg2 ? arg1 : arg2;
}

static void reverseArray(City **array, unsigned length)
{
    for (unsigned i = 0; i < (length / 2); ++i)
    {
        City *helper = array[length-1-i];
        array[length-1-i] = array[i];
        array[i] = helper;
    }
}

City *lowestDistanceNode(Map *map)
{
    unsigned lowestDistance = INFINITY;
    City *lowestNode = NULL;

    for (int i = 0; i < HASHSIZE; ++i)
    {
        for (dNode *current = map->cities->nodes[i];
             current != NULL; current = current->next)
        {
            if (!current->this->visited) // we want unvisited node
            {
                if (current->this->distance < lowestDistance)
                {
                    lowestDistance = current->this->distance;
                    lowestNode = current->this;
                }
            }

        }
    }

    // if lowest distance is INFINTY then return NULL
    return lowestNode;
}

bool thereAreUnvisitedNodes(Map *map)
{
    for (unsigned i = 0; i < HASHSIZE; ++i)
    {
        dNode *current = map->cities->nodes[i];

        while (current != NULL)
        {
            if (!current->this->visited)
            {
                return true;
            }
            else current = current->next;
        }
    }

    return false;
}

Route *dkstra(Map *map, unsigned int routeId, City *start, City *finish)
{
    // we make set of unvisited nodes
    markAllUnvisited(map->cities);

    if (map->routes[routeId] != NULL)
    {
        // if we are extending an already existing route, then we must make sure
        // it doesn`t cross itself
        for (unsigned i = 0; i < map->routes[routeId]->length; ++i)
        {
            map->routes[routeId]->howTheWayGoes[i]->visited = true;
        }
    }

    // distance is 0 for starting node
    start->distance = 0;
    start->visited = false;
    finish->visited = false;

    while (thereAreUnvisitedNodes(map))
    {
        City *actCity = lowestDistanceNode(map);
        if (actCity == NULL) return NULL; // no path from start to finish
        actCity->visited = true; // remove node from unvisited set
        if (actCity == finish) break; // we found the way so we are done

        for (RoadList *actRoad = actCity->roads;
             actRoad != NULL; actRoad = actRoad->next) // check each neighbour
        {
            City *neighbour =
                    actRoad->this->cityA == actCity ? actRoad->this->cityB
                                                    : actRoad->this->cityA;

            if (!neighbour->visited) // only check unvisited nodes
            {
                unsigned newDistance =
                        actCity->distance + actRoad->this->length;
                int newAge = min(actCity->worstAge, actRoad->this->year);

                if (newDistance < neighbour->distance)
                {
                    neighbour->distance = newDistance;
                    neighbour->previous = actCity;
                    neighbour->worstAge = newAge;
                }
                else if (newDistance == neighbour->distance)
                {
                    if (newAge > neighbour->worstAge)
                    {
                        neighbour->distance = newDistance;
                        neighbour->previous = actCity;
                        neighbour->worstAge = newAge;
                    }
                    else if (newAge == neighbour->worstAge)
                    {
                        // we cannot decide how to get to this node, so this
                        // node is unreachable
                        neighbour->previous = NULL;
                    }
                }
            }
        }
    }

    // there is no path; if it`s not NULL then there must be some way from
    // start to finish, because otherwise it would not be assigned
    if (finish->previous == NULL) return NULL;

    // we must now make new route out of our shortest path
    Route *newRoute = malloc(sizeof(Route));
    if (newRoute == NULL) return NULL;
    newRoute->length = 0;

    newRoute->howTheWayGoes = malloc(sizeof(City *) * newRoute->length);

    for (City *act = finish; act != NULL; act = act->previous)
    {
        if (act->previous == NULL && act != start)
        {
            free(newRoute->howTheWayGoes);
            free(newRoute);
            return NULL;
        }

        newRoute->length++;
        City **failInsurance = newRoute->howTheWayGoes; // in case realloc fails
        newRoute->howTheWayGoes = realloc(newRoute->howTheWayGoes,
                                          sizeof(City *) * newRoute->length);
        if (newRoute->howTheWayGoes == NULL)
        {
            free(failInsurance);
            free(newRoute);
            return NULL;
        }
        newRoute->howTheWayGoes[newRoute->length - 1] = act;
    }

    reverseArray(newRoute->howTheWayGoes, newRoute->length);
    return newRoute;
}

Road *findRoadBetween(City *start, City *finish)
{
    for (RoadList *act = start->roads; act != NULL ; act = act->next)
    {
        if (act->this->cityA == finish || act->this->cityB == finish)
        {
            return act->this;
        }
    }

    return NULL;
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
    City *newCity = malloc(sizeof(City));

    if (newCity != NULL)
    {
        newCity->name = malloc(sizeof(char) * (strlen(name) + 1));
        if (newCity->name == NULL)
        {
            free(newCity);
            return NULL;
        }

        strcpy(newCity->name, name);
        newCity->roads = NULL;

        put(map->cities, newCity);
        return newCity;
    }
    else
    {
        return NULL;
    }

}

bool makeNewRoad(City *cityA, City *cityB, unsigned length, int builtYear)
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
            road->queued = false;
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

City *findCity(Map *map, const char *cityName)
{
    return get(map->cities, cityName);
}

bool isCorrectName(const char *name)
{
    unsigned i = 0;

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
        ++i;
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

bool hasCities(Route *route, City *cityA, City *cityB)
{
    if (route == NULL) return false;
    bool cityAOK = false;
    bool cityBOK = false;

    for (unsigned i = 0; i < route->length; ++i)
    {
        if (!cityAOK)
        {
            if (route->howTheWayGoes[i] == cityA)
            {
                cityAOK = true;
            }
        }
        if (!cityBOK)
        {
            if (route->howTheWayGoes[i] == cityB)
            {
                cityBOK = true;
            }
        }
        if (cityAOK && cityBOK)
        {
            return true;
        }
    }

    return false;
}

unsigned findCityIndex(Route *route, City *cityToFind)
{
    for (unsigned i = 0; i < route->length; ++i)
    {
        if (route->howTheWayGoes[i] == cityToFind)
        {
            return i;
        }
    }

    return INFINITY;
}

void insertIntoRoute(Route *target, Route *source, unsigned from, unsigned to)
{
    if (from > to)
    {
        unsigned helper = to;
        to = from;
        from = helper;
    }

    unsigned oldLength = target->length;
    target->length += (source->length - 2);// [from] and [to] are the same so -2
    target->howTheWayGoes = realloc(target->howTheWayGoes, sizeof(City *) *
                                                           target->length);

    // we must move elements in array
    unsigned h = 0;
    for (unsigned i = oldLength-1; i >= to; i--)
    {
        ++h;
        target->howTheWayGoes[target->length-h] = target->howTheWayGoes[i];
        target->howTheWayGoes[i] = NULL;
    }

    unsigned j = from;
    for (unsigned k = 0; k < source->length; ++k)
    {
        target->howTheWayGoes[j] = source->howTheWayGoes[k];
        ++j;
    }
}

bool checkRoutesAfterRoadRemoval(Map *map, City *cityA, City *cityB)
{
    // returned true means it`s ok to remove this road and updates routes,
    // false means it`s not ok and doesn`t change anything

    Route *potentialNewRoutes[ROUTES_AMOUNT];

    for (int i = 0; i < ROUTES_AMOUNT; ++i)
    {
        potentialNewRoutes[i] = NULL;
    }

    unsigned requiredSize = 0;
    for (int j = 0; j < ROUTES_AMOUNT; ++j)
    {
        if (hasCities(map->routes[j], cityA, cityB))
        {
            unsigned start = findCityIndex(map->routes[j], cityA);
            unsigned finish = findCityIndex(map->routes[j], cityB);
            if (finish < start)
            {
                unsigned helper = start;
                start = finish;
                finish = helper;
            }

            potentialNewRoutes[j] = dkstra(map, j,
                                           map->routes[j]->howTheWayGoes[start],
                                           map->routes[j]->howTheWayGoes[finish]);
            if (potentialNewRoutes[j] == NULL) return false;
            requiredSize += potentialNewRoutes[j]->length;
        }
    }

    City **memoryCheck = malloc(sizeof(City *) * requiredSize);
    if (memoryCheck == NULL) return false;
    free(memoryCheck);

    for (int k = 0; k < ROUTES_AMOUNT; ++k)
    {
        if (potentialNewRoutes[k] != NULL)
        {
            insertIntoRoute(map->routes[k], potentialNewRoutes[k],
                            findCityIndex(map->routes[k], cityA),
                            findCityIndex(map->routes[k], cityB));
        }
    }

    for (int i = 0; i < ROUTES_AMOUNT; ++i)
    {
        if (potentialNewRoutes[i] != NULL)
        {
            free(potentialNewRoutes[i]->howTheWayGoes);
            free(potentialNewRoutes[i]);
        }
    }

    return true;
}
