#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define NOME_ARQUIVO_SAIDA "saida.txt"

/* Cadastrar nós:
 * -Nó -3:
 *  opcoes_m3[1] = {{'\n',1}};
 *  cadastrar_no(-3,
 *               "Pressione <ENTER> para continuar\n",
 *               nao_terminal, 1, opcoes_m1, colocar_10_uniforme);
 * -Nó -2:
 *  cadastrar_no(-2,
 *               "Pressione <ENTER> para continuar\n",
 *               nao_terminal, 1, opcoes_m3, colocar_boina_e_gandola);
 * -Nó 1:
 *  opcoes_1[4] = {{'C',-3},{'D', -2},{'A', 2},{'B',14}};
 *  cadastrar_no(1,
 *               "Voce eh um aluno do IME, esta na ala e quer jantar. O que voce vai fazer?\nA - Ir para o rancho\nB - Pedir comida pelo ifood\nC - Colocar o uniforme\nD - Colocar boina e gandola\n\nOpcao escolhida: ",
 *               raiz, 4, opcoes_1, NULL);
 * -Nó 2:
 *  opcoes_2[2] = {{'A',3},{'B', 4}};
 *  cadastrar_no(2,
 *               "Voce vai ate a entrada da ala e precisa ir ao segundo piso. Como voce vai fazer isso?\nA - Pelo elevador\nB - Pelas escadas\n\nOpcao escolhida: ",
 *               nao_terminal, 2, opcoes_2, NULL);
 * -Nó 3:
 *  opcoes_3[1] = {{'\n', 8}}
 *  cadastrar_no(3,
 *               "Pressione <ENTER> para continuar\n",
 *               nao_terminal, 1, opcoes_3, minigame_encontrar_general);
 * -Nó 4:
 *  opcoes_4[2] = {{'A',7},{'B', 5}};
 *  cadastrar_no(4,
 *               "Voce esta, agora, no segundo piso. Deseja seguir pelo corredor:\nA - da esquerda\nB - da direita\n\nOpcao escolhida: ",
 *               nao_terminal, 2, opcoes_4, NULL);
 * -Nó 14:
 *  opcoes_14[1] = {{'/n',15}};
 *  cadastrar_no(5,
 *               "Pressione <ENTER> para continuar\n",
 *               nao_terminal, 2, opcoes_14, escolher_pedido);
 */

 //Obs: os nós -2 e -3 servem para colocar a boina, gandola e uniforme

/*structs*/
typedef struct _pergunta {
    char enunciado[200];
    int resposta;
    int usada;
} pergunta;

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


/*Minigames*/
int minigame_continencia_andando();
int minigame_gd_surpresa();

/*Outras funções*/
int escolher_pedido();
void resetar_fardamento();
int colocar_10_uniforme();
int colocar_boina_e_gandola();
int aleatorio (int min, int max); //Não cheguei a usar, mas está aí, caso precisem
void imprimir_linha_de_separacao();
int verificar_fardamento(int parametro); //Não está implementada aqui, apenas indicada.

/*Variáveis globais*/
int dias_punido = 0;
int numero_de_repeticoes = 0; //Essa varável, a princípio, não vai ser usada, mas talvez possa acabar sendo útil.
int fardamento[2] = {0,0};  /*elemento 0 = 10 uniforme, elemento 1 = boina e gandola*/
enum comida {pizza = 0, bk = 1, salada = 2, feijao = 3, soja = 4, arroz_bife = 5};
int alimentacao = -1;

/*Referencia para o arquivo de saida*/
FILE *arquivo_saida;

int main() {
    srand(time(0));

    //Abertura do arquivo de saida
	if (!(arquivo_saida = fopen(NOME_ARQUIVO_SAIDA, "w"))) {
		printf("ERRO AO ABRIR O ARQUIVO DE SAIDA!");
		exit(1);
	}

	//escolher_pedido();

    char resposta;
    do {
        if (numero_de_repeticoes != 0) {
            puts("\n\n\n");
            fputs("\n\n\n\n", arquivo_saida);
        }
        printf("Dias punido: %d\n", dias_punido);
        fprintf(arquivo_saida, "Dias punido: %d\n", dias_punido);

        //Minigame continencia andando
        if(minigame_continencia_andando()) {
            puts("\nNada acontece.\n");
            fputs("\nNada acontece.\n\n", arquivo_saida);
        }
        else {
            puts("\nVoce foi torrado.\n");
            fputs("\nVoce foi torrado.\n\n", arquivo_saida);
            dias_punido += 2;
        }

        //Minigame GD surpresa
        if(minigame_gd_surpresa()) {
            puts("\nNada acontece.\n");
            fputs("\nNada acontece.\n\n", arquivo_saida);
        }
        else {
            puts("\nVoce foi torrado.\n");
            fputs("\nVoce foi torrado.\n\n", arquivo_saida);
            dias_punido += 2;
        }
        //Repetir ciclo
        printf("Deseja repetir? (s/n) ");
        fprintf(arquivo_saida, "Deseja repetir? (s/n) ");
        scanf(" %c", &resposta);
        fprintf(arquivo_saida, "%c", resposta);
        if (resposta != 's' && resposta != 'n') {
            puts("Vou considerar isso um nao.\n");
            fputs("Vou considerar isso um nao.\n\n", arquivo_saida);
        }
        numero_de_repeticoes++;
    } while (resposta == 's');

    //Fechar o arquivo de saida
	fclose(arquivo_saida);

    return 0;
}

int minigame_continencia_andando() {
    //Verificar fardamento
    verificar_fardamento(0);
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

int minigame_gd_surpresa() {
    //Verificar fardamento
    verificar_fardamento(0);
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

int aleatorio (int min, int max) {
    //srand(time(0));
    if (min > max) {
        int temp = min;
        min = max;
        max = temp;
    }
    return min+rand()%(max-min+1);
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

int verificar_fardamento(int parametro) {
    return 0;
}
