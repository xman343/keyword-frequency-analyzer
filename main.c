// Author: Xander Leatherwood

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define KEYS 21
#define KEYLEN 10

char* read_f(char* fname);
int word_count(char* fstr);
int key_count(char* fstr, char* keystr);
float avg_len(char* fstr, int wcount);
int sent_count(char* fstr);
float avg_sent_len(char* fstr, int scount);

int main(){
    // User inputs file name
    char fname[1024];
    printf("Enter filename:\n");
    scanf("%s", fname);

    // READ FILE
    // File management
    char* fstr = read_f(fname);

    if (fstr == NULL){
        printf("Error: File could not be read. Make sure it is in the proper directory or check for corruption.\n");
        return 1;
    }

    // Print contents

    printf("\nFile contents:\n");
    // printf("\n%s\n", fstr);

    // WORD COUNT
    // Counts number of words in text document

    int wcount = word_count(fstr);

    printf("\nWord count: %d\n", wcount);

    // KEY COUNT
    // Computes number of occurrences and frequency per 1000 words of each keyword in the text document

    // Initialize keywords
    char keys[KEYS][KEYLEN] = {"by", "from", "to", "a", "the", "an", "and", "I", "one", "for", "in", "with", "which", "that", "it", "but", "not", "on", "as", "at", "this"};

    // Loop thru each keyword
    char* keystr; 
    int i = 0;
    int kcount = 0;
    float kfreq = 0.0;

    printf("|Keyword|  Frequency  |# of Occurrences|\n----------------------------------------\n");

    for (i = 0; i < KEYS; i++){
        keystr = keys[i];
        kcount = key_count(fstr, keystr);
        kfreq = ((kcount * 1.0) / wcount) * 1000;
        printf("|%7s|%8.3f/1000|%16d|\n----------------------------------------\n", keystr, kfreq, kcount);
    }

    // AVERAGE WORD LENGTH
    // Computes average number of characters in each word

    float avglen = avg_len(fstr, wcount);

    printf("Avg word length: %f\n", avglen);

    // SENTENCE COUNT
    // Counts number of sentences in file

    int scount = sent_count(fstr);

    printf("Sentence count: %d\n", scount);

    // AVERAGE SENTENCE LENGTH
    // Counts avg number of words per sentence

    float avgsentlen = avg_sent_len(fstr, scount);

    printf("Avg sentence length: %f\n", avgsentlen);

    free(fstr);

    return 0;
}

char* read_f(char* fname){
    FILE *f;

    // Open and read file
    f = fopen(fname, "r");

    // Error handler
    if (f == NULL) return NULL;

    // Find number of characters in file
    fseek(f, 0, SEEK_END);
    int flen = ftell(f);

    // Reset pointer to start of file
    fseek(f, 0, SEEK_SET);

    // Set up char array for file storage in program
    char *fstr = malloc(sizeof(char) * (flen + 1));
    
    char c;
    int i = 0;

    while ( (c = fgetc(f)) != EOF){
        fstr[i] = c;

        i++;
    }

    fstr[i] = '\0';

    fclose(f);

    return fstr;

}

int word_count(char* fstr){
    // Initialize len and count
    int len = strlen(fstr);
    int wcount = 0;

    // Delimiters
    char delims[] = " .,;\n\t-!?`";

    int i = 0;

    // Loops thru each char in string
    for(i = 0; i < len; i++){
    
    // Steps to next word 
        while(i < len){
    // Loops thru word until delim found
            if (strchr(delims, fstr[i]) != NULL){
                break;
            }
            i++;
        }
        wcount++;
    // Loops thru multiple delim characters until word found
        while(i < len){
    // Detects when word is found
            if (strchr(delims, fstr[i]) == NULL){
                break;
            }
            i++;
        }
    }

    return wcount;
}

int key_count(char* fstr, char* keystr){
    // Initialize variables
    int slen = strlen(fstr);
    int klen = strlen(keystr);
    int strend = (slen - klen) + 1;
    int kcount = 0;
    int i = 0;
    int j = 0;

    // Checks each position
    for (i = 0; i < strend; i++){
        // Start by assuming that keyword is found
        bool key_found = true;

        for (j = 0; j < klen; j++){
            // If assumption false: 
            if (keystr[j] != fstr[i + j]){
                key_found = false;
                break;
            }
        }

        // If assumption true
        if (key_found) kcount++;
    }

    return kcount;
}

float avg_len(char* fstr, int wcount){
    char delims[] = " .',;\n\t-!?`\0";

    int i = 0;
    int j = 0;
    float n = 0.0;

    int len = strlen(fstr);

    // Checks for end of word
    for (i = 0; i < len; i++){
        while (i < len){
            if (strchr(delims, fstr[i]) !=NULL){
                break;
            } else {
                n++;
            }
            i++;
        }
    }

    // Calculates avg word legth
    float avglen = n / (wcount * 1.0);

    return avglen;
}

int sent_count(char* fstr){
    int len = strlen(fstr);
    int scount = 0;

    // Sentence end delim 
    char sdelims[] = ".?!";

    int i = 0;

    // Checks for end of sentence; if end, sentence count increases
    for (i = 0; i < len; i++){
        while (i < len){
            if (strchr(sdelims, fstr[i]) != NULL){
                break;
            }
            i++;
        }
        scount++;

        // Checks for start of new sentence
        while (i < len){
            if (strchr(sdelims, fstr[i]) == NULL){
                break;
            }
            i++;
        }
    }
    return scount;
}

float avg_sent_len(char* fstr, int scount){
    // Sets '.' as delim to count only sentences
    char sdelims[] = ".?!";

    int i = 0;
    int j = 0;
    float n = 0.0;

    int len = strlen(fstr);

    // Checks for end of sentence
    for (i = 0; i < len; i++){
        while (i < len){
            if (strchr(sdelims, fstr[i]) != NULL){
                break;
            } else {
                n++;
            }
            i++;
        }
    }

    float avgsentlen = n / (scount * 1.0);

    return avgsentlen;
}