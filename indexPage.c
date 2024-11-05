/* File: indexPage.c */
/* Author: Britton Wolfe */
/* Date: September 3rd, 2010 */

/* This program indexes a web page, printing out the counts of words on that page */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MAX_WORD_LENGTH 50
#define NUM_CHILDREN 26

struct trieNode{
  char word[MAX_WORD_LENGTH];
  int count;
  struct trieNode** children;
};

/* TODO: change this return type */
void indexPage(const char* url);

int addWordOccurrence(const char* word, const int wordLength
		       /* TODO: other parameters you need */);

void printTrieContents(struct trieNode *pNode);

int freeTrieMemory(struct trieNode *pNode);

int getText(const char* srcAddr, char* buffer, const int bufSize);

int main(int argc, char** argv){
  /* TODO: write the (simple) main function

  /* argv[1] will be the URL to index, if argc > 1 */
  if(argc < 2 || argc > 3){
    fprintf(stderr, "USAGE: %s startWord\n", argv[0]);
    return -1;
  }

  struct trieNode *pTrieRoot;

  pTrieRoot = malloc(sizeof(struct trieNode));
  if(pTrieRoot == NULL){
    fprintf(stderr, "ERROR: Could not allocate memory\n");
    return -2;
  }
  strncpy(pTrieRoot->word, argv[1], MAX_WORD_LENGTH);
  pTrieRoot->count = 0;
  pTrieRoot->children = malloc(sizeof(struct trieNode*) * NUM_CHILDREN);

  freeTrieMemory(pTrieRoot);

  return 0;
}

/* TODO: define the functions corresponding to the above prototypes */

/* TODO: change this return type */
void indexPage(const char* url)
{}

int addWordOccurrence(const char* word, const int wordLength
		       /* TODO: other parameters you need */)
{}

void printTrieContents(struct trieNode *pNode)
{}

int freeTrieMemory(struct trieNode *pNode)
{
  if(pNode == NULL) return;
  struct trieNode **pChildren;
  pChildren = pNode->children;
  free(pNode);
  int i;
  for(i = 0; i < 26; i++){
    freeTrieMemory(pChildren[i]);
  }
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
