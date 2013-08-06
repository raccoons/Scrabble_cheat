#ifndef scrab_h
#define scrab_h

#define ALPHABET_LENGTH 26
#define MAX_WORD_LENGTH 16

typedef struct {
	char word[MAX_WORD_LENGTH];
	int score;
} Word;

typedef struct {
	Word **word_list;
	int max;
	int length;
} Word_array;

typedef int (*word_compare)(const void *a, const void *b);

Word_array *create_word_list();
void Word_array_push(Word_array *WA, char *word);
void delete_word_list(Word_array *WA);
Word *new_word(char *word);
void add_to_map(char *tiles, char *tile_map);
int get_word_score(char *word);
int compare_scores(Word **a, Word **b);
int check_input(char *input);

#endif