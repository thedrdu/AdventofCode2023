#ifndef HASHTABLE_H
#define HASHTABLE_H

#define HASH_TABLE_SIZE 100

typedef struct {
    int* array;
    int size;
} IntArray;

typedef struct HashEntry {
    IntArray key;
    IntArray value;
    struct HashEntry* next;
    struct HashEntry* prevInsertOrder;
    struct HashEntry* nextInsertOrder;
} HashEntry;

typedef struct {
    HashEntry* entries[HASH_TABLE_SIZE];
    HashEntry* firstInsert;
    HashEntry* lastInsert;
} HashTable;


void initHashTable(HashTable* table);
void insert(HashTable* table, IntArray key, IntArray value);
void freeHashTable(HashTable* table);
void printHashTable(const HashTable* table);

#endif