/** @file
 * Interfejs klasy przechowującej mapę dróg krajowych
 *
 * @author Łukasz Kamiński <kamis@mimuw.edu.pl>, Marcin Peczarski <marpe@mimuw.edu.pl>
 * @copyright Uniwersytet Warszawski
 * @date 20.03.2019
 */

#ifndef __MAP_H__
#define __MAP_H__

#include <stdbool.h>
#include <limits.h>
#include "Dictionary.h"

#define ROUTES_AMOUNT 1000

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
    struct RoadList *roads;

    /**
     * @brief Najlepsza odległość, używane przez algorytm djkstry
     */
    unsigned distance;

    /**
     * @brief Flaga używana przez algorytm djkstry
     */
    bool visited;

    /**
    * @brief Wskaznik na poprzednie miasto na drodze krajowej, uzwyane przez algorytm djkstry
    */
    struct City *previous;

    /**
    * @brief Najgorsza dotychczasowa wartość wieku, używana przez algorytm djkstry
    */
    int worstAge;
};
typedef struct City City;

/**
   * Struktura przechowująca informacje o drodze.
   */
struct Road
{
    /**
     * @brief Pierwszy koniec odcinka.
     */
    struct City *cityA;

    /**
     * @brief Drugi koniec odcinka.
     */
    struct City *cityB;

    /**
     * @brief Długość odcinka.
     */
    unsigned length;

    /**
     * @brief Rok budowy lub ostatniego remontu.
     */
    int year;

    /**
     * @brief Informacja, czy dany odcinek został już zakolejkowany do usunięcia
     */
    bool queued;
};
typedef struct Road Road;

/**
 * Węzeł listy odcinków drogowych dla pojedynczego miasta.
 */
struct RoadList
{
    /**
     * Wskazywany odcinek.
     */
    struct Road *this;

    /**
     * Następny węzeł.
     */
    struct RoadList *next;
};
typedef struct RoadList RoadList;

/**
 * @brief Struktura reprezentująca pojedynczą drogę krajową.
 */
struct Route
{
    /**
    * @brief Tablica wskaznikow na kolejne miasta drogi krajowej.
    */
    struct City **howTheWayGoes;

    /**
     * @brief Informacja przez ile miast prowadzi droga krajowa
     */
    unsigned length;
};
typedef struct Route Route;

/**
 * @brief Główna struktura zawierająca wskaźnik do listy miast i tablicy dróg krajowych.
 * Inicjalizację i usuwanie struktury realizują odpowiednio funkcje newMap() i deleteMap(Map *).
 */
struct Map
{
    /**
     * @brief Słownik zawierający miasta.
     */
    struct Dictionary *cities;
    /**
     * @brief Tablica zawierająca drogi krajowe
     */
    struct Route *routes[ROUTES_AMOUNT];
};
typedef struct Map Map;


/** @brief Tworzy nową strukturę.
 * Tworzy nową, pustą strukturę niezawierającą żadnych miast, odcinków dróg ani
 * dróg krajowych.
 * @return Wskaźnik na utworzoną strukturę lub NULL, gdy nie udało się
 * zaalokować pamięci.
 */
Map* newMap(void);

/** @brief Usuwa strukturę.
 * Usuwa strukturę wskazywaną przez @p map.
 * Nic nie robi, jeśli wskaźnik ten ma wartość NULL.
 * @param[in] map        – wskaźnik na usuwaną strukturę.
 */
void deleteMap(Map *map);

/** @brief Dodaje do mapy odcinek drogi między dwoma różnymi miastami.
 * Jeśli któreś z podanych miast nie istnieje, to dodaje go do mapy, a następnie
 * dodaje do mapy odcinek drogi między tymi miastami.
 * @param[in,out] map    – wskaźnik na strukturę przechowującą mapę dróg;
 * @param[in] city1      – wskaźnik na napis reprezentujący nazwę miasta;
 * @param[in] city2      – wskaźnik na napis reprezentujący nazwę miasta;
 * @param[in] length     – długość w km odcinka drogi;
 * @param[in] builtYear  – rok budowy odcinka drogi.
 * @return Wartość @p true, jeśli odcinek drogi został dodany.
 * Wartość @p false, jeśli wystąpił błąd: któryś z parametrów ma niepoprawną
 * wartość, obie podane nazwy miast są identyczne, odcinek drogi między tymi
 * miastami już istnieje lub nie udało się zaalokować pamięci.
 */
bool addRoad(Map *map, const char *city1, const char *city2,
             unsigned length, int builtYear);

/** @brief Modyfikuje rok ostatniego remontu odcinka drogi.
 * Dla odcinka drogi między dwoma miastami zmienia rok jego ostatniego remontu
 * lub ustawia ten rok, jeśli odcinek nie był jeszcze remontowany.
 * @param[in,out] map    – wskaźnik na strukturę przechowującą mapę dróg;
 * @param[in] city1      – wskaźnik na napis reprezentujący nazwę miasta;
 * @param[in] city2      – wskaźnik na napis reprezentujący nazwę miasta;
 * @param[in] repairYear – rok ostatniego remontu odcinka drogi.
 * @return Wartość @p true, jeśli modyfikacja się powiodła.
 * Wartość @p false, jeśli wystąpił błąd: któryś z parametrów ma niepoprawną
 * wartość, któreś z podanych miast nie istnieje, nie ma odcinka drogi między
 * podanymi miastami, podany rok jest wcześniejszy niż zapisany dla tego odcinka
 * drogi rok budowy lub ostatniego remontu.
 */
bool repairRoad(Map *map, const char *city1, const char *city2, int repairYear);

/** @brief Łączy dwa różne miasta drogą krajową.
 * Tworzy drogę krajową pomiędzy dwoma miastami i nadaje jej podany numer.
 * Wśród istniejących odcinków dróg wyszukuje najkrótszą drogę. Jeśli jest
 * więcej niż jeden sposób takiego wyboru, to dla każdego wariantu wyznacza
 * wśród wybranych w nim odcinków dróg ten, który był najdawniej wybudowany lub
 * remontowany i wybiera wariant z odcinkiem, który jest najmłodszy.
 * @param[in,out] map    – wskaźnik na strukturę przechowującą mapę dróg;
 * @param[in] routeId    – numer drogi krajowej;
 * @param[in] city1      – wskaźnik na napis reprezentujący nazwę miasta;
 * @param[in] city2      – wskaźnik na napis reprezentujący nazwę miasta.
 * @return Wartość @p true, jeśli droga krajowa została utworzona.
 * Wartość @p false, jeśli wystąpił błąd: któryś z parametrów ma niepoprawną
 * wartość, istnieje już droga krajowa o podanym numerze, któreś z podanych
 * miast nie istnieje, obie podane nazwy miast są identyczne, nie można
 * jednoznacznie wyznaczyć drogi krajowej między podanymi miastami lub nie udało
 * się zaalokować pamięci.
 */
bool newRoute(Map *map, unsigned routeId,
              const char *city1, const char *city2);

/** @brief Wydłuża drogę krajową do podanego miasta.
 * Dodaje do drogi krajowej nowe odcinki dróg do podanego miasta w taki sposób,
 * aby nowy fragment drogi krajowej był najkrótszy. Jeśli jest więcej niż jeden
 * sposób takiego wydłużenia, to dla każdego wariantu wyznacza wśród dodawanych
 * odcinków dróg ten, który był najdawniej wybudowany lub remontowany i wybiera
 * wariant z odcinkiem, który jest najmłodszy.
 * @param[in,out] map    – wskaźnik na strukturę przechowującą mapę dróg;
 * @param[in] routeId    – numer drogi krajowej;
 * @param[in] city       – wskaźnik na napis reprezentujący nazwę miasta.
 * @return Wartość @p true, jeśli droga krajowa została wydłużona.
 * Wartość @p false, jeśli wystąpił błąd: któryś z parametrów ma niepoprawną
 * nazwę, nie istnieje droga krajowa o podanym numerze, nie ma miasta o podanej
 * nazwie, przez podane miasto już przechodzi droga krajowa o podanym numerze,
 * podana droga krajowa kończy się w podanym mieście, nie można jednoznacznie
 * wyznaczyć nowego fragmentu drogi krajowej lub nie udało się zaalokować
 * pamięci.
 */
bool extendRoute(Map *map, unsigned routeId, const char *city);

/** @brief Usuwa odcinek drogi między dwoma różnymi miastami.
 * Usuwa odcinek drogi między dwoma miastami. Jeśli usunięcie tego odcinka drogi
 * powoduje przerwanie ciągu jakiejś drogi krajowej, to uzupełnia ją
 * istniejącymi odcinkami dróg w taki sposób, aby była najkrótsza. Jeśli jest
 * więcej niż jeden sposób takiego uzupełnienia, to dla każdego wariantu
 * wyznacza wśród dodawanych odcinków drogi ten, który był najdawniej wybudowany
 * lub remontowany i wybiera wariant z odcinkiem, który jest najmłodszy.
 * @param[in,out] map    – wskaźnik na strukturę przechowującą mapę dróg;
 * @param[in] city1      – wskaźnik na napis reprezentujący nazwę miasta;
 * @param[in] city2      – wskaźnik na napis reprezentujący nazwę miasta.
 * @return Wartość @p true, jeśli odcinek drogi został usunięty.
 * Wartość @p false, jeśli z powodu błędu nie można usunąć tego odcinka drogi:
 * któryś z parametrów ma niepoprawną wartość, nie ma któregoś z podanych miast,
 * nie istnieje droga między podanymi miastami, nie da się jednoznacznie
 * uzupełnić przerwanego ciągu drogi krajowej lub nie udało się zaalokować
 * pamięci.
 */
bool removeRoad(Map *map, const char *city1, const char *city2);

/** @brief Udostępnia informacje o drodze krajowej.
 * Zwraca wskaźnik na napis, który zawiera informacje o drodze krajowej. Alokuje
 * pamięć na ten napis. Zwraca pusty napis, jeśli nie istnieje droga krajowa
 * o podanym numerze. Zaalokowaną pamięć trzeba zwolnić za pomocą funkcji free.
 * Informacje wypisywane są w formacie:
 * numer drogi krajowej;nazwa miasta;długość odcinka drogi;rok budowy lub
 * ostatniego remontu;nazwa miasta;długość odcinka drogi;rok budowy lub
 * ostatniego remontu;nazwa miasta;…;nazwa miasta.
 * Kolejność miast na liście jest taka, aby miasta @p city1 i @p city2, podane
 * w wywołaniu funkcji @ref newRoute, które utworzyło tę drogę krajową, zostały
 * wypisane w tej kolejności.
 * @param[in,out] map    – wskaźnik na strukturę przechowującą mapę dróg;
 * @param[in] routeId    – numer drogi krajowej.
 * @return Wskaźnik na napis lub NULL, gdy nie udało się zaalokować pamięci.
 */
char const* getRouteDescription(Map *map, unsigned routeId);

/** @brief Deklaruje nową drogę krajową o parametrach podanych przez użytkownika.
 * Tworzy drogę krajową, zaczynającą się w mieście o nazwie podanej przez użytkownika.
 * Jeżeli takie miasto nie istnieje, to tworzy je.
 * @param[in,out] map    – wskaźnik na strukturę przechowującą mapę dróg;
 * @param[in] routeId    – numer drogi krajowej;
 * @param[in] startCity  – wskaźnik na napis reprezentujący nazwę miasta początkowego;
 * @return Wskaźnik na drogę, jeśli droga krajowa została utworzona.
 * Wartość NULL, jeśli wystąpił błąd: któryś z parametrów ma niepoprawną
 * wartość, istnieje już droga krajowa o podanym numerze, lub nie udało
 * się zaalokować pamięci.
 */
Route* newCustomRoute(Map *map, unsigned routeId, const char *startCity);

/** @brief Wydłuża drogę krajową o parametrach podanych przez użytkownika.
 * Wydłuża drogę krajową, do miasta o nazwie podanej przez użytkownika, drogą o
 * podanych parametrach. Jeżeli droga między miastami już istnieje, to jeśli podany
 * rok jest wcześniejszy, to remontuje ją.
 * Jeżeli takie miasto lub droga nie istnieje, to tworzy je.
 * @param[in,out] map    – wskaźnik na strukturę przechowującą mapę dróg;
 * @param[in] routeId    – numer drogi krajowej;
 * @param[in] length     – podana długość odcinka drogi
 * @param[in] year       – podany rok budowy lub ostatniego remontu
 * @param[in] destination– wskaźnik na napis reprezentujący nazwę miasta docelowego;
 * @return Wartosć @p true, jeśli droga krajowa została wydłużona.
 * Wartość @p false, jeśli wystąpił błąd: któryś z parametrów ma niepoprawną
 * wartość, istnieje już droga pomiędzy tymi miastami o innej długości albo
 * młodszym roku budowy, lub nie udało się zaalokować pamięci.
 */
bool extendCustomRoute(Map *map, unsigned routeId, unsigned length, int year, const char *destination);

/**
 * @brief Usuwa drogę krajową
 * Usuwa z mapy dróg drogę krajową o podanym numerze. Nie usuwa żadnych odcinków
 * dróg, ani miast. Jeżeli nie ma takiej drogi, nic nie robi.
 * @param map            - wskaźnik na strukturę przechowującą mapę dróg
 * @param routeId        - numer drogi krajowej do usunięcia
 * @return Wartość @p true, jeśli droga krajowa została poprawnie usunięta
 * Wartość @p false jeśli droga krajowa o podanym numerze nie istnieje lub
 * podany numer jest niepoprawny.
 */
bool removeRoute(Map *map, unsigned routeId);
#endif /* __MAP_H__ */
