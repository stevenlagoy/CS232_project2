#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define ALPHABET_SIZE 26
#define MAX_WORD_LENGTH 50
#define BUFFER_SIZE 300000

struct TrieNode {
    int count;
    struct TrieNode* children[ALPHABET_SIZE];
};

// Function Prototypes
struct TrieNode* createTrieNode();

int indexPage(const char* url, struct trieNode *root);

int addWordOccurrence(struct trieNode *root, const char* word, const int wordLength);

void printTrieContents(struct TrieNode* root, int depth);

void freeTrieMemory(struct TrieNode* root);

int getText(const char* srcAddr, char* buffer, const int bufSize); 

int main(int argc, char** argv) {
    /* argv[1] will be the URL to index, if argc > 1 */
    if(argc < 2 || argc > 3){
        fprintf(stderr, "USAGE: %s startWord\n", argv[0]);
        return -1;
    }

    struct trieNode *pRoot = createTrieNode();

    indexPage(argv[1], pRoot);
    printTrieContents(pRoot, 0);
    freeTrieMemory(pRoot);

    return 0;
}

struct TrieNode* createTrieNode() {
    struct TrieNode* node = (struct TrieNode*)malloc(sizeof(struct TrieNode));
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

int indexPage(const char* url, struct trieNode *root){
    char buffer[BUFFER_SIZE];
    struct TrieNode* root = createTrieNode();
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
        // Add word to trie, skipping non-alphabetic characters
        addWordOccurrence(root, word);
        printf("\t%s\n", word);
        word = strtok(NULL, " \t\n\r");
    }

    char printBuffer[MAX_WORD_LENGTH]; // Max word length for buffer size
    printTrie(root, printBuffer, 0);
    freeTrie(root);
}

int addWordOccurrence(struct trieNode *root, const char* word, const int wordLength){
    struct TrieNode* current = root;
    while (*word) {
        char c = tolower(*word);
        if (c < 'a' || c > 'z') {
            word++;
            continue; // Skip non-alphabetic characters
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

void printTrieContents(struct TrieNode* root, int depth){
    if (root == NULL) return;

    char buffer[MAX_WORD_LENGTH];

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

void freeTrieMemory(struct TrieNode* root) {
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