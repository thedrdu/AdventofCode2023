#include "hashtable.h"
#include <stdio.h>
#include <stdlib.h>

unsigned int hash(Tuple key){
    return (key.x * 31 + key.y) % HASH_TABLE_SIZE;
}

void initHashTable(HashTable* table){
    for(int i = 0; i < HASH_TABLE_SIZE; ++i){
        table->entries[i] = NULL;
    }
}

void resizeArray(IntArray* arr){
    if(arr->size >= arr->capacity){
        arr->capacity *= 2;
        int* newArray = realloc(arr->array, arr->capacity * sizeof(int));
        if(newArray){
            arr->array = newArray;
        }
        else{
            fprintf(stderr, "Failed to allocate memory\n");
            exit(EXIT_FAILURE);
        }
    }
}

void insert(HashTable* table, Tuple key, int value){
    unsigned int hashValue = hash(key);
    HashEntry* entry = table->entries[hashValue];
    while(entry != NULL){
        if(entry->key.x == key.x && entry->key.y == key.y){
            resizeArray(&entry->value);
            entry->value.array[entry->value.size++] = value;
            return;
        }
        entry = entry->next;
    }

    entry = malloc(sizeof(HashEntry));
    if(entry == NULL){
        fprintf(stderr, "Failed to allocate memory\n");
        exit(EXIT_FAILURE);
    }
    entry->key = key;
    entry->value.array = malloc(2 * sizeof(int)); // arbitrary starting capacity of 2
    if(entry->value.array == NULL){
        fprintf(stderr, "Failed to allocate memory\n");
        exit(EXIT_FAILURE);
    }
    entry->value.array[0] = value;
    entry->value.size = 1;
    entry->value.capacity = 2;
    entry->next = table->entries[hashValue];
    table->entries[hashValue] = entry;
}

IntArray* search(HashTable* table, Tuple key){
    unsigned int hashValue = hash(key);
    HashEntry* entry = table->entries[hashValue];
    while(entry != NULL){
        if(entry->key.x == key.x && entry->key.y == key.y){
            return &entry->value;
        }
        entry = entry->next;
    }
    return NULL;
}

void deleteEntry(HashTable* table, Tuple key){
    unsigned int hashValue = hash(key);
    HashEntry* entry = table->entries[hashValue];
    HashEntry* prev = NULL;
    while(entry != NULL){
        if(entry->key.x == key.x && entry->key.y == key.y){
            if(prev == NULL){
                table->entries[hashValue] = entry->next;
            }
            else{
                prev->next = entry->next;
            }
            free(entry->value.array);
            free(entry);
            return;
        }
        prev = entry;
        entry = entry->next;
    }
}

void freeHashTable(HashTable* table){
    for(int i = 0; i < HASH_TABLE_SIZE; ++i){
        HashEntry* entry = table->entries[i];
        while(entry != NULL){
            HashEntry* temp = entry;
            entry = entry->next;
            free(temp->value.array);
            free(temp);
        }
    }
}

void printHashTable(const HashTable* table) {
    for(int i = 0; i < HASH_TABLE_SIZE; ++i){
        HashEntry* entry = table->entries[i];
        while(entry != NULL){
            printf("Key: (%d, %d) Values:", entry->key.x, entry->key.y);
            for(int j = 0; j < entry->value.size; ++j){
                printf(" %d", entry->value.array[j]);
            }
            printf("\n");
            entry = entry->next;
        }
    }
}