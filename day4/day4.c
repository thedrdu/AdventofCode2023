#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include "hashtable.h"

char** generateGrid(int* lines, int* line_length){
    FILE *file;
    (*lines) = 0;
    (*line_length) = 0;
    char ch;
    
    file = fopen("input.txt", "r");
    if(!file){
        fprintf(stderr, "Error opening file.");
        exit(EXIT_FAILURE);
    }

    while((ch = fgetc(file)) != EOF){
        if(ch == '\n'){
            (*lines)++;
            (*line_length) = 0;
        }
        else{
            (*line_length)++;
        }
    }

    (*lines)++; //last line doesn't have a newline character

    char **array = malloc((*lines) * sizeof(char *));
    for(int i = 0; i < (*lines); i++){
        array[i] = malloc(((*line_length) + 1) * sizeof(char)); // +1 for the null terminator
    }

    fseek(file, 0, SEEK_SET); // reset pointer and read into our grid
    for(int i = 0; i < (*lines); i++){
        fgets(array[i], (*line_length) + 1, file);
        fgetc(file); // skip the newline character
    }
    fclose(file);
    
    return array;
}

int extractNumberFromString(const char *str, int start, int end){
    char numStr[end - start + 1];
    strncpy(numStr, str + start, end - start + 1);
    numStr[end - start + 1] = '\0'; // null-terminate the string
    return atoi(numStr);
}

void generateScratchcardHashTable(HashTable* table, char** array, int lines, int line_length){
    for(int i = 0; i < lines; i++){
        bool key = true; // at the start of each new line we are guaranteed to be before the pipe
        int winning_number_count = 0;
        int our_number_count = 0;
        int keyIndex = 0, valueIndex = 0;
        int* keyArray = malloc(line_length * sizeof(int));
        int* valueArray = malloc(line_length * sizeof(int));

        IntArray keyIntArray, valueIntArray;
        for(int j = 9; j < line_length; j++){ // ignore card number
            if(array[i][j] == 124){ // we reach the pipe, all numbers after are what we have
                key = false;
            }
            if(isdigit(array[i][j])){
                if(key){ // we are before the pipe
                    int start_index = j;
                    while(j < line_length && isdigit(array[i][j])){
                        j++;
                    }
                    int end_index = j - 1;

                    int number = extractNumberFromString(array[i], start_index, end_index);
                    winning_number_count++;

                    keyArray[keyIndex++] = number;

                    j = end_index;
                }
                else{ //we are after the pipe
                    int start_index = j;
                    while(j < line_length && isdigit(array[i][j])){
                        j++;
                    }
                    int end_index = j - 1;

                    int number = extractNumberFromString(array[i], start_index, end_index);
                    our_number_count++;

                    valueArray[valueIndex++] = number;

                    j = end_index;
                }
            }
        }

        keyIntArray.array = keyArray;
        keyIntArray.size = winning_number_count;
        valueIntArray.array = valueArray;
        valueIntArray.size = our_number_count;
        insert(table, keyIntArray, valueIntArray);
    }
}

long calculateWinnings(HashTable* table){
    long sum = 0;
    long cardWinnings;
    for(int i = 0; i < HASH_TABLE_SIZE; ++i){
        HashEntry* entry = table->entries[i];
        while(entry != NULL){
            cardWinnings = 0;
            // too lazy to implement a better search algo on values or checking if key > value
            for(int j = 0; j < entry->key.size; j++){
                for(int k = 0; k < entry->value.size; k++){
                    if(entry->key.array[j] == entry->value.array[k]){
                        if(cardWinnings == 0){
                            cardWinnings = 1;
                        }
                        else{
                            cardWinnings *= 2;
                        }
                    }
                }
            }
            sum += cardWinnings;
            entry = entry->next;
        }
    }
    return sum;
}

long long calculateScratchcardCount(HashTable* table, int lines){
    long long* cardCounts = malloc(lines * sizeof(long long));
    long long* matchCounts = malloc(lines * sizeof(long long));
    int currentCardIndex = 0;
    int matchCount;

    HashEntry* entry = table->firstInsert;
    while(entry != NULL){
        matchCount = 0;

        for(int j = 0; j < entry->key.size; j++){
            for(int k = 0; k < entry->value.size; k++){
                if(entry->key.array[j] == entry->value.array[k]){
                    matchCount++;
                    break;
                }
            }
        }
        matchCounts[currentCardIndex] = matchCount;
        cardCounts[currentCardIndex] = 1;
        currentCardIndex++;

        entry = entry->nextInsertOrder;
    }

    for(int i = 0; i < lines; i++){
        int numMatches = matchCounts[i];
        for(int j = 1; j <= numMatches; j++){
            cardCounts[i+j] += cardCounts[i];
        }
    }
    long long sum = 0;
    for(int i = 0; i < lines; i++){
        sum += cardCounts[i];
    }
    return sum;
}


void part1(){
    int lines, line_length;
    char **array = generateGrid(&lines, &line_length);

    HashTable table;
    initHashTable(&table);

    generateScratchcardHashTable(&table, array, lines, line_length);

    // printHashTable(&table);

    long winnings = calculateWinnings(&table);
    printf("Winnings: %ld\n", winnings);
}

void part2(){
    int lines, line_length;
    char **array = generateGrid(&lines, &line_length);

    HashTable table;
    initHashTable(&table);

    generateScratchcardHashTable(&table, array, lines, line_length);

    // printHashTable(&table);

    long long scratchcards = calculateScratchcardCount(&table, lines);
    printf("Number of scratchcards: %lld\n", scratchcards);
}

int main(int argc, char *argv[]){
    if(argc != 2){
        fprintf(stderr, "Usage: %s <argument>\n", argv[0]);
        return EXIT_FAILURE;
    }
    if(strcmp(argv[1], "1") == 0){ //part 1
        part1();
    }
    else if(strcmp(argv[1], "2") == 0){ //part 2
        part2();
    }
    printf("Done.\n");
}