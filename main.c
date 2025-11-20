#include <stdio.h>

#define NAX_WORDS 300
#define MAX_GUESSES 10

typedef struct wordnode_s {
    char word[15];
    int point; //points assigned for each word
} wordnode;

/// <summary>
///  Randomly pick text file from 0-9. pick #2 when developing.
/// </summary>
FILE* pick_text_file()
{
    return NULL;
}

/// <summary>
/// Load words from file into words array.
/// </summary>
/// returns number of words in file.
int load_words(FILE* inp, wordnode* words, int num_words)
{
    return 0;
}

/// <summary>
/// scambled src word and store into dst.
/// </summary>
void scramble_word(char dst[], char src[], int num_letters)
{

}

void print_scramble_header(char scrambled[])
{
    /*
    @@@ --- @@@ Letters in words @@@ --- @@@
    p i t d n u
    @@@ --- @@@ --- --- --- --- @@@ --- @@@
    */
}

void print_success(int points)
{
    /*
    $$$ --- $$$ --- $$$ --- $$$ --- $$$ --- $$$
    Yay ... you got 50 points
    Total points: 50
    $$$ --- $$$ --- $$$ --- $$$ --- $$$ --- $$
    */
}

void print_failure(int num_guesses_left)
{
    /*
    :( --- :( --- :( --- :( --- :( --- :( --- :(
    Oops ... you lost 1 guess
    You have 9 guesses left
    :( --- :( --- :( --- :( --- :( --- :( --- :(
    */
}

void print_guesses(char incorrect_guess_list[][16], char correct_guess_list[][16], int incorrect_guess_cnt, int correct_guess_cnt)
{
    // Correct guesses : pundit unit
    // Wrong guesses : dunt
}

/// <summary>
/// Print title for game.
/// </summary>
void print_title()
{
    /*
    |-----------------------------------|
    |- LET's Play Word Finder -|
    |-----------------------------------|
    */
}

void print_hints()
{
    /*
    Clues to help you guess
    u _ i _
    _ u _ _
    _ u _ _ i _
    i _ _ u _
    */
}

bool check_guess(char guess[], wordnode* word_list, int num_worlds)
{
    return true;
}

int main()
{
    FILE* inp = NULL;
    wordnode* word_list = NULL;
    char scrambled[16];
    char guess[16];
    int incorrect_guess_cnt = 0, correct_guess_cnt = 0;
    char q_opt;
    int curr_num_letters = 6;
    int num_words;

    char incorrect_guess_list[MAX_GUESSES][16];
    char correct_guess_list[NAX_WORDS][16];


    inp = pick_text_file();
    if (inp == NULL)
    {
        return 0;
    }

    num_words = load_words(inp, word_list, NAX_WORDS);

    scramble_word(scrambled, word_list[0].word);

    print_title();

    while (1)
    {
        int scramble_word_idx = 0; // find index of first word with strlen == curr_num_letters.
        scramble_word(scrambled, word_list[scramble_word_idx].word, curr_num_letters);

        print_scramble_header(scrambled);

        printf("\nYou have %d guesses left\n", MAX_GUESSES - incorrect_guess_cnt, incorrect_guess_cnt, );
        print_hints();


        // while (1) {
        printf("Enter your guess (4-6 characters): ");
        scanf(" %s", guess);
        // if guess is not between 4-6 characters, ask again, otherwise break.
        //}


        bool correct = check_guess(guess, word_list, num_words);


        if (correct) {
            //copy guess to correct_guess_list[correct_guess_cnt]
            correct_guess_cnt++;
            print_success();
            curr_num_letters--;
        }
        else {
            //copy guess to incorrect_guess_list[correct_guess_cnt]
            incorrect_guess_cnt++;
            print_failure(MAX_GUESSES - incorrect_guess_cnt);
        }


        print_guesses(incorrect_guess_list, correct_guess_list, incorrect_guess_cnt, correct_guess_cnt);

        // break if incorrect_guess_cnt >= MAX_GUESSES

        // break if correct_guess_cnt >= 

        printf("continue (q to quit): ");
        scanf(" %c", &q_opt);
        if (q_opt == 'q') {
            break;
        }

    }

    

    return 0;
}