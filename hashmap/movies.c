#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "movies.h"
#include "utils.h"

void create_ht_task2(hashtable_t **movies, hashtable_t **producers)
{
	if (*movies || *producers) {
		fprintf(stderr, "Hashtables already exist\n");
		return;
	}
	*movies = ht_create(10, hash_function_string, compare_function_strings);
	*producers = ht_create(10, hash_function_producer,
				compare_function_producers);
}

void ht_add(hashtable_t *movies, hashtable_t *producers, char *command)
{
	const char delim[] = "\"\"";
	char *token = strtok(command, delim);
	token = strtok(NULL, delim);
	char movie_name[NMAX];
	strcpy(movie_name, token);
	token = strtok(NULL, delim);
	token = strtok(NULL, delim);
	char producer[NAME_LEN];
	strcpy(producer, token);
	token = strtok(NULL, delim);
	int year, rating;
	sscanf(token, "%d%d", &year, &rating);

	struct movie_info *info = (struct movie_info *)
							 malloc(sizeof(struct movie_info));
	strcpy(info->producer, producer);
	info->year = year;
	info->rating = rating;
	ht_put(movies, movie_name, NMAX, info, sizeof(struct movie_info));

	struct producer_key *key = (struct producer_key *)
							  malloc(sizeof(struct producer_key));
	struct producer_value *value = (struct producer_value *)
								  malloc(sizeof(struct producer_value));
	strcpy(key->producer, producer);
	strcpy(key->movie_name, movie_name);
	value->year = year;
	value->rating = rating;
	ht_put(producers, key, sizeof(*key), value, sizeof(*value));

	free(value);
	free(key);
	free(info);
}

void print_movie_info(hashtable_t *movies, char *command)
{
	const char delim[] = "\"\"";
	char *token = strtok(command, delim);
	token = strtok(NULL, delim);
	struct movie_info *info = (struct movie_info *)ht_get(movies, token);
	if (!info) {
		printf("Nu exista acest film in baza de date\n");
		return;
	}

	printf("Nume: %s\n", token);
	printf("Regizor: %s\n", info->producer);
	printf("An: %d\n", info->year);
	printf("Rating: %d\n", info->rating);
}

void print_producer_movies(hashtable_t *producers, char *command)
{
	const char delim[] = "\"\"";
	char *token = strtok(command, delim);
	token = strtok(NULL, delim);
	unsigned int index = hash_function_string(token) % producers->hmax;
	ll_node_t *it = producers->buckets[index]->head;
	int exists = 0;
	while (it != NULL) {
		void *key = ((struct info *)(it->data))->key;
		char *prod = ((struct producer_key *)key)->producer;
		char *movie = ((struct producer_key *)key)->movie_name;
		if (!strcmp(prod, token)) {
			printf("Nume: %s\n", movie);
			exists = 1;
		}
		it = it->next;
	}
	if (exists == 0)
		printf("Never heard about him\n");
}

void ht_quit(hashtable_t *movies, hashtable_t *producers)
{
	if (movies)
		ht_free(movies);
	if (producers)
		ht_free(producers);
}

void ht_remove(hashtable_t *movies, hashtable_t *producers)
{
	for (unsigned int i = 0; i < movies->hmax; ++i) {
		ll_node_t *it = movies->buckets[i]->head;
		while (it != NULL) {
			void *value = ((struct info *)(it->data))->value;
			void *key = ((struct info *)(it->data))->key;
			int rating = ((struct movie_info *)value)->rating;
			if (rating < 5) {
				char producer[NAME_LEN], movie_name[NMAX];
				strcpy(producer, ((struct movie_info *)value)->producer);
				strcpy(movie_name, (char *)key);
				it = it->next;
				ht_remove_entry(movies, movie_name);
				struct producer_key *p_key = (struct producer_key *)
											malloc(sizeof(*p_key));
				strcpy(p_key->producer, producer);
				strcpy(p_key->movie_name, movie_name);
				ht_remove_entry(producers, p_key);
				free(p_key);
				continue;
			}
			it = it->next;
		}
	}
}

void ht_redim_table(hashtable_t *ht)
{
	linked_list_t **tmp = (linked_list_t **)realloc(ht->buckets,
						2 * ht->hmax * sizeof(linked_list_t *));
	DIE(!tmp, "Realloc failed\n");

	ht->buckets = tmp;
	for (unsigned int i = ht->hmax; i < 2 * ht->hmax; ++i)
		ht->buckets[i] = ll_create(sizeof(struct info));

	for (unsigned int i = 0; i < ht->hmax; ++i) {
		ll_node_t *it = ht->buckets[i]->head, *curr;
		int count = 0;
		while (it != NULL) {
			void *key = ((struct info *)(it->data))->key;
			int index = ht->hash_function(key) % (2 * ht->hmax);
			it = it->next;
			++count;
			if ((unsigned int) index == i)
				continue;
			curr = ll_remove_nth_node(ht->buckets[i], count - 1);
			if (ht->buckets[i]->size == 0)
				ht->size--;
			if (ht->buckets[i + ht->hmax]->size == 0)
				++ht->size;
			ll_add_nth_node(ht->buckets[i + ht->hmax],
							ht->buckets[i + ht->hmax]->size, curr->data);
			free(curr->data);
			free(curr);
		}
	}

	ht->hmax *= 2;
}

void ht_redim(hashtable_t *movies, hashtable_t *producers)
{
	// printf("%d %d %d %d\n", movies->size, movies->hmax, producers->size, producers->hmax);
	if ((3 * movies->hmax) < (4 * movies->size)) {
		ht_redim_table(movies);
		printf("Movies resized\n");
	} else {
		printf("Movies table is empty enough: %.2lf\n",
			  ((double)movies->size) / movies->hmax);
	}
	if ((3 * producers->hmax) < (4 * producers->size)) {
		ht_redim_table(producers);
		printf("Producers resized\n");
	} else {
		printf("Producers table is empty enough: %.2lf\n",
			  ((double)producers->size) / producers->hmax);
	}
}