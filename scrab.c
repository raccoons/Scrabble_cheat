#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#include "scrab.h"

const int tile_scores[ALPHABET_LENGTH] = {
	1, 3, 3, 2, 1, 4, 2, 4, 1,
	8, 5, 1, 3, 1, 1, 3, 10, 1,
	1, 1, 1, 4, 4, 8, 4, 10
};

Word_array *create_word_list()
{
	Word_array *WA = malloc(sizeof(Word_array));
	WA->max = 10;
	WA->length = 0;
	WA->word_list = malloc(sizeof(Word) * WA->max);

	return WA;
}

void Word_array_push(Word_array *WA, char *word)
{
	Word *W = new_word(word);

	if(WA->length == WA->max) {
		WA->max += 10;
		WA->word_list = realloc(WA->word_list, sizeof(Word *) * WA->max);
	}

	WA->word_list[WA->length] = W;
	WA->length++;
}

void delete_word_list(Word_array *WA)
{
	int i;

	for(i = 0; i < WA->length; i++) {
		free(WA->word_list[i]);
	}

	free(WA->word_list);
	free(WA);
}

Word *new_word(char *word)
{
	Word *W = malloc(sizeof(Word));
	strcpy(W->word, word);
	W->score = get_word_score(word);

	return W;
}

void add_to_map(char *tiles, char *tile_map)
{
	int i;
	char c;

	for(i = 0; i < strlen(tiles); i++) {
		c = toupper(tiles[i]);
		tiles[i] = toupper(tiles[i]);
		tile_map[c-'A'] += 1;
	}
}

int get_word_score(char *word)
{
	int score = 0;
	int i;

	for(i = 0; i < strlen(word); i++) {
		score += tile_scores[word[i] - 'A'];
	}

	return score;
}

int compare_scores(Word **a, Word **b) {
	return ((*b)->score - (*a)->score);
}

int main(int argc, char *argv[])
{
	if(argc < 2) {
		printf("Error: Need letters argument\n");
		return EXIT_FAILURE;
	}

	char *tiles = argv[1];
	char tile_map[ALPHABET_LENGTH];
	char tile_map_copy[ALPHABET_LENGTH];  /* For resetting after each word */
	int i;
	FILE *fp;

	memset(tile_map, 0, sizeof(char) * ALPHABET_LENGTH);
	add_to_map(tiles, tile_map);
	memcpy(tile_map_copy, tile_map,  sizeof(char) * ALPHABET_LENGTH);

	bool found;
	char word[MAX_WORD_LENGTH] = "";
	fp = fopen("scrabdict.txt", "r");
	Word_array *found_words = create_word_list();

	while(fscanf(fp, "%s\n", word) != EOF) {
		for(i = 0; i < strlen(word); i++) {
			if(strchr(tiles, word[i]) && tile_map[word[i]-'A'] > 0) {
				tile_map[word[i]-'A'] -= 1;
				found = true;
			} else {
				found = false;
				break;
			}
		}
		if(found) {
			Word_array_push(found_words, word);
		} 
		/* Reset the tile map from the copy made at the start */
		memcpy(tile_map, tile_map_copy, sizeof(char) * ALPHABET_LENGTH); 
	} 
	
	qsort(found_words->word_list, found_words->length,
		sizeof(Word *), (word_compare)compare_scores);

	for(i = 0; i < found_words->length; i++) {
		printf("%s (%d), ", found_words->word_list[i]->word, 
				found_words->word_list[i]->score);
	} printf("\n");

	delete_word_list(found_words);
	fclose(fp);

	return EXIT_SUCCESS;
}