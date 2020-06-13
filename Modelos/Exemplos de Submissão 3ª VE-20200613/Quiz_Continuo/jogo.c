/*
TRABALHO (VE 3)
Grupo: Maj Luís, Maj Janilma, Maj Draeger e Cap Girardi
Sistema operacional: Windows
Programa em C que implementa um jogo contínuo de perguntas e respostas 
*/

////////////////////////INCLUSAO DE BIBLIOTECAS////////////////////////
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

////////////////////////DEFINICAO DE CONSTANTES////////////////////////
#define NOME_ARQUIVO_ENTRADA "entrada.txt" 
#define NOME_ARQUIVO_SAIDA "saida.txt" 
#define MAX_OPCOES 10
#define N_QUESTOES 2

//////////////////////////DEFINICAO DE ENUMS//////////////////////////
typedef enum _tipo_no {
	raiz, nao_terminal, terminal
} tipo_no;

/////////////////////////DEFINICAO DE STRUCTS/////////////////////////
typedef struct _opcao {
	char opcao_selecionada;
	int indice_proximo_no;
} opcao;

typedef struct _no {
	int indice;
	char texto[501];
	tipo_no tipo;
	int n_opcoes;
	opcao opcoes[MAX_OPCOES];
	struct _no *ant;
	struct _no *prox;
} no;

/////////////////////////VARIAVEIS GLOBAIS//////////////////////////
//Referencia para o inicio e para a posicao atual na lista encadeada
no *ptr_inicio, *ptr_atual;
//Referencia para o arquivo de saida
FILE *arquivo_saida;
//Variaveis globais
int n_acertos = 0;

////////////////////////////PROTOTIPOS//////////////////////////////
char *ler_nome_jogador(char *nome_arquivo);
void cadastrar_no(int indice, char *texto, tipo_no tipo, int n_opcoes, opcao *opcoes);
void cadastrar_nos();
no *buscar_no(int indice);
int ler_indice_proximo_no(char opcao);
void atualizar_acertos(int indice, char opcao);
void imprimir_resultado();
void apagar_lista();

///////////////////////////////MAIN///////////////////////////////
int main() { 
	
	//Abertura do arquivo de saida
	if (!(arquivo_saida = fopen(NOME_ARQUIVO_SAIDA, "w"))) {
		printf("ERRO AO ABRIR O ARQUIVO DE SAIDA!");
		exit(1); 
	}
	
	//Mensagem de boas-vindas
	char nome_jogador[100];
	strcpy(nome_jogador, ler_nome_jogador(NOME_ARQUIVO_ENTRADA));
	printf("Bem-vindo ao QUIZ do QEM, %s!\n", nome_jogador);
	fprintf(arquivo_saida, "Bem-vindo ao QUIZ do QEM, %s!\n", nome_jogador);
	
	//Montagem dos nos do jogo em uma lista encadeada
	cadastrar_nos();
	
	//Carregar no raiz
	ptr_atual = buscar_no(0);

	//laco de controle do jogo
	while(1) {
		//Se no nao eh terminal, apresentar texto e ler a opcao selecionada
		if(ptr_atual->tipo != terminal) {
			int indice_proximo_no = -1;
			char opcao;
			while(indice_proximo_no == -1) {
				printf("%s", ptr_atual->texto);
				fprintf(arquivo_saida, "%s", ptr_atual->texto);
				scanf(" %c", &opcao);
				fprintf(arquivo_saida, "%c\n", opcao);

				//Ler proximo no a partir da opcao
				indice_proximo_no = ler_indice_proximo_no(opcao);
				if(indice_proximo_no == -1) {
					printf("OPCAO INVALIDA!\n");
					fprintf(arquivo_saida, "OPCAO INVALIDA!\n");
				}
			}
			//Atualizar acertos, ler proximo no e atualizar posicao atual
			atualizar_acertos(ptr_atual->indice, opcao);
			no *ptr_proximo_no = buscar_no(indice_proximo_no);
			ptr_atual = ptr_proximo_no;
		}
		//Se no eh terminal, imprimir resultado e finalizar programa
		else {
			imprimir_resultado();
			break;
		}
	}
	
	//Apagar toda a lista e liberar espaco de memoria alocado
	apagar_lista();
	
	//Fechar o arquivo de saida
	fclose(arquivo_saida);
}

////////////////////////OUTRAS FUNCOES////////////////////////
//Funcao que le o nome do jogador a partir de um arquivo texto
char *ler_nome_jogador(char *nome_arquivo) {
	FILE *arquivo_entrada;
	char *nome = NULL, buffer[100];
	//Abrir o arquivo texto para leitura
	arquivo_entrada = fopen(nome_arquivo, "r");
	if (arquivo_entrada == NULL) {
		printf("\nERRO AO ABRIR O ARQUIVO DE ENTRADA!");
		fprintf(arquivo_saida, "\nERRO AO ABRIR O ARQUIVO DE ENTRADA!");
		exit(1); 
	}
	//Ler nome do jogador no arquivo
	if(!feof(arquivo_entrada)) {
		nome = fgets(buffer, 100, arquivo_entrada);
	}
	//Fechar o arquivo de entrada
	fclose(arquivo_entrada);
	//Retornar o nome lido
	return nome;
}

//Funcao que cadastra um novo no na lista encadeada
void cadastrar_no(int indice, char *texto, tipo_no tipo, int n_opcoes, opcao *opcoes) {
	//Alocar memoria para o novo no
	no *ptr = (no *) malloc(sizeof(no));
	if(ptr == NULL) {
		printf("\nERRO NA ALOCACAO DE MEMORIA!");
		fprintf(arquivo_saida, "\nERRO NA ALOCACAO DE MEMORIA!");
		return;
	}
	//Inicializar valores do novo no
	ptr->indice = indice;
	strcpy(ptr->texto, texto);
	ptr->tipo = tipo;
	ptr->n_opcoes = n_opcoes;
	for(int i = 0; i < n_opcoes; i++)
		ptr->opcoes[i] = opcoes[i];
	ptr->ant = NULL;
	ptr->prox = NULL;
	//Encadear novo no na lista
	no *ptr_aux = ptr_inicio;
	if(ptr_aux != NULL) { //Lista não-vazia
		while(ptr_aux->prox != NULL)
			ptr_aux = ptr_aux->prox;
		ptr_aux->prox = ptr;
		ptr->ant = ptr_aux;
	}
	else //Lista vazia
		ptr_inicio = ptr;
}

//Funcao que cadastra todos os nos da lista encadeada (carregamento da lista)
void cadastrar_nos() {
	opcao opcoes_0[5] = {{'A', 1}, {'B', 1}, {'C', 1}, {'D', 1}, {'E', 1}};
	cadastrar_no(0, 
		"\nPERGUNTA 1\nQuem eh o patrono do QEM?\nA - Caxias\nB - Ricardo Franco\nC - Sampaio\nD - Osorio\nE - Mallet\n\nOpcao escolhida: ", 
		raiz, 5, opcoes_0);
	opcao opcoes_1[5] = {{'A', 2}, {'B', 2}, {'C', 2}, {'D', 2}, {'E', 2}};
	cadastrar_no(1, 
		"\nPERGUNTA 2\nEm que dia eh comemorado o dia do QEM?\nA - 19 de abril\nB - 24 de maio\nC - 10 de junho\nD - 3 de agosto\nE - 10 de maio\n\nOpcao escolhida: ",
		nao_terminal, 5, opcoes_1);
	cadastrar_no(2, "", terminal, 0, NULL);
}

//Funcao que busca um no a partir do seu indice
no *buscar_no(int indice) {
	no *ptr_aux = ptr_inicio;
	if(ptr_aux == NULL)
		return NULL;
	else {
		while(ptr_aux != NULL) {
			if(ptr_aux->indice == indice)
				return ptr_aux;
			ptr_aux = ptr_aux->prox;
		}
		return NULL;		
	}
}

//Funcao que le o indice do proximo no da lista a partir de uma opcao selecionada no no atual
int ler_indice_proximo_no(char opcao) {
	if(ptr_atual->opcoes[0].opcao_selecionada == '*')
		return ptr_atual->opcoes[0].indice_proximo_no;
	else {
		for(int i = 0; i < ptr_atual->n_opcoes; i++) {
			if(ptr_atual->opcoes[i].opcao_selecionada == opcao)
				return ptr_atual->opcoes[i].indice_proximo_no;
		}
		return -1;
	}
}

//Funcao que atualiza o numero de acertos do jogador, caso necessario
void atualizar_acertos(int indice, char opcao) {
	if((indice == 0 && opcao == 'B') ||
		(indice == 1 && opcao == 'D'))
		n_acertos++;	
}

//Funcao que imprime o resultado do jogador
void imprimir_resultado() {
	printf("\nRESULTADO\nVoce teve %.2f%% de aproveitamento (%d acerto(s) em %d questoes).", n_acertos * 100. / N_QUESTOES, n_acertos, N_QUESTOES);
	fprintf(arquivo_saida, "\nRESULTADO\nVoce teve %.2f%% de aproveitamento (%d acerto(s) em %d questoes).", n_acertos * 100. / N_QUESTOES, n_acertos, N_QUESTOES);
}

//Funcao que apaga toda a lista, liberando os espacos de memorias alocados
void apagar_lista() {
	no *ptr_aux = ptr_inicio;
	if(ptr_aux != NULL) {
		while(ptr_aux->prox != NULL) {
			if(ptr_aux->ant != NULL) {
				free(ptr_aux->ant);
			}
			ptr_aux = ptr_aux->prox;
		}
		if(ptr_aux->ant != NULL) {
			free(ptr_aux->ant);
		}
		free(ptr_aux);
	}
	ptr_inicio = NULL;
}
