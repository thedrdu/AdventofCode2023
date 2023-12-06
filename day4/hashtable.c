#include "hashtable.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

unsigned int hash(IntArray key){
    unsigned int sum = 0;
    for (int i = 0; i < key.size; ++i) {
        sum += key.array[i];
    }
    return sum % HASH_TABLE_SIZE;
}

void initHashTable(HashTable* table){
    for(int i = 0; i < HASH_TABLE_SIZE; ++i){
        table->entries[i] = NULL;
    }
    table->firstInsert = NULL; // Initialize the firstInsert pointer
    table->lastInsert = NULL;  // Initialize the lastInsert pointer
}

void insert(HashTable* table, IntArray key, IntArray value){
    unsigned int hashValue = hash(key);
    HashEntry* entry = table->entries[hashValue];

    while(entry != NULL){
        if(entry->key.size == key.size && memcmp(entry->key.array, key.array, key.size * sizeof(int)) == 0){
            entry->value = value;
            entry->value.size++;
            return;
        }
        entry = entry->next;
    }

    // Code to create new entry and add it to the table
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
    entry->value = value;
    entry->value.size = value.size;
    entry->next = table->entries[hashValue];
    table->entries[hashValue] = entry;

    entry->prevInsertOrder = table->lastInsert;
    entry->nextInsertOrder = NULL;
    if (table->lastInsert != NULL) {
        table->lastInsert->nextInsertOrder = entry;
    }
    table->lastInsert = entry;
    if (table->firstInsert == NULL) {
        table->firstInsert = entry;
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

void printHashTable(const HashTable* table){
    printf("Hash Table:\n");
    for(int i = 0; i < HASH_TABLE_SIZE; ++i){
        HashEntry* entry = table->entries[i];
        if(entry != NULL){
            printf("Bucket %d:\n", i);
        }
        while(entry != NULL){
            printf("  Key: [");
            for(int j = 0; j < entry->key.size; ++j){
                printf("%d", entry->key.array[j]);
                if(j < entry->key.size - 1){
                    printf(", ");
                }
            }
            printf("] Values: [");
            for(int j = 0; j < entry->value.size; ++j){
                printf("%d", entry->value.array[j]);
                if(j < entry->value.size - 1){
                    printf(", ");
                }
            }
            printf("]\n");
            entry = entry->next;
        }
    }
}
