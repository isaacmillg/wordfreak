#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

struct BST {
  char letter;
  char* word;
  int fq;
  struct BST* left;
  struct BST* right;
};
//bst node constructor that dynamically allocates memory
struct BST* bst(char letter, char* word, int fq, struct BST* left, struct BST* right);

//takes 'word' param and makes 'lower_word' the same word, but all lowercase
void to_lower_word(char* lower_word, char* word);

//assumes that the word's 1st letter is the same letter BST and root param is not NULL
//if the word is already in the BST, then it increments the fq field
//if not, then it adds a new node into the BST
struct BST* add_word(char word[], struct BST* root);

//returns the number of words in the given bst
int num_words(struct BST* head);

//fills the arr with bst nodes in order
//assumes arr[] has length = num_words(root)
void bst_to_array(struct BST* arr[], struct BST* root);

//traverses the tree in postorder and frees all of the nodes
void free_bst(struct BST* root);

//returns the length of the longest word in all of the bst nodes that have been added
int longest_len();

//returns the number of digits from the frequency of the most frequent word
int longest_dig();

