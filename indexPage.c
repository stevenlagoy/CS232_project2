/* File: indexPage.c */
/* Author: Britton Wolfe */
/* Date: September 3rd, 2010 */

/* This program indexes a web page, printing out the counts of words on that page */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX_WORD_LENGTH 50
#define ALPHABET_LEN 26
#define BUFFER_SIZE 300000

struct TrieNode
{
  int count;
  char word[MAX_WORD_LENGTH];
  struct TrieNode* children[ALPHABET_LEN];
};

struct TrieNode* createTrieNode();

int indexPage(const char* url, struct TrieNode *pNode);

int addWordOccurrence(const char* word, const int wordLength, struct TrieNode *pNode);

void printTrieContents(struct TrieNode *pNode);

int freeTrieMemory(struct TrieNode *pNode);

int getText(const char* srcAddr, char* buffer, const int bufSize);

int main(int argc, char** argv){
  /* argv[1] will be the URL to index, if argc > 1 */
  if(argc < 2 || argc > 3){
    fprintf(stderr, "USAGE: %s startWord\n", argv[0]);
    return -1;
  }

  struct TrieNode *pRoot = createTrieNode();
  if(pRoot == NULL){
    printf("ERROR: Could not allocate memory.\n");
    return -2; 
  }

  indexPage(argv[1], pRoot);
  printTrieContents(pRoot);
  freeTrieMemory(pRoot);

  return 0;
}

struct TrieNode* createTrieNode() {
    // Create a TrieNode and return a pointer to it.
    struct TrieNode* pNode = malloc(sizeof(struct TrieNode));
    if (pNode == NULL) {
        return NULL;
    }
    pNode->count = 0;
    for (int i = 0; i < ALPHABET_LEN; i++) {
        pNode->children[i] = NULL;
    }
    return pNode;
}


int indexPage(const char* url, struct TrieNode *pNode){
    // Read words from the webpage, add nodes to the trie structure corresponding to the read words.

    // Read words with getText function
    char buffer[BUFFER_SIZE + 1];
    int bytesRead = getText(url, buffer, sizeof(buffer));
    if(bytesRead <= 0){
        fprintf(stderr, "ERROR: Could not read page content.\n");
        return -1;
    }

    printf("%s\n", url);  // Print URL

    // Add the read words to the Trie structure
    char word[MAX_WORD_LENGTH];
    int wordLength = 0;
    for(int i = 0; i < bytesRead; i++){
        if(isalpha(buffer[i])){
            word[wordLength++] = tolower(buffer[i]); // Build the word
            if(wordLength >= MAX_WORD_LENGTH - 1) break; // Avoid overflow
        } else if(wordLength > 0){
            word[wordLength] = '\0'; // Null-terminate the word
            printf("\t%s\n", word);  // Print word
            addWordOccurrence(word, wordLength, pNode); // Add word to trie
            wordLength = 0; // Reset for next word
        }
    }

    // Final check: Add the last word if any remains
    if(wordLength > 0){
        word[wordLength] = '\0';
        addWordOccurrence(word, wordLength, pNode);
    }

    return 0;
}

int addWordOccurrence(const char* word, const int wordLength, struct TrieNode *pNode){
    // Add a word to the Trie Structure.
    struct TrieNode* current = pNode;

    // Loop through each character in the word
    for(int i = 0; i < wordLength; i++){
        int index = word[i] - 'a';
        if(index < 0 || index >= ALPHABET_LEN){
            return -1; // Invalid character (non-alphabetic)
        }

        // If the node does not exist, allocate memory for it
        if(current->children[index] == NULL){
            current->children[index] = createTrieNode();  // Create the node for this character
            if(current->children[index] == NULL){
                return -2;  // Allocation failed
            }
        }
        current = current->children[index];
    }

    // Increment the word count and store the word at the final node
    current->count += 1;
    strncpy(current->word, word, MAX_WORD_LENGTH); // Store the word in the node (if needed)
    return 0;
}


void printTrieContents(struct TrieNode *pNode){
    // Recursively print the contents of the Trie structure in alphabetical order.
    
    // Base case
    if(pNode == NULL) return;

    // Print current node's word if it has a count > 0
    if(pNode->count > 0){
        printf("%s: %d\n", pNode->word, pNode->count);
    }

    // Check if children are allocated
    if(pNode->children == NULL) return;

    // Recursively print each child
    for(int i = 0; i < ALPHABET_LEN; ++i){
        if(pNode->children[i]){
            printTrieContents(pNode->children[i]);
        }
    }
}

int freeTrieMemory(struct TrieNode *pNode){
    // Recursively deallocate the memory for a Trie structure.
    
    // Base case
    if(pNode == NULL) return 0;

    // Recursively free each child node
    for(int i = 0; i < ALPHABET_LEN; i++){
        if(pNode->children[i] != NULL){
            freeTrieMemory(pNode->children[i]);
        }
    }

    // Free the current node after all children are freed
    free(pNode);
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
