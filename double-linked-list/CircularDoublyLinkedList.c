#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "CircularDoublyLinkedList.h"
#include "utils.h"

/*
 * Functie care trebuie apelata pentru alocarea si initializarea unei liste.
 * (Setare valori initiale pentru campurile specifice structurii LinkedList).
 */
doubly_linked_list_t*
dll_create(unsigned int data_size)
{
    doubly_linked_list_t *list = 
        (doubly_linked_list_t *)malloc(sizeof(doubly_linked_list_t));
    DIE(!list, "Malloc failed\n");

    list->data_size = data_size;
    list->head = NULL;
    list->size = 0;
    return list;
}

/*
 * Functia intoarce un pointer la nodul de pe pozitia n din lista.
 * Pozitiile din lista sunt indexate incepand cu 0 (i.e. primul nod din lista se
 * afla pe pozitia n=0). Daca n >= nr_noduri, atunci se intoarce nodul de pe
 * pozitia rezultata daca am "cicla" (posibil de mai multe ori) pe lista si am
 * trece de la ultimul nod, inapoi la primul si am continua de acolo. Cum putem
 * afla pozitia dorita fara sa simulam intreaga parcurgere? Daca n < 0, eroare.
 */
dll_node_t*
dll_get_nth_node(doubly_linked_list_t* list, unsigned int n)
{
    n %= list->size;
    dll_node_t *current = list->head;
    for (unsigned int i = 0; i < n; ++i)
        current = current->next;
    return current;
}

/*
 * Pe baza datelor trimise prin pointerul new_data, se creeaza un nou nod care e
 * adaugat pe pozitia n a listei reprezentata de pointerul list. Pozitiile din
 * lista sunt indexate incepand cu 0 (i.e. primul nod din lista se afla pe
 * pozitia n=0). Cand indexam pozitiile nu "ciclam" pe lista circulara ca la
 * get, ci consideram nodurile in ordinea de la head la ultimul (adica acel nod
 * care pointeaza la head ca nod urmator in lista). Daca n >= nr_noduri, atunci
 * adaugam nodul nou la finalul listei. Daca n < 0, eroare.
 */
void
dll_add_nth_node(doubly_linked_list_t* list, unsigned int n, const void* data)
{
    if (n < 0) {
        fprintf(stderr, "Cannot add node with negative index\n");
        return;
    }

    if (n > list->size)
        n = list->size;

    dll_node_t *new = (dll_node_t *)malloc(sizeof(dll_node_t));
    DIE(!new, "Malloc failed\n");

    new->data = malloc(list->data_size * sizeof(char));
    memcpy(new->data, data, list->data_size);
    if (list->size == 0) {
        list->head = new;
        new->next = new;
        new->prev = new;
        list->size++;
        return;
    }

    dll_node_t *it = list->head;
    if (n == 0) {
        it = it->prev;
    } else {
        for (unsigned int i = 0; i < n - 1; ++i)
            it = it->next;
    }

    new->next = it->next;
    it->next->prev = new;
    it->next = new;
    new->prev = it;

    if (n == 0)
        list->head = list->head->prev;
    list->size++;
}

/*
 * Elimina nodul de pe pozitia n din lista al carei pointer este trimis ca
 * parametru. Pozitiile din lista se indexeaza de la 0 (i.e. primul nod din
 * lista se afla pe pozitia n=0). Functia intoarce un pointer spre acest nod
 * proaspat eliminat din lista. Daca n >= nr_noduri - 1, se elimina nodul de la
 * finalul listei. Daca n < 0, eroare. Este responsabilitatea apelantului sa
 * elibereze memoria acestui nod.
 */
dll_node_t*
dll_remove_nth_node(doubly_linked_list_t* list, unsigned int n)
{
    if (n < 0) {
        fprintf(stderr, "Cannot remove node with negative index\n");
        return NULL;
    }
    if (n >= list->size)
        n = list->size - 1;
    
    dll_node_t *ret, *it;
    if (n == 0) {
        n = list->size - 1;
        list->head = list->head->next;
    }
    it = list->head;
    for (unsigned int i = 0; i < n - 1; ++i)
        it = it->next;
    ret = it->next;
    it->next = ret->next;
    ret->next->prev = it;
    ret->next = NULL;
    ret->prev = NULL;
    list->size--;
    return ret;
}

/*
 * Functia intoarce numarul de noduri din lista al carei pointer este trimis ca
 * parametru.
 */
unsigned int
dll_get_size(doubly_linked_list_t* list)
{
    return list->size;
}

/*
 * Procedura elibereaza memoria folosita de toate nodurile din lista, iar la
 * sfarsit, elibereaza memoria folosita de structura lista.
 */
void
dll_free(doubly_linked_list_t** pp_list)
{
    if ((*pp_list)->size == 0) {
        free(*pp_list);
        return;
    }

    if ((*pp_list)->size == 1) {
        free((*pp_list)->head->data);
        free(((*pp_list)->head));
        free(*pp_list);
        return;
    }

    dll_node_t *it = (*pp_list)->head->prev;
    for (unsigned int i = 0; i < (*pp_list)->size - 1; ++i) {
        it = it->prev;
        free(it->next->data);
        free(it->next);
    }

    free(it->data); // la final, it == list->head
    free(it);
    free(*pp_list);
}

/*
 * Atentie! Aceasta functie poate fi apelata doar pe liste ale caror noduri STIM
 * ca stocheaza int-uri. Functia afiseaza toate valorile int stocate in nodurile
 * din lista separate printr-un spatiu, incepand de la primul nod din lista.
 */
void
dll_print_int_list(doubly_linked_list_t* list)
{
    dll_node_t *it = list->head;;
    for (unsigned int i = 0; i < list->size; ++i) {
        int *num = (int *)it->data;
        printf("%d ", *num);
        it = it->next;
    }

    printf("\n");
}

/*
 * Atentie! Aceasta functie poate fi apelata doar pe liste ale caror noduri STIM
 * ca stocheaza string-uri. Functia afiseaza toate string-urile stocate in
 * nodurile din lista separate printr-un spatiu, incepand de la primul nod din
 * lista.
 */
void
dll_print_string_list(doubly_linked_list_t* list)
{
    dll_node_t *it = list->head;
    for (unsigned int i = 0; i < list->size; ++i) {
        char *str = (char *)it->data;
        printf("%s ", str);
        it = it->next;
    }

    printf("\n");
}

/*
 * Atentie! Aceasta functie poate fi apelata doar pe liste ale caror noduri STIM
 * ca stocheaza int-uri. Functia afiseaza o singura data toate valorile int
 * stocate in nodurile din lista, separate printr-un spatiu, incepand de la
 * nodul dat ca parametru si continuand la stanga in lista dublu inlantuita
 * circulara, pana cand sunt afisate valorile tuturor nodurilor.
 */
void
dll_print_ints_left_circular(dll_node_t* start)
{
    dll_node_t *it = start;
    int *num = (int *)it->data;
    printf("%d ", *num);
    it = it->prev;
    while (it != start) {
        num = (int *)it->data;
        printf("%d ", *num);
        it = it->prev;
    }

    printf("\n");
}

/*
 * Atentie! Aceasta functie poate fi apelata doar pe liste ale caror noduri STIM
 * ca stocheaza int-uri. Functia afiseaza o singura data toate valorile int
 * stocate in nodurile din lista, separate printr-un spatiu, incepand de la
 * nodul dat ca parametru si continuand la dreapta in lista dublu inlantuita
 * circulara, pana cand sunt afisate valorile tuturor nodurilor.
 */
void
dll_print_ints_right_circular(dll_node_t* start)
{
    dll_node_t *it = start;
    int *num = (int *)it->data;
    printf("%d ", *num);
    it = it->next;
    while (it != start) {
        num = (int *)it->data;
        printf("%d ", *num);
        it = it->next;
    }

    printf("\n");
}

void
swap(dll_node_t **a, dll_node_t **b)
{
    dll_node_t *aux = *a;
    *a = *b;
    *b = aux;
}

/*
 * Functie care rezolva task2.c, si anume inverseaza nodurile unei liste
 * circulare dublu inlantuite (inverseaza pointerii unui nod, corespunzatori
 * campurilor next si prev)
 */
void
dll_reverse_list(doubly_linked_list_t *list)
{
    dll_node_t *it = list->head;
    for (unsigned int i = 0; i < list->size; ++i) {
        swap(&it->prev, &it->next);
        it = it->prev;
    }
    list->head = list->head->next;
}
