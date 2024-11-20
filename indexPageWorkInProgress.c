/* File: indexPage.c */
/* Author: Britton Wolfe */
/* Date: September 3rd, 2010 */

/* This program indexes a web page, printing out the counts of words on that page */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define ALPHABET_SIZE 26

typedef struct TrieNode {
    int count;
    struct TrieNode* children[ALPHABET_SIZE];
} TrieNode;

TrieNode* createTrieNode() {
    TrieNode* node = (TrieNode*)malloc(sizeof(TrieNode));
    if (node == NULL) {
        printf("Error allocating memory.\n");
        return NULL;
    }
    node->count = 0;
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        node->children[i] = NULL;
    }
    return node;
}

void freeTrie(TrieNode* root) {
    if (root == NULL) {
        return;
    }
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        if (root->children[i]) {
            freeTrie(root->children[i]);
        }
    }
    free(root);
}

void addWordOccurrence(TrieNode* root, const char* word) {
    TrieNode* current = root;
    while (*word) {
        char c = tolower(*word);
        if (c < 'a' || c > 'z') {
            word++;
            continue;
        }
        int index = c - 'a';
        if (current->children[index] == NULL) {
            current->children[index] = createTrieNode();
        }
        current = current->children[index];
        word++;
    }
    current->count++;
}

void printTrie(TrieNode* root, char* buffer, int depth) {
    if (root == NULL) return;

    if (root->count > 0) {
        buffer[depth] = '\0';
        printf("%s: %d\n", buffer, root->count);
    }

    for (int i = 0; i < ALPHABET_SIZE; i++) {
        if (root->children[i]) {
            buffer[depth] = 'a' + i;
            printTrie(root->children[i], buffer, depth + 1);
        }
    }
}

void indexPage(const char* url) {
    char buffer[300000];
    TrieNode* root = createTrieNode();
    if (root == NULL) return;

    int bytesRead = getText(url, buffer, sizeof(buffer));
    if (bytesRead <= 0) {
        printf("Error: Could not load page.\n");
        freeTrie(root);
        return;
    }

    printf("%s\n", url);
    char* word = strtok(buffer, " \t\n\r");
    while (word != NULL) {
        addWordOccurrence(root, word);
        printf("\t%s\n", word);
        word = strtok(NULL, " \t\n\r");
    }

    char printBuffer[300000];  // this may be too much idk
    printTrie(root, printBuffer, 0);
    freeTrie(root);
}

int main(int argc, char** argv) {
    if (argc < 2) {
        printf("Usage: %s <URL>\n", argv[0]);
        return 1;
    }

    indexPage(argv[1]);

    return 0;
}