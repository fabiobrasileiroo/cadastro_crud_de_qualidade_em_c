#include <stdio.h>
#include <unistd.h>

int main(int argc, char** argv) {
  if(access("arquivo.txt", F_OK)) {
    printf("O arquivo nao existe\n");
    return 1;
  }
  const int TAM = 100;
  FILE *file = fopen("arquivo.txt", "r");
  char conteudo[TAM];
  if(fgets(conteudo, TAM, file)) {
    printf("%s", conteudo);
  }
  printf("%s\n", conteudo);
  fclose(file);
  return 0;
}