
#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <stdbool.h>
#define MAX_INPUT_SIZE 1024
#define MAX_TOKEN_SIZE 64
#define MAX_NUM_TOKENS 64
#define WRITE_END 1
#define READ_END 0
int quit_command(char **argv)
{ //checks if string "quit" is inputed and catches command
if(strcmp("quit",argv[0]
)==0)
{
printf("Exiting Shell\n");
exit(0);
}
return 0;
}
int count_tokens(char** tokens)
{ //counts number of tokens
char **newtokens = (char **)malloc(MAX_NUM_TOKENS * sizeof(char *)
);
int count = 0;
newtokens = tokens;
while(*newtokens)
{
newtokens++;
count += 1;
}
return count;
}
bool is_pipe(char** tokens)
{//checks if tokens has a "|"
char **newtokens = tokens;
int n = count_tokens(newtokens);
for(int i = 0; i < n; i++)
{
if(
!strcmp(tokens[i],"|")
)
{
return true;
}
}
return false;
}
int check_pipe(char**tokens)
{
char **newtokens = tokens;
int n = count_tokens(newtokens);
int index = 0;
for(int i = 0; i < n; i++)
{
if(
!strcmp(tokens[i],"|")
)
{
return index;
}
index++;
}
return index;
}
char** beginning_pipe(char**tokens, int index)
{
char **newtokens = (char **)malloc(MAX_NUM_TOKENS * sizeof(char *)
);
//printf("first command\n");
for(int i = 0; i < index; i++)
{
newtokens[i] = tokens[i];
}
return newtokens;
}
char** ending_pipe(char**tokens, int index)
{
//printf("this is my index: %i", index);
int n = count_tokens(tokens);
int j = 0;
char **newtokens = (char **)malloc(MAX_NUM_TOKENS * sizeof(char *)
);
//printf("Second commmand\n");
for(int i = index + 1; i < n; i++)
{
newtokens[j] = tokens[i];
//printf("%s\n",newtokens[j]);
j++;
}
return newtokens;
}
char** breakdown(char** tokens)
{
char **newtokens = (char **)malloc(MAX_NUM_TOKENS * sizeof(char *)
);
int n = count_tokens(tokens);
for(int i = 0; i < n-2; i++)
{
newtokens[i] = tokens[i];
}
return newtokens;
}
char **tokenize(char *line)
{ // Splits the string by space and returns the array of tokens{
char **tokens = (char **)malloc(MAX_NUM_TOKENS * sizeof(char *)
);
char *token = (char *)malloc(MAX_TOKEN_SIZE * sizeof(char)
);
int i, tokenIndex = 0, tokenNo = 0;
for(i =0; i < strlen(line); i++)
{
char readChar = line[i];
if (readChar == ' ' |
| readChar == '\n' |
| readChar == '\t'
)
{
token[tokenIndex] = '\0';
if (tokenIndex != 0)
{
tokens[tokenNo] = (char*)malloc(MAX_TOKEN_SIZE*sizeof(char)
);
strcpy(tokens[tokenNo++], token);
tokenIndex = 0;
}
} else {
token[tokenIndex++] = readChar;
}
}
free(token);
tokens[tokenNo] = NULL ;
return tokens;
}
int main(int argc, char* argv[]
) {
char line[MAX_INPUT_SIZE];
char **tokens;
char **newtokens;
int i;
int index;
int file_d[2];
while(1) {
/* BEGIN: TAKING INPUT */
bzero(line, sizeof(line)
);
char cwd[256];
getcwd(cwd, sizeof(cwd)
); //getting the parent directory
printf("%s",cwd);
printf(" minershell$ "); //emiting shell prompt
scanf("%[^\n]", line);
getchar();
/* END: TAKING INPUT */
line[strlen(line)
] = '\n'; //terminate with new line
tokens = tokenize(line);
newtokens = tokens;
if(tokens[0] == NULL)
{ //controls empty input into shell
continue;
}
if(
!strcmp(tokens[0],"cd")
)
{ //controls change of directory in shell considering folders with spaces
int n = count_tokens(tokens);
char *dir = tokens[1];
for(int i = 1; i < n-1; i++)
{
strcat(dir, "\\ ");
strcat(dir, tokens[i+1]
);
}
//printf("%s\n",dir);
if(chdir(dir)
)
{
perror("Directory not changed: ");
};
continue;
}
if(
!quit_command(tokens)
)
{ //controls quit command in shell, else execute command!
//file_d[0] = read
//file_d[1] = write
int n = count_tokens(tokens);//counts the tokens in the argument inputted
pipe(file_d);
