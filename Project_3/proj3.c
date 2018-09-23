/**
 * Kostra programu pro 3. projekt IZP 2017/18
 *
 * Jednoducha shlukova analyza
 * Unweighted pair-group average
 * https://is.muni.cz/th/172767/fi_b/5739129/web/web/usrov.html
 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h> // sqrtf
#include <limits.h>


/*****************************************************************
 * Ladici makra. Vypnout jejich efekt lze definici makra
 * NDEBUG, napr.:
 *   a) pri prekladu argumentem prekladaci -DNDEBUG
 *   b) v souboru (na radek pred #include <assert.h>
 *      #define NDEBUG
 */
#ifdef NDEBUG
#define debug(s)
#define dfmt(s, ...)
#define dint(i)
#define dfloat(f)
#else

// vypise ladici retezec
#define debug(s) printf("- %s\n", s)

// vypise formatovany ladici vystup - pouziti podobne jako printf
#define dfmt(s, ...) printf(" - "__FILE__":%u: "s"\n",__LINE__,__VA_ARGS__)

// vypise ladici informaci o promenne - pouziti dint(identifikator_promenne)
#define dint(i) printf(" - " __FILE__ ":%u: " #i " = %d\n", __LINE__, i)

// vypise ladici informaci o promenne typu float - pouziti
// dfloat(identifikator_promenne)
#define dfloat(f) printf(" - " __FILE__ ":%u: " #f " = %g\n", __LINE__, f)

#endif

/*****************************************************************
 * Deklarace potrebnych datovych typu:
 *
 * TYTO DEKLARACE NEMENTE
 *
 *   struct obj_t - struktura objektu: identifikator a souradnice
 *   struct cluster_t - shluk objektu:
 *      pocet objektu ve shluku,
 *      kapacita shluku (pocet objektu, pro ktere je rezervovano
 *          misto v poli),
 *      ukazatel na pole shluku.
 */

struct obj_t {
    int id;
    float x;
    float y;
};

struct cluster_t {
    int size;
    int capacity;
    struct obj_t *obj;
};

/*****************************************************************
 * Deklarace potrebnych funkci.
 *
 * PROTOTYPY FUNKCI NEMENTE
 *
 * IMPLEMENTUJTE POUZE FUNKCE NA MISTECH OZNACENYCH 'TODO'
 *
 */

/*
 * Inicializace shluku 'c'. Alokuje pamet pro cap objektu (kapacitu).
 * Ukazatel NULL u pole objektu znamena kapacitu 0.
 *
 * @ post
 * Zhluk c bude mať alokovanú pamäť, veľkosť bude nastavená
 * na 0 a kapacita na 1. Ak bude alokácia neúspešná, ukazateľ
 * bude NULL a kapacita 0.
 *
 * @param c zhluk, ktorý bude inicializovaný
 * @param cap začiatočná kapacita
*/
void init_cluster(struct cluster_t *c, int cap) {
    assert(c != NULL);
    assert(cap >= 0);

    c->size = 0;    // default size for every cluster is 0 at beginning
    unsigned int howmuchObject = sizeof(struct obj_t) * cap;   // size of struct_obj_t * cap

    c->obj = (struct obj_t *) malloc(howmuchObject);            // allocation of memory
    if (c->obj == NULL) {                                       // checks whether allocation was successful
        fprintf(stderr, "Allocation of memory failed\n");
        c->capacity = 0;                                        // if not, capacity is set to 0
        return;
    }
    c->capacity = cap;                                          // if yes, capacity is set to value of cap
}

/*
 * Odstraneni vsech objektu shluku a inicializace na prazdny shluk.
 *
 * @pre
 * Zhluk c má alokovanú pamäť.
 *
 * @post
 * Alokovaná pamäť pre všetky objekty zhluku bude uvoľnená,
 * veľkosť aj kapacita bude nastavená na 0.
 *
 * @param c zhluk, ktorého pamäť má byť uvoľnená
 */
void clear_cluster(struct cluster_t *c) {
    free(c->obj);       // frees pointer at struct obj_t
    c->size = 0;        // size and capacity are set to 0
    c->capacity = 0;

}

/// Chunk of cluster objects. Value recommended for reallocation.
const int CLUSTER_CHUNK = 10;

/*
 *Zmena kapacity shluku 'c' na kapacitu 'new_cap'.
 *
 * @post
 * Kapacita zhluku c bude zmenená z pôvodnej kapacity na kapacitu
 * s veľkosťou new_cap (ak nastane chyba, vráti sa NULL).
 *
 * @param c zhluk, ktorého veľkosť bude zväčšená na veľkosť new_cap
 * @param new_cap nová kapacita zhluku c
 * @return zhluk s novou kapacitou (ak nastane chyba, vráti NULL)
 */
struct cluster_t *resize_cluster(struct cluster_t *c, int new_cap) {
    // TUTO FUNKCI NEMENTE
    assert(c);
    assert(c->capacity >= 0);
    assert(new_cap >= 0);

    if (c->capacity >= new_cap)
        return c;

    size_t size = sizeof(struct obj_t) * new_cap;
    void *arr = realloc(c->obj, size);
    if (arr == NULL)
        return NULL;

    c->obj = (struct obj_t *) arr;
    c->capacity = new_cap;
    return c;
}

/*
 * Prida objekt 'obj' na konec shluku 'c'. Rozsiri shluk, pokud se do nej objekt
 * nevejde.
 *
 * @pre
 * Veľkosť zhluku c bude menšia ako kapacita zhluku c, aby sa tam
 * mohol pridať objekt obj.
 *
 * @post
 * Na poslednom mieste zhluku c bude objekt obj ak nenastane chyba
 * pri realokácii.
 *
 * @param c zhluk, do ktorého sa pridá objekt
 * @param obj objekt, ktorý bude pridaný do zhluku c
 *
 */
void append_cluster(struct cluster_t *c, struct obj_t obj) {

    if(c->size >= c->capacity)
    {
        c=resize_cluster(c, c->capacity+CLUSTER_CHUNK); // allocates more memory
        if(c==NULL)                                     // checks whether allocation was successful
        {
            fprintf(stderr, "Failed at allocating more memory\n");
            return;
        }
    }
    c->obj[c->size] = obj;                  // saves object at the end of cluster
    c->size++;                              // current size is incremented
}

/*
 Seradi objekty ve shluku 'c' vzestupne podle jejich identifikacniho cisla.
 */
void sort_cluster(struct cluster_t *c);

/*
 * Do shluku 'c1' prida objekty 'c2'. Shluk 'c1' bude v pripade nutnosti rozsiren.
 * Objekty ve shluku 'c1' budou serazeny vzestupne podle identifikacniho cisla.
 * Shluk 'c2' bude nezmenen.
 *
 * @pre
 * V zhluku c2 sa bude nachádzať aspoň jedeň zhluk.
 *
 * @post
 * Zhluk c1 bude rozšírený o objekty zhluku c2 ak
 * nenastane chyba v alokácii.
 *
 * @param c1 zhluk, do ktorého budú pridané objekty zhluku c2
 * @param c2 zhluk, ktorého objekty budú pridané do zhluku c1
 *
 */
void merge_clusters(struct cluster_t *c1, struct cluster_t *c2) {
    assert(c1 != NULL);
    assert(c2 != NULL);

    for (int i = 0; i < c2->size; i++) {  // will append all objects in cluster c2
        if ((c2->size + c1->size) >= c1->capacity) {    // checks whether capacity is sufficient
            c1 = resize_cluster(c1, c1->capacity + CLUSTER_CHUNK);  // if not, resizes
            if (c1 == NULL) {              // checks whether resizing was successful
                fprintf(stderr, "Failed to allocate more memory\n");
                return;
            }
        }
        append_cluster(c1, c2->obj[i]); // appends all objects of c2 to c1
    }
    sort_cluster(c1);                   // sorts cluster
}

/**********************************************************************/
/* Prace s polem shluku */

/*
 * Odstrani shluk z pole shluku 'carr'. Pole shluku obsahuje 'narr' polozek
 * (shluku). Shluk pro odstraneni se nachazi na indexu 'idx'. Funkce vraci novy
 * pocet shluku v poli.
 *
 * @post
 * Z pola zhlukov carr bude odstránený prvok, ktorý ma index idx.
 * Počet prvkov pola carr sa zmenší o 1.
 *
 * @param carr pole zhluku
 * @param narr počet zhlukov pola
 * @param idx index zhluku, ktorý má byť odstránený
 * @return nový počet zhlukov v poli
*/
int remove_cluster(struct cluster_t *carr, int narr, int idx) {
    assert(idx < narr);
    assert(narr > 0);

    struct cluster_t tempCluster;
    int tempNarr = narr - 1;                // help variable
    tempCluster = carr[idx];                // element to be removed is temporarily saved in tempCluster

    for(int i = idx; i < tempNarr; i++){    // elements in between are moved
        int next = i+1;
        carr[i] = carr[next];
    }

    carr[tempNarr] = tempCluster;           // element to be removed is moved to the last place
    clear_cluster(&carr[tempNarr]);         // clears the last cluster

    return tempNarr;
}

/*
 * Pocita Euklidovskou vzdalenost mezi dvema objekty.
 *
 * @param o1 objekt
 * @param o2 objekt
 * @return Euklidovská vzdialenosť medzi objektom o1 a objektom o2
 */
float obj_distance(struct obj_t *o1, struct obj_t *o2) {
    assert(o1 != NULL);
    assert(o2 != NULL);

    double xDif, yDif;      // variables for difference
    float result;           // variable for result of the calculation
    xDif = o1->x - o2->x;   // difference between x coordinates
    yDif = o1->y - o2->y;   // difference between y coordinates

    result = sqrtf((xDif * xDif) + (yDif * yDif));
    return result;          // returns the distance (size of vector)
}

/*
 * Pocita vzdalenost dvou shluku.
 *
 * @pre
 * Zhluky c1 a c2 musia obsahovať aspoň jeden objekt.
 *
 * @param c1 zhluk
 * @param c2 zhluk
 * @return vracia vzdialenosť medzi zhlukmi c1 a c2
*/
float cluster_distance(struct cluster_t *c1, struct cluster_t *c2)
{
    assert(c1 != NULL);
    assert(c1->size > 0);
    assert(c2 != NULL);
    assert(c2->size > 0);

    float distance;
    struct obj_t f1, f2;    // temporary structures for coordinates
    f1.x = 0;               // default temporary coordinates
    f1.y = 0;
    f2.x = 0;
    f2.y = 0;

    if (c1->size > 1){  // calculates average coordinates for temporary structure f1
        for(int i = 0; i < c1->size; i++) {
            f1.x += (c1->obj[i].x); // adds up all x coordinates
            f1.y += (c1->obj[i].y); // adds up all y coordinates
        }
        f1.x = f1.x/c1->size;   // divides them by amount
        f1.y = f1.y/c1->size;
    }else{
        f1.x = c1->obj->x;  // if c1->size is 1, averages is not calculated
        f1.y = c1->obj->y;
    }

    if (c2->size > 1){  // calculates average coordinates for temporary structure f2
        for(int j = 0; j < c2->size; j++) {
            f2.x += (c2->obj[j].x); // adds up all x coordinates
            f2.y += (c2->obj[j].y); // adds up all y coordinates
        }
        f2.x = f2.x/c2->size;   // divides them by amount
        f2.y = f2.y/c2->size;
    } else {
        f2.x = c2->obj->x;  // if c2->size is 1, average is not calculated
        f2.y = c2->obj->y;
    }

    distance = obj_distance(&f1, &f2);  // calculates the distance between 2 structures
    return distance;                    // returns smallest distance
}

/*
 * Funkce najde dva nejblizsi shluky. V poli shluku 'carr' o velikosti 'narr'
 * hleda dva nejblizsi shluky. Nalezene shluky identifikuje jejich indexy v poli
 * 'carr'. Funkce nalezene shluky (indexy do pole 'carr') uklada do pameti na
 * adresu 'c1' resp. 'c2'.
 *
 * @post
 * Indexy dvoch najbližších zhlukov budú uložené do c1 a c2.
 *
 * @param carr pole zhluku
 * @param narr veľkosť zhluku carr
 * @param c1 index prvého nájdeného zhluku
 * @param c2 infex druhého nájdeného zhluku
*/
void find_neighbours(struct cluster_t *carr, int narr, int *c1, int *c2) {
    assert(narr > 0);

    float smallestDistance = INT_MAX;       // smallest distance is set to the highest possible integer
    float distance;                         // variable that saves calculated distance

    for (int i = 0; i < (narr-1); i++) {                            // rows of the matrix
        for (int j = (i + 1); j < narr; j++) {                      // columns of the matrix, j can't equal i because size would be 0
            if (&carr[i] && &carr[j]) {
                distance = cluster_distance(&carr[i], &carr[j]);    // calculates cluster distance
                if (distance < smallestDistance) {
                    smallestDistance = distance;                    // smallest distance is saved in distance
                    *c1 = i;                                        // index of the smallest distance points are saved
                    *c2 = j;
                }
            }
        }
    }
}

// pomocna funkce pro razeni shluku
static int obj_sort_compar(const void *a, const void *b) {
    // TUTO FUNKCI NEMENTE
    const struct obj_t *o1 = (const struct obj_t *) a;
    const struct obj_t *o2 = (const struct obj_t *) b;
    if (o1->id < o2->id) return -1;
    if (o1->id > o2->id) return 1;
    return 0;
}

/*
 * Razeni objektu ve shluku vzestupne podle jejich identifikatoru.
 *
 * @post
 * Prvky zhluku c budú zoradené vzostupne podľa identifikátora
 *
 * @param c zhluk, ktorého prvky budú zoradené
*/
void sort_cluster(struct cluster_t *c) {
    // TUTO FUNKCI NEMENTE
    qsort(c->obj, c->size, sizeof(struct obj_t), &obj_sort_compar);
}

/*
 * Tisk shluku 'c' na stdout.
 *
 * @post
 * Objekty zhluku c budú vypísané na stdout.
 *
 * @param c zhluk, ktorý má byť vypísaný
*/
void print_cluster(struct cluster_t *c) {
    // TUTO FUNKCI NEMENTE
    for (int i = 0; i < c->size; i++) {
        if (i) putchar(' ');
        printf("%d[%g,%g]", c->obj[i].id, c->obj[i].x, c->obj[i].y);
    }
    putchar('\n');
}

/*
 Ze souboru 'filename' nacte objekty. Pro kazdy objekt vytvori shluk a ulozi
 jej do pole shluku. Alokuje prostor pro pole vsech shluku a ukazatel na prvni
 polozku pole (ukalazatel na prvni shluk v alokovanem poli) ulozi do pameti,
 kam se odkazuje parametr 'arr'. Funkce vraci pocet nactenych objektu (shluku).
 V pripade nejake chyby uklada do pameti, kam se odkazuje 'arr', hodnotu NULL.
 *
 * @pre
 * Bude existovať súbor "filename", z ktorého bude možné čítať objekty.
 * Údaje v súbore budú v správnom formáte.
 *
 * @ post
 * Bude alokované pole arr pre všetky zhluky ak nenastane chyba. Následne budú
 * alokované prvky pola, ktoré budú uložené v poli zhlukov arr.
 *
 * @param filename názov súboru pre načítanie objektov
 * @param arr ukazateľ na pole zhlukov (vráti NULL ak sa alokovanie nepodarilo)
 * @return počet načítaných zhlukov (vráti -1 ak sa niečo nepodarilo vykonať správne)
 *
*/
int load_clusters(char *filename, struct cluster_t **arr) {
    FILE *objects;
    int count = 0;
    struct obj_t instance;  // help variable
    int initCapacity = 1;   // starting capacity

    assert(arr != NULL);

    objects = fopen(filename, "r");                   // opens the text file
    if (objects == NULL) {                            // checks whether opening was successful
        fprintf(stderr, "File failed to open\n");
        return -1;
    }

    fscanf(objects, "count=%d", &count);            // reads how many clusters should be created

    *arr = malloc(count * sizeof(struct cluster_t));    // allocates memory for all clusters
    if (*arr == NULL) {
        fprintf(stderr, "Failed at allocating memory\n");
        return -1;
    }

    for (int i = 0; i < count; i++) // initializes memory for all elements
        init_cluster(&(*arr)[i], initCapacity);

    for (int i = 0; i < count; i++)  // reads the file from 1 to (count+1) row
    {
        fscanf(objects, "%d %f %f", &instance.id, &instance.x, &instance.y); //
        if (!(instance.x >= 0 && instance.x <= 1000) || !(instance.y >= 0 && instance.y <= 1000)) {
            fprintf(stderr, "Coordinates of x or y is outside the acceptable range\n");     //check for acceptable range of coordinates
            for (int j = 0; j < count; j++) {                   // frees all elements of cluster
                clear_cluster(arr[i]);
            }
            free(arr);
            fclose(objects);
            return -1;
        }
        append_cluster(&(*arr)[i], instance);               // appends cluster
    }
    if (fclose(objects) == EOF) {                           // closes the files and checks if it was successful
        fprintf(stderr, "File failed to close\n");
        for (int i = 0; i < count; i++) {                   // frees all elements of cluster
            clear_cluster(arr[i]);
        }
        free(arr);
        return -1;
    }

    return count;                                           // returns count from the file
}

/*
 * Tisk pole shluku. Parametr 'carr' je ukazatel na prvni polozku (shluk).
 * Tiskne se prvnich 'narr' shluku.
 *
 * @post
 * Na stdout bude vypísaných prvých narr prvkov zhluku carr
 *
 * @param carr ukazateľ na prvú položku zhluku
 * @param narr počet prvých narr prvkoch zhluku, ktoré budú vypísané
*/
void print_clusters(struct cluster_t *carr, int narr) {
    printf("Clusters:\n");
    for (int i = 0; i < narr; i++) {
        printf("cluster %d: ", i);
        print_cluster(&carr[i]);
    }
}

int main(int argc, char *argv[]) {
    struct cluster_t *clusters;     // pointer at struct clusters
    int clt;                        // how many clusters should be present (only positive number)
    char *filename;                 // name of the file containing objects
    int count;                      // how many clusters are present in file (count=%d)
    int c1, c2;                     // help variables

    if (argc != 2 && argc != 3) {   // checks for amount of arguments
        fprintf(stderr, "Invalid amount of arguments\n");
        return 1;
    }

    if (argv[1])                // checks whether argument is present
        filename = argv[1];     // name of file is loaded into filename
    else {                      // if argument isn't present, programme exits
        fprintf(stderr, "Missing text file for loading arguments\n");
        return 1;
    }

    if (argv[2]) {              // checks whether argument is present
        clt = atoi(argv[2]);
        if (clt <= 0) {         // checks whether argument is positive
            fprintf(stderr, "Invalid cluster argument. The number must be positive\n");
            return 1;
        }
    } else                                          // default number of clusters is 1 if no argument entered
        clt = 1;

    count = load_clusters(filename, &clusters);     // loads the clusters and returns how many were loaded

    if (clt == count) {   // number of clusters in file equals number of clustered entered in argument
        print_clusters(clusters, count);            // cluster are printed from 0 to (count-1)
        for (int i = 0; i < count; i++) {           // frees all elements of clusters
            clear_cluster(&clusters[i]);
        }
        free(clusters);                             // frees clusters and exits
        return 0;
    } else if (count == -1) {                       // error in load_clusters
        return 1;
    } else if (count != -1 && clt > count) { // there are more required clusters than there really are
        fprintf(stderr, "Required amount of clusters is higher than amount of all objects\n");
        for (int i = 0; i < count; i++) {                   // frees elements of cluster
            clear_cluster(&clusters[i]);
        }
        free(clusters);                                     // frees clusters and exits
        return 1;
    } else if (clt < count) {   // argument is lower than the count, clusters will be merged
        while (count != clt) {
            find_neighbours(clusters, count, &c1, &c2);     // finds the closes neighbours
            merge_clusters(&clusters[c1], &clusters[c2]);   // merges the 2 closest clusters
            count = remove_cluster(clusters, count, c2);    // removes the second merged cluster
        }
        print_clusters(clusters, clt);                      // prints all clusters
        for (int i = 0; i < count; i++) {   // frees elements of cluter
            if(&clusters[i])
                clear_cluster(&clusters[i]);
        }
        free(clusters);     // frees cluster and exits
    }

    return 0;
}

