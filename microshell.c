#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <dirent.h>
#include <string.h>
#include <signal.h>
#include <curses.h>
#include <ncurses.h>

#define TRUE 1
#define FALSE 0
#define BUFFER_SIZE 1024


char buffer[BUFFER_SIZE];
char command[BUFFER_SIZE];
char arguments[BUFFER_SIZE];
char path[BUFFER_SIZE];
char path2[BUFFER_SIZE] = {'\0'};
char tokens[5][BUFFER_SIZE] = {'\0'};
char **arr_tokens;
char corrector[] = " ";
char * temp;
int tokens_number = 0;
char buffer_text[BUFFER_SIZE];
int fd_file, fd_new_file, num;
char history[200][BUFFER_SIZE];
int history_number = 1;
int i,j;

void char_group_tokens(char const buffer[BUFFER_SIZE]) {
	tokens_number = 0;
	j = 0;

	for(i = 0; i < strlen(buffer); i++) {
		history[history_number][i] = buffer[i];
	}

	history_number++;

	for(i = 0; i < strlen(buffer); i++) {
		if((buffer[i] == ' ' || i == strlen(buffer) - 1) && j == 0) {
			while(j < i) {
				command[j] = buffer[j];
				j++;
			}
		}
	}

	j=0;

	for(i = 0; i < strlen(buffer) - 1; i++) {
		arguments[j] = buffer[i];
		j++;
	}

	if(strlen(arguments) != 0) {
		temp = strtok(arguments,corrector);

		for(i = 0; i < strlen(temp); i++) {
			tokens[tokens_number][i] = temp[i];
		}

		tokens_number++;

		while(temp != NULL) {
			temp = strtok(NULL, corrector);

			if(temp != NULL) {
				for(i = 0; i < strlen(temp); i++) {
					tokens[tokens_number][i] = temp[i];
				}

				tokens_number++;
			}
		}
	}

	arr_tokens = (char**)malloc(tokens_number*sizeof(char*));

	for(i = 0; i <= tokens_number; i++) {
		arr_tokens[i] = (char*)malloc(256*sizeof(char));
	}

	for(i = 0; i <= tokens_number; i++) {
		arr_tokens[i] = tokens[i];
	}

	arr_tokens[tokens_number] = NULL;

	for(j = 0; j < tokens_number; j++) {
		for(i = 0; i < strlen(tokens[j]); i++) {
			if(tokens[j][i] == '_') {
				tokens[j][i] = ' ';
			}
		}
	}
}

void help_cmd_pl() {
	printf("Projekt Microshell\n");
	printf("Autor: Adam Knapik\n\n");
	printf("Komendy:\n");
	printf("help - informacje na temat programu\n");
	printf("pwd - sciezka dostepu do obecnego katalogu\n");
	printf("cd - zmiana katalogu\n");
	printf("cp - kopiowanie pliku\n");
	printf("mv - przenoszenie (zmiana nazwy) pliku\n");
	printf("cat - laczenie oraz wyswietlanie plikow\n");
	printf("history - historia uzytych komend\n");
	printf("logname lub whoami - nazwa uzytkownika\n");
	printf("exit - wyjscie z programu\n");
}

void help_cmd_en() {
    printf("Microshell Project\n");
    printf("Author: Adam Knapik\n\n");
    printf("List of commands:\n");
    printf("help - information about program\n");
    printf("pwd - print working directory\n");
    printf("cd - change directory\n");
    printf("cp - copy a files\n");
    printf("mv - move (change name) file\n");
    printf("cat - concatenate, display the contents of a file\n");
    printf("history - history command\n");
    printf("logname or whoami - display user name\n");
    printf("exit - exit\n");
}

void pwd_cmd() {
	printf("%s\n",path);
}

void cd_cmd() {
	if(tokens_number > 2) {
		printf("\033[1;31m");
		printf("bash: %s: too many arguments\n",command);
		printf("\033[0m");
	}

	else if(tokens_number == 2) {
        if(strcmp(tokens[1], "-") == 0){
            if(chdir(path2) == -1) {
			    perror("error cd command");
		    }

		    else {
                printf("\033[1;34m");
                printf("%s\n",path2);
                printf("\033[0m");

                memset(path2, '\0', sizeof(path2));

                for(i = 0; i < strlen(path); i++) {
                    path2[i] = path[i];
                }

		    	getcwd(path,sizeof path);
		    }
        }

		else if(strcmp(tokens[1], "~") == 0) {
			if(chdir(getenv("HOME")) == -1) {
				perror("error cd command");
			}

			else {
				memset(path2, '\0', sizeof(path2));

				for(i=0; i<strlen(path); i++) {
					path2[i] = path[i];
				}
				getcwd(path, sizeof path);
			}

		}

		else if(chdir(tokens[1]) == -1) {
			perror("error cd command");
		}

		else {
            memset(path2, '\0', sizeof(path2));

            for(i = 0; i < strlen(path); i++) {
                path2[i] = path[i];
            }

			getcwd(path,sizeof path);
		}
	}

	else {
		if(chdir(getenv("HOME")) == -1) {
			perror("error cd command");
		}

		else {
            memset(path2, '\0', sizeof(path2));

            for(i = 0; i < strlen(path); i++) {
                path2[i] = path[i];
            }

            getcwd(path,sizeof path);
		}
	}
}

void cp_mv_cmd() {
	if(tokens_number == 3) {
		fd_file = open(tokens[1], O_RDONLY);

        if(fd_file != -1) {
        	fd_new_file = open(tokens[2], O_WRONLY|O_CREAT|O_TRUNC,0666);

            if(fd_new_file != -1) {
                while((num = read(fd_file, &buffer_text, BUFFER_SIZE)) > 0) {
                    write(fd_new_file, &buffer_text, num);
                }

                close(fd_file);
                close(fd_new_file);

				if(strcmp(command, "mv") == 0) {
					remove(tokens[1]);
				}
            }

            else {
                perror("error creating file");
            }
        }

        else {
			printf("\033[1;31m");
			printf("%s: cannot stat '%s': No such file or directory\n", command, tokens[1]);
			printf("\033[0m");
        }
    }

	else if(tokens_number > 3) {
		printf("\033[1;31m");
		printf("%s: target '%s' is not a directory\n",command, tokens[2]);
		printf("\033[0m");
	}

    else if(tokens_number == 2) {
		printf("\033[1;31m");
       	printf("%s: missing destination file operand after '%s'\n",command,tokens[1]);
		printf("\033[0m");
    }

    else {
		printf("\033[1;31m");
		printf("%s: missing file operand\n",command);
		printf("\033[0m");
    }
}

void cat_cmd() {
	if(tokens_number == 1) {
		printf("\033[1;31m");
		printf("%s: missing file operand\n", command);
		printf("\033[0m");
	}

	else {
		for(i = 1; i < tokens_number; i++) {
			memset(buffer_text, '\0', sizeof(buffer_text));
			fd_file = open(tokens[i], O_RDONLY);

			if(fd_file == -1) {
				printf("\033[1;31m");
            	printf("error open file: %s\n",tokens[i]);
            	printf("\033[0m");
			}

			else {
				read(fd_file, buffer_text, BUFFER_SIZE);
				printf("%s\n", buffer_text);
			}

			close(fd_file);
		}
	}
}

void history_cmd() {
	if(tokens_number > 2) {
		printf("\033[1;31m");
		printf("bash: %s: too many arguments\n", command);
		printf("\033[0m");
	}

	else if(tokens_number == 1) {
		for(i = 1; i < history_number; i++) {
			printf("%d  %s", i, history[i]);
		}
	}

	else if(tokens_number == 2) {
		char *chck_ascii = tokens[1];
		int history_error = 0;

		while(*chck_ascii != '\0') {
			if(*chck_ascii <= 48 || *chck_ascii >= 57) {
				printf("\033[1;31m");
				printf("bash: %s: %s: numberic argument required\n", command, tokens[1]);
				printf("\033[0m");
				hisotry_error = 1;
				break;
			}
			chck_ascii++;
		}

        if(history_error != 1) {
            int i_tokens = atoi(tokens[1]);
            int i_history = history_number - i_tokens;

            if(i_history <= 0) {
                i_history = 1;
            }

            for(i = i_history; i < history_number; i++) {
                printf("%d %s", i, history[i]);
            }
        }
	}
}

void login_cmd() {
	if(tokens_number > 1) {
		printf("\033[1;31m");
		printf("%s: extra operand '%s'", command, tokens[1]);
		printf("\033[0m");
	}

	else {
    	printf("\033[1;34m");
		printf("%s\n",getenv("USER"));
    	printf("\033[0m");
	}
}

void exit_cmd() {
	exit(EXIT_SUCCESS);
}

void exec_cmd(char *arr_tokens[]) {
	int pid = fork();

	if(pid != 0) {
		wait(NULL);
	}

	else {
		execvp(arr_tokens[0], arr_tokens);
		perror("bash error");
		exit(EXIT_SUCCESS);
	}
}

int main() {
	getcwd(path,sizeof path);

  	while(TRUE) {
		memset(command, '\0', sizeof(command));
		memset(buffer, '\0', sizeof(buffer));
		memset(arguments, '\0', sizeof(arguments));
        for(i = 0; i < tokens_number; i++) {
			memset(tokens[i], '\0', sizeof(tokens[i]));
		}

		printf("[");
		printf("\033[1;34m");
		printf("%s",getenv("USER"));
		printf("\033[0m");
		printf(":");
		printf("\033[1;32m");
       	printf("%s",path);
		printf("\033[0m");
		printf("]\n$ ");

		fgets(buffer, sizeof buffer, stdin);

		for(i = 0; i < strlen(buffer); i++) {
			if(buffer[i] == '"') {
				buffer[i] = ' ';
				i++;

				while(buffer[i] != '"' && i < strlen(buffer)) {
					if(buffer[i] == ' ') {
						buffer[i] = '_';
					}
					i++;
				}
				buffer[i] = ' ';
			}
		}

		char_group_tokens(buffer);

    	if(strcmp(command, "help") == 0) {
        	help_cmd_pl();
        	printf("\n\n\n\n");
        	help_cmd_en();
        }

		else if(strcmp(command, "pwd") == 0) {
        	pwd_cmd();
		}

		else if(strcmp(command, "cd") == 0) {
			cd_cmd();
		}

        else if(strcmp(command, "cp") == 0) {
            cp_mv_cmd();
        }

		else if(strcmp(command, "mv") == 0) {
			cp_mv_cmd();
		}

		else if(strcmp(command, "cat") == 0) {
			cat_cmd();
		}

		else if(strcmp(command, "history") == 0) {
			history_cmd();
		}

		else if( (strcmp(command, "logname") == 0) || (strcmp(command, "whoami") == 0) ) {
			login_cmd();
		}

		else if(strcmp(command, "exit") == 0) {
			exit_cmd();
		}

		else {
			exec_cmd(arr_tokens);
		}
  	}

	return 0;
}
