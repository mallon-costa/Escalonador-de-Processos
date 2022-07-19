#include <stdio.h>
#include <stdlib.h>
#include "tadFunc.h"
#include <time.h>
#include <locale.h>
#define QUANT 15

int main(){
	setlocale(LC_ALL, "Portuguese");
	srand(time(NULL));
	
	FilaProcesso* filaPronto1 = fila_cria();
	FilaProcesso* filaPronto2 = fila_cria();
	int contador;
	
	printf("PROCESSOS COM PRIORIDADE 0: \n\n");
	for(contador = 0; contador < 7; contador++){
		fila_insere_final(filaPronto1, 0, rand() % 1000, 1+rand() % 5, 0, rand() % 2);
	}
	escalonaProcesso(filaPronto1);	
	system("cls");
	
	printf("\nPROCESSOS COM VÁRIAS PRIORIDADES: \n\n");
	for(contador = 0; contador < QUANT; contador++){
		fila_insere_final(filaPronto2, 0, rand() % 1000, 1+rand() % 5, rand() % 5, rand() % 2);
	}
	escalonaProcesso(filaPronto2);
	
	return 0;
}
