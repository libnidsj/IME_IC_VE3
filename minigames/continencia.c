#include <stdio.h>

int minigame_continencia_andando();

int main (void) {
    printf("Neste minigame, voce esta andando em direcao a um oficial parado e precisa prestar\ncontinecia ao estar a dois carateres de distancia e desfaze-la ao ultrapassar o oficial.\n\n");
    if (minigame_continencia_andando())
        puts("Nada acontece");
    else
        puts("Seu bizonho! Torrado, duvidas?");
    return 0;
}

int minigame_continencia_andando() {
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
        puts("Pressione <ENTER> para avancar e <SPACE>+<ENTER> para prestar continencia");
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
