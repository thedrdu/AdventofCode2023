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

bool isSpecialCharacter(char c){
    if(c != 46 && (c < 48 || c > 57)){
        return true;
    }
    return false;
}

bool isGear(char c){
    return c == 42;
}

int extractNumberFromString(const char *str, int start, int end){
    char numStr[end - start + 1];
    strncpy(numStr, str + start, end - start + 1);
    numStr[end - start + 1] = '\0'; // null-terminate the string
    return atoi(numStr);
}

bool checkSurroundingsForSpecialCharacter(char **array, int lines, int line_length, int i, int start_index, int end_index){
    //let's start by checking the 4 corners if they exist
    if(i > 0 && start_index > 0){
        if(isSpecialCharacter(array[i-1][start_index-1])){
            return true;
        }
    }
    if(i > 0 && end_index < line_length - 1){
        if(isSpecialCharacter(array[i-1][end_index+1])){
            return true;
        }
    }
    if(i < lines - 1 && start_index > 0){
        if(isSpecialCharacter(array[i+1][start_index-1])){
            return true;
        }
    }
    if(i < lines - 1 && end_index < line_length - 1){
        if(isSpecialCharacter(array[i+1][end_index+1])){
            return true;
        }
    }

    //top
    if(i > 0){
        for(int k = start_index; k <= end_index; k++){
            if(isSpecialCharacter(array[i-1][k])){
                return true;
            }
        }
    }
    //bottom
    if(i < lines - 1){
        for(int k = start_index; k <= end_index; k++){
            if(isSpecialCharacter(array[i+1][k])){
                return true;
            }
        }
    }
    //left
    if(start_index > 0){
        if(isSpecialCharacter(array[i][start_index-1])){
            return true;
        }
    }
    //right
    if(end_index < line_length - 1){
        if(isSpecialCharacter(array[i][end_index+1])){
            return true;
        }
    }
    return false;
}

Tuple* checkSurroundingsForGear(char **array, int lines, int line_length, int i, int start_index, int end_index, int *gear_count){
    int max_bordering_tiles = ((end_index - start_index + 1) * 2) + 6;
    Tuple *gear_list = malloc(max_bordering_tiles * sizeof(Tuple));
    int index = 0;
    (*gear_count) = 0;
    
    //let's start by checking the 4 corners if they exist
    if(i > 0 && start_index > 0){
        if(isGear(array[i-1][start_index-1])){
            gear_list[index].x = i-1;
            gear_list[index].y = start_index-1;
            (*gear_count)++;
        }
    }
    if(i > 0 && end_index < line_length - 1){
        if(isGear(array[i-1][end_index+1])){
            gear_list[index].x = i-1;
            gear_list[index].y = end_index+1;
            (*gear_count)++;
        }
    }
    if(i < lines - 1 && start_index > 0){
        if(isGear(array[i+1][start_index-1])){
            gear_list[index].x = i+1;
            gear_list[index].y = start_index-1;
            (*gear_count)++;
        }
    }
    if(i < lines - 1 && end_index < line_length - 1){
        if(isGear(array[i+1][end_index+1])){
            gear_list[index].x = i+1;
            gear_list[index].y = end_index+1;
            (*gear_count)++;
        }
    }

    //top
    if(i > 0){
        for(int k = start_index; k <= end_index; k++){
            if(isGear(array[i-1][k])){
                gear_list[index].x = i-1;
                gear_list[index].y = k;
                (*gear_count)++;
            }
        }
    }
    //bottom
    if(i < lines - 1){
        for(int k = start_index; k <= end_index; k++){
            if(isGear(array[i+1][k])){
                gear_list[index].x = i+1;
                gear_list[index].y = k;
                (*gear_count)++;
            }
        }
    }
    //left
    if(start_index > 0){
        if(isGear(array[i][start_index-1])){
            gear_list[index].x = i;
            gear_list[index].y = start_index-1;
            (*gear_count)++;
        }
    }
    //right
    if(end_index < line_length - 1){
        if(isGear(array[i][end_index+1])){
            gear_list[index].x = i;
            gear_list[index].y = end_index+1;
            (*gear_count)++;
        }
    }
    return gear_list;
}

int sumSpecialNumbers(char **array, int lines, int line_length){
    int sum = 0;
    bool is_part_number;
    for(int i = 0; i < lines; i++){
        for(int j = 0; j < line_length; j++){
            if(isdigit(array[i][j])){
                int start_index = j;
                while(j < line_length && isdigit(array[i][j])){
                    j++;
                }
                int end_index = j - 1;

                int number = extractNumberFromString(array[i], start_index, end_index);
                is_part_number = checkSurroundingsForSpecialCharacter(array, lines, line_length, i, start_index, end_index);

                if(is_part_number){
                    sum += number;
                }
                j = end_index; // move past the number
            }
        }
    }
    return sum;
}

void generateGearHashmap(HashTable* table, char** array, int lines, int line_length){
    int sum = 0;
    for(int i = 0; i < lines; i++){
        for(int j = 0; j < line_length; j++){
            if(isdigit(array[i][j])){
                int start_index = j;
                while(j < line_length && isdigit(array[i][j])){
                    j++;
                }
                int end_index = j - 1;

                int number = extractNumberFromString(array[i], start_index, end_index);
                int gear_count;
                Tuple* gear_list = checkSurroundingsForGear(array, lines, line_length, i, start_index, end_index, &gear_count);
                if(gear_list == NULL){
                    printf("GEARLIST NULL\n");
                    exit(EXIT_FAILURE);
                }
                for(int i = 0; i < gear_count; i++){
                    insert(table, gear_list[i], number);
                }

                j = end_index;
            }
        }
    }
}

long long sumGearRatios(const HashTable* table){
    long sum = 0;
    for(int i = 0; i < HASH_TABLE_SIZE; ++i){
        HashEntry* entry = table->entries[i];
        while(entry != NULL){
            if(entry->value.size == 2) {
                sum += entry->value.array[0] * entry->value.array[1];
            }
            entry = entry->next;
        }
    }
    return sum;
}


void part1() {
    int lines, line_length;
    char **array = generateGrid(&lines, &line_length);
    int sum = sumSpecialNumbers(array, lines, line_length);
    printf("Sum of part numbers: %d\n", sum);
    for(int i = 0; i < lines; i++){
        free(array[i]);
    }
    free(array);
}

void part2() {
    int lines, line_length;
    char **array = generateGrid(&lines, &line_length);
    
    HashTable table;
    initHashTable(&table);

    generateGearHashmap(&table, array, lines, line_length);
    // printHashTable(&table);

    long long sum_of_gear_ratios = sumGearRatios(&table);
    printf("%lld\n", sum_of_gear_ratios);

    freeHashTable(&table);

    for(int i = 0; i < lines; i++){
        free(array[i]);
    }
    free(array);
}

int main(int argc, char *argv[]) {
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