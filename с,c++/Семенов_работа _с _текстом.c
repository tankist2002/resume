#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <limits.h>

#define MAXLEN 1024

/*#define OS_TYPE linux*/
#ifdef OS_TYPE
#define CLS system("clear")
#else
#define CLS system("cls")
#endif

void print_text(char** text, int text_size) {
    int i;

    for(i=0; i<text_size; printf("%s\n",text[i++]));
    puts("");

    return;
}

int get_text_file(char** text, int text_size) {
    FILE *df;
    char file_name[MAXLEN], current_string[MAXLEN];
    int i;
    int string_count, current_string_len;

    /*INITIALIZATION*/
    string_count = 0;

            /*SCAN NAME OF FILE*/
    printf("Put on name of file: ");
    fgets(file_name,MAXLEN,stdin);
    file_name[strlen(file_name) - 1] = '\0';
    puts("");


            /*TRYING TO OPEN THE FILE*/
    df = fopen(file_name, "r");

    if (df != NULL) {
        fseek(df,0,SEEK_SET);

        for (i=0;i<text_size && !feof(df);i++) {
                /*READING A NEW STRING*/
            fgets(current_string, MAXLEN, df);
            current_string_len = strlen(current_string);
            current_string[current_string_len-1] = '\0';

                /*MALLOC FOR STRING OF TEXT*/
            text[i] = (char*)malloc(current_string_len*sizeof(char));

            if (text[i] != NULL) {
                string_count++;
                strcpy(text[i], current_string);
            } else {
                i = text_size;
                puts("Error at string allocation! Array is incomplete.");
            }


        }

        if(fclose(df)==EOF) printf("Closing error code: %d\n", errno);

    } else puts("Error at opening file!");

    return string_count;
}

int get_text_console(char** text, int text_size) {
    char current_string[MAXLEN];
    int i;
    int string_count, current_string_len;

    /*INITIALIZATION*/
    string_count = 0;

    printf("Put down your text with %d strings:\n",text_size);

    for(i=0;i<text_size;i++) {
        /*READING A NEW STRING*/
        fgets(current_string, MAXLEN, stdin);
        current_string_len = strlen(current_string);
        current_string[current_string_len-1] = '\0';

            /*MALLOC FOR STRING OF TEXT*/
        text[i] = (char*)malloc(current_string_len*sizeof(char));

        if (text[i] != NULL) {
            string_count++;
            strcpy(text[i], current_string);
        } else {
            i = text_size;
            puts("Error at string allocation! Array is incomplete.");
        }
    }


    return string_count;
}

int menu() {
    int option;
    do {
        CLS;
        printf("Functions to start:\n");
        printf("1 - File start\n");
        printf("2 - Console start\n");
        printf("Enter your choice: ");

        scanf("%d",&option);
    } while((option<1)||(option>2));
    puts("");

    return option;
}

int count_num_of_words(char* string, char* seps) {
    int i, j, flag;
    int word_count, current_word_len;
    int string_len, seps_len;


    word_count = 0; current_word_len = 0;
    string_len = strlen(string);
    seps_len = strlen(seps);

    for(i=0;i<=string_len;i++) {
        flag = 0;
        for(j=0;j<=seps_len;j++) {
            /*CHECKING IN SEP SYMBOL*/
            if (string[i] == seps[j]) flag = 1;
        }
        if (flag) {
            /*CHECKING WORD EXISTS*/
            if (current_word_len != 0) word_count++;

            /*RESET OF CURRENT WORD*/
            current_word_len = 0;

        } else current_word_len++; /*WORD BECOMES BIGGER*/
    }
    return word_count;
}

void delete_the_smallest_words(char* string, char* separators) {
    int i, j, k, flag;
    int string_len, seps_len;
    int min_word_len, current_word_len;
    int current_word_start, current_word_end;


    string_len = strlen(string);
    seps_len = strlen(separators);

    min_word_len = INT_MAX;
    current_word_len = 0;

    /*SEARCHING MINIMAL LEN*/
    for(i=0;i<=string_len;i++) {
        flag = 0;
        for (j=0;j<=seps_len;j++) if (string[i] == separators[j]) flag = 1;

        if (flag) {
            if (current_word_len <= min_word_len && current_word_len != 0) min_word_len = current_word_len;/*MIN REASSIGNMENT*/
            /*RESET OF CURRENT WORD*/
            current_word_len = 0;
        } else current_word_len++;
    }

    /*DELETING MINIMAL WORDS*/
    current_word_start = -1; current_word_end = 0;
    i=0;
    do {
        /*CHECK ON SEPARATOR*/
        flag = 0;
        for(j=0;j<=seps_len;j++) if (string[i] == separators[j]) flag = 1;

        /*IF SEPARATOR*/
        if (flag) {
            current_word_end = i;
            /*CHECK ON WORD LEN*/
            if ((current_word_end - current_word_start) - 1 == min_word_len) {
                /*DELETING WORD*/
                for(j=0;j<min_word_len;j++){
                        for(k=current_word_start+1; string[k-1] != '\0'; k++) string[k] = string[k+1]; /*DELETING SYMBOL*/
                   }
                i = -1;/*RESET OF CYCLE*/
            }
            current_word_start = i; /*NEW START*/
        }


        i++; string_len = strlen(string);
    } while (i<=string_len);


    return;
}

int main(){
    int option;
    int i, c;
    char **text, **restext;
    char separators[MAXLEN];
    int num_of_strings, num_of_ready_strings;
    int num_of_res_strings, num_of_ready_res_strings;
    int current_num_of_words;

    /*MENU*/
    option = menu();
            /*SCAN NUM OF STRINGS*/
    printf("Enter num of strings in the text: ");
    scanf("%d", &num_of_strings);
    puts("");

    /*BUFFER CLEAN*/
    do {c = getchar();} while (c != EOF && c != '\n');
    fflush(stdin);

            /*MEMORY ALLOCATION FOR THE TEXT*/
    text = (char**)malloc(num_of_strings * sizeof(char*));

    if (text!=NULL) {

        if (option == 1) {
            num_of_ready_strings = get_text_file(text, num_of_strings); /*WORKING WITH FILE*/
        } else num_of_ready_strings = get_text_console(text, num_of_strings); /*WORKING WITH CONSOLE*/


        /*CHECKING ON READY STRINGS IN TEXT*/
        if (num_of_ready_strings == num_of_strings) {
                /*WORK WITH TEXT*/

            /*GETTING SEPARATORS*/
            puts("Put down string of separators");
            fgets(separators, MAXLEN, stdin);
            separators[strlen(separators)-1] = '\0';

            /*COUNT NUM OF STRINGS IN RESULT TEXT*/
            num_of_res_strings = 0;
            for(i=0;i<num_of_strings;i++) {
                    current_num_of_words = count_num_of_words(text[i], separators);
                    if (current_num_of_words % 2 == 1) num_of_res_strings++;
            }

            /*MALLOC FOR RESTEXT*/
            restext = (char**)malloc(num_of_res_strings*sizeof(char*));

            if (restext != NULL) {
                num_of_ready_res_strings = 0;
                /*COPYING STRINGS WITH ODD AMOUNT OF WORDS*/
                for(i=0;i<num_of_strings;i++) {
                    current_num_of_words = count_num_of_words(text[i], separators);
                    /*FINDING ODD AMOUNT OF WORDS*/
                    if (current_num_of_words % 2 == 1) {
                        /*MALLOC WITH SIZE OF CURRENT TEXT STRING*/
                        restext[num_of_ready_res_strings] = (char*)malloc((strlen(text[i])+1)*sizeof(char));
                        /*CHECKING MALLOC*/
                        if (restext[num_of_ready_res_strings] != NULL) {
                            /*COPYING STRING TO RESTEXT*/
                            strcpy(restext[num_of_ready_res_strings++], text[i]);
                        } else {
                            puts("Error at restext string memory allocation! Restext incompleted!");
                            i = num_of_strings;
                        }
                    }
                }

                /*CHECK OF GOOD MALLOC*/
                if (num_of_ready_res_strings == num_of_res_strings) {
                    for(i=0;i<num_of_ready_res_strings;i++) delete_the_smallest_words(restext[i], separators);
                    /*INITIAL TEXT*/
                    puts("\nInitial text:\n");
                    print_text(text, num_of_strings);
                    puts("--------------");
                    /*RESULT TEXT*/
                    printf("\nResult text:\n");
                    print_text(restext, num_of_res_strings);
                }
                /*FREE RESTEXT MEMORY*/
                for(i=0;i<num_of_ready_res_strings;i++) {
                    free(restext[i]);
                    restext[i] = NULL;
                }
                free(restext);
                restext = NULL;
            } else puts("Error at restext malloc!");


        } else puts("Error at string alloc!");


        /*FREE MEMORY*/
        for (i=0;i<num_of_ready_strings;i++) {
            free(text[i]);
            text[i] = NULL;
        }
        free(text);
        text = NULL;

    } else puts("Error at text malloc!");


    return 0;
}
