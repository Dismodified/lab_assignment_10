#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Trie structure
struct TrieNode {
    struct TrieNode *children[26];
    int isEndOfWord;
};

struct Trie {
    struct TrieNode *root;
};

// Inserts the word to the trie structure
void insert(struct Trie *pTrie, char *word) {
    struct TrieNode *current = pTrie->root;
    for (int i = 0; word[i] != '\0'; i++) {
        int index = word[i] - 'a';
        if (current->children[index] == NULL) {
            current->children[index] = (struct TrieNode *)malloc(sizeof(struct TrieNode));
            for (int j = 0; j < 26; j++) {
                current->children[index]->children[j] = NULL;
            }
            current->children[index]->isEndOfWord = 0;
        }
        current = current->children[index];
    }
    current->isEndOfWord = 1;
}

// Computes the number of occurrences of the word
int numberOfOccurances(struct Trie *pTrie, char *word) {
    struct TrieNode *current = pTrie->root;
    for (int i = 0; word[i] != '\0'; i++) {
        int index = word[i] - 'a';
        if (current->children[index] == NULL) {
            return 0; // Word not found
        }
        current = current->children[index];
    }
    return current->isEndOfWord;
}

// Deallocate the trie structure
void deallocateTrieHelper(struct TrieNode *root) {
    if (root == NULL) return;
    for (int i = 0; i < 26; i++) {
        if (root->children[i]) {
            deallocateTrieHelper(root->children[i]);
        }
    }
    free(root);
}

// Deallocate the trie structure
struct Trie *deallocateTrie(struct Trie *pTrie) {
    if (pTrie == NULL) {
        return NULL; // Trie is already deallocated
    }

    // Recursively deallocate all nodes starting from the root
    deallocateTrieHelper(pTrie->root);

    // Free the Trie structure itself
    free(pTrie);

    return NULL; // Return NULL after deallocation
}

// Initializes a trie structure
struct Trie *createTrie() {
    struct Trie *pTrie = (struct Trie *)malloc(sizeof(struct Trie));
    if (pTrie != NULL) {
        pTrie->root = (struct TrieNode *)malloc(sizeof(struct TrieNode));
        if (pTrie->root != NULL) {
            for (int i = 0; i < 26; i++) {
                pTrie->root->children[i] = NULL;
            }
            pTrie->root->isEndOfWord = 0;
        } else {
            // Failed to allocate memory for the root node
            free(pTrie);
            pTrie = NULL;
        }
    }
    return pTrie;
}

// this function will return number of words in the dictionary,
// and read all the words in the dictionary to the structure words
int readDictionary(char *filename, char **pInWords) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error opening file: %s\n", filename);
        return -1; // Return -1 to indicate failure
    }

    int numWords = 0;
    char word[256];

    // Count the number of words in the file
    while (fscanf(file, "%s", word) != EOF) {
        numWords++;
    }

    // Allocate memory to store the words
    *pInWords = (char **)malloc(numWords * sizeof(char *));
    if (*pInWords == NULL) {
        printf("Memory allocation failed\n");
        fclose(file);
        return -1; // Return -1 to indicate failure
    }

    // Reset file pointer to the beginning of the file
    fseek(file, 0, SEEK_SET);

    // Read words from file and store them in the array
    for (int i = 0; i < numWords; i++) {
        fscanf(file, "%s", word);
        (*pInWords)[i] = strdup(word); // Store a copy of the word in the array
    }

    fclose(file);
    return numWords; // Return the number of words read from the file
}

int main(void) {
    char *inWords[256];

    // read the number of the words in the dictionary
    int numWords = readDictionary("dictionary.txt", inWords);
    if (numWords == -1) {
        return 1; // Exit with failure if reading the dictionary failed
    }

    for (int i = 0; i < numWords; ++i) {
        printf("%s\n", inWords[i]);
    }

    struct Trie *pTrie = createTrie();
    for (int i = 0; i < numWords; i++) {
        insert(pTrie, inWords[i]);
    }

    // parse line by line, and insert each word to the trie data structure
    char *pWords[] = {"notaword", "ucf", "no", "note", "corg"};
    for (int i = 0; i < 5; i++) {
        printf("\t%s : %d\n", pWords[i], numberOfOccurances(pTrie, pWords[i]));
    }

    pTrie = deallocateTrie(pTrie);
    if (pTrie != NULL) {
        printf("There is an error in this program\n");
        return 1; // Exit with failure if there's an error
    }

    return 0;
}
