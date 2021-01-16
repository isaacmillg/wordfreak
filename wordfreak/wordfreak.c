#include "bst.h"                                                 
#include <sys/types.h>                                                                                                        
#include <sys/stat.h>
#include <fcntl.h> 
#include <unistd.h>   
#define MAX_WORD_LEN 100
#define BUF_LEN 1024

//alphebetical list of BSTs
struct BST* arr_of_bst[26] = { NULL };

//"hash function" for finding the index of the correct bst for a word
//takes the 1st letter of the word as the parameter
int index_of_letter(char letter) {
  return letter - 'a' < 0 ? letter - 'A': letter - 'a';
}
//helper function to process_file
//'contents' -> buffer as a string that we are parsing through
//'remainder' -> if it is not the end of the file, then from the last space before we are less than MAX_WORD_LEN away from the end of 'contents' is stored in this param
//'isEOF' -> 1 if it is the end of the file, and 0 if it isn't
void process_string(char* contents, char* remainder, int isEOF) {
  char* word = (char*) calloc(MAX_WORD_LEN, sizeof(char));
  int contents_i = 0;
  //copys characters into the 'word' var, then after it encounters a non alpha char, it adds that word into the correct bst
  while (contents_i < strlen(contents)) {
    if (isalpha(contents[contents_i]) != 0) {
      strncat(word, &contents[contents_i], 1);
    } else if (strlen(word) != 0) {
      if (arr_of_bst[index_of_letter(word[0])] == NULL) {
	char* lower = (char*) calloc(strlen(word), sizeof(char));
	to_lower_word(lower, word);
	arr_of_bst[index_of_letter(word[0])] = bst(lower[0], lower, 1, NULL, NULL);;
      } else {
	add_word(word, arr_of_bst[index_of_letter(word[0])]);
      }
      word = (char*) calloc(MAX_WORD_LEN, sizeof(char));
      //if 'contents' isnt the last buffer of the file, and if there is less than MAX_WORD_LEN between the current non-alpha to the end:
      //then create the remainder to have everything from the current 'contents_i' to the end of 'contents' 
      if (isEOF == 0 && MAX_WORD_LEN > (int)strlen(contents) - contents_i) {
	for (int i = contents_i; i < strlen(contents); i++) {
	  strncat(remainder, &contents[i], 1);
	}
	return;
      }
    }
    contents_i++;
  }
}

//processes the file's words into the correct BST
void process_file(char* filepath) {
  int fdin;
  if (strcmp("STDIN_FILENO", filepath) == 0) {
    fdin = 0;
  } else { 
    fdin = open(filepath, O_RDONLY);
  }
  if (fdin == -1) {
    printf("There was an error opening %s\n", filepath);
    return;
  }
  char* contents = (char*) calloc(BUF_LEN + MAX_WORD_LEN, sizeof(char));
  char* remainder = (char*) calloc(MAX_WORD_LEN, sizeof(char));
  char* buf = (char*) calloc (BUF_LEN, sizeof(char));
  int cnt = 0;
  while (cnt = read(fdin, buf, BUF_LEN) > 0) {
    //adds remainder from last buffer as well as the current buffer to the 'contents' string
    strcat(contents, remainder);
    strcat(contents, buf);
    remainder = (char*) calloc(MAX_WORD_LEN, sizeof(char));
    if (strlen(buf) == BUF_LEN) {
      process_string(contents, remainder, 0);
    } else {
      process_string(contents, remainder, 1);
    }
    contents = (char*) calloc(BUF_LEN + MAX_WORD_LEN, sizeof(char));
    buf = (char*) calloc(BUF_LEN, sizeof(char));
  }
  close(fdin);
}

//writes the output of the program into 'filepath'
void write_output() {
  int fdout = open("output.txt", O_RDWR|O_CREAT|O_TRUNC|O_APPEND, 0644);
  if (fdout == -1) {
    printf("There was an error opening 'output.txt'");
  }
  for (int i = 0; i < 26; i++) {
    if (arr_of_bst[i] != NULL) {
      int len = num_words(arr_of_bst[i]);
      struct BST* arr[len];
      bst_to_array(arr, arr_of_bst[i]);
      for (int j = 0; j < len; j++) {
	char* buf = (char*) calloc(strlen(arr[j] -> word) + MAX_WORD_LEN, sizeof(char));
	sprintf(buf, "%-*s : %*i\n", longest_len() + 1, arr[j] -> word, longest_dig(), arr[j] -> fq);
	write(fdout, buf, strlen(buf));
      }
      write(fdout, ". . .\n", strlen(". . .\n"));
    } 
  }
  close(fdout);
}

void main(int argc, char* argv[]) {
  //processes standard input
  process_file("STDIN_FILENO");
  //processes all files that are in argv
  if (argc != 1) {
    for (int i = 1; i < argc; i++) {
      process_file(argv[i]);
    }
  } 
  //processes the file if it is in the WORD_FREAK environ var
  char* env = getenv("WORD_FREAK");
  if (env != NULL) {
    process_file(env);
  }
  write_output();
  for (int i = 0; i < 26; i++) {
    if (arr_of_bst[i] != NULL) {
      free_bst(arr_of_bst[i]);
    }
  }
}
