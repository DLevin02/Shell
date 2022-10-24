#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

char *direction[100];
void respond_problem(){
  char problem[30] = "An error has occurred\n";
  write(STDERR_FILENO, problem, strlen(problem)); 
}

int main_assist(char *commands, char *args[]){
  char path_find[100];
  int randInt = 0;
  int end = 1;
  if (strcmp(args[0], "cd") == 0){
    int point = 0;
    while (args[point] != NULL){
      point++;
    }
    if (point != 2){ 
      respond_problem();
      return -1;
    }else{
      int whatReturn = chdir(args[1]);
      if (whatReturn == 0){ 
        return 1;
      }else{
        respond_problem();
        return -1;
      }
    }
  }
  while (direction[randInt] != NULL){
    strcpy(path_find, direction[randInt]);
    strcat(path_find, "/");
    strcat(path_find, commands);

    if (access(path_find,X_OK) != 0){
      randInt++;
      continue;
    } 
    int track = fork();
    if (track == 0){
      execv(path_find, args);
      respond_problem();
    }else{
      int status;
      waitpid(track, &status, 0);
      end = WEXITSTATUS(status);
    }
    randInt++;
  }
  return end;
}
int when_help_cmd(char *args[]){
  char place1[100], place2[100], comparator[100], returnPiece[100];
  char *arguments1[100];
  char *arguments2[100];
  int comparison = 0;

  int entice = 0;
  while (args[entice] != NULL){
    entice++;
  }

  if (entice == 6 && strcmp(args[4],"then") == 0 && strcmp(args[5],"fi") == 0){
    return -1;
  }
  
  int containC = -1;
  int happensIF = -1;
  int happensToHaveFi = -1;
  int containT = -1;

  int point = 0;
  while (args[point] != NULL){
    if (strcmp(args[point], "if") == 0 && happensIF == -1){
      happensIF = point;
    }
    if (strcmp(args[point], "fi") == 0 && happensToHaveFi == -1){
      happensToHaveFi = point;
    }
    if (strcmp(args[point], "then") == 0 && containT == -1){
      containT = point;
    }
    if ((strcmp(args[point], "==") == 0 || strcmp(args[point], "!=") == 0)){
      if (containT == -1){
        containC = point;
      }
    }
    point++;
  }
  if (happensIF == -1 || happensToHaveFi == -1 || containT == -1 || containC == -1){
    respond_problem();
    return -1;
  }

  if (entice < 7){
    respond_problem();
    return -1;
  }

  if(strcmp(args[0],"if") != 0){
    respond_problem();
    return -1;
  }
  if (happensIF != 0 || happensToHaveFi != entice-1){
    respond_problem();
    return -1;
  }
  int i = happensIF + 1;
  while(i< containC){
    arguments1[i-happensIF-1] = args[i];
    i++;
  }
  arguments1[i-happensIF-1] = NULL;

  i = containT + 1;
  while (i < happensToHaveFi){
    arguments2[i-containT-1] = args[i];
    i++;
  }
  
  arguments2[i-containT-1] = NULL;

  strcpy(place1, arguments1[0]);
  strcpy(place2, arguments2[0]);
  strcpy(comparator, args[containC]);

  if (strcmp(comparator, "==") == 0){
    comparison = 1;
  }
  strcpy(returnPiece, args[containC + 1]);

  int returnUNO = main_assist(place1,arguments1);

  if (comparison == 1){
    if (returnUNO == atoi(returnPiece)){
      main_assist(place2, arguments2);
    }
  }else{
    if (returnUNO != atoi(returnPiece)){
      main_assist(place2, arguments2);
    }
  }
  return 1;
}

int change_bash(char *point, char *newDir){
  FILE *filepath;
  char path_find[100];
  char *args[100];
  char *blank[100];
  char *endpoint;
  char *currentPoint;
  char *plain_cmd;
  char *next_point;
  plain_cmd = strtok(point, ">");
  int pointer = 0;
  while (plain_cmd != NULL){
    blank[pointer] = plain_cmd;
    plain_cmd = strtok(NULL, ">");
    pointer++;
  }
  blank[pointer] = NULL; 
  if (pointer != 2){
    respond_problem();
    return -1;
  }
  
  pointer = 0;
  currentPoint = blank[0];
  plain_cmd = strtok(currentPoint, " \t\r\a\n");
  while (plain_cmd != NULL){
    args[pointer] = plain_cmd;
    pointer++;
    plain_cmd = strtok(NULL, " \t\r\a\n");
  }
  args[pointer] = NULL;
  endpoint = blank[1];
  endpoint = strtok(endpoint, " \t\r\a\n");
  next_point = strtok(NULL, " \t\r\a\n");
  if (next_point != NULL){
    respond_problem();
    return -1;
  }
  int randInt = 0;
  while (direction[randInt] != NULL){
    int track = fork();
    if (track == 0){
      strcpy(path_find, direction[randInt]);
      strcat(path_find, "/");
      strcat(path_find, args[0]);

      filepath = fopen(endpoint, "w");
      int oldFilePath = fileno(filepath);
      dup2(oldFilePath, STDOUT_FILENO);
      execv(path_find, args);
      respond_problem();
    }else{
      int status;
      waitpid(track, &status, 0);
      int end = WEXITSTATUS(status);
      return end;
    }
    randInt++;
  }
  return 1;
}

int parse_input(char *args[], FILE *filepath){
  char *domino = malloc(100 * sizeof(char));
  char *currentPoint;
  size_t hence_s;
  int parse_a;
  char *change_c = NULL;

  parse_a = getline(&domino, &hence_s, filepath);
  if (parse_a == -1){
    return -1;
  }
  int domino_l = strlen(domino);
  if (domino_l == 0 || strcmp(domino, "") == 0 || strcmp(domino, "\n") == 0){
     return 1;
  }

  if ('\n' == domino[domino_l - 1]){
    domino[domino_l - 1] = '\0';
  }
  if (domino[0] == EOF){
    return -1;
  }

  change_c = strchr(domino, '>');
  if (change_c != NULL){
    change_bash(domino, change_c);
    return 1; 
  }
  int point = 0;
  currentPoint = strtok(domino, " \t\r\a\n");
  while (currentPoint != NULL){
    args[point] = strdup(currentPoint);
    point++;
    currentPoint = strtok(NULL, " \t\r\a\n");
  }
  args[point] = NULL; 
  if (point == 0) {
    return 1;
  }

  if (strcmp(args[0], "exit") == 0){
    if (args[1] != NULL){
      respond_problem();
      return -1;
    }
    exit(0);
 }else if (strcmp(args[0], "path") == 0){
    int point = 0;
    for(;;){
      direction[point] = args[point+1];
      point++;
      if (args[point + 1] == NULL){
        break;
      }
    }
    int holder = 0;
    while (direction[holder] != NULL){
      holder++;
    }
    return 1;
  }else if (strcmp(args[0], "if") == 0){
    return when_help_cmd(args);
  }else if (strcmp(args[0], "cd") == 0){
    if (point != 2){ 
      respond_problem();
      return -1;
    }else{
      int testValue = chdir(args[1]);
      if (testValue == 0){
        return 1;
      }else{
        respond_problem();
        return -1;
      }
    }
    }else if(strcmp(args[0],"bad") == 0){
    respond_problem();
    return -1;
  }
  return 0;
}

int command_assist(char *args[]){
  char path_find[300];
  if (direction[0] == NULL || args == NULL || args[0] == NULL){
    respond_problem();
    return -1;
  }
  int randInt = 0;
  while (direction[randInt] != NULL){
    strcpy(path_find, direction[randInt]);
    strcat(path_find, "/");
    strcat(path_find, args[0]);
    randInt++;

    if (access(path_find,X_OK) != 0 && direction[1] == NULL){
      respond_problem();
      return -1;
    }

    if (access(path_find,X_OK) != 0){
      randInt++;
      continue;
    }

    int track = fork();
    if (track == 0){
      execv(path_find, args);
      respond_problem();
    }else{
      int holdPlace;
      waitpid(track, &holdPlace, 0);
    }
    randInt++;
  }
  return 0;
}

int main(int argc, char *argv[]){
  int mode_b = 0;
  FILE *filepath;
  char **aar;
  int finish_b = 0;
  direction[0] = "/bin";
  char *pwd;


  if (argc == 2){
    pwd = argv[1];
    filepath = fopen(pwd, "r");
    if (!(filepath)){
      respond_problem();
      exit(1);
    }
    mode_b = 1;
  }
  
  if (argc > 2){
    respond_problem();
    exit(1);
  } 
  while (1){
    if (mode_b == 1){
      for(;;){
        aar = malloc(sizeof(char*) * 100);
        for(int i = 0; i < 100; i++){
          aar[i] = malloc(sizeof(char) * 256);
        }
        int tranverse_c = parse_input(aar, filepath);
        if (tranverse_c == -1){
          finish_b = 1;
          for(int i = 0; i < 100; i++){
            free(aar[i]);
          }
          free(aar);
          break;
        }else if (tranverse_c == 1){
          continue;
        }else{ 
          int run_bin_cmd = command_assist(aar);
          if (run_bin_cmd == -1){
            continue;
          }
        }
        for(int i = 0; i < 100; i++){
          free(aar[i]);
        }
        free(aar);
      }
      break;
    }else{                   
      printf("wish> "); 
      aar = malloc(sizeof(char*) * 100);
      for(int i = 0; i < 100; i++){
        aar[i] = malloc(sizeof(char) * 256);
      }
      int tranverse_c = parse_input(aar, stdin);
      if (tranverse_c == -1){
        for(int i = 0; i < 100; i++){
          free(aar[i]);
        }
        free(aar);
        break;
      }else if (tranverse_c == 1){
        continue;
      }else{ 
        int b_r_c = command_assist(aar);
        if (b_r_c == -1){ 
          continue;
        }
      }
      for(int i = 0; i < 100; i++){
        free(aar[i]);
      }
      free(aar);
    }
    if(finish_b == 1){
      break;
    }
  }
  return 0;
}
