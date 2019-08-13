/** @file
 * Deklaracje operacji pomocniczych dla głównego modułu
 *
 * @author Filip Bieńkowski 407686
 * @copyright Uniwersytet Warszawski
 */

#ifndef DROGI_MAP_OPERATIONS_H
#define DROGI_MAP_OPERATIONS_H

#include "map.h"

#define CHAR_BUFFER 1024
#define INFINITY UINT_MAX
#define YEAR_INFINTY INT_MAX

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
 * @brief Usuwa odcinek drogowy z listy odcinków danego miasta.
 * @param pRoad -- wskaznik na usuwany odcinek drogi
 * @param pCity -- wskaznik na miasto z którego listy usuwamy odcinek drogi
 */
void removeFromRoadList(Road *pRoad, City *pCity);

/** @brief Sprawdza czy istnieje miasto o danej nazwie.
 * Funkcja szuka na liście miast będacej częścią mapy
 * miasta o podanej nazwie.
 * @param[in] map        – wskaźnik na strukturę przechowującą mapę dróg;
 * @param[in] cityName   – wskaźnik na napis reprezentujący nazwę miasta;
 * @return Wskaźnik na miasto, jesli takie miasto istnieje
 * lub NULL, jeśli nie ma takiego miasta.
 */
City* findCity(Map *map, const char *cityName);

/** @brief Sprawdza czy nazwa jest poprawną nazwą dla miasta.
 * Funkcja szuka w ciągu znaków znaków o kodach od 0 do 31, średników, oraz
 * sprawdza czy ciąg nie jest pusty.
 * @param[in] name       – ciąg znaków testowany czy nadaje się na nazwę miasta;
 * @return Wartość @p true, jesli wszystkie znaki w ciągu są dozwolone
 * Wartość @p false, jesli nie wszystkie znaki są dozwolone.
 */
bool isCorrectName(const char *name);

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
 * @param map -- wskaźnik na mapę
 * @param name -- łańcuch znaków zawierający nazwę miasta
 * @return Wskaźnik do utworzonej struktury miasta.
 */
City *makeNewCity(Map *map, const char *name);

/**
 * @brief Implementacja algorytmu djkstry
 * Znajduje najkrótszą drogę z miasta 'start' do miasta 'finish'
 * @param map -- wskaźnik na mapę
 * @param routeId -- numer drogi krajowej
 * @param start -- miasto początkowe
 * @param finish -- miasto końcowe
 * @return Potencjalna droga krajowa, albo NULL jeśli nie ma drogi z A do B
 */
Route *dkstra(Map *map, unsigned int routeId, City *start, City *finish);

/**
 * @brief Wstawia jedną drogę krajową w drugą. Zakładamy że mamy miejsce.
 * @param target -- wskaźnik na drogę do której dodajemy drugą
 * @param source -- wskaźnik na dodawaną drogę
 * @param from -- wstawiamy od miasta o tym indeksie włącznie
 * @param to -- wstawiamy do miasta o tym indeksie włącznie
 */
void insertIntoRoute(Route *target, Route *source, unsigned from, unsigned to);

/**
 * @brief Sprawdza poprawność potencjalnej drogi krajowej
 * Funkcja sprawdza czy droga krajowa która powstanie po usunięciu odcinka drogi
 * będzie poprawną funkcją
 * @param map -- wskaźnik na mapę dróg krajowych
 * @param cityA -- miasto początkowe
 * @param cityB -- miasto końcowe
 * @return Wartość @p true jeśli jest poprawna, wartość @p false w przeciwnym wypadku
 */
bool checkRoutesAfterRoadRemoval(Map *map, City *cityA, City *cityB);

/**
 * @brief Sprawdza czy na mapie są jeszcze nieodwiedzone węzły
 * @param map -- wskaźnik na mapę dróg krajowych
 * @return Wartość @p true jeśli tak, wartość @p false w przeciwnym wypadku
 */
bool thereAreUnvisitedNodes(Map *map);

/**
 * @brief Funkcja używana przez algorytm djkstry
 * Funkcja znajduje węzeł o najmniejszej odległosci od startu
 * @param map -- wskaźnik na mapę dróg krajowych
 * @return Wskaźnik na najbliższy węzeł, null jeśli nie zostały nieodwiedzone węzłyu
 */
City *lowestDistanceNode(Map *map);

/**
 * @brief Funkcja znajdująca odcinek drogi pomiędzy danymi miastami
 * @param start -- wskaźnik na pierwsze miasto
 * @param finish -- wskaźnik na drugie miasto
 * @return Wskaźnik na drogę, NULL jeśli nie ma
 */
Road *findRoadBetween(City *start, City *finish);

/**
 * @brief Funkcja sprawdza czy droga krajowa jest pusta
 * @param route -- wskaźnik na drogę krajową
 * @param cityA -- wskaźnik na pierwsze miasto
 * @param cityB -- wskaźnik na drugie miasto
 * @return Wartość @p true jeśli nie jest, wartość @p false jeśli nie
 */
bool hasCities(Route *route, City *cityA, City *cityB);

/**
 * @brief Znajduje kolejność miasta na drodze krajowej
 * @param route -- wskaźnik na drogę krajową
 * @param cityA -- wskaźnik na miasto
 * @return Kolejność miasta na drodze, albo nieskończoność jeśli nie ma takiego miasta
 */
unsigned findCityIndex(Route *route, City *cityToFind);

#endif //DROGI_MAP_OPERATIONS_H
