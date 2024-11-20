/* File: indexPage.c */
/* Author: Britton Wolfe */
/* Date: September 3rd, 2010 */

/* This program indexes a web page, printing out the counts of words on that page */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX_WORD_LENGTH 50
#define NUM_CHILDREN 26
#define BUFFER_SIZE 300000

struct trieNode{
  char word[MAX_WORD_LENGTH];
  int count;
  struct trieNode** children;
};

/* TODO: change this return type */
int indexPage(const char* url, struct trieNode *pNode);

int addWordOccurrence(const char* word, const int wordLength, struct trieNode *pNode);

void printTrieContents(struct trieNode *pNode);

int freeTrieMemory(struct trieNode *pNode);

int getText(const char* srcAddr, char* buffer, const int bufSize);

int main(int argc, char** argv){
  /* argv[1] will be the URL to index, if argc > 1 */
  if(argc < 2 || argc > 3){
    fprintf(stderr, "USAGE: %s startWord\n", argv[0]);
    return -1;
  }

  struct trieNode *pRoot = malloc(sizeof(struct trieNode));
  if (pRoot == NULL) {
      fprintf(stderr, "ERROR: Could not allocate memory\n");
      return -2;
  }

  memset(pRoot, 0, sizeof(struct trieNode));
  pRoot->children = malloc(sizeof(struct trieNode*) * NUM_CHILDREN);
  if (pRoot->children == NULL) {
      fprintf(stderr, "ERROR: Could not allocate memory for children\n");
      free(pRoot);
      return -2;
  }

  for (int i = 0; i < NUM_CHILDREN; i++) {
      pRoot->children[i] = NULL;
  }


  indexPage(argv[1], pRoot);
  printTrieContents(pRoot);
  freeTrieMemory(pRoot);

  return 0;
}

/* TODO: define the functions corresponding to the above prototypes */

/* TODO: change this return type */
int indexPage(const char* url, struct trieNode *pNode) {
    char buffer[BUFFER_SIZE + 1];
    int bytesRead = getText(url, buffer, sizeof(buffer));
    if (bytesRead <= 0) {
        fprintf(stderr, "ERROR: Could not read page content.\n");
        return -1;
    }

    printf("%s\n", url);  // Print URL

    char word[MAX_WORD_LENGTH];
    int wordLength = 0;
    for (int i = 0; i < bytesRead; i++) {
        if (isalpha(buffer[i])) {
            word[wordLength++] = tolower(buffer[i]); // Build the word
            if (wordLength >= MAX_WORD_LENGTH - 1) break; // Avoid overflow
        } else if (wordLength > 0) {
            word[wordLength] = '\0'; // Null-terminate the word
            printf("\t%s\n", word);  // Print word
            addWordOccurrence(word, wordLength, pNode); // Add word to trie
            wordLength = 0; // Reset for next word
        }
    }

    // Final check: Add the last word if any remains
    if (wordLength > 0) {
        word[wordLength] = '\0';
        addWordOccurrence(word, wordLength, pNode);
    }

    return 0;
}

int addWordOccurrence(const char* word, const int wordLength, struct trieNode *pNode) {
    struct trieNode* current = pNode;

    for (int i = 0; i < wordLength; i++) {
        int index = word[i] - 'a';
        if (index < 0 || index >= NUM_CHILDREN) {
            return -1; // Invalid character
        }

        // Allocate children array if not already allocated
        if (current->children == NULL) {
            current->children = malloc(sizeof(struct trieNode*) * NUM_CHILDREN);
            if (current->children == NULL) {
                return -2; // Allocation failed
            }
            for (int j = 0; j < NUM_CHILDREN; ++j) {
                current->children[j] = NULL; // Initialize all child pointers
            }
        }

        // Allocate the next node if it does not exist
        if (current->children[index] == NULL) {
            current->children[index] = malloc(sizeof(struct trieNode));
            if (current->children[index] == NULL) {
                return -2; // Allocation failed
            }
            memset(current->children[index], 0, sizeof(struct trieNode)); // Initialize
        }

        current = current->children[index];
    }

    // Update word and increment count
    current->count += 1;
    strncpy(current->word, word, MAX_WORD_LENGTH);
    return 0;
}

void printTrieContents(struct trieNode *pNode) {
    if (pNode == NULL) return;

    // TODO: complete print function
    return;
}

int freeTrieMemory(struct trieNode *pNode) {
    if (pNode == NULL) return 0;

    // TODO: complete free memory function
    return 0;
}


/* Don't modify this function */
int getText(const char* srcAddr, char* buffer, const int bufSize){
  FILE *pipe;
  int bytesRead;

  snprintf(buffer, bufSize, "curl -s \"%s\" | python3 getText.py", srcAddr);

  pipe = popen(buffer, "r");
  if(pipe == NULL){
    fprintf(stderr, "ERROR: could not open the pipe for command %s\n",
	    buffer);
    return 0;
  }

  bytesRead = fread(buffer, sizeof(char), bufSize-1, pipe);
  buffer[bytesRead] = '\0';

  pclose(pipe);

  return bytesRead;
}
