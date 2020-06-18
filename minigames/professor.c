#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int minigame_professor();

int main (void) {
    if (minigame_professor())
        puts("Nada acontece");
    else
        puts("Seu bizonho! Trancado, duvidas?");
	
	getchar();
    return 0;
}

int minigame_professor() {
	
	int determinante[2];
	int var_alt;
	int matriz[3][3];
	srand(time(NULL));
	
	puts("Neste minigame, você é abordado pelo professor Ramón, que tem uma dúvida de Álgebra Linear!");
	puts("\nDiga! Diga! Não consigo calcular o determinante dessa matriz: \n");
	
	for(int i = 0; i < 3; i++){

		for(int j = 0; j < 3; j++){	
		
			var_alt = rand() % 5;
		
			matriz[i][j] = var_alt;
			fprintf(stdout, "%d ", var_alt);

		}
		
		fprintf(stdout, "\n");
		
	}
	
	determinante[0] = matriz[0][0]*matriz[1][1]*matriz[2][2] + matriz[0][1]*matriz[1][2]*matriz[2][0] + matriz[0][2]*matriz[1][0]*matriz[2][1] - matriz[0][2]*matriz[1][1]*matriz[2][0] - matriz[1][2]*matriz[2][1]*matriz[0][0] - matriz[2][2]*matriz[0][1]*matriz[1][0];
	
	puts("");
	puts("Diga o valor do determinante: ");
	scanf("%d", &determinante[1]);
	fflush(stdin);
	
	if(determinante[0] == determinante[1]){
		return 1;
	}else{
		return 0;
	}

}
