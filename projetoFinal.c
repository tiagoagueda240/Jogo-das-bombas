/* Desenvolvido por: Tiago Águeda - a22001757 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "messages.h"
#define length 24 * 24

/* estrutura com as infos de cada bomba */
typedef struct Bomba {
  int coordx;
  int coordy;
  int temp;
}Bomba;

/* estrutura de cada Node */
typedef struct No {
  Bomba * bomba;
  struct No * prox;

}No;

/* estrutura da Lista */
typedef struct Lista {
  No * ini;

}Lista;


/* Função que aloca memoria para a Lista */
Lista * newLista() {
  Lista * aux = (Lista * ) malloc(sizeof(Lista));
  aux -> ini = NULL;
  return aux;
}

/* Função que cria um Node */
No * newNo(int coordx, int coordy, int temp) {
  No * aux = (No * ) malloc(sizeof(No));
  Bomba * bomb = (Bomba * ) malloc(sizeof(Bomba));
  bomb -> coordx = coordx;
  bomb -> coordy = coordy;
  bomb -> temp = temp;
  aux -> bomba = bomb;
  aux -> prox = NULL;
  return aux;
}

/* Função que verifica se a head está a Null */
int isEmpty(Lista * li) {
  return li -> ini == NULL;
}


/* Adiciona o Node na lista */
void addFim(Lista * li, int coordx, int coordy, int temp) {
  No * no = newNo(coordx, coordy, temp);
  if (isEmpty(li)) { /* Verifica se a head está a Null */
    li -> ini = no;
  } else {
    No * aux = li -> ini;
    /*while utilizado para ir percorrendo tendo em conta o tempo das bombas*/
    while (aux -> prox != NULL && aux -> prox -> bomba -> temp < no -> bomba -> temp) {
      aux = aux -> prox;
    }
    if (aux -> prox != NULL) {
      no -> prox = aux -> prox;
    }
    aux -> prox = no;
  }
}


/*Verifica se a bomba já existe na lista*/
int jaExiste(int x, int y, Lista * li) {
  No * aux = li -> ini;
  while (aux != NULL) {
    if (aux -> bomba -> coordx == x && aux -> bomba -> coordy == y) { /* verifica se já existem as coordenadas na lista*/
      return 1;
    }

    aux = aux -> prox;
  }

  return 0;
}

/*Função criada para evitar a criação de vários ifs semelhantes, serve para adicionar as bombas a lista*/
void colocaBomba(char ** mapa, int xTeclado, int yTeclado, int numeroLinhas, int numeroColunas, Lista * li, int tempo) {
  /*Verifica se o ponto passado nos parametros está dentro dos limites da Matriz e se é uma bomba*/
  if (xTeclado >= 0 && yTeclado >= 0 && xTeclado < numeroLinhas && yTeclado < numeroColunas && mapa[xTeclado][yTeclado] == '.') {
    /*Se for uma bomba verifica se essa bomba já foi adicionado à lista*/
    if (!jaExiste(xTeclado, yTeclado, li)) {
      /* Se não está na lista, adiciona o novo ponto à lista*/
      addFim(li, xTeclado, yTeclado, tempo);
    }
  }
}

/*Função que verifica as bombas à volta*/
void ler_dados(char ** mapa, int xTeclado, int yTeclado, int numeroLinhas, int numeroColunas, Lista * li, int tempo) {

  /*Chama 8 vezes a função que verifica se existe bombas à volta*/
  colocaBomba(mapa, xTeclado - 1, yTeclado, numeroLinhas, numeroColunas, li, tempo + 10);
  colocaBomba(mapa, xTeclado - 1, yTeclado - 1, numeroLinhas, numeroColunas, li, tempo + 11);
  colocaBomba(mapa, xTeclado, yTeclado - 1, numeroLinhas, numeroColunas, li, tempo + 12);
  colocaBomba(mapa, xTeclado + 1, yTeclado - 1, numeroLinhas, numeroColunas, li, tempo + 13);
  colocaBomba(mapa, xTeclado + 1, yTeclado, numeroLinhas, numeroColunas, li, tempo + 14);
  colocaBomba(mapa, xTeclado + 1, yTeclado + 1, numeroLinhas, numeroColunas, li, tempo + 15);
  colocaBomba(mapa, xTeclado, yTeclado + 1, numeroLinhas, numeroColunas, li, tempo + 16);
  colocaBomba(mapa, xTeclado - 1, yTeclado + 1, numeroLinhas, numeroColunas, li, tempo + 17);

}

/*Faz print ao tempo e coordenadas no log*/
void print(Lista * li, char ** mapa) {
  No * aux = li -> ini;

  while (li -> ini != NULL) {
    aux = li -> ini;
	mapa[aux -> bomba -> coordx][aux -> bomba -> coordy] = '*';
    printf("%d[%d, %d]\n", li -> ini -> bomba -> temp, li -> ini -> bomba -> coordx, li -> ini -> bomba -> coordy);
    li -> ini = aux -> prox;
    /*Apaga o espaço de memoria allocado para a lista*/
    free(aux->bomba);
    free(aux);
  }
}


/*Mostra o menu inicial*/
void menu() {
  puts(MSG_TITLE);
  puts("show                - show the mine map");
  puts("propagate<x><y>   - explode bomb at<x><y>");
  puts("log<x> <y>	      - explode bomb at<x><y>");
  puts("plant<x><y>       - place bomb at<x><y>");
  puts("export<filename>   - save file with current map");
  puts("quit                - exit program");
  puts("sos                 - show menu");
  puts(MSG_ENDTITLE);
}

/*Mostra o mapa*/
void mostrarMapa(char ** mapa, int linha, int coluna) {
  int i = 0, f = 0;
  for (i = 0; i < linha; i++) {
    for (f = 0; f < coluna; f++) {
      printf("%c", mapa[i][f]);
    }

    printf("\n");
  }
}

/* Coloca a matriz só com '_' */
void limparMatriz(char ** mapa, int linha, int coluna) {
  int i = 0, f = 0;
  for (i = 0; i < linha; i++) {
    for (f = 0; f < coluna; f++) {
      mapa[i][f] = '_';
    }
  }
}

/* Guarda as coordenadas das bombas num ficheiro*/
void exportar(char ** mapa, char * nomeFicheiro, int numeroLinhas, int numeroColunas) {
  int linha = 0, coluna = 0;
  FILE * f_escrita = NULL;
  f_escrita = fopen(nomeFicheiro, "w");
  fprintf(f_escrita, "%d %d\n", numeroLinhas, numeroColunas);/*Exporta o numero de colunas e linhas*/
  for (linha = 0; linha < numeroLinhas; linha++) {
    for (coluna = 0; coluna < numeroColunas; coluna++) {
      if (mapa[linha][coluna] == '.' || mapa[linha][coluna] == '*') {
        fprintf(f_escrita, "%c %i %i\n", mapa[linha][coluna], linha, coluna);/*Exporta as coordenadas de todas as bombas*/
      }
    }
  }

  fclose(f_escrita);
}

int main(int argc, char ** agrv) { /*Parâmetros utilizados para se ler da linha de comandos*/
  FILE * f_leitura = NULL;
  Lista * li;
  No * aux;
  int numeroLinhas = 1;
  int numeroColunas = 1, i = 0;
  char ** mapa;
  int linha, coluna, linhaTeclado, colunaTeclado, verifica = 0, tempo = 0;
  char opcao[80], nomeFicheiro[length], mapaLinha[300], caracter;

  if (argc < 2) {
    puts(MSG_FNAME);
    exit(0);
  }

  f_leitura = fopen(agrv[1], "r");

  if (f_leitura == NULL) {
    puts(MSG_FILE_OPN);
    exit(0);
  } else {
    while (!feof(f_leitura)) { /*Até chegar À ultima linha do ficheiro*/
      fgets(mapaLinha, 200, f_leitura);
      if (mapaLinha[0] == '\n' || mapaLinha[0] == '\r' || mapaLinha[0] == '#') {/*Verifica se é uma linha vazia ou comentário #*/
        continue;
      }

      if (!verifica) {/*Entra apenas uma vez*/
        if (sscanf(mapaLinha, "%i %i", & numeroLinhas, & numeroColunas) == 2) { /*Verifica se a linha tem 2 valores inteiros*/
          /*Aloca memória para a Matriz*/
          mapa = (char ** ) malloc(numeroLinhas * sizeof(char * ));
          for (i = 0; i < numeroLinhas; i++) {
            mapa[i] = (char * ) malloc(numeroColunas * sizeof(char));
          }
          /*Limpa a matriz*/
          limparMatriz(mapa, numeroLinhas, numeroColunas);
          verifica = 1;
        } else {
          puts(MSG_FILE_CRP);
          exit(0);
        }
      } else if (sscanf(mapaLinha, "%c %i %i", & caracter, & linha, & coluna) == 3) { /*Se a tem uma char e 2 ints*/
        /*Verifica se o char é igual a * ou . */
        if ((caracter == '*' || caracter == '.') && linha >= 0 && linha < numeroLinhas && coluna >= 0 && coluna < numeroColunas) {
          mapa[linha][coluna] = caracter;/* coloca o char na posição correta da matriz*/
        } else {
          printf(MSG_FILE_CRP "\n");
          exit(0);
        }
      } else {
        printf(MSG_FILE_CRP "\n");
         exit(0);
      }
    }
  }

  fclose(f_leitura);/*Termina a leitura*/

  menu();
  do {
    printf(MSG_PROMPT);
    scanf("%s", opcao);

    if (!strcmp(opcao, "show")) {
      mostrarMapa(mapa, numeroLinhas, numeroColunas);
    } else if (!strcmp(opcao, "propagate")) {
      scanf("%i %i", & linhaTeclado, & colunaTeclado);
      /*Verifica se a bomba está dentro dos limites da matriz*/
      if (linhaTeclado < 0 || colunaTeclado < 0 || linhaTeclado >= numeroLinhas || colunaTeclado >= numeroColunas) {
        puts(MSG_INVAL_CRD);
        /*Verifica se existe uma bomba nas coordenadas passadas*/
      } else if (mapa[linhaTeclado][colunaTeclado] != '.' && mapa[linhaTeclado][colunaTeclado] != '*') {
        puts(MSG_NOBOMB);
        continue;
      } else {
        li = newLista(); /*Alloca memoria para a lista*/
        addFim(li, linhaTeclado, colunaTeclado, tempo); /*Adiciona a bomba passada na linha de comandos*/
        ler_dados(mapa, linhaTeclado, colunaTeclado, numeroLinhas, numeroColunas, li, tempo);/*Verifica as bombas a volta da bomba passada na linha de comandos*/
        aux = li -> ini;
        /*Vai verificando e adicionando todas as bombas*/
        while (aux != NULL) {
          ler_dados(mapa, aux -> bomba -> coordx, aux -> bomba -> coordy, numeroLinhas, numeroColunas, li, aux -> bomba -> temp);
          tempo = aux -> bomba -> temp;
          aux = aux -> prox;
        }
        aux = li -> ini;
        /*Altera as bombas de . para * e vai fazendo print do mapa*/
        while (aux != NULL) {
          mapa[aux -> bomba -> coordx][aux -> bomba -> coordy] = '*';
          mostrarMapa(mapa, numeroLinhas, numeroColunas);
          aux = aux -> prox;
        }

/*Apaga o espaço de memoria allocado para a lista*/
        while (li -> ini != NULL) {
          aux = li -> ini;
          li -> ini = aux -> prox;
          free(aux->bomba);
          free(aux);
        }

      }
    } else if (!strcmp(opcao, "log")) {
      scanf("%i %i", & linhaTeclado, & colunaTeclado);
      /*Verifica se a bomba está dentro dos limites da matriz*/
      if (linhaTeclado < 0 || colunaTeclado < 0 || linhaTeclado >= numeroLinhas || colunaTeclado >= numeroColunas) {
        puts(MSG_INVAL_CRD);
        /*Verifica se existe uma bomba nas coordenadas passadas*/
      } else if (mapa[linhaTeclado][colunaTeclado] != '.' && mapa[linhaTeclado][colunaTeclado] != '*') {
        puts(MSG_NOBOMB);
        continue;
      } else {
        li = newLista();/*Alloca memoria para a lista*/
        addFim(li, linhaTeclado, colunaTeclado, tempo);/*Adiciona a bomba passada na linha de comandos*/
        ler_dados(mapa, linhaTeclado, colunaTeclado, numeroLinhas, numeroColunas, li, tempo);/*Verifica as bombas a volta da bomba passada na linha de comandos*/
        aux = li -> ini;
        /*Vai verificando e adicionando todas as bombas*/
        while (aux != NULL) {
          ler_dados(mapa, aux -> bomba -> coordx, aux -> bomba -> coordy, numeroLinhas, numeroColunas, li, aux -> bomba -> temp);
          tempo = aux -> bomba -> temp;
          aux = aux -> prox;
        }
        /*Chama a função que faz print da lista de bombas*/
        print(li, mapa);
      }
    } else if (!strcmp(opcao, "plant")) {
      scanf("%i %i", & linhaTeclado, & colunaTeclado);
      if (linhaTeclado > numeroLinhas || colunaTeclado > numeroColunas || linhaTeclado < 0 || colunaTeclado < 0) {
        puts(MSG_INVAL_CRD);
        continue;
      } else {
        /* Adiciona a bomba nas coordenadas passadas */
        mapa[linhaTeclado][colunaTeclado] = '.';
      }
    } else if (!strcmp(opcao, "sos")) {
      menu();
    } else if (!strcmp(opcao, "export")) {
      scanf("%s", nomeFicheiro);
      /*Chama a função que esporta as coordenadas das bombas*/
      exportar(mapa, nomeFicheiro, numeroLinhas, numeroColunas);

    } else if (!strcmp(opcao, "quit")) {
      exit(0);
    } else {
      puts(MSG_INVAL_CMD);
      continue;
    }
  } while (strcmp(opcao, "quit"));

  exit(0);
}