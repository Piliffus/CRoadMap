#include "map.h"

#include <stdlib.h>
#include <string.h>

struct City
{
	char* name;
};

struct CityList
{
	City* this;
	City* next;
};

struct Road
{
	unsigned int length;
	int year;
};

struct RoadList
{
	Road* this;
	Road* next;
};

struct Map
{
	RoadList* roads;
	CityList* cities;
};

Map* newMap(void)
{
	Map* newMap = malloc(sizeof(Map));
	
	if (newMap != NULL) 
	{
		newMap->cities->this = NULL;
		newMap->cities->next = NULL;
		newMap->roads->this = NULL;
		newMap->roads->next = NULL;
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
bool findCity(Map *map, const char *cityName) 
{
	CityList* current = map->cities;

	while (current != NULL) 
	{
		// if current is not null then it must have `this`
		
		if (current->this->name == cityName) 
		{
			return true;
		}
		current = current->next;
	}

	return false;
}

bool addRoad(Map *map, const char *city1, const char *city2,
	unsigned length, int builtYear) 
{
	
}