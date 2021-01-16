#include "bst.h"

static int _longest_len = 0;
static int _longest_fq = 0;

struct BST* bst(char letter, char* word, int fq, struct BST* left, struct BST* right) {
  struct BST out = {letter, word, fq, left, right};
  struct BST* ptr = (struct BST*) malloc(sizeof(struct BST));
  *ptr = out;
  return ptr;
}

void to_lower_word(char* lower_word, char* word) {
  for (int i = 0; i < strlen(word); i++) {
    char letter = tolower(word[i]);
    strncat(lower_word, &letter, 1);
  }
}

struct BST* add_word(char* word, struct BST* root) {
  //makes a var that is same as the word but lowercase
  char* lower_word = (char*) calloc(strlen(word), sizeof(char));
  to_lower_word(lower_word, word);
  //updates longest length
  _longest_len = _longest_len < strlen(lower_word) ? strlen(lower_word) : _longest_len;
  if (root == NULL) {
    return bst(word[0], lower_word, 1, NULL, NULL);
  }
  if (strcmp(lower_word, root -> word) ==  0) {
    root -> fq += 1;
    //update longest_fq
    _longest_fq = _longest_fq < root -> fq ? root -> fq : _longest_fq;
  } else if (strcmp(lower_word, root -> word) < 0) {
    root -> left = add_word(word, root -> left);
  } else {
    root -> right = add_word(word, root -> right);
  }
  return root;
}

int num_words(struct BST* head) {
  if (head == NULL) {
    return 0;
  }
  return 1 + num_words(head -> left) + num_words(head -> right);
}
//helper funtion for to_array
void _bst_to_array(struct BST* arr[], struct BST* node, int* i) {
  if (node == NULL) {
    return;
  }
  _bst_to_array(arr, node -> left, i);
  arr[*i] = node;
  (*i)++;
  _bst_to_array(arr, node -> right, i);
}
void bst_to_array(struct BST* arr[], struct BST* root) {
  int i = 0;
  _bst_to_array(arr, root, &i);
}

void free_bst(struct BST* root) {
  if (root == NULL) {
    return;
  }
  free_bst(root -> left);
  free_bst(root -> right);
  free(root);
}

int longest_len() {
  return _longest_len;
}
int longest_dig() {
  int dig = 0;
  int temp = _longest_fq;
  while (temp != 0) {
    temp /= 10;
    dig++;
  }
  return dig;
}
