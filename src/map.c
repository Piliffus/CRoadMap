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
	City* next;
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
	Road* next;
};

struct Map
{
	CityList* cities;
};

Map* newMap(void)
{
	Map* newMap = malloc(sizeof(Map));
	
	if (newMap != NULL) 
	{
		newMap->cities->this = NULL;
		newMap->cities->next = NULL;
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

void makeNewRoad(City *cityA, City *cityB, unsigned length, int builtYear) // helper
{
	// TODO, has no declaration
}

City* makeNewCity(Map *map, const char *name) // helper
{
	// TODO, has no declaration
	return NULL;
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
	}

	if (cityB == NULL)
	{
		cityB = makeNewCity(map, city2);
	}

	makeNewRoad(cityA, cityB, length, builtYear);

	return true;
}