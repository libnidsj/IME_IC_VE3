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
