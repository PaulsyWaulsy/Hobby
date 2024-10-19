#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>
#include <ctype.h>

/*-------------------------------------------CONSTANTS-------------------------------------------*/
const uint32_t PRIME_MULTIPLIER = 16777619;
const uint8_t INITIAL_CAPACITY = 32;
const uint8_t WORD_BUFFER = 128;
const float LOAD_FACTOR = 7.5f;
const char* DELIMITERS = " ,.'!?-_#:;(){}[]<>";

/*-------------------------------------------STUCTURES-------------------------------------------*/
typedef struct {
    char* key; // String
    int value; // Could be the frequency of a word occurance in some text
} Entry;

typedef struct Node {
    struct Node* next;
    struct Node* previous;
    Entry* entry;
} Node;

typedef struct {
    uint size;
    Node* head;
    Node* tail;
} DoublyLinkedList;

typedef struct {
    uint32_t size;                            // Number of elements int the hash table.
    uint32_t capacity;                        // Total number of elements the hash table can support.
    uint32_t load;
    DoublyLinkedList* arrayEntries;           // Allows for and unknown size of inputs
    DoublyLinkedList** arrayDoublyLinkedList; // The Entries
} HashTable;

/*------------------------------------------PROTOTYPES-------------------------------------------*/
Node* node_create(Entry* entry);
void node_free(Node* node);
DoublyLinkedList* doubly_linked_list_create(void);
void doubly_linked_list_free(DoublyLinkedList* doublyLinkedList);
void doubly_linked_list_append(DoublyLinkedList* doublyLinkedList, Entry* entry);
Node* doubly_linked_list_pop(DoublyLinkedList* doublyLinkedList);
Entry* entry_create(char* key, uint32_t value);
void entry_free(Entry* entry);
HashTable* hash_create(uint64_t capacity);
uint32_t hash_generate(char* data, size_t lengthBytes);
int hash_is_occupied(HashTable* hashTable, uint32_t index);
void hash_insert(HashTable* hashTable, char* data);
void hash_sort(HashTable* hashTable);
uint32_t hash_get(HashTable* hashTable, char* data);
void hash_resize(HashTable* hashTable); 
void hash_sort(HashTable* hashTable);
int file_is_delimiter(char ch);
char* file_read_word(FILE* file);

/*------------------------------------------FUNCTIONS--------------------------------------------*/
Node* node_create(Entry* entry) {
    Node* node = malloc(sizeof(Node));
    if (node == NULL) {
        fprintf(stderr, "ERROR: Not able to initialize node\n");
        exit(1);
    }
    node->entry = entry;
    node->next = NULL;
    node->previous = NULL;
    return node;
}

void node_free(Node* node) {
    entry_free(node->entry);
    free(node);
}

DoublyLinkedList* doubly_linked_list_create() {
    DoublyLinkedList* doublyLinkedList = malloc(sizeof(DoublyLinkedList));
    if (doublyLinkedList == NULL) {
        fprintf(stderr, "ERROR: Not able to initialize node\n");
        exit(1);
    }
    doublyLinkedList->head = NULL;
    doublyLinkedList->tail = NULL;
    doublyLinkedList->size = 0;
    return doublyLinkedList;
}

void doubly_linked_list_free(DoublyLinkedList* doublyLinkedList) {
    if (doublyLinkedList == NULL) {
        return;
    }

    Node* current = doublyLinkedList->head; 
    Node* next;

    while (current != NULL) {
        next = current->next;
        node_free(current);
        current = next;
    }
    free(doublyLinkedList);
}

void doubly_linked_list_append(DoublyLinkedList* doublyLinkedList, Entry* entry) {
    Node* node = node_create(entry);
    if (doublyLinkedList->size < 1) {
        doublyLinkedList->head = node;
        doublyLinkedList->tail = node;
    } else {
        doublyLinkedList->tail->next = node;
        node->previous = doublyLinkedList->tail;
        doublyLinkedList->tail = node;
    }
    doublyLinkedList->size++;
}

Node* doubly_linked_list_pop(DoublyLinkedList* doublyLinkedList) {
    if (doublyLinkedList->size <= 0) {
        return NULL;
    }

    Node* node = doublyLinkedList->tail;
    doublyLinkedList->tail = node->previous;

    if (doublyLinkedList->tail != NULL) {
        doublyLinkedList->tail->next = NULL;
    } else {
        doublyLinkedList->head = NULL;
    }

    node->previous = NULL;
    doublyLinkedList->size--;
    return node;
}

void entry_free(Entry* entry) {
    free(entry->key);
    free(entry);
}

Entry* entry_create(char* key, uint32_t value) {
    Entry* entry = malloc(sizeof(Entry));
    if (entry == NULL) {
        fprintf(stderr, "ERROR: Not able to initialize entry\n");
        exit(1);
    }
    entry->key = strdup(key);
    entry->value = value;

    return entry;
}

void hash_resize(HashTable* hashTable) {
    uint32_t newCapacity = hashTable->capacity * 2 - 1; // Often results in odd numbers
    hashTable->load = newCapacity * LOAD_FACTOR / 10;
    DoublyLinkedList* newArrayEntries = doubly_linked_list_create();
    DoublyLinkedList** newArrayDoublyLinkedList = malloc(sizeof(DoublyLinkedList*) * newCapacity);
    if (newArrayDoublyLinkedList == NULL) {
        fprintf(stderr, "ERROR: Not able to initialize new array of DoublyLinkedList in resize\n");
        free(hashTable);
        exit(1);
    }

    for (uint32_t i = 0; i < newCapacity; i++) {
        newArrayDoublyLinkedList[i] = NULL;
    }

    char* word;
    uint32_t hash;
    uint32_t frequency;
    uint32_t newIndex;
    uint32_t size = hashTable->arrayEntries->size;
    for (uint32_t i = 0; i < size; i++) {
        Node* node = doubly_linked_list_pop(hashTable->arrayEntries);
        if (node == NULL) {
            continue; // this should not execute
        }
        word = node->entry->key; // will get the tail

        hash = hash_generate(word, strlen(word));
        frequency = hash_get(hashTable, word);
        newIndex = (hash + newCapacity) % newCapacity;
        doubly_linked_list_append(newArrayEntries, entry_create(word, newIndex));

        Entry* entry = entry_create(word, frequency);
        if (newArrayDoublyLinkedList[newIndex] == NULL) {
            DoublyLinkedList* doublyLinkedList = doubly_linked_list_create();
            doubly_linked_list_append(doublyLinkedList, entry);
            newArrayDoublyLinkedList[newIndex] = doublyLinkedList;
        } else {
            doubly_linked_list_append(newArrayDoublyLinkedList[newIndex], entry);
        }

        // free the old node
        node_free(node);
    }

    for (uint32_t i = 0; i < hashTable->capacity; i++) {
        if (hashTable->arrayDoublyLinkedList[i] != NULL) {
            doubly_linked_list_free(hashTable->arrayDoublyLinkedList[i]);
        }
    }
    doubly_linked_list_free(hashTable->arrayEntries);
    free(hashTable->arrayDoublyLinkedList);

    hashTable->arrayDoublyLinkedList = newArrayDoublyLinkedList;
    hashTable->arrayEntries = newArrayEntries;
    hashTable->capacity = newCapacity;
}


HashTable* hash_create(uint64_t capacity) {
    HashTable* hashTable = malloc(sizeof(HashTable));
    if (hashTable == NULL) {
        fprintf(stderr, "ERROR: Not able to initialize hash table\n");
        exit(1);
    }

    hashTable->arrayDoublyLinkedList = malloc(sizeof(DoublyLinkedList*) * capacity);
    if (hashTable->arrayDoublyLinkedList == NULL) {
        fprintf(stderr, "ERROR: Not able to initialize hash table\n");
        free(hashTable);
        exit(1);
    }

    for (uint32_t i = 0; i < capacity; i++) {
        hashTable->arrayDoublyLinkedList[i] = NULL;
    }


    hashTable->arrayEntries = doubly_linked_list_create();
    if (hashTable->arrayEntries == NULL) {
        fprintf(stderr, "ERROR: Not able to initialize hash table\n");
        free(hashTable->arrayDoublyLinkedList);
        free(hashTable);
        exit(1);
    }


    hashTable->capacity = capacity;
    hashTable->load = capacity * LOAD_FACTOR / 10;
    hashTable->size = 0;
    return hashTable;
}

uint32_t hash_generate(char* data, size_t lengthBytes) {
    uint32_t hash = 0;
    for (size_t i = 0; i < lengthBytes; i++) {
        hash += hash * PRIME_MULTIPLIER + data[i];
    }
    return hash;
}

int hash_is_occupied(HashTable* hashTable, uint32_t index) {
    if (index >= hashTable->capacity) {
        fprintf(stderr, "ERROR: Index out of range\n");
        exit(1);
    }
    return hashTable->arrayDoublyLinkedList[index] != NULL;
}

void hash_insert(HashTable* hashTable, char* data) {
    uint32_t hash = hash_generate(data, strlen(data));
    uint32_t index = (hash + hashTable->capacity) % hashTable->capacity;

    if (hashTable->arrayDoublyLinkedList[index] == NULL) {
        Entry* entry = entry_create(data, 1);
        DoublyLinkedList* doublyLinkedList = doubly_linked_list_create();
        doubly_linked_list_append(doublyLinkedList, entry);
        hashTable->arrayDoublyLinkedList[index] = doublyLinkedList;
        doubly_linked_list_append(hashTable->arrayEntries,
                                  entry_create(data, hashTable->arrayEntries->size));
    } else {
        Node* node = hashTable->arrayDoublyLinkedList[index]->head;
        // Search if that word is in the list
        while (node != NULL) {
            if (strcmp(node->entry->key, data) == 0) {
                node->entry->value++;
                return;
            }
            node = node->next;
        }
        Entry* entry = entry_create(data, 1);
        doubly_linked_list_append(hashTable->arrayDoublyLinkedList[index], entry);
        doubly_linked_list_append(hashTable->arrayEntries,
                                  entry_create(data, hashTable->arrayEntries->size));
    }
    hashTable->size++;

    if (hashTable->size > hashTable->load) {
        hash_resize(hashTable);
    }
}

uint32_t hash_get(HashTable* hashTable, char* data) {
    uint32_t hash = hash_generate(data, strlen(data));
    uint32_t index = (hash + hashTable->capacity) % hashTable->capacity;

    if (hashTable->arrayDoublyLinkedList[index] == NULL) {
        return 0;
    } else {
        Node* node = hashTable->arrayDoublyLinkedList[index]->head;
        // Search if that word is in the list
        while (node != NULL) {
            if (strcmp(node->entry->key, data) == 0) {
                return node->entry->value;
            }
            node = node->next;
        }
    }
    return 0;
}

void hash_free(HashTable* hashTable) {
    for (uint32_t i = 0; i < hashTable->capacity; i++) {
        if (hashTable->arrayDoublyLinkedList[i] != NULL) {
            doubly_linked_list_free(hashTable->arrayDoublyLinkedList[i]);
        }
    }
    free(hashTable->arrayDoublyLinkedList);

    
    if (hashTable->arrayEntries != NULL) {
        doubly_linked_list_free(hashTable->arrayEntries);
    }
    free(hashTable);
}

void hash_sort(HashTable* hashTable) {
    // Iterate through each item in the hash table
}

int file_is_delimiter(char ch) {
    if (ch == '\n' || ch == '\t') {
        return 1;
    }

    for (size_t i = 0; DELIMITERS[i] != '\0'; i++) {
        if (ch == DELIMITERS[i]) {
            return 1;
        }
    }
    return 0;
}

char* file_read_word(FILE* file) {
    char* word = malloc(WORD_BUFFER);
    if (word == NULL) {
        fprintf(stderr, "ERROR: unable to allocate memory\n");
        exit(1);
    }

    uint8_t index = 0;
    char ch;
    while ((ch = fgetc(file)) != EOF) {
        if (file_is_delimiter(ch)) {
            if (index > 0) {
                word[index] = '\0';
                index = 0;
                return word;
            }
        } else {
            if (index < WORD_BUFFER - 1) {
                word[index++] = tolower(ch);
            } else {
                fprintf(stderr, "ERROR: word is too long\n");
                exit(1);
            }
        }
    }

    if (index > 0) {
        word[index] = '\0';
        return word;
    }

    free(word);
    return NULL;
}

char* file_string_copy(const char* string) {
    char* copy = malloc(strlen(string) + 1);
    if (copy == NULL) {
        fprintf(stderr, "ERROR: unable to alloc memory for string");
        exit(1);
    }
    strcpy(copy, string);
    return copy;
}

int main(int argc, char* argv[]) {
    clock_t start = clock();
    HashTable* hashTable = hash_create(INITIAL_CAPACITY);

    char* filePath = argv[argc - 1];
    FILE* fileInput = fopen(filePath, "r");
    if (fileInput == NULL) {
        fprintf(stderr, "ERROR: unable to open file '%s' for reading\n", filePath);
        exit(1);
    }

    uint32_t numTokens = 0;
    char* word;
    while ((word = file_read_word(fileInput)) != NULL) {
        hash_insert(hashTable, word);
        free(word);
        numTokens++;
    }

    size_t numEntries = hashTable->arrayEntries->size;
    printf("Words: %d\n", numTokens);
    printf("Unique Words: %lu\n", numEntries);
    /*uint32_t entryFrequency;*/
    /*char* entryWord;*/
    /*for (size_t i = 0; i < numEntries; i++) {*/
    /*    entryWord = doubly_linked_list_pop(hashTable->arrayEntries)->entry->key;*/
    /*    entryFrequency = hash_get(hashTable, entryWord);*/
    /*    printf("%s: %lu\n", entryWord, (unsigned long)entryFrequency);*/
    /*}*/

    clock_t end = clock();
    double time_spent = (double)(end - start) / CLOCKS_PER_SEC;
    printf("Time elapsed: %f seconds\n", time_spent);


    hash_free(hashTable);
    fclose(fileInput);
    return 0;
}
