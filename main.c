#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

#define NAX_WORDS   300
#define MAX_GUESSES 10

typedef struct wordnode_s {
    char word[15];
    int  point;    // points assigned for each word
    int  guessed;  // 0 = not guessed, 1 = correctly guessed
} wordnode;

/* ---------- FILE PICKING ---------- */

/// Randomly pick text file from WordSet0..WordSet9.

FILE* pick_text_file(void)
{
    char filename[32];
    int idx = 2;// rand() % 10;       // 0–9

    sprintf(filename, "WordSet%d.txt", idx);

    FILE *fp = fopen(filename, "r");
    if (!fp) {
        perror("Cannot open word set file");
    }
    return fp;
}

/* ---------- LOADING WORDS ---------- */

/// Load words from file into words array.
/// File format: each line is "word point", e.g. "pundit 50".
/// Returns number of words loaded.
int load_words(FILE* inp, wordnode* words, int max_words)
{
    int count = 0;
    char temp_word[15];
    int temp_point;

    // fscanf skips whitespace; extra blank line at bottom is fine.
    while (count < max_words &&
           fscanf(inp, "%14s %d", temp_word, &temp_point) == 2) {
        strcpy(words[count].word, temp_word);
        words[count].point   = temp_point;
        words[count].guessed = 0;
        count++;
    }

    return count;
}

/* ---------- SCRAMBLING / DISPLAY ---------- */

/// Scramble src word and store into dst (first num_letters chars).
void scramble_word(char dst[], const char src[], int num_letters)
{
    int i;

    // Copy up to num_letters or until end of string
    for (i = 0; i < num_letters && src[i] != '\0'; i++) {
        dst[i] = src[i];
    }
    dst[i] = '\0';
    num_letters = i; // in case src shorter than requested

    // Fisher–Yates shuffle
    for (i = num_letters - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        char tmp = dst[i];
        dst[i] = dst[j];
        dst[j] = tmp;
    }
}

void print_scramble_header(char scrambled[])
{
    int i;
    printf("\n@@@ --- @@@ Letters in words @@@ --- @@@\n");
    for (i = 0; scrambled[i] != '\0'; i++) {
        printf(" %c", scrambled[i]);
    }
    printf("\n@@@ --- @@@ --- --- --- --- @@@ --- @@@\n\n");
}

void print_success(int gained_points, int total_points)
{
    printf("\n$$$ --- $$$ --- $$$ --- $$$ --- $$$ --- $$$\n");
    printf("Yay ... you got %d points\n", gained_points);
    printf("Total points: %d\n", total_points);
    printf("$$$ --- $$$ --- $$$ --- $$$ --- $$$ --- $$$\n\n");
}

void print_failure(int num_guesses_left)
{
    printf("\n:( --- :( --- :( --- :( --- :( --- :( --- :(\n");
    printf("Oops ... you lost 1 guess\n");
    printf("You have %d guesses left\n", num_guesses_left);
    printf(":( --- :( --- :( --- :( --- :( --- :( --- :(\n\n");
}

void print_guesses(char incorrect_guess_list[][16],
                   char correct_guess_list[][16],
                   int incorrect_guess_cnt,
                   int correct_guess_cnt)
{
    int i;

    printf("Correct guesses:");
    if (correct_guess_cnt > 0) {
        for (i = 0; i < correct_guess_cnt; i++) {
            printf(" %s", correct_guess_list[i]);
        }
    }
    printf("\n");

    printf("Wrong guesses:");
    if (incorrect_guess_cnt > 0) {
        for (i = 0; i < incorrect_guess_cnt; i++) {
            printf(" %s", incorrect_guess_list[i]);
        }
    }
    // If no wrong guesses, leave line blank after colon 
    printf("\n\n");
}

/// Print title for game
void print_title(void)
{
    printf("|-----------------------------------|\n");
    printf("|- LET's Play Word Finder -|\n");
    printf("|-----------------------------------|\n\n");
}

/* ---------- HINTS ---------- */

void print_masked_word(const char *word)
{
    const char *vowels = "aeiouAEIOU";
    for (int i = 0; word[i] != '\0'; i++) {
        if (strchr(vowels, word[i]) != NULL) {
            printf("%c ", word[i]);
        } else {
            printf("_ ");
        }
    }
    printf("\n");
}

/*
 * Display at most 4 clues.
 * Use first 2 unguessed words and last 2 unguessed words,
 * showing vowels and masking consonants. 
 */
void print_hints(wordnode *word_list, int num_words)
{
    int shown = 0;
    int used_idx[4];
    for (int i = 0; i < 4; i++) used_idx[i] = -1;

    printf("Clues to help you guess\n");

    // First 2 from the front
    for (int i = 0; i < num_words && shown < 2; i++) {
        if (!word_list[i].guessed) {
            print_masked_word(word_list[i].word);
            used_idx[shown] = i;
            shown++;
        }
    }

    // Last 2 from the back (avoid duplicates)
    for (int i = num_words - 1; i >= 0 && shown < 4; i--) {
        if (!word_list[i].guessed) {
            bool already = false;
            for (int k = 0; k < shown; k++) {
                if (used_idx[k] == i) {
                    already = true;
                    break;
                }
            }
            if (!already) {
                print_masked_word(word_list[i].word);
                used_idx[shown] = i;
                shown++;
            }
        }
    }

    if (shown == 0) {
        printf("(no clues left – you found all the words!)\n");
    }
    printf("\n");
}

/* ---------- CHECKING GUESSES ---------- */

/*
 * Returns true if guess is a **new correct** word in word_list.
 * Sets *gained_points to that word's points if correct.
 * If the word was already guessed before, returns false and gained_points = 0.
 */

bool check_guess(char guess[], wordnode* word_list,
                 int num_words, int *gained_points)
{
    for (int i = 0; i < num_words; i++) {
        if (strcmp(guess, word_list[i].word) == 0) {
            if (word_list[i].guessed) {
                *gained_points = 0;
                return false;   // already guessed before
            } else {
                word_list[i].guessed = 1;
                *gained_points = word_list[i].point;
                return true;
            }
        }
    }
    return false;   // not found in list
}

/* ---------- UNGUESSED WORDS / ENDING MESSAGES ---------- */

void print_unguessed_words(wordnode *word_list, int num_words)
{
    printf("Unguessed words:\n");
    for (int i = 0; i < num_words; i++) {
        if (!word_list[i].guessed) {
            printf("%s, ", word_list[i].word);
        }
    }
    printf("\n");
}

void end_quit(int total_points, wordnode *word_list, int num_words)
{
    printf("\n|--- G o o d --- T r y ---|\n");
    printf("|--- You got %d points ---|\n\n", total_points);
    print_unguessed_words(word_list, num_words);
    printf("Good Game ... Good Bye\n");
}

void end_out_of_guesses(int total_points)
{
    printf("\n:( --- :( --- :( --- :( --- :(\n");
    printf("Oops, you used up all your guesses, You got %d points\n",
           total_points);
    printf("Better luck next time\n");
    printf("Good Game ... Good Bye\n");
}

void end_all_correct(int total_points)
{
    printf("\n!!! --- !!! --- !!! --- !!! --- !!!\n");
    printf("You got %d points\n", total_points);
    printf("Congrats, You completed the list!\n");
    printf("Good Game ... Good Bye\n");
}

int main(void)
{
    FILE* inp = NULL;
    wordnode word_list[NAX_WORDS];
    char scrambled[16];
    char guess[16];

    int incorrect_guess_cnt = 0;
    int correct_guess_cnt   = 0;
    int total_points        = 0;

    char incorrect_guess_list[MAX_GUESSES][16];
    char correct_guess_list[NAX_WORDS][16];

    int num_words;
    int max_letters;
    char q_opt;

    srand((unsigned) time(NULL));

    inp = pick_text_file();
    if (inp == NULL) {
        return 0;
    }

    num_words = load_words(inp, word_list, NAX_WORDS);
    fclose(inp);

    if (num_words == 0) {
        printf("No words loaded.\n");
        return 0;
    }

    // Spec: first word in every text file is the longest word.
    max_letters = (int)strlen(word_list[0].word);

    print_title();

    while (1) {
        // Scramble the letters of the longest word every round.
        scramble_word(scrambled, word_list[0].word, max_letters);
        print_scramble_header(scrambled);

        printf("You have %d guesses left\n", MAX_GUESSES - incorrect_guess_cnt);
        print_hints(word_list, num_words);

        // Get a guess between 4 letters and the longest word length.
        while (1) {
            printf("Enter your guess (4-%d characters): ", max_letters);
            scanf("%15s", guess);
            int len = (int)strlen(guess);
            if (len >= 4 && len <= max_letters) {
                break;
            }
            // Keep asking until valid length – invalid ones do NOT cost guesses.
        }

        int gained_points = 0;
        bool correct = check_guess(guess, word_list, num_words, &gained_points);

        if (correct && gained_points > 0) {
            strcpy(correct_guess_list[correct_guess_cnt], guess);
            correct_guess_cnt++;
            total_points += gained_points;
            print_success(gained_points, total_points);
        } else {
            // Wrong guess (or repeated word) -> lose one guess.
            strcpy(incorrect_guess_list[incorrect_guess_cnt], guess);
            incorrect_guess_cnt++;
            print_failure(MAX_GUESSES - incorrect_guess_cnt);
        }

        print_guesses(incorrect_guess_list, correct_guess_list,
                      incorrect_guess_cnt, correct_guess_cnt);

        // Ending condition: used up all guesses
        if (incorrect_guess_cnt >= MAX_GUESSES) {
            end_out_of_guesses(total_points);
            break;
        }

        // Ending condition: guessed all words 
        if (correct_guess_cnt >= num_words) {
            end_all_correct(total_points);
            break;
        }

        // Ask whether user wants to continue
        printf("continue (q to quit): ");
        scanf(" %c", &q_opt);
        if (q_opt == 'q' || q_opt == 'Q') {
            end_quit(total_points, word_list, num_words);
            break;
        }
    }

    return 0;
}
