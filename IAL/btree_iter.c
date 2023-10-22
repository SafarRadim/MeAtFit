/*
 * Binárny vyhľadávací strom — iteratívna varianta
 *
 * S využitím dátových typov zo súboru btree.h, zásobníkov zo súborov stack.h a
 * stack.c a pripravených kostier funkcií implementujte binárny vyhľadávací
 * strom bez použitia rekurzie.
 */

#include "../btree.h"
#include "stack.h"
#include <stdio.h>
#include <stdlib.h>

/*
 * Inicializácia stromu.
 *
 * Užívateľ musí zaistiť, že incializácia sa nebude opakovane volať nad
 * inicializovaným stromom. V opačnom prípade môže dôjsť k úniku pamäte (memory
 * leak). Keďže neinicializovaný ukazovateľ má nedefinovanú hodnotu, nie je
 * možné toto detegovať vo funkcii.
 */
void bst_init(bst_node_t **tree)
{
    *tree = NULL;
}

/*
 * Nájdenie uzlu v strome.
 *
 * V prípade úspechu vráti funkcia hodnotu true a do premennej value zapíše
 * hodnotu daného uzlu. V opačnom prípade funckia vráti hodnotu false a premenná
 * value ostáva nezmenená.
 *
 * Funkciu implementujte iteratívne bez použitia vlastných pomocných funkcií.
 */
bool bst_search(bst_node_t *tree, char key, int *value)
{
    while (tree) {
        if (tree->key == key) {
            *value = tree->value;
            return true;
        }
        if (tree->key > key) {
            tree = tree->left;
        } else {
            tree = tree->right;
        }
    }
    return false;
}

/*
 * Vloženie uzlu do stromu.
 *
 * Pokiaľ uzol so zadaným kľúčom v strome už existuje, nahraďte jeho hodnotu.
 * Inak vložte nový listový uzol.
 *
 * Výsledný strom musí spĺňať podmienku vyhľadávacieho stromu — ľavý podstrom
 * uzlu obsahuje iba menšie kľúče, pravý väčšie.
 *
 * Funkciu implementujte iteratívne bez použitia vlastných pomocných funkcií.
 */
void bst_insert(bst_node_t **tree, char key, int value)
{
    bst_node_t *tmp = NULL;
    bst_node_t *prev = NULL;
    bool side;

    if (*tree) {
        if ((*tree)->key == key) {
            (*tree)->value = value;
            return;
        }

        if ((*tree)->key > key) {
            side = true;
            prev = *tree;
            tmp = (*tree)->left;
        } else {
            side = false;
            prev = *tree;
            tmp = (*tree)->right;
        }
    }

    while(tmp) {
        if (tmp->key == key) {
            tmp->value = value;
            return;
        }

        if (tmp->key > key) {
            side = true;
            prev = tmp;
            tmp = tmp->left;
        } else {
            side = false;
            prev = tmp;
            tmp = tmp->right;
        }
    }

    tmp = malloc(sizeof(bst_node_t));
    tmp->key = key;
    tmp->value = value;
    tmp->left = NULL;
    tmp->right = NULL;

    if (!(*tree)) *tree = tmp;
    else side ? (prev->left = tmp) : (prev->right = tmp);

}

/*
 * Pomocná funkcia ktorá nahradí uzol najpravejším potomkom.
 *
 * Kľúč a hodnota uzlu target budú nahradené kľúčom a hodnotou najpravejšieho
 * uzlu podstromu tree. Najpravejší potomok bude odstránený. Funkcia korektne
 * uvoľní všetky alokované zdroje odstráneného uzlu.
 *
 * Funkcia predpokladá že hodnota tree nie je NULL.
 *
 * Táto pomocná funkcia bude využitá pri implementácii funkcie bst_delete.
 *
 * Funkciu implementujte iteratívne bez použitia vlastných pomocných funkcií.
 */
void bst_replace_by_rightmost(bst_node_t *target, bst_node_t **tree)
{
    bst_node_t *ptr = (*tree)->right;
    bst_node_t *sec = *tree;

    while (ptr->right) {
       sec = ptr;
       ptr = ptr->right;
    }

    bst_node_t *tmp = ptr->left;
    target->key = ptr->key;
    target->value = ptr->value;
    free(ptr);
   
    sec->right = tmp;
}

/*
 * Odstránenie uzlu v strome.
 *
 * Pokiaľ uzol so zadaným kľúčom neexistuje, funkcia nič nerobí.
 * Pokiaľ má odstránený uzol jeden podstrom, zdedí ho otec odstráneného uzla.
 * Pokiaľ má odstránený uzol oba podstromy, je nahradený najpravejším uzlom
 * ľavého podstromu. Najpravejší uzol nemusí byť listom!
 * Funkcia korektne uvoľní všetky alokované zdroje odstráneného uzlu.
 *
 * Funkciu implementujte iteratívne pomocou bst_replace_by_rightmost a bez
 * použitia vlastných pomocných funkcií.
 */
void bst_delete(bst_node_t **tree, char key)
{
    bst_node_t *tmp;
    bst_node_t *ptr;

    bool side;
    
    if ((*tree)->key == key) {
        tmp = *tree;
    } else if ((*tree)->key > key) {
        side = true;
        ptr = (*tree);
        tmp = (*tree)->left;
    } else if ((*tree)->key < key) {
        side = false;
        ptr = (*tree);
        tmp = (*tree)->right;
    }

    while (tmp) {
        if (tmp->key == key) break;
        
        if (tmp->key > key) {
            side = true;
            ptr = tmp;
            tmp = tmp->left;
        } else {
            side = false;
            ptr = tmp;
            tmp = tmp->right;
        }
    }

    if (!tmp) return;
    
    if (tmp->right && tmp->left) {
        bst_node_t *left = tmp->left;
        bst_replace_by_rightmost(tmp, &left);
        tmp->left = left;
    } else if (tmp->right) {
        side ? (ptr->left = tmp->right) : (ptr->right = tmp->right);
        free(tmp);
    } else if (tmp->left) {
        side ? (ptr->left = tmp->left) : (ptr->right = tmp->left);
        free(tmp);
    } else {
        free(tmp);
        side ? (ptr->left = NULL) : (ptr->right = NULL);
        tmp = NULL;
    }
}

/*
 * Zrušenie celého stromu.
 *
 * Po zrušení sa celý strom bude nachádzať v rovnakom stave ako po
 * inicializácii. Funkcia korektne uvoľní všetky alokované zdroje rušených
 * uzlov.
 *
 * Funkciu implementujte iteratívne pomocou zásobníku uzlov a bez použitia
 * vlastných pomocných funkcií.
 */
void bst_dispose(bst_node_t **tree)
{ 
    stack_bst_t *stack = malloc(sizeof(stack_bst_t));
    stack_bst_init(stack);

    bst_node_t *next = *tree;
    
    while (!stack_bst_empty(stack) || next) {
        *tree = next;
        if ((*tree)->right) stack_bst_push(stack, (*tree)->right);

        next = (*tree)->left;
        free(*tree);
        *tree = NULL;

        if (!next && !stack_bst_empty(stack)) next = stack_bst_pop(stack);
    }

    free(stack);
}

/*
 * Pomocná funkcia pre iteratívny preorder.
 *
 * Prechádza po ľavej vetve k najľavejšiemu uzlu podstromu.
 * Nad spracovanými uzlami zavola bst_print_node a uloží ich do zásobníku uzlov.
 *
 * Funkciu implementujte iteratívne pomocou zásobníku uzlov a bez použitia
 * vlastných pomocných funkcií.
 */
void bst_leftmost_preorder(bst_node_t *tree, stack_bst_t *to_visit)
{
    while (tree) {
        stack_bst_push(to_visit, tree);
        bst_print_node(tree);
        tree = tree->left;
    } 
}

/*
 * Preorder prechod stromom.
 *
 * Pre aktuálne spracovávaný uzol nad ním zavolajte funkciu bst_print_node.
 *
 * Funkciu implementujte iteratívne pomocou funkcie bst_leftmost_preorder a
 * zásobníku uzlov bez použitia vlastných pomocných funkcií.
 */
void bst_preorder(bst_node_t *tree)
{
    stack_bst_t *stack = malloc(sizeof(stack_bst_t));
    stack_bst_init(stack);

    while (!stack_bst_empty(stack) || tree) {
        bst_leftmost_preorder(tree, stack);
        if (!stack_bst_empty(stack)) tree = stack_bst_pop(stack);
        if (tree) tree = tree->right;
    }

    free(stack);
}

/*
 * Pomocná funkcia pre iteratívny inorder.
 *
 * Prechádza po ľavej vetve k najľavejšiemu uzlu podstromu a ukladá uzly do
 * zásobníku uzlov.
 *
 * Funkciu implementujte iteratívne pomocou zásobníku uzlov a bez použitia
 * vlastných pomocných funkcií.
 */
void bst_leftmost_inorder(bst_node_t *tree, stack_bst_t *to_visit)
{
    while (tree) {
        stack_bst_push(to_visit, tree);
        tree = tree->left;
    } 
}

/*
 * Inorder prechod stromom.
 *
 * Pre aktuálne spracovávaný uzol nad ním zavolajte funkciu bst_print_node.
 *
 * Funkciu implementujte iteratívne pomocou funkcie bst_leftmost_inorder a
 * zásobníku uzlov bez použitia vlastných pomocných funkcií.
 */
void bst_inorder(bst_node_t *tree)
{
    stack_bst_t *stack = malloc(sizeof(stack_bst_t));
    stack_bst_init(stack);

    while (!stack_bst_empty(stack) || tree) {
        bst_leftmost_inorder(tree, stack);
        if (!stack_bst_empty(stack)) tree = stack_bst_pop(stack);
        bst_print_node(tree);
        if (tree) tree = tree->right;
    }

    free(stack);
}

/*
 * Pomocná funkcia pre iteratívny postorder.
 *
 * Prechádza po ľavej vetve k najľavejšiemu uzlu podstromu a ukladá uzly do
 * zásobníku uzlov. Do zásobníku bool hodnôt ukladá informáciu že uzol
 * bol navštívený prvý krát.
 *
 * Funkciu implementujte iteratívne pomocou zásobníkov uzlov a bool hodnôt a bez použitia
 * vlastných pomocných funkcií.
 */
void bst_leftmost_postorder(bst_node_t *tree, stack_bst_t *to_visit, stack_bool_t *first_visit)
{
    while (tree) {
        stack_bst_push(to_visit, tree);
        stack_bool_push(first_visit, true);
        tree = tree->left;
    } 
}

/*
 * Postorder prechod stromom.
 *
 * Pre aktuálne spracovávaný uzol nad ním zavolajte funkciu bst_print_node.
 *
 * Funkciu implementujte iteratívne pomocou funkcie bst_leftmost_postorder a
 * zásobníkov uzlov a bool hodnôt bez použitia vlastných pomocných funkcií.
 */
void bst_postorder(bst_node_t *tree)
{
    bool side;

    stack_bst_t *stack = malloc(sizeof(stack_bst_t));
    stack_bst_init(stack);

    stack_bool_t *bool_stack = malloc(sizeof(stack_bool_t));
    stack_bool_init(bool_stack);

    bst_leftmost_postorder(tree, stack, bool_stack);

    while(!stack_bst_empty(stack)) {
        tree = stack_bst_top(stack);
        side = stack_bool_pop(bool_stack);

        if (side) {
            stack_bool_push(bool_stack, false);
            bst_leftmost_postorder(tree->right, stack, bool_stack);
        } else {
            stack_bst_pop(stack);
            bst_print_node(tree);
        }
    }

    free(stack);
    free(bool_stack);
}
