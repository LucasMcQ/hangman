#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <time.h>

void guess(char*);
int char_in_word(char*, char*, char);
void insert(char);
int in_list(char);
void hang(int);
void draw(int guesses, char *user_word, int guess_code);
int get_mode(int argc, char **argv);
char* get_rand_word(int fileloc, char **argv); 


typedef struct Node{

	char elm;
	struct Node *next;

} Node;

Node *hd = NULL;

int main(int argc, char **argv){

	int mode = get_mode(argc, argv);

	char word[45];		// the longest word in the english alphabet

	if(mode == 0){
		scanf("%44s", word);
		guess(word);
	}
	else{
		guess(get_rand_word(mode, argv));
	}



} // main

int get_mode(int argc, char **argv){

	int file_loc;

	if(argc == 1){
		return 0;
	}

	else if(argc == 3){
		if(strcmp(argv[1], "-d") == 0){
			file_loc = 2;
		}
		if(strcmp(argv[2], "-d") == 0){
			file_loc = 1;
		}
		else{
			printf("Error: invalid usage. [0] = %s, [1] = %s, [2] = %s\n", argv[0], argv[1], argv[2]);
			exit(1);
		}
	}

	else{
		printf("Error: invalid usage. [0] = %s, [1] = %s, [2] = %s argc = %d\n", argv[0], argv[1], argv[2], argc);
		exit(1);
	}

	
	return file_loc;

}


char* get_rand_word(int file_loc, char **argv){

//	printf("file_name = %s\n", argv[file_loc]);

	FILE *fp = fopen(argv[file_loc], "r");
	if(fp == NULL){
		fprintf(stderr, "Error: file does not exist!\n");
		exit(1);
	}
	

	size_t sz = 0;
	char *line = NULL;

	int lines = 0;

	while(getline(&line, &sz, fp) != EOF){
		
//		printf("the line=%s, len = %d\n", line, (int)strlen(line));
		lines++;

	} // while

	fclose(fp);

	free(line);

	char* line2 = NULL;

	FILE *fp2 = fopen(argv[file_loc], "r");
	if(fp == NULL){
		fprintf(stderr, "Error: file does not exist!\n");
		exit(1);
	}

//	char *rand_word;

	time_t t;
	srand((unsigned) time(&t));


	int rand_num = (rand() % lines - 1); 

//	printf("num = %d\n", rand_num);

	int count = 0;

	size_t sz2 = 0;

	while(getline(&line2, &sz2, fp2) != EOF){

		if(rand_num == count){
			line[strlen(line)-1] = '\0';			
			break;
		}

		count++;

	} // while

//	printf("word = %s len = %d\n", line2, (int)strlen(line2));

	fclose(fp2);
//	free(line2);

	return line2;


}


void guess(char *word){

	int word_length = (int)strlen(word);
	int guesses = 0;

	int guess_code = -1;

	char *user_word = malloc(sizeof(char) * word_length);

	int i;
	int count = 0;
	for(i = 0; word[i] != '\0'; i++){
		user_word[i] = '_';
		count++;
	} // for
	user_word[count] = '\0';
	printf("%s\n", user_word);

	draw(guesses, user_word, guess_code);

	char guess;

	while(guesses <= 6){
		
		guess = getc(stdin);

		if(in_list(guess) == 1){

			guess_code = 2;
		} // if
		else if(char_in_word(word, user_word, guess) == 1){
			insert(guess);
			guess_code = 1;
		} // if
		else if(isspace(guess) == 0){
			insert(guess);
			guess_code = 0;
			guesses++;
		} // else

		if(strcmp(word, user_word) == 0){
			draw(guesses, user_word, -1);
			printf("You win!\n");
			break;
		} // if
		

		draw(guesses, user_word, guess_code);

		if(guesses == 6){
			printf("You lose!\n");
			printf("The word was: %s\n", word);
			break;
		} // if

	} // while

} // word


int char_in_word(char *word, char *user_word, char guess){

	int i;
	int in_word = 0;

	for(i = 0; word[i] != '\0'; i++){
		if(tolower(word[i]) == tolower(guess)){
			user_word[i] = guess;
			in_word = 1;
		} // if
	} // for

	return in_word;

} // char_in_word


void insert(char elm){

	if(hd == NULL){
		hd = malloc(sizeof(Node));
		if(hd == NULL){
			fprintf(stderr, "There was an error allcoating memory.\n");
			exit(1);
		} // if
		hd->next = NULL;
		hd->elm = elm;
		return;
	} // if

	Node *cursor = hd;

	while(cursor->next != NULL){
		cursor = cursor->next;
	} // while

	Node *new_node = malloc(sizeof(Node));
	
	if(new_node == NULL){
		fprintf(stderr, "There was an error allcoating memory.\n");
		exit(1);
	}

	new_node->next = NULL;
	new_node->elm  = elm;

	cursor->next = new_node;

} // insert


int in_list(char find){

	Node *temp_node = hd;

	while(temp_node != NULL){
		if(temp_node->elm == find){
			return 1;
		}
		temp_node = temp_node->next;
	} // while

	return 0;

} // in_list


void print_guesses(){

	printf("Guesses:");

	Node *temp_node = hd;

	while(temp_node != NULL){
		printf(" %c", temp_node->elm);
		temp_node = temp_node->next;
	} // while

	printf("\n");

} // print_guesses


void hang(int guesses){

	if(guesses >= 1){
		printf("\t\u263A\n");
	} // if
	if(guesses >=2){
		printf("       /");
	}
	if(guesses >=3){
		printf("|");
	}
	if(guesses >=4){
		printf("\\\n");
	}
	if(guesses >=5){
		printf("       / ");
	}
	if(guesses >=6){
		printf("\\\n");
	}

} // hang


void draw(int guesses, char *user_word, int guess_code){

	system("clear");

	print_guesses();

	printf("\n\n\n");

	hang(guesses);

	printf("\n\n\n");

	if(guess_code == 1){
		printf("\n\nCorrect guess!\n");
	}
	else if(guess_code == 0){
		printf("\n\nIncorrect guess!\n");
	}
	else if(guess_code == 2){
		printf("You have already guessed that letter.\n");
	}

	printf("\n\n%s\n", user_word);

} // draw
