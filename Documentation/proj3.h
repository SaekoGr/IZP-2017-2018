/**
 * Kostra hlavickoveho souboru 3. projekt IZP 2017/18
 * a pro dokumentaci Javadoc.
 */

/**
 * @brief Struktura, ktora obsahuje objekty charakterizovane identifikatorom a suradnicami
 */
struct obj_t {
    int id;     ///< identifikacne cislo objektu
    float x;    ///< suradnica osi x
    float y;    ///< suradnica osi y
};

/**
 * @brief Struktura, ktora obsahuje zhluky s urcitou velkostou, kapacitou a ukazatelom na adresu objektov
 */
struct cluster_t {
    int size;   ///< velkost zhluku - sucasny pocet objektov v zhluku
    int capacity;   ///< kapacita zhluku - najvasci mozny pocet objektov v zhluku
    struct obj_t *obj;  ///< pole objektov zhluku
};

/**
 * @defgroup ZHLUK Praca so zhlukmi
 * @brief Praca s jednotlivými zhlukmi
 * @{
 */

/**
 * @brief Inicializace shluku 'c'. Alokuje pamet pro cap objektu (kapacitu). Ukazatel NULL u pole objektu znamena kapacitu 0.
 *
 * @post Zhluk c bude mat alokovanu pamat, velkost bude nastavena na 0 a kapacita na 1. Ak bude alokacia neuspesna, ukazatel bude NULL a kapacita 0.
 *
 * @param c zhluk, ktory bude inicializovany
 * @param cap zaciatocna kapacita
 */
void init_cluster(struct cluster_t *c, int cap);

/**
 * @brief Odstraneni vsech objektu shluku a inicializace na prazdny shluk.
 *
 * @pre Zhluk c ma alokovanu pamat.
 *
 * @post Alokovana pamat pre vsetky objekty zhluku bude uvolnena, velkost aj kapacita bude nastavena na 0.
 *
 * @param c zhluk, ktoreho pamat ma byt uvolnena
 */
void clear_cluster(struct cluster_t *c);

/**
 * @brief Doporucena hodnota pre realokaciu zhluku
 */
extern const int CLUSTER_CHUNK;

/**
 * @brief Zmena kapacity shluku 'c' na kapacitu 'new_cap'.
 *
 * @post Kapacita zhluku c bude zmenena z povodnej kapacity na kapacitu s velkostou new_cap (ak nastane chyba, vrati sa NULL).
 *
 * @param c zhluk, ktoreho velkost bude zvacsena na velkost new_cap
 * @param new_cap nova kapacita zhluku c
 * @return zhluk s novou kapacitou (ak nastane chyba, vrati NULL)
 */
struct cluster_t *resize_cluster(struct cluster_t *c, int new_cap);

/**
 * @brief Prida objekt 'obj' na konec shluku 'c'. Rozsiri shluk, pokud se do nej objekt
 * nevejde.
 *
 * @pre Velkost zhluku c bude mensia ako kapacita zhluku c, aby sa tam mohol pridat objekt obj.
 *
 * @post Na poslednom mieste zhluku c bude objekt obj ak nenastane chyba pri realokacii.
 *
 * @param c zhluk, do ktoreho sa prida objekt
 * @param obj objekt, ktory bude pridany do zhluku c
 *
 */
void append_cluster(struct cluster_t *c, struct obj_t obj);

/**
 * @brief Do shluku 'c1' prida objekty 'c2'. Shluk 'c1' bude v pripade nutnosti rozsiren. Objekty ve shluku 'c1' budou serazeny vzestupne podle identifikacniho cisla. Shluk 'c2' bude nezmenen.
 *
 * @pre V zhluku c2 sa bude nachadzat aspoň jedeň zhluk.
 *
 * @post Zhluk c1 bude rozsireny o objekty zhluku c2 ak nenastane chyba v alokacii.
 *
 * @param c1 zhluk, do ktoreho budu pridane objekty zhluku c2
 * @param c2 zhluk, ktoreho objekty budu pridane do zhluku c1
 *
 */
void merge_clusters(struct cluster_t *c1, struct cluster_t *c2);

/**
 * @brief Odstrani shluk z pole shluku 'carr'. Pole shluku obsahuje 'narr' polozek (shluku). Shluk pro odstraneni se nachazi na indexu 'idx'. Funkce vraci novy pocet shluku v poli.
 *
 * @post Z pola zhlukov carr bude odstraneny prvok, ktory ma index idx. Pocet prvkov pola carr sa zmensi o 1.
 *
 * @param carr pole zhluku
 * @param narr pocet zhlukov pola
 * @param idx index zhluku, ktory ma byt odstraneny
 * @return novy pocet zhlukov v poli
 */

/**
 * @}
 */

/**
 * @defgroup PRVKY Praca s polom zhluku
 * @brief Praca s jednotlivymi prvkami zhluku
 * @{
 */

/**
 * @brief Odstrani shluk z pole shluku 'carr'. Pole shluku obsahuje 'narr' polozek (shluku). Shluk pro odstraneni se nachazi na indexu 'idx'. Funkce vraci novy pocet shluku v poli.
 *
 * @post Z pola zhlukov carr bude odstránený prvok, ktorý ma index idx. Počet prvkov pola carr sa zmenší o 1.
 *
 * @param carr pole zhluku
 * @param narr počet zhlukov pola
 * @param idx index zhluku, ktorý má byť odstránený
 * @return nový počet zhlukov v poli
 */
int remove_cluster(struct cluster_t *carr, int narr, int idx);

/**
 * Pocita Euklidovskou vzdalenost mezi dvema objekty.
 *
 * @param o1 objekt
 * @param o2 objekt
 * @return Euklidovska vzdialenost medzi objektom o1 a objektom o2
 */
float obj_distance(struct obj_t *o1, struct obj_t *o2);

/**
 * @brief Pocita vzdalenost dvou shluku.
 *
 * @pre
 * Zhluky c1 a c2 musia obsahovat aspoň jeden objekt.
 *
 * @param c1 zhluk
 * @param c2 zhluk
 * @return vracia vzdialenost medzi zhlukmi c1 a c2
 */
float cluster_distance(struct cluster_t *c1, struct cluster_t *c2);

/**
 * @brief Funkce najde dva nejblizsi shluky. V poli shluku 'carr' o velikosti 'narr'
 * hleda dva nejblizsi shluky. Nalezene shluky identifikuje jejich indexy v poli
 * 'carr'. Funkce nalezene shluky (indexy do pole 'carr') uklada do pameti na
 * adresu 'c1' resp. 'c2'.
 *
 * @post
 * Indexy dvoch najblizsich zhlukov budu ulozene do c1 a c2.
 *
 * @param carr pole zhluku
 * @param narr velkost zhluku carr
 * @param c1 index prveho najdeneho zhluku
 * @param c2 infex druheho najdeneho zhluku
 */
void find_neighbours(struct cluster_t *carr, int narr, int *c1, int *c2);

/**
 * @brief Razeni objektu ve shluku vzestupne podle jejich identifikatoru.
 *
 * @post
 * Prvky zhluku c budu zoradene vzostupne podla identifikatora
 *
 * @param c zhluk, ktoreho prvky budu zoradene
 */
void sort_cluster(struct cluster_t *c);

/**
 * @brief Tisk shluku 'c' na stdout.
 *
 * @post
 * Objekty zhluku c budu vypisane na stdout.
 *
 * @param c zhluk, ktory ma byt vypisany
 */
void print_cluster(struct cluster_t *c);

/**
 * @brief Ze souboru 'filename' nacte objekty. Pro kazdy objekt vytvori shluk a ulozi jej do pole shluku. Alokuje prostor pro pole vsech shluku a ukazatel na prvni polozku pole (ukalazatel na prvni shluk v alokovanem poli) ulozi do pameti, kam se odkazuje parametr 'arr'. Funkce vraci pocet nactenych objektu (shluku). V pripade nejake chyby uklada do pameti, kam se odkazuje 'arr', hodnotu NULL.
 *
 * @pre Bude existovat subor "filename", z ktoreho bude mozne citat objekty. Udaje v subore budu v spravnom formate.
 *
 * @post Bude alokovane pole arr pre vsetky zhluky ak nenastane chyba. Nasledne budu alokovane prvky pola, ktore budu ulozene v poli zhlukov arr.
 *
 * @param filename nazov suboru pre nacitanie objektov
 * @param arr ukazatel na pole zhlukov (vrati NULL ak sa alokovanie nepodarilo)
 * @return pocet nacitanych zhlukov (vrati -1 ak sa nieco nepodarilo vykonat spravne)
 *
 */
int load_clusters(char *filename, struct cluster_t **arr);

/**
 * @brief Tisk pole shluku. Parametr 'carr' je ukazatel na prvni polozku (shluk). Tiskne se prvnich 'narr' shluku.
 *
 * @post
 * Na stdout bude vypisanych prvych narr prvkov zhluku carr
 *
 * @param carr ukazatel na prvu polozku zhluku
 * @param narr pocet prvych narr prvkoch zhluku, ktore budu vypisane
 */
void print_clusters(struct cluster_t *carr, int narr);

/**
 * @}
 */
