#include "map.h"

#include <stdlib.h>
#include <string.h>

struct City
{
	char* name;
	RoadList* roads;
};

struct CityList
{
	City* this;
	CityList* next;
};

struct Road
{
	City* cityA;
	City* cityB;
	unsigned length;
	int year;
};

struct RoadList
{
	Road* this;
	RoadList* next;
};

struct Map
{
	CityList* cities;
};

CityList* findEmptySpotForCityList(Map *map);

bool addToRoadList(City *whereRoad, RoadList *whatRoad);

Road *findRoadBetweenCities(Map* map, const char *city1, const char *city2);

Map *newMap(void)
{
    Map *newMap = malloc(sizeof(Map));

    if (newMap != NULL)
    {
        newMap->cities = malloc(sizeof(CityList));

        if (newMap->cities != NULL)
        {
            newMap->cities->this = NULL;
            newMap->cities->next = NULL;
        }
        else
        {
            return NULL;
        }
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

City* findCity(Map *map, const char *cityName) 
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
		if (name[i] <= 31 || name[i] == ';')
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
    RoadList *act = whereRoad->roads;

    if (act == NULL)
    {
        whereRoad->roads = addedRoad;
    }

    else
    {
        while (act != NULL)
        {
            act = act->next;
        }

        act = addedRoad;
    }
}

makeNewRoad(City *cityA, City *cityB, unsigned length, int builtYear) // helper, true = ok, false error
{
    RoadList *newNode = malloc(sizeof(RoadList));
    if (newNode != NULL)
    {
        newNode->next = NULL;
        newNode->this = malloc(sizeof(Road));
        if (newNode->this != NULL)
        {
            newNode->this->cityA = cityA;
            newNode->this->cityB = cityB;
            newNode->this->length = length;
            newNode->this->year = builtYear;
            addToRoadList(cityA, newNode);
            addToRoadList(cityB, newNode);
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
    CityList *where = findEmptySpotForCityList(map);
    where->next = malloc(sizeof(CityList *));
    if (where->next != NULL)
    {
        where->next->this = malloc(sizeof(City));
        if (where->next->this != NULL)
        {
            where->next->this->name = name; // TODO: will it even work?
            where->next->this->roads = NULL;
            return where->next->this; // TODO: first node has no this
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

CityList* findEmptySpotForCityList(Map *map)
{
    CityList* whereCity = map->cities;
    CityList* act = map->cities;

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
	City* cityA = NULL;
	City* cityB = NULL;
	
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
