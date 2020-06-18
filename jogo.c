/*
TRABALHO (VE 3)
Grupo: Al. Burin, Al. Libni, Al. Meneses, Al. Zamperlini
Sistema operacional: Windows
Programa em C que implementa um jogo de labirinto
*/

////////////////////////INCLUSAO DE BIBLIOTECAS////////////////////////
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<string.h>
#include<locale.h>

////////////////////////DEFINICAO DE CONSTANTES////////////////////////
#define NOME_ARQUIVO_ENTRADA "entrada.txt"
#define NOME_ARQUIVO_SAIDA "saida.txt"
#define MAX_OPCOES 10

//////////////////////////DEFINICAO DE ENUMS//////////////////////////
typedef enum _tipo_no {
	raiz, nao_terminal, reinicio, terminal
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
	int (*fun_ptr)();
} no;

typedef struct _pergunta {
    char enunciado[200];
    char resposta;
} pergunta;

/////////////////////////VARIAVEIS GLOBAIS//////////////////////////
//Referencia para o inicio e para a posicao atual na lista encadeada
no *ptr_inicio, *ptr_atual;
//Referencia para o arquivo de saida
FILE *arquivo_saida;
//Criterios globais
int fardamento[2];				/* {0,0} -> Paisano // {0,1} -> Farda sem gandola // {1,0} -> Farda sem gandola // {1,1} -> Fardamento completo */
int dias_punido = 0;			/* Contagem de dias punidos */
int elevador_alunos = 0;		/* 0 -> Não esperou // 1 -> Esperou */
enum comida {pizza = 0, bk = 1, salada = 2, feijao = 3, soja = 4, arroz_bife = 5}alimentacao;		/* Para uso na checagem da comida pedida pelo ifood ou no rancho */

////////////////////////////PROTOTIPOS//////////////////////////////
char *ler_nome_jogador(char *nome_arquivo);
void cadastrar_no(int indice, char *texto, tipo_no tipo, int n_opcoes, opcao *opcoes, int (*fun_ptr)());
void cadastrar_nos();
no *buscar_no(int indice);
int ler_indice_proximo_no(char opcao);
void apagar_lista();

/* Minigames */
int encontrar_oficial();
int encontrar_sargenteante();
int encontrar_general();
int gd_surpresa();
int encontrar_professor();

/* Funções auxiliares */
int aleatorio(int val_min, int val_max);		/* Função que gera um número aleatório entre 0 e o máximo dado */
int verificar_fardamento(int minigame);			/* Função que verifica o fardamento com base na posição, recebe o parametro minigame, com base no indice (0 - encontrar_sargenteante / 1 - encontrar_general ). Baseada na original verificar_criterio_acesso / Note que encontrar_oficial não necessita verificar fardamento, já que paisano, fardamento básico ou completo satisfazem as condições */
int torrado();			/* Função que incrementa a variável global dias_punido */


///////////////////////////////MAIN///////////////////////////////
int main() {
	
	srand(time(NULL));
	setlocale(LC_ALL, "Portuguese");
	
	//Abertura do arquivo de saida
	if (!(arquivo_saida = fopen(NOME_ARQUIVO_SAIDA, "w"))) {
		printf("ERRO AO ABRIR O ARQUIVO DE SAIDA!");
		exit(1);
	}

	//Mensagem de boas-vindas
	char nome_jogador[100];
	strcpy(nome_jogador, ler_nome_jogador(NOME_ARQUIVO_ENTRADA));
	printf("Bem-vindo ao LABIRINTO de GD, %s!\n", nome_jogador);
	fprintf(arquivo_saida, "Bem-vindo ao LABIRINTO de GD, %s!\n", nome_jogador);

	//Montagem dos nos do jogo em uma lista encadeada
	cadastrar_nos();

	//Carregar no raiz
	ptr_atual = buscar_no(0);

	//laco de controle do jogo
	while(1) {
		
		int var_saida = 1;
        if (ptr_atual->fun_ptr != NULL){
            var_saida = ptr_atual->fun_ptr();
		}
		
		//Se no nao eh terminal, apresentar texto e ler a opcao selecionada
		if(ptr_atual->tipo == nao_terminal && var_saida != 0 || ptr_atual->tipo == raiz && var_saida != 0) {
			int indice_proximo_no = -1;
			while(indice_proximo_no == -1) {
				char opcao;
				printf("%s", ptr_atual->texto);
				fprintf(arquivo_saida, "%s", ptr_atual->texto);
				if(ptr_atual->n_opcoes > 1){
					scanf(" %c", &opcao);
					fprintf(arquivo_saida, "%c\n", opcao);
				} else {
					opcao = '*';
				}

				//Ler proximo no a partir da opcao
				indice_proximo_no = ler_indice_proximo_no(opcao);
				no *ptr_proximo_no = buscar_no(indice_proximo_no);
				ptr_atual = ptr_proximo_no;
				if(indice_proximo_no == -1) {
					printf("OPCAO INVALIDA!\n");
					fprintf(arquivo_saida, "OPCAO INVALIDA!\n");
				}
			}
		}
		
		/* Se é nó de reinicio, o texto já vai ter sido mostrado e o número de dias torrado alterado, apenas deve-se trocar o endereço para o ínicio do jogo e mostrar a quantos dias punidos se está */ 
		else if (ptr_atual->tipo == reinicio || var_saida == 0){
			torrado();
			if(var_saida != 0){		/* Note que quando a saida é pela condição expressa no minigame, o texto deve ser feito pelo mesmo */
				printf("%s", ptr_atual->texto);
				fprintf(arquivo_saida, "%s", ptr_atual->texto);
			}
			printf("\n\nVoce esta a %d dias punido. \n", dias_punido);
			fprintf(arquivo_saida, "\n\nVoce está a %d dias punido. \n", dias_punido);
			ptr_atual = buscar_no(0);
		}
		
		//Se no eh terminal, apresentar texto e finalizar programa
		else if (ptr_atual->tipo == terminal){
			printf("%s", ptr_atual->texto);
			fprintf(arquivo_saida, "%s", ptr_atual->texto);
			break;
		}
		
	}

	//Apagar toda a lista e liberar espaco de memoria alocado
	apagar_lista();

	//Fechar o arquivo de saida
	fclose(arquivo_saida);
	
	getchar();
	getchar();
	
	return 1;
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
void cadastrar_no(int indice, char *texto, tipo_no tipo, int n_opcoes, opcao *opcoes, int (*fun_ptr)()) {
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
	ptr->fun_ptr = fun_ptr;
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
	
	/* Bifurcação */
	/* Caminho 1 */
	opcao opcoes_0[1] = {{'*',8}};
	cadastrar_no(0,"",raiz,1,opcoes_0,encontrar_general);

	/* Caminho 2 */
	opcao opcoes_1[2] = {{'D',4}, {'E', 3}};
	cadastrar_no(1,"Voce esta no segundo piso, pode ir a: D- Direita, E- Esquerda",nao_terminal,2,opcoes_1,NULL);

	opcao opcoes_2[1] = {{'*',3}};
	cadastrar_no(2,"",nao_terminal,1,opcoes_2,verificar_fardamento);

	opcao opcoes_3[1] = {{'*',8}};
	cadastrar_no(3,"",nao_terminal,1,opcoes_3,gd_surpresa);

	opcao opcoes_4[1] = {{'*',5}};
	cadastrar_no(4,"",nao_terminal,1,opcoes_4,encontrar_professor);

	opcao opcoes_5[2] = {{'A',6}, {'B',7}};
	cadastrar_no(5,"Agora, voce deseja: A- volta, B- seguir pelo corredor?",nao_terminal,2,opcoes_5,verificar_fardamento);

	opcao opcoes_6[1] = {{'*',8}};
	cadastrar_no(6,"",nao_terminal,1,opcoes_6,gd_surpresa);

	opcao opcoes_7[1] = {{'*',8}};
	cadastrar_no(7,"",nao_terminal,1,opcoes_7,encontrar_oficial);

	/* Fim da bifurcação */
	opcao opcoes_8[4] = {{'A',1},{'B',1},{'C',1},{'D',2}};
	cadastrar_no(8,"\nVoce chegou ao rancho!\nAgora, o que deseja fazer?\nA-Comer feijao\nB-Comer soja\nC-Comer arroz e bife\nD-Consultar Sargenteante\nOpcao escolhida:  ",nao_terminal,4,opcoes_8,NULL);

	cadastrar_no(9,"Nao esqueceu de nada?\nTorrado!Duvidas?",reinicio,0,NULL,NULL);

	opcao opcoes_10[3] = {{'A',3},{'B',4},{'C',5}};
	cadastrar_no(10,"\nAgora que voce esta arranchado, o que deseja comer?\nA-Comer feijao\nB-Comer soja\nC-Comer arroz e bife\nOpcao escolhida:  ",nao_terminal,3,opcoes_10,encontrar_sargenteante);

	cadastrar_no(11,"Passou mal no TFM!\nTorrado\nDuvidas?",reinicio,0,NULL,NULL);

	cadastrar_no(12,"Paisanaria!\nTorrado!\nDuvidas?",reinicio,0,NULL,NULL);

	cadastrar_no(13,"Parabens!\nVoce nao foi torrado esse fim de semana!",terminal,0,NULL,NULL);
		
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
	if(ptr_atual->opcoes[0].opcao_selecionada == '*') {
		return ptr_atual->opcoes[0].indice_proximo_no;
	}
	else {
		for(int i = 0; i < ptr_atual->n_opcoes; i++) {
			if(ptr_atual->opcoes[i].opcao_selecionada == opcao)
				return ptr_atual->opcoes[i].indice_proximo_no;
		}
		return -1;
	}
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

/*Minigames*/
int encontrar_oficial() {
	
	if(verificar_fardamento(0) == 0){
		return 0;
	}
	
    const int linhas = 3, colunas = 11;
    char espaco[linhas][colunas];
    //Preencher matriz com espaços
    for (int i = 0; i < linhas; i++)
        for (int j = 0; j < colunas; j++)
            espaco[i][j] = ' ';
    //Posicionar oficial
    const int linha_oficial = linhas-1, coluna_oficial = colunas/2;
    espaco[linha_oficial][coluna_oficial] = 'o';
    //Posicionar aluno
    const int linha_aluno = 0;
    int coluna_aluno = 0;
    espaco[linha_aluno][coluna_aluno] = 'o';
    //Execução
    char movimento;
    int continencia = 0;
    while (coluna_aluno != colunas) {
        printf("Pressione <ENTER> para avancar e <SPACE>+<ENTER> para prestar ou desfazer continencia");
        //Imprimir matriz
        for (int i = -1; i <= linhas; i++) {
            for (int j = 0; j < colunas; j++) {
                if (i == -1 || i == linhas)
                    putchar('-');
                else
                    printf("%c", espaco[i][j]);
            }
            putchar('\n');
        }
        //Receber e executar comando
        fflush(stdin);
        movimento = (char) getchar();
        fflush(stdin);
        if (movimento == ' ')
            continencia = !continencia;
        else if (movimento != '\n')
            continue;
        espaco[linha_aluno][coluna_aluno++] = ' ';
        espaco[linha_aluno][coluna_aluno] = continencia ? '*' : 'o';
        //Checar continencia
        const int distancia_continencia = 2;
        int distancia = coluna_oficial-coluna_aluno;
        if (continencia && (distancia > distancia_continencia || coluna_aluno > coluna_oficial))
            return 0;
        if (!continencia && distancia >= 0 && distancia <= distancia_continencia)
            return 0;
    }
    return 1;
	
}

int encontrar_sargenteante(){
	
	/* JOGO DE AZAR!! */
	
	if(verificar_fardamento(1) == 0){
		return 0;
	}
	
	int d1,d2,d3,d4,d5,hj;
	printf("Quais 5 dias do mes voce se arranchou?\n");
	fprintf(arquivo_saida, "Quais 5 dias do mes voce se arranchou?\n");
	scanf("%d %d %d %d %d",&d1,&d2,&d3,&d4,&d5);
	fprintf(arquivo_saida, "%d %d %d %d %d",&d1,&d2,&d3,&d4,&d5);
	hj=1+rand()%30;
	if(d1==hj||d2==hj||d3==hj||d4==hj||d5==hj){
		printf("Hoje eh dia %d e voce nao esta arranchado, torrado!\n",hj);
		fprintf(arquivo_saida, "Hoje eh dia %d e voce nao esta arranchado, torrado!\n",hj);
		return 0;
	}
	else{
		printf("Voce esta arranchado\n");
		fprintf(arquivo_saida, "Voce esta arranchado\n");
		return 1;
	}

}

int encontrar_general(){
	return 1;
}

int gd_surpresa(){
	return 1;
}

int encontrar_professor(){
	return 1;
}

/* Funções auxiliares */

/* Função que gera um número aleatório */
int aleatorio (int val_min, int val_max){
	
	int val_alt = 0;
	val_alt = rand() % val_max;
	val_alt = val_alt + val_min;
	return val_alt;
	
}

/* Funcao que verifica o fardamento */
int verificar_fardamento(int minigame) {
	if(minigame == 0 && fardamento[0] == 0 && fardamento[1] == 1) {
		printf("Mas que paisanaria é essa, nao teve adaptacao? Torrado! \n");
		fprintf(arquivo_saida, "Mas que paisanaria é essa, nao teve adaptacao? Torrado! \n");
		return 0;
	}
	if(minigame == 1 && fardamento[0] == 0) {
		printf("Paisanaria, onde está a farda aluno? Torrado, dúvidas? \n");
		fprintf(arquivo_saida, "Paisanaria, onde está a farda aluno? Torrado, dúvidas? \n");
		return 0;
	}
	if(minigame == 2 && fardamento[0] == 1) {
		printf("Fardamento incompleto aluno. Torrado! \n");
		fprintf(arquivo_saida, "Fardamento incompleto aluno. Torrado! \n");
		return 0;
	}
	if(minigame == 2 && fardamento[0] == 0) {
		printf("Esqueceu da farda aluno? Torrado! \n");
		fprintf(arquivo_saida, "Esqueceu da farda aluno? Torrado! \n");
		return 0;
	}
	return 1;;
}

/* Função que incrementa a variável global dias_punido */
int torrado(){
	dias_punido = dias_punido + 2;
	return 1;
}
