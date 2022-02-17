#ifndef MOVIES_H
#define MOVIES_H

#include "Hashtable.h"

void create_ht_task2(hashtable_t **movies, hashtable_t **producers);

void ht_add(hashtable_t *movies, hashtable_t *producers, char *command);

void print_movie_info(hashtable_t *movies, char *command);

void print_producer_movies(hashtable_t *producers, char *command);

void ht_quit(hashtable_t *movies, hashtable_t *producers);

void ht_remove(hashtable_t *movies, hashtable_t *producers);

void ht_redim(hashtable_t *movies, hashtable_t *producers);

void ht_redim_table(hashtable_t *ht);

#endif
