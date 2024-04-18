#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct TrieNode {
    struct TrieNode *children[26];
    int isEndOfWord;
};

// Trie structure
struct Trie {
    struct TrieNode *root;
};

// Create a new trie node
struct TrieNode *createNode() {
    struct TrieNode *node = (struct TrieNode *)malloc(sizeof(struct TrieNode));
    node->isEndOfWord = 0;
    for (int i = 0; i < 26; i++) {
        node->children[i] = NULL;
    }
    return node;
}

struct Trie *createTrie() {
    struct Trie *trie = (struct Trie *)malloc(sizeof(struct Trie));
    trie->root = createNode();
    return trie;
}

// Inserts the word into the Trie structure
void insert(struct Trie *pTrie, char *word) {
    struct TrieNode *current = pTrie->root;
    for (int i = 0; word[i] != '\0'; i++) {
        int index = word[i] - 'a';
        if (current->children[index] == NULL) {
            current->children[index] = createNode();
        }
        current = current->children[index];
        if (word[i + 1] == '\0') {
            current->isEndOfWord++;
        }
    }
}

// Search for a word in the trie and return the number of occurrences
int numberOfOccurrences(struct Trie *trie, char *word) {
    struct TrieNode *curr = trie->root;
    int n = strlen(word);
    for (int i = 0; i < n; i++) {
        int index = word[i] - 'a';
        if (!curr->children[index]) {
            return 0;
        }
        curr = curr->children[index];
    }
    return curr->isEndOfWord;
}

// Deallocate memory used by the trie
void deallocateTrieNode(struct TrieNode *node) {
    if (node) {
        for (int i = 0; i < 20; i++) {
            deallocateTrieNode(node->children[i]);
        }
        free(node);
    }
}

void deallocateTrie(struct Trie *trie) {
    deallocateTrieNode(trie->root);
    free(trie);
}

// Read dictionary file and store words in pInWords array
int readDictionary(char *filename, char **pInWords) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error opening file\n");
        return 0;
    }

    int wordCount = 0;
    char word[100];
    while (fscanf(file, "%s", word) == 1) {
        pInWords[wordCount] = strdup(word);
        wordCount++;
    }

    fclose(file);
    return wordCount;
}

int main(void) 
{
        char *inWords[256];

        int numWords = readDictionary("dictionary.txt", inWords);
        for (int i = 0; i < numWords; ++i) {
            printf("%s\n", inWords[i]);
        }
        struct Trie *trie = createTrie();
        for (int i = 0; i < numWords; i++) {
            insert(trie, inWords[i]);
        }
        char *pWords[] = {"notaword", "ucf", "no", "note", "corg"};
        for (int i = 0; i < 5; i++) {
            printf("\t%s : %d\n", pWords[i], numberOfOccurrences(trie, pWords[i]));
        }
        deallocateTrie(trie);
        if (trie != NULL) {
            printf("There is an error in this program\n");
        }
        return 0;
}
