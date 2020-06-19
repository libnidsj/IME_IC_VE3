/*---------------------------------------------------------------*/
/*
						TRABALHO (VE 3)
Grupo: Al. Burin, Al. Libni, Al. Meneses, Al. Zamperlini
Sistema operacional: Windows
Programa em C que implementa um jogo de labirinto				 */
/*---------------------------------------------------------------*/


/*------------------------------------------*/
/*			Incluindo Bibliotecas			*/
/*------------------------------------------*/
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<string.h>
#include<locale.h>


/*------------------------------------------*/
/*			Definindo constantes			*/
/*------------------------------------------*/
#define NOME_ARQUIVO_ENTRADA "entrada.txt"
#define NOME_ARQUIVO_SAIDA "saida.txt"
#define MAX_OPCOES 10


/*----------------------------------*/
/*			Definindo ENUMS			*/
/*----------------------------------*/
typedef enum _tipo_no {
	raiz, nao_terminal, reinicio, terminal
} tipo_no;


/*--------------------------------------*/
/*			Definindo Structs			*/
/*--------------------------------------*/
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
    int resposta;
    int usada;
} pergunta;



/*--------------------------------------*/
/*			Variáveis Globais			*/
/*--------------------------------------*/

no *ptr_inicio, *ptr_atual;		/* Referencia para o inicio e para a posicao atual na lista encadeada */
FILE *arquivo_saida;			/* Referencia para o arquivo de saida */

/* Criterios globais */
int fardamento[2];												/* {0,0} -> Paisano // {0,1} -> Farda sem gandola // {1,0} -> Farda sem gandola // {1,1} -> Fardamento completo */
int dias_punido = 0;											/* Contagem de dias punidos */
int elevador_alunos = 0;										/* 0 -> Não esperou // 1 -> Esperou */
enum comida {pizza = 0, bk = 1, salada = 2}alimentacao;			/* Para uso na checagem da comida pedida pelo ifood ou no rancho */

/*Cadastrar perguntas de GD*/
const int num_perguntas_gd = 8;
pergunta gd[] = {{"Atitude, aspecto e duracao sao os elementos essenciais da continencia individual?", 0, 0},
                 {"O Vice-presidente da republica tem direito a continencia??", 1, 0},
                 {"Na marinha, Capitao-de-Corveta equivale ao posto de Tenente-Coronel?", 0, 0},
                 {"O fardo de bagagem eh destinado para a conducao do material estritamente necessario para o cumprimento da missao de combate?", 0, 0},
                 {"Em uma situacao de sobreaviso, eh o Cmt U que decide todas ordens e toques gerais de sua OM?", 0, 0},
                 {"Quando dois militares se deslocam juntos, o de menor antiguidade da a direita ao superior?", 1, 0},
                 {"Eh atribuicao do Cabo de dia  consentir ou nao a entrada de civis no alojamento sem que estejam devidamente acompanhados por um oficial ou sargento?", 0, 0},
                 {"Eh atribuicao do  Cabo de dia dar sinal de 'silencio' imediatamente apos ultima nota do respectivo toque?", 0, 0},
                 };


/*------------------------------*/
/*			Protótipos			*/
/*------------------------------*/

/* Funções de manipulação */
char *ler_nome_jogador(char *nome_arquivo);
void cadastrar_no(int indice, char *texto, tipo_no tipo, int n_opcoes, opcao *opcoes, int (*fun_ptr)());
void cadastrar_nos();
void apagar_lista();
int ler_indice_proximo_no(char opcao);
no *buscar_no(int indice);

/* Minigames */
int encontrar_oficial();
int encontrar_sargenteante();
int encontrar_general();
int gd_surpresa();
int encontrar_professor();

/* Funções auxiliares */
int aleatorio(int val_min, int val_max);		/* Função que gera um número aleatório entre 0 e o máximo dado */
int verificar_fardamento(int minigame);			/* Função que verifica o fardamento com base na posição, recebe o parametro minigame */
int torrado();									/* Função que incrementa a variável global dias_punido */
int verificar_comida();							/* Função que checa variáveis globais, para diversos finais */
int escolher_pedido();
int colocar_10_uniforme();
int colocar_boina_e_gandola();
void imprimir_linha_de_separacao();
void resetar_fardamento();



/*--------------------------*/
/*			MAIN			*/
/*--------------------------*/

int main() {
	
	srand(time(NULL));
	setlocale(LC_ALL, "Portuguese");
	
	/* Abrindo arquivo de saída */
	if (!(arquivo_saida = fopen(NOME_ARQUIVO_SAIDA, "w"))) {
		printf("ERRO AO ABRIR O ARQUIVO DE SAIDA!");
		exit(1);
	}

	/* Mensagem inicial */
	char nome_jogador[100];
	strcpy(nome_jogador, ler_nome_jogador(NOME_ARQUIVO_ENTRADA));
	printf("Bem-vindo ao LABIRINTO de GD, %s!\n", nome_jogador);
	fprintf(arquivo_saida, "Bem-vindo ao LABIRINTO de GD, %s!\n", nome_jogador);
	
	/* Criar lista encadeada dos nós */
	cadastrar_nos();

	/* Carregar nó raiz (1) */
	ptr_atual = buscar_no(1);

	/* Laço de execução */
	while(1) {
		
		int var_saida = 1;
        if (ptr_atual->fun_ptr != NULL){
            var_saida = ptr_atual->fun_ptr();
		}
		
		/* Se é nó raiz ou não terminal, rodar a função, apresentar o texto e ler a opcao selecionada, seguindo restrições */
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
				
				/* Note que deve estar encadeado com o nó de escolha de caminho (Escala / Elevador de alunos / Elevador de oficiais) */
				if(ptr_atual == buscar_no(16) && opcao == 'C'){
					elevador_alunos = 1;
				} else {}

				/* Ler proximo no a partir da opcao */
				indice_proximo_no = ler_indice_proximo_no(opcao);
				no *ptr_proximo_no = buscar_no(indice_proximo_no);
				ptr_atual = ptr_proximo_no;
				if(indice_proximo_no == -1) {
					printf("OPCAO INVALIDA!\n");
					fprintf(arquivo_saida, "OPCAO INVALIDA!\n");
					resetar_fardamento();
					ptr_atual = buscar_no(1);
				}
			}
		}
		
		/* Se é nó de reinicio, o texto já vai ter sido mostrado e o número de dias torrado alterado, apenas deve-se trocar o endereço para o ínicio do jogo e mostrar a quantos dias punidos se está */ 
		else if (ptr_atual->tipo == reinicio || var_saida == 0){
			torrado();
			resetar_fardamento();
			if(var_saida != 0){			/* Note que quando a saida é pela condição expressa no minigame, o texto deve ser feito pelo mesmo */
				printf("%s", ptr_atual->texto);
				fprintf(arquivo_saida, "%s", ptr_atual->texto);
			}
			printf("\n\nVoce esta a %d dias punido. \n", dias_punido);
			fprintf(arquivo_saida, "\n\nVoce está a %d dias punido. \n", dias_punido);
			ptr_atual = buscar_no(1);
		}
		
		/* Se no é nó terminal, apresentar texto e finalizar programa */
		else if (ptr_atual->tipo == terminal){
			printf("%s", ptr_atual->texto);
			fprintf(arquivo_saida, "%s", ptr_atual->texto);
			break;
		}
		
	}

	/* Apagar toda a lista e liberar espaco de memoria alocado */
	apagar_lista();

	//Fechar o arquivo de saida
	fclose(arquivo_saida);
	
	getchar();
	getchar();
	
	return 1;
}



/*--------------------------------------------------------------*/
/*			Funções de manipulação e criação da lista 			*/
/*--------------------------------------------------------------*/

char *ler_nome_jogador(char *nome_arquivo) {			/* Funcao que le o nome do jogador a partir de um arquivo texto */
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

void cadastrar_no(int indice, char *texto, tipo_no tipo, int n_opcoes, opcao *opcoes, int (*fun_ptr)()) {			/* Funcao que cadastra um novo no na lista encadeada */
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

void cadastrar_nos() {			/* Funcao que cadastra todos os nos da lista encadeada (carregamento da lista) */
	
	/* Nós que chamam funções auxiliares, ou são apenas métodos para imprimir na tela algo */
	/* Nó -2 */
	opcao opcoes_m2[1] = {{'*',1}};
	cadastrar_no(-2,
                "Pressione <ENTER> para continuar\n",
                nao_terminal, 1, opcoes_m2, colocar_boina_e_gandola);
	
	/* Nó -3 */	
	opcao opcoes_m3[1] = {{'*',1}};
	cadastrar_no(-3,
                "Pressione <ENTER> para continuar\n",
                nao_terminal, 1, opcoes_m3, colocar_10_uniforme);
				
	/* Nó -4 */
	cadastrar_no(-4,
				"Nao esqueceu de nada?\nTorrado!Duvidas?",
				reinicio,0,NULL,NULL);
	
	/* Nó -5 */
	cadastrar_no(-5,
				"",
				nao_terminal, 0, NULL, verificar_comida);
				
	/* Nó -6 */
	opcao opcoes_m6[1] = {{'*', -5}};
	cadastrar_no(-6, "\nVoce passa pelas catracas...\n", nao_terminal, 1, opcoes_m6, NULL);	
	
	/* Nós do encadeamento */
	/* Nó 1 */
	opcao opcoes_1[4] = {{'C',-3},{'D', -2},{'A', 2},{'B',14}};
	cadastrar_no(1,
                "Voce eh um aluno do IME, esta na ala e quer jantar. O que voce vai fazer?\nA - Ir para o rancho\nB - Pedir comida pelo ifood\nC - Colocar o uniforme\nD - Colocar boina e gandola\n\nOpcao escolhida: ",
                raiz, 4, opcoes_1, NULL);
	
	/*--------------*/
	/* Bifurcação 1 */
	/*--------------*/
	
	/* Caminho 1 (Ir para o rancho) */
			
	opcao opcoes_2[2] = {{'A',3},{'B', 4}};
	cadastrar_no(2,
                "Voce vai ate a entrada da ala e precisa ir ao segundo piso. Como voce vai fazer isso?\nA - Pelo elevador\nB - Pelas escadas\n\nOpcao escolhida: ",
                nao_terminal, 2, opcoes_2, NULL);
				
	/*--------------*/
	/* Bifurcação 2 */
	/*--------------*/
	
	/* Caminho 1 (Usar elevador) */

	opcao opcoes_3[1] = {{'*', 8}};
	cadastrar_no(3,
                "Pressione <ENTER> para continuar\n",
                nao_terminal, 1, opcoes_3, encontrar_general);

	/* Caminho 2 (Usar escada) */

	opcao opcoes_4[2] = {{'A',5},{'B', 6}};
	cadastrar_no(4,
                "Voce esta, agora, no segundo piso. Deseja seguir pelo corredor:\nA - da esquerda\nB - da direita\n\nOpcao escolhida: ",
                nao_terminal, 2, opcoes_4, NULL);
	
	/*--------------*/
	/* Bifurcação 3 */
	/*--------------*/

	/* Caminho 1 (Direita) */

	opcao opcoes_5[1] = {{'*',9}};
	cadastrar_no(5,
				"",
				nao_terminal,1,opcoes_5,gd_surpresa);
	
	/* Caminho 2 (Esquerda) */
	
	opcao opcoes_6[1] = {{'*',7}};
	cadastrar_no(6,
				"",
				nao_terminal,1,opcoes_6,encontrar_professor);

	opcao opcoes_7[2] = {{'A',5}, {'B',8}};
	cadastrar_no(7,
				"Agora, voce deseja: A- voltar, B- seguir pelo corredor?",
				nao_terminal,2,opcoes_7,verificar_fardamento);

	opcao opcoes_8[1] = {{'*',9}};
	cadastrar_no(8,
				"",
				nao_terminal,1,opcoes_8,encontrar_oficial);
	
	/*--------------------*/
	/* Bifurcação 3 (FIM) */
	/*--------------------*/
	
	/*--------------------*/
	/* Bifurcação 2 (FIM) */
	/*--------------------*/
	
	opcao opcoes_9[4] = {{'A',-4},{'B',-4},{'C',-4},{'D',10}};
	cadastrar_no(9,
				"\nVoce chegou ao rancho!\nAgora, o que deseja fazer?\nA-Comer feijao\nB-Comer soja\nC-Comer arroz e bife\nD-Consultar Sargenteante\nOpcao escolhida:  ",
				nao_terminal,4,opcoes_9,NULL);

	opcao opcoes_10[3] = {{'A',11},{'B',12},{'C',13}};
	cadastrar_no(10,
				"\nAgora que voce esta arranchado, o que deseja comer?\nA-Comer feijao\nB-Comer soja\nC-Comer arroz e bife\nOpcao escolhida:  ",
				nao_terminal,3,opcoes_10,encontrar_sargenteante);

	cadastrar_no(11,
				"Passou mal no TFM!\nTorrado\nDuvidas?",
				reinicio,0,NULL,NULL);

	cadastrar_no(12,
				"Paisanaria!\nTorrado!\nDuvidas?",
				reinicio,0,NULL,NULL);

	cadastrar_no(13,
				"Parabens!\nVoce nao foi torrado esse fim de semana!",
				terminal,0,NULL,NULL);
	/* Caminho 1 (FIM) */
	
	/* Caminho 2 (Pedir iFood) */
	opcao opcoes_14[1] = {{'*', 15}};
	cadastrar_no(14,
                "",
                nao_terminal, 1, opcoes_14, escolher_pedido);
	
	opcao opcoes_15[1] = {{'*', 16}};
	cadastrar_no(15, "", nao_terminal, 1, opcoes_15, encontrar_oficial);
	
	opcao opcoes_16[3] = {{'A', -6}, {'B', 17}, {'C', -6}};
	cadastrar_no(16,
				"\nAgora, voce se encontra de frente aos elevadores e do lado das escadas.\nO que voce vai fazer?\n\nA - Descer pelas escadas\nB - Usar o elevador dos oficiais\nC - Usar o elevador das pracas\n\nOpcao selecionada: ",
				nao_terminal, 3, opcoes_16, NULL);
	
	opcao opcoes_17[1] = {{'*', -6}};
	cadastrar_no(17,
				"", nao_terminal,
				1, opcoes_17, encontrar_general);
	
	cadastrar_no(18,
				"\nVoce perdeu a entrega! Nao devia ter esperado o elevador das pracas...\n",
				reinicio, 0, NULL, NULL);
	
	cadastrar_no(19,
				"\nVoce passou mal no TFM devido as suas escolhas alimentares! Por isso foi...\nTorrado! Duvidas?\n", 
				reinicio, 0, NULL, NULL);
	
	cadastrar_no(20,
				"\nPARABENS! Voce conseguiu se alimentar sem ser torrado, sem passar mal e sem ser rematriculado!\nFim de jogo!",
				terminal, 0, NULL, NULL);
	
	/* Caminho 2 (FIM) */
	
		
}

no *buscar_no(int indice) {			/* Funcao que busca um no a partir do seu indice */
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

int ler_indice_proximo_no(char opcao) {			/* Funcao que le o indice do proximo no da lista a partir de uma opcao selecionada no no atual */
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

void apagar_lista() {		/* Funcao que apaga toda a lista, liberando os espacos de memorias alocados */
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



/*----------------------*/
/*		Minigames		*/
/*----------------------*/

int encontrar_oficial() {
    //Verificar fardamento
    if(verificar_fardamento(1) == 0){
		return 0;
	}
    //Declaração de variáveis
    const int distancia_continencia = 2;
    const int linhas = 3, colunas = 11;
    const int linha_oficial = linhas-1, coluna_oficial = colunas/2;
    const int linha_aluno = 0;
    int coluna_aluno = 0;
    char movimento;
    int continencia = 0;
    int tentativas = 1;
    char espaco[linhas][colunas];
    //Mensagem de início
    imprimir_linha_de_separacao();
    printf("Enquanto andava pelo corredor, voce encontra um oficial e precisa prestar continencia ao estar a %d caractere%s de distancia dele e desfaze-la ao ultrapassa-lo.\n\n", distancia_continencia, distancia_continencia==1? "" : "s");
    fprintf(arquivo_saida, "Enquanto andava pelo corredor, voce encontra um oficial e precisa prestar continencia ao estar a %d caractere%s de distancia dele e desfaze-la ao ultrapassa-lo.\n\n", distancia_continencia, distancia_continencia==1? "" : "s");
    //Preencher matriz com espaços
    inicio_minigame_continencia_andando:
    for (int i = 0; i < linhas; i++)
        for (int j = 0; j < colunas; j++)
            espaco[i][j] = ' ';
    //Posicionar aluno e oficial
    coluna_aluno = 0;
    espaco[linha_oficial][coluna_oficial] = 'o';
    espaco[linha_aluno][coluna_aluno] = 'o';
    continencia = 0;
    //Execução
    while (coluna_aluno != colunas) {
        puts("Pressione <ENTER> para avancar e <SPACE>+<ENTER> para prestar ou desfazer continencia\n");
        fputs("Pressione <ENTER> para avancar e <SPACE>+<ENTER> para prestar ou desfazer continencia\n\n", arquivo_saida);
        //Imprimir matriz
        for (int i = -1; i <= linhas; i++) {
            for (int j = 0; j < colunas; j++) {
                if (i == -1 || i == linhas) {
                    putchar('-');
                    fprintf(arquivo_saida, "-");
                }
                else {
                    printf("%c", espaco[i][j]);
                    fprintf(arquivo_saida, "%c", espaco[i][j]);
                }
            }
            putchar('\n');
            fprintf(arquivo_saida, "\n");
        }
        //Receber e executar comando
        fflush(stdin);
        movimento = (char) getchar();
        fflush(stdin);
        fprintf(arquivo_saida, "\n");
        if (movimento == ' ')
            continencia = !continencia;
        else if (movimento != '\n')
            continue;
        espaco[linha_aluno][coluna_aluno++] = ' ';
        espaco[linha_aluno][coluna_aluno] = continencia ? '*' : 'o';
        //Checar continencia
        int distancia = coluna_oficial-coluna_aluno;
        if (continencia && distancia > distancia_continencia) {
            if (tentativas == 1) {
                puts("Aluno bizonho! Prestou continencia cedo demais. Agora volte faz direito.");
                puts("Pressione <ENTER> para voltar ao inicio.\n");
                fputs("Aluno bizonho! Prestou continencia cedo demais. Agora volte faz direito.\n", arquivo_saida);
                fputs("Pressione <ENTER> para voltar ao inicio.\n\n", arquivo_saida);
                getchar();
                tentativas++;
                goto inicio_minigame_continencia_andando;
            }
            imprimir_linha_de_separacao();
            return 0;
        }
        if (continencia && coluna_aluno > coluna_oficial) {
            if (tentativas == 1) {
                puts("Aluno bizonho! Nao desfez a continencia direito. Agora volte faz direito.");
                puts("Pressione <ENTER> para voltar ao inicio.");
                fputs("Aluno bizonho! Nao desfez a continencia direito. Agora volte faz direito.\n", arquivo_saida);
                fputs("Pressione <ENTER> para voltar ao inicio.\n", arquivo_saida);
                getchar();
                tentativas++;
                goto inicio_minigame_continencia_andando;
            }
            imprimir_linha_de_separacao();
            return 0;
        }
        if (!continencia && distancia >= 0 && distancia <= distancia_continencia) {
            if (tentativas == 1) {
                puts("Que eh isso, aluno, nao vai prestar continencia nao?! Volte e faz direito.");
                fputs("Que eh isso, aluno, nao vai prestar continencia nao?! Volte e faz direito.\n", arquivo_saida);
                tentativas++;
                goto inicio_minigame_continencia_andando;
            }
            imprimir_linha_de_separacao();
            return 0;
        }
    }
    imprimir_linha_de_separacao();
    return 1;
}

/* JOGO DE AZAR!! */
int encontrar_sargenteante(){
	
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
	
	if(verificar_fardamento(2) == 0){
		return 0;
	}
	
	char resposta, resposta2;
	int validade = 1;
	
	printf("Voce entrou no elevador!\n");
	printf("Para sua surpresa, o general logo tambem entra no andar de baixo e prontamente te deseja um \"Bom-dia!\"\n");
	printf("\nO que voce vai fazer?\n");
	
	do{
	printf("\nA - Estender a mao para cumprimenta-lo\nB - Permanecer na posicao de sentido calado\nC - Responder \"E ai chefinho?\"\nD - Responder \"Bom dia, senhor general!\", prestar continencia e permanecer na posicao de sentido\n\nOpcao selecionada: ");
	scanf(" %c", &resposta);
	
	if(resposta != 'A' && resposta != 'B' && resposta != 'C' && resposta != 'D' && resposta != 'a' && resposta != 'b' && resposta != 'c' && resposta != 'd'){
		printf("\nOpcao invalida!\n");
		validade = 0;
	}
	else validade = 1;
	 
	}while(validade == 0);
	
	switch(resposta){
		
		case 'A':
		case 'a':
			printf("\nAluno bizonho! A iniciativa do aperto de mao vem do militar mais antigo!\nTorrado! Duvidas?");
			return 0;
		
		case 'B':
		case 'b':
			printf("\nO general te deseja um \"Bom-dia!\" de maneira mais energica e clara.\nO que voce vai fazer?\n");
			
			do{
				printf("\nA - Permanecer calado\nB - Responder \"Bom dia, senhor general!\", prestar continencia e permanecer na posicao de sentido.\n\nOpcao selecionada: ");
					scanf(" %c", &resposta2);
				if(resposta2 != 'A' && resposta2 != 'a' && resposta2 != 'B' && resposta2 != 'b'){
						printf("\nOpcao invalida!\n");
						validade = 0;	
				}
				else validade = 1;
				
			}while(validade == 0);
			
				switch(resposta2){
					
					case 'A':
					case 'a':
						printf("\nAluno ignorou o general!\nTorrado! Duvidas?");
						return 0;
					
					case 'B':
					case 'b':
						printf("\nPreste mais atencao, voce pode ser torrado na proxima vez...\n");
						break;
				}
				
			break;
			
		case 'C':
		case 'c':
			printf("\nNao tenho nem o que dizer sobre isso...\nTorrado! Duvidas?");
			return 0;
		
		case 'D':
		case 'd':
			break;			
	}
	
	printf("\n-Excelente dia, aluno. Como foi esse periodo de EAD? Pergunta o general");
	
	do{
		printf("\nSelecione uma resposta:\n\n");
		printf("A - \"Muito bom! Consegui colocar o sono em dia! Marcava presenca no EAD e voltava para a cama!\"\nB - \"Nao achei muito produtivo... foi dificil acompanhar a materia\"\nC - \"Achei bom, aprendi muito com o EAD!\"\n\nOpcao selecionada: ");
		scanf(" %c", &resposta);
		
		if(resposta != 'A' && resposta != 'a' && resposta != 'B' && resposta != 'b' && resposta != 'C' && resposta != 'c'){
						printf("\nOpcao invalida!\n");
						validade = 0;
		}
		else validade = 1;
		
	}while(validade == 0);
		
	switch(resposta){
		
		case 'A':
		case 'a':
			printf("\nGolpe do aluno plotado pelo general!\nTorrado! Duvidas?");
			return 0;
		
		case 'B':
		case 'b':
			printf("\n-Tenho certeza que voce vai conseguir!\n");
			break;
			
		case 'C':
		case 'c':
			printf("\n-Que bom! Com esse desempenho todo, voce sera o proximo auxiliar de comando!\n");
			break;
	}
	
	printf("\nVoce e o general saem do elevador e seguem caminhos diferentes...\nVoce nao foi torrado, por enquanto...\n");
	return 1;
}

int gd_surpresa() {
    //Verificar fardamento
    if(verificar_fardamento(1) == 0){
		return 0;
	}
    //Mensagem inicial
    imprimir_linha_de_separacao();
    puts("Enquanto andava, voce se deparou com o seu tenente instrutor de GD e ele lhe fez a seguinte pergunta:");
    fputs("Enquanto andava, voce se deparou com o seu tenente instrutor de GD e ele lhe fez a seguinte pergunta:\n", arquivo_saida);
    //Resetar perguntas, caso necessário
    int todas_perguntas_usadas = 1;
    for (int i = 0; i < num_perguntas_gd; i++)
        if (!gd[i].usada) {
            todas_perguntas_usadas = 0;
            break;
        }
    if(todas_perguntas_usadas)
        for (int i = 0; i < num_perguntas_gd; i++)
            gd[i].usada = 0;
    //Fazer pergunta aleatória
    int pergunta_aleatoria;
    do {
        pergunta_aleatoria = rand()%num_perguntas_gd;
    } while (gd[pergunta_aleatoria].usada);
    printf("\n%s", gd[pergunta_aleatoria].enunciado);
    fprintf(arquivo_saida, "\n%s", gd[pergunta_aleatoria].enunciado);
    //Atualizar status da pergunta
    gd[pergunta_aleatoria].usada = 1;
    //Receber e analisar entrada do usuário
    char resposta;
    printf("\nResposta(V/F): ");
    fprintf(arquivo_saida, "\nResposta(V/F): ");
    scanf(" %c", &resposta);
    fprintf(arquivo_saida, "%c\n", resposta);
    int resposta_correta;
    switch (resposta) {
    case 'v':
    case 'V':
        resposta_correta = gd[pergunta_aleatoria].resposta;
        break;
    case 'f':
    case 'F':
        resposta_correta = !gd[pergunta_aleatoria].resposta;
        break;
    }
    if (resposta_correta) {
        puts("\nTenente: Ta sabendo, hein, aluno? Agora some daqui!");
        fputs("\nTenente: Ta sabendo, hein, aluno? Agora some daqui!\n", arquivo_saida);
        imprimir_linha_de_separacao();
        return 1;
    }
    puts("\nTenente: Que baixaria eh essa, aluno? Tem que papirar mais!");
    fputs("\nTenente: Que baixaria eh essa, aluno? Tem que papirar mais!\n", arquivo_saida);
    imprimir_linha_de_separacao();
    return 0;
}

/* Tem que complementar */
int encontrar_professor() {
	
	int determinante[2];
	int var_alt;
	int matriz[3][3];
	
	fprintf(stdout, "\nVoce esta andando pelo segundo andar do IME e surge do alem o professor Ramon, que tem uma dúvida de algebra muito complexa!");
	fprintf(stdout, "\nRamon: - Diga! Diga! Não consigo calcular o determinante dessa matriz: \n");
	
	for(int i = 0; i < 3; i++){

		for(int j = 0; j < 3; j++){	
		
			var_alt = rand() % 5;
		
			matriz[i][j] = var_alt;
			fprintf(stdout, "%d ", var_alt);

		}
		
		fprintf(stdout, "\n");
		
	}
	
	determinante[0] = matriz[0][0]*matriz[1][1]*matriz[2][2] + matriz[0][1]*matriz[1][2]*matriz[2][0] + matriz[0][2]*matriz[1][0]*matriz[2][1] - matriz[0][2]*matriz[1][1]*matriz[2][0] - matriz[1][2]*matriz[2][1]*matriz[0][0] - matriz[2][2]*matriz[0][1]*matriz[1][0];
	
	fprintf(stdout, "\nDiga o valor do determinante: ");
	scanf("%d", &determinante[1]);
	fflush(stdin);
	
	if(determinante[0] == determinante[1]){
		fprintf(stdout, "Muito inteligente menino! ");
		return 1;
	}else{
		fprintf(stdout, "Aluno que nao sabe o basico e esta no IME. Trancado! ");
		return 0;
	}

}



/*-------------------------------*/
/* 		Funções auxiliares		 */
/*-------------------------------*/

/* Função que gera um número aleatório */
int aleatorio (int val_min, int val_max){
	
	int val_alt = 0;
	val_alt = rand() % val_max;
	val_alt = val_alt + val_min;
	return val_alt;
	
}

/* Funcao que verifica o fardamento, , com base no indice (0 - encontrar_sargenteante / 1 - encontrar_general ) */
int verificar_fardamento(int minigame) {
	if(fardamento[0] == 0 && fardamento[1] == 1) {
		printf("Mas que paisanaria é essa, nao teve adaptacao? Torrado! \n");
		fprintf(arquivo_saida, "Mas que paisanaria é essa, nao teve adaptacao? Torrado! \n");
		return 0;
	}
	if(minigame == 1 && fardamento[0] == 0) {
		printf("Paisanaria, onde está a farda aluno? Torrado, dúvidas? \n");
		fprintf(arquivo_saida, "Paisanaria, onde está a farda aluno? Torrado, dúvidas? \n");
		return 0;
	}
	if(minigame == 2 && fardamento[0] == 1 && fardamento[1] == 0) {
		printf("Fardamento incompleto aluno. Torrado! \n");
		fprintf(arquivo_saida, "Fardamento incompleto aluno. Torrado! \n");
		return 0;
	}
	if(minigame == 2 && fardamento[0] == 0) {
		printf("Esqueceu da farda aluno? Torrado! \n");
		fprintf(arquivo_saida, "Esqueceu da farda aluno? Torrado! \n");
		return 0;
	}
	if(minigame == 3 && fardamento[0] == 1 && fardamento[1] == 0){
		printf("Saindo do IME sem gandola aluno? Torrado! \n");
		fprintf(arquivo_saida, "Saindo do IME sem gandola aluno? Torrado! \n");
		return 0;
	}
	return 1;;
}

/* Função que incrementa a variável global dias_punido */
int torrado(){
	dias_punido = dias_punido + 2;
	return 1;
}

/* Função que checa variáveis globais, para diversos finais */
int verificar_comida(){
	
	if (verificar_fardamento(3) == 0){
		return 0;
	}
	
	/* Note que os nós devem ser encadeados corretamente */
	if(elevador_alunos == 1){
		no *ptr_proximo_no = buscar_no(6);
		ptr_atual = ptr_proximo_no;
		return 1;
	}
	
	if (alimentacao == 0 || alimentacao == 1){
		no *ptr_proximo_no = buscar_no(19);
		ptr_atual = ptr_proximo_no;
		return 1;
	} else {
		no *ptr_proximo_no = buscar_no(20);
		ptr_atual = ptr_proximo_no;
		return 1;
	}
	
}

int escolher_pedido() {
    printf("Voce escolheu pedir comida pelo ifood, muito bem. Agora, faca o pedido:\nA - BK\nB - Pizza\nC - Salada\n\n");
    fprintf(arquivo_saida, "Voce escolheu pedir comida pelo ifood, muito bem. Agora, faca o pedido:\nA - BK\nB - Pizza\nC - Salada\n\n");
    char escolha;
    fazer_pedido:
    printf("Qual pedido voce vai fazer? ");
    fprintf(arquivo_saida, "Qual pedido voce vai fazer? ");
    fflush(stdin);
    scanf("%c", &escolha);
    fflush(stdin);
    fprintf(arquivo_saida, "%c", escolha);
    switch (escolha) {
    case 'a':
    case 'A':
        printf("\nVoce pediu um BK...\n");
        fprintf(arquivo_saida, "\nVoce pediu um BK...\n");
        alimentacao = bk;
        break;
    case 'b':
    case 'B':
        printf("\nVoce pediu uma Pizza...\n");
        fprintf(arquivo_saida, "\nVoce pediu uma Pizza...\n");
        alimentacao = pizza;
        break;
    case 'c':
    case 'C':
        printf("\nVoce pediu uma salada...\n");
        fprintf(arquivo_saida, "\nVoce pediu uma salada...\n");
        alimentacao = salada;
        break;
    default:
        printf("\nNao ha essa opcao!\n");
        fprintf(arquivo_saida, "\nNao ha essa opcao!\n\n");
        goto fazer_pedido;
    }
    printf("Depois de uns 40 min, a entrega ja esta quase chegando. Voce, entao, vai descer para recolhe-la.\n\n");
    fprintf(arquivo_saida, "Depois de uns 40 min, a entrega ja esta quase chegando. Voce, entao, vai descer para recolhe-la.\n\n");
    return 1;
}

void resetar_fardamento() {
    fardamento[0] = fardamento[1] = 0;
}

int colocar_10_uniforme() {
    if (fardamento[0]) {
        printf("Voce ja esta de uniforme!\n\n");
        fprintf(arquivo_saida, "Voce ja esta de uniforme!\n\n");
    } else if(fardamento[1]) {
        fardamento[0] = 1;
        printf("Voce tira a boina e a gandola e coloca o uniforme. Depois voce coloca a boina e a gandola de novo.\n\n");
        fprintf(arquivo_saida, "Voce tira a boina e a gandola e coloca o uniforme. Depois voce coloca a boina e a gandola de novo.\n\n");
    } else {
        fardamento[0] = 1;
        printf("Agora voce esta de uniforme.\n\n");
        fprintf(arquivo_saida, "Agora voce esta de uniforme.\n\n");
    }
    return 1;
}

int colocar_boina_e_gandola() {
    if (fardamento[1]) {
        printf("Voce ja esta de boina e gandola!\n\n");
        fprintf(arquivo_saida, "Voce ja esta de boina e gandola!\n\n");
    } else {
        fardamento[1] = 1;
        printf("Agora voce esta de boina e gandola.\n\n");
        fprintf(arquivo_saida, "Agora voce esta de boina e gandola.\n\n");
    }
    return 1;
}

void imprimir_linha_de_separacao() {
    putchar('\n');
    fprintf(arquivo_saida, "\n");
    for (int i = 0; i < 100; i++) {
        putchar('*');
        fprintf(arquivo_saida, "*");
    }
    putchar('\n');
    fprintf(arquivo_saida, "\n");
}
