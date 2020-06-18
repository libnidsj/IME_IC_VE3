#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct _PERGUNTA {
    char enunciado[200];
    int resposta;
	int usado;
} PERGUNTA;

int minigame_ten_sugao();

int main(void) {
    srand(time(0));
    printf("\n%s\n\n", minigame_ten_sugao() ? "Nao fez mais que sua obrigacao." : "Cade o papiro?! Torrado, duvidas?");
    printf("\n%s\n\n", minigame_ten_sugao() ? "Nao fez mais que sua obrigacao." : "Cade o papiro?! Torrado, duvidas?");
    printf("\n%s\n\n", minigame_ten_sugao() ? "Nao fez mais que sua obrigacao." : "Cade o papiro?! Torrado, duvidas?");
    return 0;
}

void minigame_ten_sugao() {
	char resposta;
	int aleatorio;
    const int num_perguntas = 3;
    PERGUNTA conjunto[] = {{"Atitude, aspecto e duracao sao os elementos essenciais da continência individual?", 0, 0},
                           {"O Vice-presidente da republica tem direito a continencia??", 1, 0},
                           {"Na marinha, Capitao-de-Corveta equivale ao posto de Tenente-Coronel?", 0, 0},
                            };
    
	for(int i = 0; i < 3; i++){
		aleatorio = rand()%num_perguntas;
		printf("%s", conjunto[aleatorio].enunciado);
		conjunto[aleatorio].usado = 1;
		printf("\nResposta(V/F): ");
		scanf(" %c", &resposta);
		switch (resposta) {
			case 'v':
			case 'V':
				return conjunto[aleatorio].resposta;
				break;
			case 'f':
			case 'F':
				return !conjunto[aleatorio].resposta;
		}
	}
}
