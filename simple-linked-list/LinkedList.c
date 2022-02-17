#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "LinkedList.h"
#include "utils.h"

linked_list_t*
ll_create(unsigned int data_size)
{
    linked_list_t *list = (linked_list_t *)malloc(sizeof(linked_list_t));
    DIE(!list, "Malloc failed\n");
    list->head = NULL;
    list->size = 0;
    list->data_size = data_size;
    return list;
}

/*
 * Pe baza datelor trimise prin pointerul new_data, se creeaza un nou nod care e
 * adaugat pe pozitia n a listei reprezentata de pointerul list. Pozitiile din
 * lista sunt indexate incepand cu 0 (i.e. primul nod din lista se afla pe
 * pozitia n=0). Daca n >= nr_noduri, noul nod se adauga la finalul listei. Daca
 * n < 0, eroare.
 */
void
ll_add_nth_node(linked_list_t* list, unsigned int n, const void* new_data)
{
    if (n < 0)
        return;

    ll_node_t *newnode = (ll_node_t *)malloc(sizeof(ll_node_t));
    DIE(!newnode, "Malloc failed\n");

    newnode->data = malloc(list->data_size * sizeof(char));
    DIE(!newnode->data, "Malloc failed\n");

    memcpy(newnode->data, new_data, list->data_size);

    if (n == 0 || list->size == 0) {
        newnode->next = list->head;
        list->head = newnode;
        list->size++;
        return;
    }

    ll_node_t *current = list->head;
    if (n > list->size)
        n = list->size;

    for (unsigned int i = 0; i < n - 1; ++i) {
        current = current->next;
    }
    newnode->next = current->next;
    current->next = newnode;
    list->size++;
}

/*
 * Elimina nodul de pe pozitia n din lista al carei pointer este trimis ca
 * parametru. Pozitiile din lista se indexeaza de la 0 (i.e. primul nod din
 * lista se afla pe pozitia n=0). Daca n >= nr_noduri - 1, se elimina nodul de
 * la finalul listei. Daca n < 0, eroare. Functia intoarce un pointer spre acest
 * nod proaspat eliminat din lista. Este responsabilitatea apelantului sa
 * elibereze memoria acestui nod.
 */

/* Atentie !!! Am dat free la nodul extras din lista pentru ca programul creat
 * sa functioneze corect, fara memory leak-uri, intrucat am observat ca in
 * TestLinkedList.c se doreste distrugerea nodului propriu-zis, fara extragerea
 * acestuia. Am lasat comentata si secventa de cod corecta care corespunde
 * 100% cerintei
 */
ll_node_t*
ll_remove_nth_node(linked_list_t* list, unsigned int n)
{
    ll_node_t *todelete;
    if (n == 0) {
        todelete = list->head;
        list->head = list->head->next;
        list->size--;
        return todelete;
    }

    if (n >= list->size)
        n = list->size - 1;

    todelete = list->head;
    for (unsigned int i = 0; i < n - 1; ++i) { // mergem la nodul n - 1
        todelete = todelete->next;
    }
    ll_node_t *prevnode = todelete;
    todelete = todelete->next;
    prevnode->next = todelete->next;
    list->size--;

    free(todelete->data); // partea de eliberare nu apare in rezolvarea cerintei
    free(todelete);
    return NULL; // in cazul cerintei: return todelete; (fara liniile 97, 98)
}

/*
 * Functia intoarce numarul de noduri din lista al carei pointer este trimis ca
 * parametru.
 */
unsigned int
ll_get_size(linked_list_t* list)
{
    return list->size;
}

/*
 * Procedura elibereaza memoria folosita de toate nodurile din lista, iar la
 * sfarsit, elibereaza memoria folosita de structura lista si actualizeaza la
 * NULL valoarea pointerului la care pointeaza argumentul (argumentul este un
 * pointer la un pointer).
 */
void
ll_free(linked_list_t** pp_list)
{
    ll_node_t *current = (*pp_list)->head;
    ll_node_t *nextnode = current->next;
    for (unsigned int i = 0; i < (*pp_list)->size - 1; ++i) {
        free(current->data);
        free(current);
        current = nextnode;
        nextnode = nextnode->next;
    }
    free(current->data); // pentru ultimul nod
    free(current);
    free(*pp_list);
    *pp_list = NULL;
}

/*
 * Atentie! Aceasta functie poate fi apelata doar pe liste ale caror noduri STIM
 * ca stocheaza int-uri. Functia afiseaza toate valorile int stocate in nodurile
 * din lista inlantuita separate printr-un spatiu.
 */
void
ll_print_int(linked_list_t* list)
{
    ll_node_t *current = list->head;
    for (unsigned int i = 0; i < list->size; ++i) {
        int *number = (int *)(current->data);
        printf("%d ", *number);
        current = current->next;
    }

    printf("\n");
}

/*
 * Atentie! Aceasta functie poate fi apelata doar pe liste ale caror noduri STIM
 * ca stocheaza string-uri. Functia afiseaza toate string-urile stocate in
 * nodurile din lista inlantuita, separate printr-un spatiu.
 */
void
ll_print_string(linked_list_t* list)
{
    ll_node_t *current = list->head;
    for (unsigned int i = 0; i < list->size; ++i) {
        char *str = (char *)(current->data);
        printf("%s ", str);
        current = current->next;
    }

    printf("\n");
}

/* Functie special scrisa, folosita la sortarea crescatoare a unei liste
*/
void
sort_ints(void *a, void *b)
{
    int *a_int = (int *)a, *b_int = (int *)b;
    if (*a_int <= *b_int)
        return;

    void *aux = malloc(sizeof(int));
    DIE(!aux, "Malloc failed\n");

    memcpy(aux, a, sizeof(int));
    memcpy(a, b, sizeof(int));
    memcpy(b, aux, sizeof(int));
    free(aux);
}

/*
 * O functie care realizeaza sortarea unei liste simplu inlantuite cu intregi.
 * Selection sort adaptat la liste simplu inlantuite (sortare crescatoare).
 */
void
ll_sort(linked_list_t *list)
{
    ll_node_t *node_i = list->head, *node_j;
    while (node_i->next != NULL) {
        node_j = node_i->next;
        while (node_j != NULL) {
            sort_ints(node_i->data, node_j->data);
            node_j = node_j->next;
        }
        node_i = node_i->next;
    }
}

/*
 * O functie care intoarce un pointer la o lista simplu inlantuita obtinuta din
 * interclasarea a 2 liste sortate, primite ca parametru.
 */
linked_list_t *
ll_merge(linked_list_t *list1, linked_list_t *list2)
{
    ll_node_t *current1 = list1->head, *current2 = list2->head;
    linked_list_t *list = ll_create(list1->data_size);
    while (current1 != NULL && current2 != NULL) {
        int value1 = *((int *)current1->data);
        int value2 = *((int *)current2->data);
        if (value1 > value2) {
            ll_add_nth_node(list, list->size, current2->data);
            current2 = current2->next;
        } else if (value1 < value2) {
            ll_add_nth_node(list, list->size, current1->data);
            current1 = current1->next;
        } else {
            ll_add_nth_node(list, list->size, current1->data);
            ll_add_nth_node(list, list->size, current2->data);
            current1 = current1->next;
            current2 = current2->next;
        }
    }
    if (current1 == NULL && current2 == NULL) {
        return list;
    } else if (current1 != NULL) {
        while (current1 != NULL) {
            ll_add_nth_node(list, list->size, current1->data);
            current1 = current1->next;
        }
    } else {
        while (current2 != NULL) {
            ll_add_nth_node(list, list->size, current2->data);
            current2 = current2->next;
        }
    }
    return list;
}
