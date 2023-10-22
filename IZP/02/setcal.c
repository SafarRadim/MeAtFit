/* 
    Name: IZP Project 2 - Set Calculator
    Authors: Daniel Pindur, Radim Safar, Jan Usak
*/
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>
#include <errno.h>
#include <limits.h>

#define DEC_BASE 10
#define IS_ALPHA(X) ((X >= 65 && X <= 90) || (X >= 97 && X <= 122))

#define DYNAMIC_ARRAY_DEC(T, TNAME)                                             \
    typedef struct {                                                            \
        T* data;                                                                \
        int len;                                                                \
    } DA_##TNAME##_T;                                                           \
                                                                                \
void DA_##TNAME##_Init(DA_##TNAME##_T* array);                                  \
int DA_##TNAME##_Insert(DA_##TNAME##_T* array, T item);                         \
void DA_##TNAME##_Free(DA_##TNAME##_T* array);

#define DYNAMIC_ARRAY_DEF(T, TNAME)                                             \
void DA_##TNAME##_Init(DA_##TNAME##_T* array) {                                 \
    array->data = NULL;                                                         \
    array->len = 0;                                                             \
}                                                                               \
                                                                                \
int DA_##TNAME##_Insert(DA_##TNAME##_T* array, T item) {                        \
    array->len++;                                                               \
    array->data = (T*) realloc(array->data, array->len * sizeof(T));            \
    if (!array->data) {                                                         \
        fprintf(stderr, "ERROR: Error during realloc of dynamic array!\n");     \
        return EXIT_FAILURE;                                                    \
    }                                                                           \
    array->data[array->len - 1] = item;                                         \
    return EXIT_SUCCESS;                                                        \
}                                                                               \
void DA_##TNAME##_Free(DA_##TNAME##_T* array) {                                 \
    for (int i = 0; i < array->len; i++) {                                      \
        free(array->data[i]);                                                   \
    }                                                                           \
    free(array->data);                                                          \
    DA_##TNAME##_Init(array);                                                   \
}                            

// --------------------------
#define LIST_DEC(T, TNAME)                                                      \
   typedef struct list_##TNAME##_elem {                                         \
      struct list_##TNAME##_elem* next;                                         \
      T data;                                                                   \
   } L_##TNAME##_Elem;                                                          \
                                                                                \
   typedef struct {                                                             \
        L_##TNAME##_Elem* first;                                                \
    } LIST_##TNAME##_T;                                                         \
                                                                                \
void LIST_##TNAME##_Init(LIST_##TNAME##_T* list);                               \
int LIST_##TNAME##_InsertFirst(LIST_##TNAME##_T* list, T item);                 \
void LIST_##TNAME##_Free(LIST_##TNAME##_T* list);

#define LIST_DEF(T, TNAME)                                                      \
void LIST_##TNAME##_Init(LIST_##TNAME##_T* list) {                              \
    list->first = NULL;                                                         \
}                                                                               \
                                                                                \
int LIST_##TNAME##_InsertFirst(LIST_##TNAME##_T* list, T elem) {                \
    L_##TNAME##_Elem* new_elem = (L_##TNAME##_Elem*) malloc(sizeof(L_##TNAME##_Elem)); \
                                                                                \
    if (!new_elem) {                                                            \
        fprintf(stderr, "ERROR: Error during alloc fo List element");           \
        return EXIT_FAILURE;                                                    \
    }                                                                           \
                                                                                \
    new_elem->data = elem;                                                      \
    new_elem->next = list->first;                                               \
    list->first = new_elem;                                                     \
                                                                                \
    return EXIT_SUCCESS;                                                        \
}                                                                               \
                                                                                \
void LIST_##TNAME##_Free(LIST_##TNAME##_T* list) {                              \
    while (list->first) {                                                       \
        L_##TNAME##_Elem* to_remove = list->first;                              \
        list->first = to_remove->next;                                          \
        free(to_remove);                                                        \
    }                                                                           \
    free(list);                                                                 \
}     

// -------------------------
//      Declarations
// -------------------------
#define MAX_STRING_LEN 31
#define MAX_COMMAND_LEN 21

const int banned_words_count = 21;
const char* banned_words[] = {
    "true",
    "false",
    "empty",
    "card",
    "complement",
    "union",
    "intersect",
    "minus",
    "subseteq",
    "subset",
    "equals",
    "reflexive",
    "symmetric",
    "antisymmetric",
    "transitive",
    "function",
    "domain",
    "codomain",
    "injective",
    "surjective",
    "bijective"
};

static int line_number = 1;

typedef struct {
    int x, y;
} Pair;

struct set_s;
struct relation_s;

DYNAMIC_ARRAY_DEC(char*, Lines)
DYNAMIC_ARRAY_DEF(char*, Lines)
DYNAMIC_ARRAY_DEC(char*, Universum)
DYNAMIC_ARRAY_DEF(char*, Universum)
LIST_DEC(int, Number)
LIST_DEF(int, Number)
DYNAMIC_ARRAY_DEC(struct set_s*, Sets)
DYNAMIC_ARRAY_DEF(struct set_s*, Sets)
LIST_DEC(Pair, Pairs)
LIST_DEF(Pair, Pairs)
DYNAMIC_ARRAY_DEC(struct relation_s*, Relations)
DYNAMIC_ARRAY_DEF(struct relation_s*, Relations)

typedef struct set_s {
    LIST_Number_T* Set;
    int line_number;
} Set_T;

typedef struct relation_s {
   LIST_Pairs_T* Relation;
   int line_number; 
} Relation_T;

DA_Sets_T SetsGarbageCollection;
DA_Relations_T RelationsGarbageCollection;


// -------------------------

/**
 * @brief Searches given Universum array for specified string element.
 * 
 * @param array Universum array
 * @param elem string element to be converted to index in array
 * @return int - position index of specified string element in Universum array, if not found returns -1
 */
int UniversumSearch(DA_Universum_T* array, char* elem) {
    for (int i = 0; i < array->len; i++) {
        if (!strcmp(array->data[i], elem)) {
            return i;
        }
    }
    return -1;
}


/**
 * @brief Checks if current character is followed by space
 * 
 * @param line currently parsed line
 * @param current_char currently parsed character
 * @return true if character is followed by space else false
 */
bool IsFollowedBySpace(char* line, int* current_char) {
    if (line[*current_char] != ' ') {
        return false;
    }

    (*current_char)++;
    return true;
}

/**
 * @brief Searches selected Set List for specified element
 *
 * @param set List containing set elements
 * @param elem Set element to be looked up in the list
 * @return true if element has been found in the Set List otherwise false 
 */
bool SetSearch(LIST_Number_T* set, int elem) {
    L_Number_Elem* current_ptr = set->first;

    while (current_ptr) {
        if (current_ptr->data == elem) {
            return true;
        }
        current_ptr = current_ptr->next;
    }

    return false;
}


/**
 * @brief Searches selected Relation List for specified element Pair
 * 
 * @param relation List containing relation pairs
 * @param elem Pair to be looked up in the list
 * @return true if element has been found in the Set List otherwise false 
 */
bool PairSearchByPair(LIST_Pairs_T* relation, Pair elem) {
    L_Pairs_Elem* current_ptr = relation->first;

    while (current_ptr) {
        if (current_ptr->data.x == elem.x && current_ptr->data.y == elem.y) {
            return true;
        }
        current_ptr = current_ptr->next;
    }
    return false;
}

/**
 * @brief Searches selected Relation List for element specified by its x andy y portion
 * 
 * @param relation List containing relation pairs
 * @param x first element in pair
 * @param y second element in pair
 * @return true if element has been found in the Set List otherwise false 
 */
bool PairSearchByXY(LIST_Pairs_T* relation, int x, int y) {
    L_Pairs_Elem* current_ptr = relation->first;

    while (current_ptr) {
        if (current_ptr->data.x == x && current_ptr->data.y == y) {
            return true;
        }
        current_ptr = current_ptr->next;
    }
    return false;
}


/**
 * @brief Create a Universum Set
 * 
 * @param universum Universum array
 * @return LIST_Number_T* Universum set
 */
LIST_Number_T* CreateUniversumSet(DA_Universum_T* universum) {
    LIST_Number_T* universum_set = (LIST_Number_T*) malloc(sizeof(LIST_Number_T));
    if (!universum_set) {
        fprintf(stderr, "ERROR: Failed malloc for Universum Set!\n");
        return NULL;
    }
    LIST_Number_Init(universum_set);

    for (int i = 0; i < universum->len; i++) {
        if (LIST_Number_InsertFirst(universum_set, i) == EXIT_FAILURE) {
            return NULL;
        }
    }

    return universum_set;
}


/**
 * @brief Skips parsing of all whitespace characters following the current character
 * 
 * @param line currently parsed line
 * @param current_char currently parsed character
 */
void SkipWhiteChars(char* line, int* current_char) {
    while (line[*current_char] == ' ' || line[*current_char] == '\t') {
        (*current_char)++;
    }
}


/**
 * @brief Get the Element object
 * 
 * @param line currently parsed line
 * @param current_char currently parsed character
 * @param new_element_ptr new parsed token
 * @return EXIT_FAILURE if error occured during the processing otherwive returns EXIT_SUCCESS 
 */
int GetElement(char* line, int* current_char, char** new_element_ptr) {
    char* new_element = (char*) malloc(sizeof(char) * MAX_STRING_LEN);
    if (!new_element) {
        fprintf(stderr, "ERROR: Error during malloc for Universum element!\n");
        *new_element_ptr = NULL;
        return EXIT_FAILURE;
    }

    // Skip white chars
    SkipWhiteChars(line, current_char); 

    if (line[*current_char] == '\0') {
        free(new_element);
        *new_element_ptr = NULL;
        return EXIT_SUCCESS;
    }

    for (int i = 0; i < (MAX_STRING_LEN); i++) {
        char new_char = line[*current_char + i];
        
        if (new_char == ' ' || new_char == '\t' || new_char == '\0' || new_char == ')') {
            new_element[i] = '\0';
            (*current_char) += i;
            *new_element_ptr = new_element;
            return EXIT_SUCCESS;
        }

        if (i == MAX_STRING_LEN - 1) {
            break;
        }

        new_element[i] = new_char;
    }

    fprintf(stderr, "ERROR: Element is too long, maximal allowed length is 30 chars!\n");
    free(new_element);
    *new_element_ptr = NULL;
    return EXIT_FAILURE;
}


/**
 * @brief Checks if current token is allowed token
 * 
 * @param element currently processed token
 * @return true is element is banned symbol or does not consist of alphabet characters
 */
bool IsBanned(char* element) {
    int current = 0;

    while (element[current] != '\0') {
        if (!IS_ALPHA(element[current])) {
            return true;
        }
        current++;
    }

    for (int i = 0; i < banned_words_count; i++) {
        if (!strcmp(element, banned_words[i])) {
            return true;
        }
    }

    return false;
}


/**
 * @brief Checks if element is in the Universum array
 * 
 * @param element currently parsed token
 * @param universum Universum array
 * @return true if current element is present in the Universum array else false
 */
bool IsInUniversum(char* element, DA_Universum_T* universum) {
    for (int i = 0; i < universum->len; i++) {
        if (!strcmp(element, universum->data[i])) {
            return true;
        }
    }
    return false;
}


/**
 * @brief Parse line containing Universum definition
 * 
 * @param line currenlty parsed line
 * @param universum Universum array
 * @return EXIT_FAILURE if error occured during the processing otherwive returns EXIT_SUCCESS
 */
int ParseUniversum(char* line, DA_Universum_T* universum) {
    DA_Universum_Free(universum);
    DA_Universum_Init(universum);
    int current_char = 1;

    if (!IsFollowedBySpace(line, &current_char)) {
        if (line[current_char] == '\0') {
            // Empty universum
            return EXIT_SUCCESS;
        }

        fprintf(stderr, "ERROR: Universum identifier U not followed by space on line %d!\n", line_number);
        return EXIT_FAILURE;
    }

    while (true) {
        char* new_element = NULL;
        int exit_code = GetElement(line, &current_char, &new_element);
        if (!new_element) {
            if (exit_code == EXIT_SUCCESS) {
                break;
            }
            else {
                return EXIT_FAILURE;
            }
            
        }

        if (IsBanned(new_element)) {
            fprintf(stderr, "ERROR: Element on line %d contains not allowed characters or is banned word!\n", line_number);
            free(new_element);
            return EXIT_FAILURE;
        }
        
        if (IsInUniversum(new_element, universum)) {
            fprintf(stderr, "ERROR: Element on line %d is alredy in universum!\n", line_number);
            free(new_element);
            return EXIT_FAILURE;
        }

        if (DA_Universum_Insert(universum, new_element) == EXIT_FAILURE) {
            free(new_element);
            return EXIT_FAILURE;   
        }
    }

    return EXIT_SUCCESS;    
}


/**
 * @brief Create an Empty Set
 * 
 * @return LIST_Number_T* empty set
 */
LIST_Number_T* CreateEmptySet() {
    LIST_Number_T* empty_set = (LIST_Number_T*) malloc(sizeof(LIST_Number_T));
    if (!empty_set) {
        fprintf(stderr, "ERROR: Failed malloc for Empty Set!\n");
        return NULL;
    }
    LIST_Number_Init(empty_set);
    return empty_set;
}


/**
 * @brief Create a Empty Relation
 * 
 * @return LIST_Pairs_T* empty relation
 */
LIST_Pairs_T* CreateEmptyRelation() {
    LIST_Pairs_T* empty_relation = (LIST_Pairs_T*) malloc(sizeof(LIST_Pairs_T));
    if (!empty_relation) {
        fprintf(stderr, "ERROR: Failed malloc for Empty Relation!\n");
        return NULL;
    }
    LIST_Pairs_Init(empty_relation);
    return empty_relation;
}


/**
 * @brief Parse line containing Set definition
 * 
 * @param line currently parsed line
 * @param universum Universum array
 * @return LIST_Number_T* new set
 */
LIST_Number_T* ParseSet(char* line, DA_Universum_T* universum) {
    LIST_Number_T* new_set = (LIST_Number_T*) malloc(sizeof(LIST_Number_T));
    LIST_Number_Init(new_set);

    int current_char = 1;

    if (!IsFollowedBySpace(line, &current_char)) {
        LIST_Number_Free(new_set);
        if (line[current_char] == '\0') {
            // Empty set
            return CreateEmptySet();
        }
        fprintf(stderr, "ERROR: Set identifier S not followed by space on line %d!\n", line_number);
        return NULL;
    }

    while (true) {
        char* new_element = NULL;
        int exit_code = GetElement(line, &current_char, &new_element);
        if (!new_element) {
            if (exit_code == EXIT_SUCCESS) {
                break;
            }
            else {
                LIST_Number_Free(new_set);
                return NULL;
            }
        }
        
        int converted = UniversumSearch(universum, new_element);

        if (converted == -1) {
            fprintf(stderr, "ERROR: Element '%s' is not a part of the universum!\n", new_element);
            LIST_Number_Free(new_set);
            free(new_element);
            return NULL;
        }

        if (SetSearch(new_set, converted)) {            
            fprintf(stderr, "ERROR: Element '%s' is already in set!\n", new_element);
            LIST_Number_Free(new_set);
            free(new_element);
            return NULL;
        }

        if (LIST_Number_InsertFirst(new_set, converted) == EXIT_FAILURE) {
            LIST_Number_Free(new_set);
            free(new_element);
            return NULL;
        }

        free(new_element);

    }
    return new_set;
}


/**
 * @brief Get the Pair
 * 
 * @param line currently parsed line
 * @param universum Universum array
 * @param current_char currently parsed character
 * @param pair new pair return
 * @return EXIT_FAILURE if error occured during the processing otherwive returns EXIT_SUCCESS 
 */
int GetPair(char* line, DA_Universum_T* universum, int* current_char, Pair* pair) {
    SkipWhiteChars(line, current_char);
    if (line[*current_char] == '\0') {
        return -1;
    }

    if (line[*current_char] != '(') {
        return EXIT_FAILURE;
    }
    (*current_char)++;

    char* new_element_x = NULL;
    GetElement(line, current_char, &new_element_x);
    if (!new_element_x) {
        return EXIT_FAILURE;
    }
    char* new_element_y = NULL;
    GetElement(line, current_char, &new_element_y);
    if (!new_element_y) {
        free(new_element_x);
        return EXIT_FAILURE;
    }

    if (line[*(current_char)] != ')') {
        free(new_element_x);
        free(new_element_y);
        return EXIT_FAILURE;
    }
    (*current_char)++;

    pair->x = UniversumSearch(universum, new_element_x);
    pair->y = UniversumSearch(universum, new_element_y);

    free(new_element_x);
    free(new_element_y);
    return EXIT_SUCCESS;
}


/**
 * @brief Parse line containing Relation definition
 * 
 * @param line currently parsed line
 * @param universum Universum array
 * @return LIST_Pairs_T* new relation
 */
LIST_Pairs_T* ParseRelation(char* line, DA_Universum_T* universum) {
    LIST_Pairs_T* new_relation = (LIST_Pairs_T*) malloc(sizeof(LIST_Pairs_T));
    LIST_Pairs_Init(new_relation);

    if (!new_relation) {
        fprintf(stderr, "ERROR: Error during malloc for relation during relation parsing!\n");
        return NULL;
    }

    int current_char = 1;

    if (!IsFollowedBySpace(line, &current_char)) {
        LIST_Pairs_Free(new_relation);
        if (line[current_char] == '\0') {
            // Empty relation
            return CreateEmptyRelation();
        }
        fprintf(stderr, "ERROR: Relation identifier R not followed by space on line %d!\n", line_number);
        return NULL;
    }

    while (true) {
        Pair new_pair;
        int exit_code = GetPair(line, universum, &current_char, &new_pair);
        if (exit_code == -1) {
            break;
        }        
        if (exit_code != EXIT_SUCCESS) {
            LIST_Pairs_Free(new_relation);
            return NULL;
        }


        if (new_pair.x == -1 || new_pair.y == -1) {
            LIST_Pairs_Free(new_relation);
            fprintf(stderr, "ERROR: Element (on line %d) in pair is not a part of the universe!\n", line_number);
            return NULL;
        }

        
        if (PairSearchByPair(new_relation, new_pair)) {
            LIST_Pairs_Free(new_relation);
            fprintf(stderr, "ERROR: Pair (on line %d) is already in relation!\n", line_number);
            return NULL;
        }

        if (LIST_Pairs_InsertFirst(new_relation, new_pair) == EXIT_FAILURE) {
            LIST_Pairs_Free(new_relation);
            return NULL;
        }        
    }
    
    return new_relation;
}


/**
 * @brief Finds set in Array containing all defined sets by the line number on which the set was defined
 * 
 * @param sets Array containing all defined sets
 * @param line_number Line number of set to be looked up
 * @return Index in the Set Array on which the selected Set List can be found
 */
int FindSet(DA_Sets_T* sets, int line_number) {
    for (int i = 0; i < sets->len; i++) {
        if (sets->data[i]->line_number == line_number) {
            return i;
        }
    }

    return -1;
}


/**
 * @brief Converts string to integer including all error checks
 * 
 * @param input string to be converted to int
 * @param result converted integer
 * @return false if there was an error during conversion else true
 */
bool conv_str_to_int(char* input, int* result){
      char* start = input;
      char* end;
      errno = 0;
      long val = strtol(start, &end, DEC_BASE);
      
      if (errno == ERANGE && (val == LONG_MAX || val == LONG_MIN)){
            fprintf(stderr, "Line %d: Error during string to long conversion - conversion overflow!\n", line_number);
            return false;
      }
      else if (errno != 0 && val == 0){
            fprintf(stderr, "Line %d: Error during string to long conversion - no conversion has been done!\n", line_number);
            return false;
      }
      else if (end == start){
            fprintf(stderr, "Line %d: Error during string to long conversion - no conversion has been done!\n", line_number);
            return false;
      }
      else {
            // Correct reading
            *result = (int) val;
            return true;
      }
}


/**
 * @brief Inserts new set to Set array
 * 
 * @param set set to be inserted
 * @param set_da Set array to be inserted into
 * @return EXIT_FAILURE if error occured during the processing otherwive returns EXIT_SUCCESS 
 */
int InsertSetListToDA(LIST_Number_T* set, DA_Sets_T* set_da) {
    Set_T* new_set = (Set_T*) malloc(sizeof(Set_T));
    if (!new_set) {
        fprintf(stderr, "ERROR: Error during malloc for Set_T!\n");
        return EXIT_FAILURE;
    }
    new_set->line_number = line_number;
    new_set->Set = set;

    if (DA_Sets_Insert(set_da, new_set) != EXIT_SUCCESS) {
        free(new_set);
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}


/**
 * @brief Inserts new relation to Relation array
 * 
 * @param rel relation to be inserted
 * @param rel_da Relation array to be inserted
 * @return EXIT_FAILURE if error occured during the processing otherwive returns EXIT_SUCCESS
 */
int InsertRelationListToDA(LIST_Pairs_T* rel, DA_Relations_T* rel_da) {
    Relation_T* new_rel = (Relation_T*) malloc(sizeof(Relation_T));
    if (!new_rel) {
        fprintf(stderr, "ERROR: Error during malloc for Relation_T!\n");
        return EXIT_FAILURE;
    }
    new_rel->line_number = line_number;
    new_rel->Relation = rel;

    if (DA_Relations_Insert(rel_da, new_rel) != EXIT_SUCCESS) {
        free(new_rel);
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}


/**
 * @brief Get the Set
 * 
 * @param line currently processed line
 * @param current_char currently processed character
 * @param sets Sets array
 * @param all_lines All lines to be parsed array
 * @param universum Universum array
 * @return LIST_Number_T* returned set
 */
LIST_Number_T* GetSet(char* line, int* current_char, DA_Sets_T* sets, DA_Lines_T* all_lines, DA_Universum_T* universum) {
    int set_number = -1;

    char* set_number_string = NULL;
    GetElement(line, current_char, &set_number_string);
    if (!set_number_string) {
        fprintf(stderr, "ERROR: No set specified on line %d!\n", line_number);
        return NULL;   
    }

    if (!conv_str_to_int(set_number_string, &set_number)) {
        free(set_number_string);
        return NULL;
    }

    free(set_number_string);

    if (set_number < 0) {
        fprintf(stderr, "ERROR: Trying to access set on negative line (line number %d)!\n", set_number);
        return NULL;
    }
    
    if (set_number >= all_lines->len) {
        fprintf(stderr, "ERROR: Trying to access set on out of range line (line number %d)!\n", set_number);
        return NULL;
    }

    if (set_number > line_number && set_number < (all_lines->len)) {
        LIST_Number_T* empty_set = CreateEmptySet();
        if (InsertSetListToDA(empty_set, &SetsGarbageCollection) != EXIT_SUCCESS) {
            LIST_Number_Free(empty_set);
            return NULL;
        }
        return empty_set;
    }

    if (set_number == 1) {
        LIST_Number_T* universum_set = CreateUniversumSet(universum);
        if (InsertSetListToDA(universum_set, &SetsGarbageCollection) != EXIT_SUCCESS) {
            LIST_Number_Free(universum_set);
            return NULL;
        }
        return universum_set;
    }

    int set_index = FindSet(sets, set_number);

    if (set_index == -1) {
        fprintf(stderr, "ERROR: Trying to access non-existent set (line number %d)!\n", set_number);
        return NULL;
    }    
    return sets->data[set_index]->Set;
}


/**
 * @brief Finds relation in Array containing all defined relations by the line number on which the relation was defined
 * 
 * @param relations Array containing all defined relation
 * @param line_number Line number of relation to be looked up
 * @return Index in the Relation Array on which the selected Relation List can be found 
 */
int FindRelation(DA_Relations_T* relations, int line_number) {
    for (int i = 0; i < relations->len; i++) {
        if (relations->data[i]->line_number == line_number) {
            return i;
        }
    }

    return -1;
}


/**
 * @brief Get the Relation
 * 
 * @param line currently processed line
 * @param current_char currently processed character
 * @param relations Relation array
 * @param all_lines All lines to be parsed array
 * @return LIST_Pairs_T* returned relation
 */
LIST_Pairs_T* GetRelation(char* line, int* current_char, DA_Relations_T* relations, DA_Lines_T* all_lines) {
    int rel_number = -1;

    char* rel_number_string = NULL;
    GetElement(line, current_char, &rel_number_string);
    if (!rel_number_string) {
        fprintf(stderr, "ERROR: No set specified on line %d!\n", line_number);
        return NULL;   
    }

    if (!conv_str_to_int(rel_number_string, &rel_number)) {
        free(rel_number_string);
        return NULL;
    }

    free(rel_number_string);

    if (rel_number < 0) {
        fprintf(stderr, "ERROR: Trying to access relation on negative line (line number %d)!\n", rel_number);
        return NULL;
    }
    
    if (rel_number >= all_lines->len) {
        fprintf(stderr, "ERROR: Trying to access relation on out of range line (line number %d)!\n", rel_number);
        return NULL;
    }

    if (rel_number > line_number && rel_number < (all_lines->len)) {
        LIST_Pairs_T* empty_relation = CreateEmptyRelation();
        if (InsertRelationListToDA(empty_relation, &RelationsGarbageCollection) != EXIT_SUCCESS) {
            free(empty_relation);
            return NULL;
        }
        return empty_relation;
    }

    int rel_index = FindRelation(relations, rel_number);

    if (rel_index == -1) {
        fprintf(stderr, "ERROR: Trying to access non-existent relation (line number %d)!\n", rel_number);
        return NULL;
    }    

    return relations->data[rel_index]->Relation;
}


/**
 * @brief Prints all elements of Universum to the stdin in format specified in requirements
 * 
 * @param universum Universum array to be printed
 */
void PrintUniversum(DA_Universum_T* universum) {
    printf("U");
    for (int i = 0; i < universum->len; i++) {
        printf(" %s", universum->data[i]);
    }
    printf("\n");     
}


/**
 * @brief Prints all elements of Set to the stdin in format specified in requirements
 * 
 * @param set Set to be printed
 * @param universum Universum array for set element conversion to string
 */
void PrintSet(LIST_Number_T* set, DA_Universum_T* universum) {
    printf("S");

    L_Number_Elem* current_ptr = set->first; 

    while (current_ptr) {
        printf(" %s", universum->data[current_ptr->data]);
        current_ptr = current_ptr->next;
    }

    printf("\n"); 
}


/**
 * @brief Prints all elements of Relation to the stdin in format specified in requirements
 * 
 * @param relation Relation to be printed
 * @param universum Universum array for relation pair conversion to string
 */
void PrintRelation(LIST_Pairs_T* relation, DA_Universum_T* universum) {
    printf("R");

    L_Pairs_Elem* current_ptr = relation->first; 

    while (current_ptr) {
        printf(" (%s %s)", universum->data[current_ptr->data.x], universum->data[current_ptr->data.y]);
        current_ptr = current_ptr->next;
    }

    printf("\n");
}


/**
 * @brief Checks if set is empty, if yes prints "true\n" otherwise "false\n"
 * 
 * @param set Set List to be checked 
 */
void Empty(LIST_Number_T* set) {
    L_Number_Elem* current_ptr = set->first;
    if(current_ptr == NULL){
        printf("true\n");
    } else {
        printf("false\n");
    }
}


/**
 * @brief Counts elements in set
 * 
 * @param set to be counted
 * @return returned count 
 */
int Card(LIST_Number_T* set) {
    L_Number_Elem* current_ptr = set->first;
    int count_set = 0;
    while (current_ptr){
        count_set = count_set + 1;
        current_ptr = current_ptr->next;
    }
    return count_set;
}


/**
 * @brief Prints complement of set
 * 
 * @param set Set List for to be complemented
 * @param universum Universum Array for index to string conversion for printing
 * @param sets Sets array
 * @return EXIT_FAILIURE if there was an error during processing otherwise EXIT_SUCCESS
 */
int Complement(LIST_Number_T* set, DA_Universum_T* universum, DA_Sets_T* sets) {
    LIST_Number_T* universum_set = CreateUniversumSet(universum);
    if (!universum_set) {
        return EXIT_FAILURE;
    }
    LIST_Number_T* complement_set = (LIST_Number_T*) malloc(sizeof(LIST_Number_T));
    if (!complement_set) {
        fprintf(stderr, "ERROR: Failed malloc for complement_set in Complement!\n");
        LIST_Number_Free(universum_set);
        return EXIT_FAILURE;
    }
    LIST_Number_Init(complement_set);
    
    L_Number_Elem* current_ptr = universum_set->first;
    
    while (current_ptr) { 
        if(!SetSearch(set, current_ptr->data)){
            if(LIST_Number_InsertFirst(complement_set, current_ptr->data) == EXIT_FAILURE) {
                LIST_Number_Free(universum_set);
                LIST_Number_Free(complement_set);            
                return EXIT_FAILURE;
            }
            current_ptr = current_ptr->next;
        } else {
            current_ptr = current_ptr->next;
        }
    }
    PrintSet(complement_set, universum);

    if (InsertSetListToDA(complement_set, sets) != EXIT_SUCCESS) {
        LIST_Number_Free(universum_set);
        LIST_Number_Free(complement_set);
        return EXIT_FAILURE;
    }

    LIST_Number_Free(universum_set);
    return EXIT_SUCCESS;

}


/**
 * @brief Prints union of sets
 * 
 * @param set_A First Set List
 * @param set_B Second Set List
 * @param universum Universum Array for index to string conversion for printing
 * @param sets Sets array
 * @return EXIT_FAILIURE if there was an error during processing otherwise EXIT_SUCCESS
 */
int Union(LIST_Number_T* set_A, LIST_Number_T* set_B, DA_Universum_T* universum, DA_Sets_T* sets) {
    LIST_Number_T* union_set = (LIST_Number_T*) malloc(sizeof(LIST_Number_T));
    if (!union_set) {
        fprintf(stderr, "ERROR: Failed malloc for union_set in Union!\n");
        return EXIT_FAILURE;
    }
    LIST_Number_Init(union_set);

    // Insert setA
    L_Number_Elem* current_ptr = set_A->first;
    while (current_ptr) {
        if(LIST_Number_InsertFirst(union_set, current_ptr->data) == EXIT_FAILURE) {
            LIST_Number_Free(union_set);            
            return EXIT_FAILURE;
        }
        current_ptr = current_ptr->next;
    }

    current_ptr = set_B->first;
    while (current_ptr) {
        if(SetSearch(union_set, current_ptr->data)) {
            current_ptr = current_ptr->next;
            continue;
        }

        if(LIST_Number_InsertFirst(union_set, current_ptr->data) == EXIT_FAILURE) {
            LIST_Number_Free(union_set);            
            return EXIT_FAILURE;
        }
        current_ptr = current_ptr->next;
    }

    PrintSet(union_set, universum);
   
    if (InsertSetListToDA(union_set, sets) != EXIT_SUCCESS) {
        LIST_Number_Free(union_set);
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}


/**
 * @brief Prints intersect of sets
 * 
 * @param set_A First Set List
 * @param set_B Second Set List
 * @param universum Universum Array for index to string conversion for printing
 * @param sets Sets array
 * @return EXIT_FAILIURE if there was an error during processing otherwise EXIT_SUCCESS 
 */
int Intersect(LIST_Number_T* set_A, LIST_Number_T* set_B, DA_Universum_T* universum, DA_Sets_T* sets) {
    LIST_Number_T* intersect_set = (LIST_Number_T*) malloc(sizeof(LIST_Number_T));
    if (!intersect_set) {
        fprintf(stderr, "ERROR: Failed malloc for intersect_set in Intersect!\n");
        return EXIT_FAILURE;
    }
    LIST_Number_Init(intersect_set);

    L_Number_Elem* current_ptr = set_A->first;
    
    while (current_ptr) {
        if(SetSearch(set_B, current_ptr->data)){
            if(LIST_Number_InsertFirst(intersect_set, current_ptr->data) == EXIT_FAILURE) {
                LIST_Number_Free(intersect_set);            
                return EXIT_FAILURE;
            }
            current_ptr = current_ptr->next;
        } else {
            current_ptr = current_ptr->next;
        }
    }
    PrintSet(intersect_set, universum);
    
    if (InsertSetListToDA(intersect_set, sets) != EXIT_SUCCESS) {
        LIST_Number_Free(intersect_set);
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}


/**
 * @brief Prints minus of sets -> what is in set_A but not in set_B
 * 
 * @param set_A First Set List
 * @param set_B Second Set List
 * @param universum Universum Array for index to string conversion for printing
 * @param sets Sets array
 * @return EXIT_FAILIURE if there was an error during processing otherwise EXIT_SUCCESS 
 */
int Minus(LIST_Number_T* set_A, LIST_Number_T* set_B, DA_Universum_T* universum, DA_Sets_T* sets) {
    LIST_Number_T* minus_set = (LIST_Number_T*) malloc(sizeof(LIST_Number_T));
    if (!minus_set) {
        fprintf(stderr, "ERROR: Failed malloc for minus_set in Minus!\n");
        return EXIT_FAILURE;
    }
    LIST_Number_Init(minus_set);

    L_Number_Elem* current_ptr = set_A->first;
    
    while (current_ptr) {
        if(!SetSearch(set_B, current_ptr->data)){
            if(LIST_Number_InsertFirst(minus_set, current_ptr->data) == EXIT_FAILURE) {
                LIST_Number_Free(minus_set);            
                return EXIT_FAILURE;
            }
            current_ptr = current_ptr->next;
        } else {
            current_ptr = current_ptr->next;
        }
    }
    PrintSet(minus_set, universum);

    if (InsertSetListToDA(minus_set, sets) != EXIT_SUCCESS) {
        LIST_Number_Free(minus_set);
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}


/**
 * @brief Checks if set_A is subset of set_B
 * if all elements from set_A are in set_B prints "true\n" otherwise prints "false\n"
 * 
 * @param set_A First Set List
 * @param set_B Second Set List
 */
bool Subseteq(LIST_Number_T* set_A, LIST_Number_T* set_B) {
    L_Number_Elem* current_ptr = set_A->first;
    while (current_ptr){
        if(SetSearch(set_B, current_ptr->data)) {
            current_ptr = current_ptr->next;
            continue;
        } else {
            return false;
        }
    }
    return true;
}


/**
 * @brief Checks if set_A is proper subset of set_B
 * if all elements from set_A are in set_B and set_A != set_B prints "true\n" otherwise prints "false\n"
 * 
 * @param set_A First Set List
 * @param set_B Second Set List
 */
void Subset(LIST_Number_T* set_A, LIST_Number_T* set_B) {
    if(Subseteq(set_A, set_B)){
        if(Card(set_A) != Card(set_B)){
            printf("true\n");
            return;
        }
    }
    printf("false\n");
}


/**
 * @brief Checks if set_A equals set_B, if yes -> prints "true\n" otherwise prints "false\n"
 * 
 * @param set_A First Set List
 * @param set_B Second Set List
 */
void Equals(LIST_Number_T* set_A, LIST_Number_T* set_B) {
    if (Subseteq(set_A, set_B) && Subseteq(set_B, set_A)){
        printf("true\n");
    } else {
        printf("false\n");
    }
}

/**
 * @brief Counts pairs in relation
 * 
 * @param rel relation to count
 * @return Amount of pairs
 */
int relLen(LIST_Pairs_T *rel) {
    L_Pairs_Elem *main_ptr = rel->first;
    int cnt = 0;

    while(main_ptr){
        cnt++;
        main_ptr = main_ptr->next;
    }

    return cnt;
}

/**
 * @brief Checks if rel_A is reflexive, if yes -> prints "true\n" otherwise prints "false\n"
 * 
 * @param rel_A relation to check
 * @param universum Universum array
 * @return EXIT_FAILIURE if there was an error during processing otherwise EXIT_SUCCESS 
 */
int Reflexive(LIST_Pairs_T* rel_A, DA_Universum_T* universum) {
    LIST_Number_T* universum_set = CreateUniversumSet(universum);
    if (!universum_set) {
        return EXIT_FAILURE;
    }

    L_Number_Elem* current_ptr = universum_set->first;    
    while(current_ptr){
        Pair pair = { .x = current_ptr->data, .y = current_ptr->data };
        if(!PairSearchByPair(rel_A, pair)){
            printf("false\n");
            LIST_Number_Free(universum_set);
            return EXIT_SUCCESS;
        }
        current_ptr = current_ptr->next;
    }

    printf("true\n");
    LIST_Number_Free(universum_set);
    return EXIT_SUCCESS;
}


/**
 * @brief Checks if rel_A is symmetric, if yes -> prints "true\n" otherwise prints "false\n"
 * 
 * @param rel_A Relation List
 */
void Symmetric(LIST_Pairs_T* rel_A) {
    L_Pairs_Elem *current_ptr = rel_A->first;

    while(current_ptr){
        Pair pair = current_ptr->data;
        if(!PairSearchByXY(rel_A, pair.y, pair.x)){
            printf("false\n");
            return;
        }
        current_ptr = current_ptr->next; 
    }

    printf("true\n");
    return;
}


/**
 * @brief Checks if rel_A is anti-symmetric, if yes -> prints "true\n" otherwise prints "false\n"
 * 
 * @param rel_A Relation List
 */
void Antisymmetric(LIST_Pairs_T* rel_A) {
    L_Pairs_Elem *current_ptr = rel_A->first;

    while(current_ptr){
        Pair pair = current_ptr->data;
        if(PairSearchByXY(rel_A, pair.y, pair.x) && pair.x != pair.y){
            printf("false\n");
            return;
        }
        current_ptr = current_ptr->next; 
    }

    printf("true\n");
    return;
}


/**
 * @brief Checks if rel_A is transitive, if yes -> prints "true\n" otherwise prints "false\n"
 * 
 * @param rel_A Relation List
 */
void Transitive(LIST_Pairs_T* rel_A) {
    L_Pairs_Elem *main_ptr = rel_A->first;
    L_Pairs_Elem *sec_ptr;

    while(main_ptr){
        sec_ptr = rel_A->first->next;
        while(sec_ptr){
            if(main_ptr->data.y == sec_ptr->data.x){
                if(!PairSearchByXY(rel_A, main_ptr->data.x, sec_ptr->data.y)){
                    printf("false\n");
                    return;
                }
            }
            sec_ptr = sec_ptr->next;
        }
        main_ptr = main_ptr->next;
    }
    printf("true\n");
    return;
}


/**
 * @brief Checks if rel_A is a function, if yes -> returns EXIT_SUCCESS, otherwise -1
 * 
 * @param rel_A Relation List
 * 
 * @return EXIT_FAILIURE if there was an error during processing, EXIT_SUCCESS if true, -1 if false
 */
int IsFunction(LIST_Pairs_T* rel_A) {
    LIST_Number_T *func_set = (LIST_Number_T*) malloc(sizeof(LIST_Number_T));
    if(!func_set) {
        fprintf(stderr, "ERROR: Failed malloc for func_set in IsFunction!\n");
        return EXIT_FAILURE;
    }
    LIST_Number_Init(func_set);
    
    L_Pairs_Elem *current_ptr = rel_A->first;
    while(current_ptr) {

        if(SetSearch(func_set, current_ptr->data.x)){
            LIST_Number_Free(func_set);            
            return -1;
        }

        if(LIST_Number_InsertFirst(func_set, current_ptr->data.x) == EXIT_FAILURE) {
            LIST_Number_Free(func_set);            
            return EXIT_FAILURE;
        }
        current_ptr = current_ptr->next;
    }

    LIST_Number_Free(func_set);
    return EXIT_SUCCESS;
}


/**
 * @brief Prints domain of relation -> set containing x-elements of relation
 * 
 * @param rel_A Relation List
 * @param universum Universum Array for index to string conversion for printing
 * @param sets Set array
 *
 * @return EXIT_FAILIURE if there was an error during processing otherwise EXIT_SUCCESS 
 */
int Domain(LIST_Pairs_T* rel_A, DA_Universum_T* universum, DA_Sets_T* sets) {
    LIST_Number_T *domain_set = (LIST_Number_T*) malloc(sizeof(LIST_Number_T));
    if(!domain_set){
        fprintf(stderr, "ERROR: Failed to malloc for domain_set in Domain!\n");
        return EXIT_FAILURE;
    }
    LIST_Number_Init(domain_set);

    L_Pairs_Elem *current_ptr = rel_A->first;
    
    while(current_ptr) {
        if(SetSearch(domain_set, current_ptr->data.x)){
            current_ptr = current_ptr->next;
            continue;
        }

        if(LIST_Number_InsertFirst(domain_set, current_ptr->data.x) == EXIT_FAILURE){
            LIST_Number_Free(domain_set);
            return EXIT_FAILURE;
        }
        current_ptr = current_ptr->next;
    }

    PrintSet(domain_set, universum);
    
    if (InsertSetListToDA(domain_set, sets) != EXIT_SUCCESS) {
        LIST_Number_Free(domain_set);
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}


/**
 * @brief Prints codomain of relation -> set containing y-elements of relation
 * 
 * @param rel_A Relation List
 * @param universum Universum Array for index to string conversion for printing
 * @param sets Set array
 *
 * @return EXIT_FAILIURE if there was an error during processing otherwise EXIT_SUCCESS 
 */
int Codomain(LIST_Pairs_T* rel_A, DA_Universum_T* universum, DA_Sets_T* sets) {
    LIST_Number_T *codomain_set = (LIST_Number_T*) malloc(sizeof(LIST_Number_T));
    if(!codomain_set){
        fprintf(stderr, "ERROR: Failed to malloc for domain_set in Domain!\n");
        return EXIT_FAILURE;
    }
    LIST_Number_Init(codomain_set);

    L_Pairs_Elem *current_ptr = rel_A->first;
    while(current_ptr) {
        if(SetSearch(codomain_set, current_ptr->data.y)){
            current_ptr = current_ptr->next;
            continue;
        }

        if(LIST_Number_InsertFirst(codomain_set, current_ptr->data.y) == EXIT_FAILURE){
            LIST_Number_Free(codomain_set);
            return EXIT_FAILURE;
        }
        current_ptr = current_ptr->next;
    }

    PrintSet(codomain_set, universum);
    
    if (InsertSetListToDA(codomain_set, sets) != EXIT_SUCCESS) {
        LIST_Number_Free(codomain_set);
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}


/**
 * @brief Checks if function rel_A is injective, includes check for function
 * if yes -> prints "true\n" otherwise prints "false\n"
 * 
 * @param rel_A Relation List
 * @param set_A Set A
 * @param set_B Set B
 * 
 * @return EXIT_FAILIURE if there was an error during processing otherwise EXIT_SUCCESS 
 */

int Injective(LIST_Pairs_T* rel_A, LIST_Number_T* set_A, LIST_Number_T* set_B) {
    LIST_Pairs_T *inj_set = (LIST_Pairs_T*) malloc(sizeof(LIST_Pairs_T));
    if(!inj_set) {
        fprintf(stderr, "ERROR: Failed malloc for inj_set in Injective!\n");
        return EXIT_FAILURE;
    }
    LIST_Pairs_Init(inj_set);

    int func = IsFunction(rel_A);
    if(func == EXIT_FAILURE){
        LIST_Pairs_Free(inj_set);
        return EXIT_FAILURE;
    } else if (func == -1){
        printf("false\n");
        LIST_Pairs_Free(inj_set);
        return EXIT_SUCCESS;
    }

    // checks duplicates in y
    LIST_Number_T *func_set = (LIST_Number_T*) malloc(sizeof(LIST_Number_T));
    if(!func_set) {
        fprintf(stderr, "ERROR: Failed malloc for func_set in Injective!\n");
        LIST_Pairs_Free(inj_set);
        return EXIT_FAILURE;
    }
    LIST_Number_Init(func_set);
    
    L_Pairs_Elem *current_ptr = rel_A->first;
    while(current_ptr) {

        if(SetSearch(func_set, current_ptr->data.y)){
            LIST_Number_Free(func_set);            
            printf("false\n");
            LIST_Pairs_Free(inj_set);
            return EXIT_SUCCESS;
        }

        if(LIST_Number_InsertFirst(func_set, current_ptr->data.y) == EXIT_FAILURE) {
            LIST_Number_Free(func_set);
            LIST_Pairs_Free(inj_set);            
            return EXIT_FAILURE;
        }
        current_ptr = current_ptr->next;
    }

    LIST_Number_Free(func_set);

    if(Card(set_A) != relLen(rel_A)){
        LIST_Pairs_Free(inj_set);
        printf("false\n");
        return EXIT_SUCCESS;
    }

    // all from set_A used && all y from set_B
    L_Pairs_Elem *main_ptr = rel_A->first;
    L_Number_Elem *setA_ptr = set_A->first;
    int flag = 0;

    if(!setA_ptr && main_ptr){
        LIST_Pairs_Free(inj_set);
        printf("false\n");
        return EXIT_SUCCESS;
    }

    while(setA_ptr){
        main_ptr = rel_A->first;
        while(main_ptr){
            if(main_ptr->data.x == setA_ptr->data && SetSearch(set_B, main_ptr->data.y)){
                flag = 1;
                break;
            } else {
                main_ptr = main_ptr->next;
            }
        }

        if(!flag){
            printf("false\n");
            LIST_Pairs_Free(inj_set);
            return EXIT_SUCCESS;
        }
        flag = 0;

        setA_ptr = setA_ptr->next;
    }

    printf("true\n");
    LIST_Pairs_Free(inj_set);
    return EXIT_SUCCESS;
    // rel needs to be func
    // every item in set_A needs to be used
}


/**
 * @brief Checks if function rel_A is injective, includes check for function
 * if yes -> prints "true\n" otherwise prints "false\n"
 * 
 * @param rel_A Relation List
 * @param set_A Set A
 * @param set_B Set B
 * @return EXIT_FAILIURE if there was an error during processing otherwise EXIT_SUCCESS 
 */

int Surjective(LIST_Pairs_T* rel_A, LIST_Number_T* set_A, LIST_Number_T* set_B) {
    LIST_Pairs_T *sur_set = (LIST_Pairs_T*) malloc(sizeof(LIST_Pairs_T));
    if(!sur_set) {
        fprintf(stderr, "ERROR: Failed malloc for sur_set in Surjective!\n");
        return EXIT_FAILURE;
    }
    LIST_Pairs_Init(sur_set);

    int func = IsFunction(rel_A);
    if(func == EXIT_FAILURE){
        LIST_Pairs_Free(sur_set);
        return EXIT_FAILURE;
    } else if (func == -1){
        printf("false\n");
        LIST_Pairs_Free(sur_set);
        return EXIT_SUCCESS;
    }

    // all from set_B used && all x from set_A
    L_Number_Elem *setB_ptr = set_B->first;
    L_Pairs_Elem *main_ptr = rel_A->first;
    int flag = 0;

    if(setB_ptr && !main_ptr){
        printf("false\n");
        LIST_Pairs_Free(sur_set);
        return EXIT_SUCCESS;
    }

    if(!setB_ptr && main_ptr){
        printf("false\n");
        LIST_Pairs_Free(sur_set);
        return EXIT_SUCCESS;
    }

    if(!set_A->first && main_ptr){
        printf("false\n");
        LIST_Pairs_Free(sur_set);
        return EXIT_SUCCESS;
    }

    while(main_ptr){
        if(!SetSearch(set_A, main_ptr->data.x)){
            printf("false\n");
            LIST_Pairs_Free(sur_set);
            return EXIT_SUCCESS;
        }
        main_ptr = main_ptr->next;
    }

    while(setB_ptr){
        main_ptr = rel_A->first;
        while(main_ptr){
            if(main_ptr->data.y == setB_ptr->data){
                if(!SetSearch(set_A, main_ptr->data.x)){
                    printf("false\n");
                    LIST_Pairs_Free(sur_set);
                    return EXIT_SUCCESS;
                }
                flag = 1;
                break;
            } else {
                main_ptr = main_ptr->next;
            }
        }

        if(!flag){
            printf("false\n");
            LIST_Pairs_Free(sur_set);
            return EXIT_SUCCESS;
        }
        flag = 0;

        setB_ptr = setB_ptr->next;
    }

    printf("true\n");
    LIST_Pairs_Free(sur_set);
    return EXIT_SUCCESS;
    
    // rel needs to be func
    // every item in set_B needs to be used
}


/**
 * @brief Checks if function rel_A is injective, includes check for function
 * if yes -> prints "true\n" otherwise prints "false\n"
 * 
 * @param rel_A Relation List
 * @param set_A Set A
 * @param set_B Set B
 * @return EXIT_FAILIURE if there was an error during processing otherwise EXIT_SUCCESS 
 */
int Bijective(LIST_Pairs_T* rel_A, LIST_Number_T* set_A, LIST_Number_T* set_B) {
    LIST_Pairs_T *bij_set = (LIST_Pairs_T*) malloc(sizeof(LIST_Pairs_T));
    if(!bij_set) {
        fprintf(stderr, "ERROR: Failed malloc for bij_set in Bijective!\n");
        return EXIT_FAILURE;
    }
    LIST_Pairs_Init(bij_set);

    int func = IsFunction(rel_A);
    if(func == EXIT_FAILURE){
        LIST_Pairs_Free(bij_set);
        return EXIT_FAILURE;
    } else if (func == -1){
        printf("false\n");
        LIST_Pairs_Free(bij_set);
        return EXIT_SUCCESS;
    }

    if(Card(set_A) != relLen(rel_A)){
        printf("false\n");
        LIST_Pairs_Free(bij_set);
        return EXIT_SUCCESS;
    }

    if(Card(set_B) != relLen(rel_A)){
        printf("false\n");
        LIST_Pairs_Free(bij_set);
        return EXIT_SUCCESS;
    }

    // all from set_A used
    L_Number_Elem *setA_ptr = set_A->first;
    L_Pairs_Elem *main_ptr = rel_A->first;
    int flag = 0;

    if(!setA_ptr && main_ptr){
        printf("false\n");
        LIST_Pairs_Free(bij_set);
        return EXIT_SUCCESS;
    }

    while(setA_ptr){
        main_ptr = rel_A->first;
        while(main_ptr){
            if(main_ptr->data.x == setA_ptr->data){
                flag = 1;
                break;
            } else {
                main_ptr = main_ptr->next;
            }
        }

        if(!flag){
            printf("false\n");
            LIST_Pairs_Free(bij_set);
            return EXIT_SUCCESS;
        }
        flag = 0;

        setA_ptr = setA_ptr->next;
    }

    // all from set_B used
    L_Number_Elem *setB_ptr = set_B->first;

    while(setB_ptr){
        main_ptr = rel_A->first;
        while(main_ptr){
            if(main_ptr->data.y == setB_ptr->data){
                flag = 1;
                break;
            } else {
                main_ptr = main_ptr->next;
            }
        }
        
        if(!flag){
            printf("false\n");
            LIST_Pairs_Free(bij_set);
            return EXIT_SUCCESS;
        }
        flag = 0;

        setB_ptr = setB_ptr->next;
    }

    printf("true\n");
    LIST_Pairs_Free(bij_set);
    return EXIT_SUCCESS;

    // rel needs to be func
    // every item in set_A AND set_B needs to be usedwa
}


/**
 * @brief Safe exit function for Command parsing
 * 
 * @param exit_code to be passed along
 * @param command command name
 * @return exit code passed along 
 */
int CommandSafeExit(int exit_code, char* command) {
    free(command);
    return exit_code;
}

bool MoreParams(char* line, int* current_char) {
    char* new_element = NULL;
    int exit_code = GetElement(line, current_char, &new_element);

    if (!new_element && exit_code == EXIT_SUCCESS) {
        return false;
    }

    free(new_element);
    return true;
}


/**
 * @brief Function for Command line parsing - executes specified function
 * 
 * @param line File that is being processed
 * @param universum Universum array
 * @param sets Array containing all defined sets
 * @param relations Array containing all defined relations
 * @return EXIT_FAILURE if error occured during processing otherwise EXIT_SUCCESS 
 */
int ParseCommand(char* line, DA_Universum_T* universum, DA_Sets_T* sets, DA_Relations_T* relations, DA_Lines_T* all_lines) {
    char* command = NULL;
    int current_char = 1;

    if (!IsFollowedBySpace(line, &current_char)) {
        fprintf(stderr, "ERROR: Command identifier C not followed by space on line %d!\n", line_number);
        return EXIT_FAILURE;
    }

    GetElement(line, &current_char, &command);
    if (!command) {
        fprintf(stderr, "ERROR No command selected on line %d\n", line_number);
        return EXIT_FAILURE;
    }

    if (!strcmp(command, "empty")) {
        LIST_Number_T* set = GetSet(line, &current_char, sets, all_lines, universum);
        if (!set) {
            return CommandSafeExit(EXIT_FAILURE, command);
        }

        if (MoreParams(line, &current_char)) {
            fprintf(stderr, "ERROR Too many parameters provided on line %d\n", line_number);
            return CommandSafeExit(EXIT_FAILURE, command);
        }

        Empty(set);
    }

    else if (!strcmp(command, "card")) {
        LIST_Number_T* set = GetSet(line, &current_char, sets, all_lines, universum);
        if (!set) {
            return CommandSafeExit(EXIT_FAILURE, command);
        }

        if (MoreParams(line, &current_char)) {
            fprintf(stderr, "ERROR Too many parameters provided on line %d\n", line_number);
            return CommandSafeExit(EXIT_FAILURE, command);
        }

        printf("%d\n",Card(set));
    }

    else if (!strcmp(command, "complement")) {
        LIST_Number_T* set = GetSet(line, &current_char, sets, all_lines, universum);
        if (!set) {
            return CommandSafeExit(EXIT_FAILURE, command);
        }

        if (MoreParams(line, &current_char)) {
            fprintf(stderr, "ERROR Too many parameters provided on line %d\n", line_number);
            return CommandSafeExit(EXIT_FAILURE, command);
        }

        if (Complement(set, universum, sets) == EXIT_FAILURE) {
            return CommandSafeExit(EXIT_FAILURE, command);
        }        
    }

    else if (!strcmp(command, "union")) {
        LIST_Number_T* set_A = GetSet(line, &current_char, sets, all_lines, universum);
        LIST_Number_T* set_B = GetSet(line, &current_char, sets, all_lines, universum);
        if (!set_A || !set_B) {
            return CommandSafeExit(EXIT_FAILURE, command);
        }

        if (MoreParams(line, &current_char)) {
            fprintf(stderr, "ERROR Too many parameters provided on line %d\n", line_number);
            return CommandSafeExit(EXIT_FAILURE, command);
        }

        if (Union(set_A, set_B, universum, sets) == EXIT_FAILURE) {
            return CommandSafeExit(EXIT_FAILURE, command);
        } 
    }

    else if (!strcmp(command, "intersect")) {
        LIST_Number_T* set_A = GetSet(line, &current_char, sets, all_lines, universum);
        LIST_Number_T* set_B = GetSet(line, &current_char, sets, all_lines, universum);
        if (!set_A || !set_B) {
            return CommandSafeExit(EXIT_FAILURE, command);
        }

        if (MoreParams(line, &current_char)) {
            fprintf(stderr, "ERROR Too many parameters provided on line %d\n", line_number);
            return CommandSafeExit(EXIT_FAILURE, command);
        }

        if (Intersect(set_A, set_B, universum, sets) == EXIT_FAILURE) {
            return CommandSafeExit(EXIT_FAILURE, command);
        }         
    }

    else if (!strcmp(command, "minus")) {
        LIST_Number_T* set_A = GetSet(line, &current_char, sets, all_lines, universum);
        LIST_Number_T* set_B = GetSet(line, &current_char, sets, all_lines, universum);
        if (!set_A || !set_B) {
            return CommandSafeExit(EXIT_FAILURE, command);
        }

        if (MoreParams(line, &current_char)) {
            fprintf(stderr, "ERROR Too many parameters provided on line %d\n", line_number);
            return CommandSafeExit(EXIT_FAILURE, command);
        }

        if (Minus(set_A, set_B, universum, sets) == EXIT_FAILURE) {
            return CommandSafeExit(EXIT_FAILURE, command);
        } 
    }

    else if (!strcmp(command, "subseteq")) {
        LIST_Number_T* set_A = GetSet(line, &current_char, sets, all_lines, universum);
        LIST_Number_T* set_B = GetSet(line, &current_char, sets, all_lines, universum);
        if (!set_A || !set_B) {
            return CommandSafeExit(EXIT_FAILURE, command);
        }

        if (MoreParams(line, &current_char)) {
            fprintf(stderr, "ERROR Too many parameters provided on line %d\n", line_number);
            return CommandSafeExit(EXIT_FAILURE, command);
        }

        printf("%s\n", Subseteq(set_A, set_B) ? "true" : "false");
    }

    else if (!strcmp(command, "subset")) {
        LIST_Number_T* set_A = GetSet(line, &current_char, sets, all_lines, universum);
        LIST_Number_T* set_B = GetSet(line, &current_char, sets, all_lines, universum);
        if (!set_A || !set_B) {
            return CommandSafeExit(EXIT_FAILURE, command);
        }
        
        if (MoreParams(line, &current_char)) {
            fprintf(stderr, "ERROR Too many parameters provided on line %d\n", line_number);
            return CommandSafeExit(EXIT_FAILURE, command);
        }

        Subset(set_A, set_B);        
    }

    else if (!strcmp(command, "equals")) {
        LIST_Number_T* set_A = GetSet(line, &current_char, sets, all_lines, universum);
        LIST_Number_T* set_B = GetSet(line, &current_char, sets, all_lines, universum);
        if (!set_A || !set_B) {
            return CommandSafeExit(EXIT_FAILURE, command);
        }
        
        if (MoreParams(line, &current_char)) {
            fprintf(stderr, "ERROR Too many parameters provided on line %d\n", line_number);
            return CommandSafeExit(EXIT_FAILURE, command);
        }
        
        Equals(set_A, set_B);      
    }

    else if (!strcmp(command, "reflexive")) {
        LIST_Pairs_T* rel = GetRelation(line, &current_char, relations, all_lines);
        if (!rel) {
            return CommandSafeExit(EXIT_FAILURE, command);
        }
        
        if (MoreParams(line, &current_char)) {
            fprintf(stderr, "ERROR Too many parameters provided on line %d\n", line_number);
            return CommandSafeExit(EXIT_FAILURE, command);
        }
        
        int exit_code = Reflexive(rel, universum);
        if (exit_code != EXIT_SUCCESS) {
            return EXIT_FAILURE;
        }
    }

    else if (!strcmp(command, "symmetric")) {
        LIST_Pairs_T* rel = GetRelation(line, &current_char, relations, all_lines);
        if (!rel) {
            return CommandSafeExit(EXIT_FAILURE, command);
        }
        
        if (MoreParams(line, &current_char)) {
            fprintf(stderr, "ERROR Too many parameters provided on line %d\n", line_number);
            return CommandSafeExit(EXIT_FAILURE, command);
        }
        
        Symmetric(rel);
    }

    else if (!strcmp(command, "antisymmetric")) {
        LIST_Pairs_T* rel = GetRelation(line, &current_char, relations, all_lines);
        if (!rel) {
            return CommandSafeExit(EXIT_FAILURE, command);
        }
        
        if (MoreParams(line, &current_char)) {
            fprintf(stderr, "ERROR Too many parameters provided on line %d\n", line_number);
            return CommandSafeExit(EXIT_FAILURE, command);
        }

        Antisymmetric(rel);
    }

    else if (!strcmp(command, "transitive")) {
        LIST_Pairs_T* rel = GetRelation(line, &current_char, relations, all_lines);
        if (!rel) {
            return CommandSafeExit(EXIT_FAILURE, command);
        }
        
        if (MoreParams(line, &current_char)) {
            fprintf(stderr, "ERROR Too many parameters provided on line %d\n", line_number);
            return CommandSafeExit(EXIT_FAILURE, command);
        }

        Transitive(rel);
    }
    else if (!strcmp(command, "function")) {
        int out;
        LIST_Pairs_T* rel = GetRelation(line, &current_char, relations, all_lines);
        if (!rel) {
            return CommandSafeExit(EXIT_FAILURE, command);
        }
        
        if (MoreParams(line, &current_char)) {
            fprintf(stderr, "ERROR Too many parameters provided on line %d\n", line_number);
            return CommandSafeExit(EXIT_FAILURE, command);
        }
        
        if ((out = IsFunction(rel)) == EXIT_FAILURE) {
            return CommandSafeExit(EXIT_FAILURE, command);
        }

        if(!out){
            printf("true\n");
        } else {
            printf("false\n");
        }
    }

    else if (!strcmp(command, "domain")) {
        LIST_Pairs_T* rel = GetRelation(line, &current_char, relations, all_lines);
        if (!rel) {
            return CommandSafeExit(EXIT_FAILURE, command);
        }
        
        if (MoreParams(line, &current_char)) {
            fprintf(stderr, "ERROR Too many parameters provided on line %d\n", line_number);
            return CommandSafeExit(EXIT_FAILURE, command);
        }

        if (Domain(rel, universum, sets) == EXIT_FAILURE) {
            return CommandSafeExit(EXIT_FAILURE, command);
        }
    }

    else if (!strcmp(command, "codomain")) {
        LIST_Pairs_T* rel = GetRelation(line, &current_char, relations, all_lines);
        if (!rel) {
            return CommandSafeExit(EXIT_FAILURE, command);
        }
        
        if (MoreParams(line, &current_char)) {
            fprintf(stderr, "ERROR Too many parameters provided on line %d\n", line_number);
            return CommandSafeExit(EXIT_FAILURE, command);
        }

        if (Codomain(rel, universum, sets) == EXIT_FAILURE) {
            return CommandSafeExit(EXIT_FAILURE, command);
        }
    }

    else if (!strcmp(command, "injective")) {
        LIST_Pairs_T* rel = GetRelation(line, &current_char, relations, all_lines);
        if (!rel) {
            return CommandSafeExit(EXIT_FAILURE, command);
        }
        LIST_Number_T* set_A = GetSet(line, &current_char, sets, all_lines, universum);
        LIST_Number_T* set_B = GetSet(line, &current_char, sets, all_lines, universum);
        if (!set_A || !set_B) {
            return CommandSafeExit(EXIT_FAILURE, command);
        }
        
        if (MoreParams(line, &current_char)) {
            fprintf(stderr, "ERROR Too many parameters provided on line %d\n", line_number);
            return CommandSafeExit(EXIT_FAILURE, command);
        }
        
        if (Injective(rel, set_A, set_B) == EXIT_FAILURE) {
            return CommandSafeExit(EXIT_FAILURE, command);
        }
    }

    else if (!strcmp(command, "surjective")) {
        LIST_Pairs_T* rel = GetRelation(line, &current_char, relations, all_lines);
        if (!rel) {
            return CommandSafeExit(EXIT_FAILURE, command);
        }
        LIST_Number_T* set_A = GetSet(line, &current_char, sets, all_lines, universum);
        LIST_Number_T* set_B = GetSet(line, &current_char, sets, all_lines, universum);
        if (!set_A || !set_B) {
            return CommandSafeExit(EXIT_FAILURE, command);
        }
        
        if (MoreParams(line, &current_char)) {
            fprintf(stderr, "ERROR Too many parameters provided on line %d\n", line_number);
            return CommandSafeExit(EXIT_FAILURE, command);
        }
        
        if (Surjective(rel, set_A, set_B) == EXIT_FAILURE) {
            return CommandSafeExit(EXIT_FAILURE, command);
        }
    }

    else if (!strcmp(command, "bijective")) {
        LIST_Pairs_T* rel = GetRelation(line, &current_char, relations, all_lines);
        if (!rel) {
            return CommandSafeExit(EXIT_FAILURE, command);
        }
        LIST_Number_T* set_A = GetSet(line, &current_char, sets, all_lines, universum);
        LIST_Number_T* set_B = GetSet(line, &current_char, sets, all_lines, universum);
        if (!set_A || !set_B) {
            return CommandSafeExit(EXIT_FAILURE, command);
        }
        
        if (MoreParams(line, &current_char)) {
            fprintf(stderr, "ERROR Too many parameters provided on line %d\n", line_number);
            return CommandSafeExit(EXIT_FAILURE, command);
        }
        
        if (Bijective(rel, set_A, set_B) == EXIT_FAILURE) {
            return CommandSafeExit(EXIT_FAILURE, command);
        }
    }

    else {  
        fprintf(stderr, "ERROR: Unknown command: '%s'\n", command);
        return CommandSafeExit(EXIT_FAILURE, command);
    }

    return CommandSafeExit(EXIT_SUCCESS, command);
}


/**
 * @brief Safe exit functions that propages received EXIT_CODE and frees all used memory
 * 
 * @param exit_code Exit code
 * @param u Universum Array
 * @param s Array containing all Sets
 * @param r Array containing all Relations
 * @return Propagated Exit code
 */
int SafeExit(int exit_code, DA_Universum_T* u, DA_Sets_T* s, DA_Relations_T* r) {
    DA_Universum_Free(u);

    for (int i = 0; i < s->len; i++) {
        LIST_Number_Free(s->data[i]->Set);
    }
    DA_Sets_Free(s);

    for (int i = 0; i < r->len; i++) {
        LIST_Pairs_Free(r->data[i]->Relation);
    }
    DA_Relations_Free(r);

    for (int i = 0; i < SetsGarbageCollection.len; i++) {
        LIST_Number_Free(SetsGarbageCollection.data[i]->Set);
    }
    DA_Sets_Free(&SetsGarbageCollection);

    for (int i = 0; i < RelationsGarbageCollection.len; i++) {
        LIST_Pairs_Free(RelationsGarbageCollection.data[i]->Relation);
    }
    DA_Relations_Free(&RelationsGarbageCollection);

    return exit_code;
}


/**
 * @brief Parses all loaded lines
 * 
 * @param all_lines All lines to be parsed array
 * @return EXIT_FAILIURE if there was an error during processing otherwise EXIT_SUCCESS 
 */
int ParseLines(DA_Lines_T* all_lines) {
    DA_Universum_T universum;
    DA_Universum_Init(&universum);

    DA_Sets_T sets;
    DA_Sets_Init(&sets);

    DA_Relations_T relations;
    DA_Relations_Init(&relations);

    DA_Sets_Init(&SetsGarbageCollection);
    DA_Relations_Init(&RelationsGarbageCollection);

    for (int array_index = 0; array_index < all_lines->len; array_index++) {
        char* current_line = all_lines->data[array_index];
        char first_char = current_line[0];

        switch (first_char)
        {
            case 'U':
                // Universum definition
                if(ParseUniversum(all_lines->data[array_index], &universum) == EXIT_FAILURE) {
                    return SafeExit(EXIT_FAILURE, &universum, &sets, &relations);
                }

                PrintUniversum(&universum);
                break;
            
            case 'S':
            {
                // Set definition                
                LIST_Number_T* new_elements = ParseSet(all_lines->data[array_index], &universum);
                if (!new_elements) {
                    return SafeExit(EXIT_FAILURE, &universum, &sets, &relations);
                }

                if (InsertSetListToDA(new_elements, &sets) != EXIT_SUCCESS) {
                    LIST_Number_Free(new_elements);
                    return SafeExit(EXIT_FAILURE, &universum, &sets, &relations);
                }
                PrintSet(new_elements, &universum);

                break;
            }                
            
            case 'R':
            {
                // Relation definition
                LIST_Pairs_T* new_pairs = ParseRelation(all_lines->data[array_index], &universum);
                if (!new_pairs) {
                    return SafeExit(EXIT_FAILURE, &universum, &sets, &relations);
                }

                if (InsertRelationListToDA(new_pairs, &relations) != EXIT_SUCCESS) {
                    LIST_Pairs_Free(new_pairs);
                    return SafeExit(EXIT_FAILURE, &universum, &sets, &relations);
                }
                PrintRelation(new_pairs, &universum);

                break;
            }

            case 'C':
                // Command
                if (ParseCommand(all_lines->data[array_index], &universum, &sets, &relations, all_lines) == EXIT_FAILURE) {
                    return SafeExit(EXIT_FAILURE, &universum, &sets, &relations);
                }
                break;

            case '\0':
                if (array_index == all_lines->len - 1) {
                    // Last empty line after \n after previous line
                    continue;
                }
                fprintf(stderr, "ERROR: Empty line detected!\n");

                return SafeExit(EXIT_FAILURE, &universum, &sets, &relations);

            default:
                fprintf(stderr, "ERROR: Unexpected character at the start of the row! - '%c'\n", first_char);
                return SafeExit(EXIT_FAILURE, &universum, &sets, &relations);   
        }
        line_number++;  
    }

    return SafeExit(EXIT_SUCCESS, &universum, &sets, &relations);   
}

/**
 * @brief Loads single line terminated by \n from the fiel
 * 
 * @param f processed file
 * @param new_line return line
 * @return EXIT_FAILIURE if there was an error during processing otherwise EXIT_SUCCESS 
 */
int LoadLine(FILE* f, char** new_line) {
    int len = 0;
    char* line = NULL;

    char c;

    do {
        c = fgetc(f);
        if (c == EOF) {
            free(line);
            return EOF;
        }

        len++;
        char* temp = (char*) realloc(line, sizeof(char) * len);
        if (!temp) {
            fprintf(stderr, "ERROR: During realloc of new line\n");
            free(line);
            return EXIT_FAILURE;
        }
        line = temp;
        line[len - 1] = c;
    } while (c != '\n');
    line[len - 1] = '\0';
    *new_line = line;
    return EXIT_SUCCESS;
}


/**
 * @brief Loads all lines from file
 * 
 * @param f file to process
 * @param lines All loaded lines return
 * @return EXIT_FAILIURE if there was an error during processing otherwise EXIT_SUCCESS 
 */
int LoadAllLines(FILE* f, DA_Lines_T* lines) {
    while (true) {
        char* new_line;
        int exit_code = LoadLine(f, &new_line);
        if (exit_code == EOF) {
            return EXIT_SUCCESS;
        }
        else if (exit_code != EXIT_SUCCESS) {
            return exit_code;
        }

        exit_code = DA_Lines_Insert(lines, new_line);
        if (exit_code != EXIT_SUCCESS) {
            return exit_code;
        } 

        if (lines->len > 1000) {

            fprintf(stderr, "ERROR: Maximal allowed line count is 1000!\n");
            return EXIT_FAILURE;
        }      
    }
}

int main(int argc, char* argv[]) {
    // Program argument check
    if (argc != 2) {
        fprintf(stderr, "ERROR: Expected one program argument - input data file location!\n");
        return EXIT_FAILURE;
    } 

    // Load file
    char* input_file_location = argv[1];
    FILE* input_file = fopen(input_file_location, "r");
    if (!input_file) {
        fprintf(stderr, "ERROR: Failed to open selected input file!\n");
        return EXIT_FAILURE;
    }
    DA_Lines_T lines;
    DA_Lines_Init(&lines);
    if (LoadAllLines(input_file, &lines) != EXIT_SUCCESS) {
        DA_Lines_Free(&lines);
        fclose(input_file);
        return EXIT_FAILURE;
    }

    // Parse file
    int exit_code = ParseLines(&lines); 
    DA_Lines_Free(&lines);
    fclose(input_file);
    return exit_code;
}