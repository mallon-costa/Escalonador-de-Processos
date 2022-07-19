#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <String.h>
#include <locale.h>
#include "tadFunc.h"

const int quantum = 1;
const int velocidade = 800;

enum tipo{
	CPU,
	ES
};

struct filaprocessono{
	int id;
	int tmpDuracao;
	int prioridade;
	Tipo tipo;
	int cont;
	int quant;
	
	FilaProcessoNo* prox;
};

struct filaprocesso{
	FilaProcessoNo* ini;
	FilaProcessoNo* fim;
};

FilaProcesso* fila_cria(){
	setlocale(LC_ALL, "Portuguese");
	FilaProcesso* novaFila = (FilaProcesso*) malloc(sizeof(FilaProcesso));
	
	if (novaFila == NULL) {
        printf("Erro Fatal: Falha Alocação de memória.\nFinalizar.\n");
        exit(1);
    };
	
	novaFila->ini = novaFila->fim = NULL;
	return novaFila;
}

void fila_insere_inicio(FilaProcesso* fila, int quant, int id, int tempoDuracao, int prioridade, int tipo){
	FilaProcessoNo* novo = (FilaProcessoNo*) malloc (sizeof(FilaProcessoNo));
	
	novo->id = id;
	novo->tmpDuracao = tempoDuracao;
	novo->prioridade = prioridade;
	novo->tipo = tipo;
	novo->prox = NULL;
	novo->cont = 0;
	novo->quant = quant;
	
	novo->prox = fila->ini;
	fila->ini = novo;
}

void fila_insere_final(FilaProcesso* fila, int quant, int id, int tempoDuracao, int prioridade, int tipo){
	FilaProcessoNo* novo = (FilaProcessoNo*) malloc(sizeof(FilaProcessoNo));
	
	novo->id = id;
	novo->tmpDuracao = tempoDuracao;
	novo->prioridade = prioridade;
	novo->tipo = tipo;
	novo->prox = NULL;
	novo->cont = 0;
	novo->quant = quant;
	
	
	if(fila->fim != NULL){
		fila->fim->prox = novo;
	}else{
		fila->ini = novo;
	}

	fila->fim = novo;
}

FilaProcessoNo* fila_retira(FilaProcesso* fila){
	FilaProcessoNo* aux = fila->ini;
	
	fila->ini = aux->prox;
	if(fila->ini == NULL){
		fila->fim = NULL;
	}

	return aux;	
}

void fila_libera(FilaProcesso* fila){
	FilaProcessoNo* aux = fila->ini;
	
	while(aux != NULL){
		FilaProcessoNo* t = aux->prox;
		free(aux);
		aux = t;
	}
	
	free(fila);
}

void escalonaProcesso(FilaProcesso* pronto){
	int ciclos = 0;
	float  tmpTotal = 0;

	FilaProcessoNo* aux;
	FilaProcessoNo *atual;
	
	clock_t t;
	
	FilaProcesso* espera = fila_cria();
	FilaProcesso* resultado = fila_cria();
	
	atual = maiorPrioridade(pronto);
	
	listaProcessos(pronto);
	system("Pause");
	system("cls");
	
	while(pronto->ini != NULL){
		t = clock();
		
		aux = retiraPorId(pronto, atual->id);
		tmpTotal = tmpTotal + aux->tmpDuracao;
		
		aux->quant++;
		printf("\nA fila de pronto: \n"); fila_imprime(pronto);
		
		Sleep(400);
		printf("\n\nO processo em execução: %d", aux->id);
		Sleep(400);
		printf("\b\b\b %s", "       ");
		
		if(ciclos > 10){
			if(aux->tmpDuracao > quantum){
				aux->tmpDuracao = aux->tmpDuracao - quantum;
				if(verificaExitenciaDoProcesso(pronto, aux->id) != 1){
					fila_insere_inicio(pronto, aux->quant, aux->id, aux->tmpDuracao, aux->prioridade, aux->tipo);
				}else{
					retiraPorId(pronto, aux->id);
					fila_insere_inicio(pronto, aux->quant, aux->id, aux->tmpDuracao, aux->prioridade, aux->tipo);
				}
			}
		}
		
		atual = maiorPrioridade(pronto);
		
		if(aux->tipo == 1 && aux->cont == 0){
			aux->cont++;
			aux->tipo = 0;
			
			adicionaNoDe(espera, aux);
			
			fila_insere_inicio(pronto, aux->quant, aux->id, aux->tmpDuracao, aux->prioridade, 0);	
		}
		
		printf("\n\nEm Espera: \n"); fila_imprime(espera);
		
		if(espera->ini != NULL){
			fila_retira(espera);
		}
		
		if(verificaExitenciaDoProcesso(resultado, aux->id) != 1){
			fila_insere_inicio(resultado, aux->quant, aux->id, aux->tmpDuracao, aux->prioridade, aux->tipo);
		}else{
			retiraPorId(resultado, aux->id);
			fila_insere_inicio(resultado, aux->quant, aux->id, aux->tmpDuracao, aux->prioridade, aux->tipo);
		}
		
		ciclos++;
		Sleep(velocidade);
		system("cls");
	}
	t = clock() - t;
	printf("Tempo Real de execucao: %lfms", ((double)t)/((CLOCKS_PER_SEC/1000)));
	printf("\nCiclos da CPU: %d", ciclos);
	printf("\nTempo total da CPU: %.2fs", tmpTotal);
	imprimeResultadoFinal(resultado);
	fila_libera(resultado);
	fila_libera(espera);
	fila_libera(pronto);
	printf("\n");
	system("Pause");
}


void fila_imprime(FilaProcesso* fila){
	FilaProcessoNo *aux;
	
	for(aux = fila->ini; aux != NULL; aux = aux->prox){
		printf("%d\t", aux->id);
	}
}

FilaProcessoNo* retiraPorId(FilaProcesso* fila, int id){
	FilaProcessoNo* ant = NULL;
	FilaProcessoNo* p = fila->ini;
	
	while(p != NULL && p->id != id){
		ant = p;
		p = p->prox;
	}
	
	if(p != NULL){
		if(ant == NULL){
			fila->ini = p->prox;
		} else {
			ant->prox = p->prox;
		}
	}
	p->prox = NULL;
	return p;
}

void listaProcessos(FilaProcesso* fila){
	FilaProcessoNo* aux;
	char tipo[20];
	
	printf("----------------- Listagem de Processos -----------------\n\n");
	for(aux = fila->ini; aux != NULL; aux = aux->prox){
		if(aux->tipo == 0){
			strcpy(tipo, "CPU-Bound");
		} else{
			strcpy(tipo, "I/O-Bound");
		}
		printf("Processo: %d\t Prioridade: %d\t Tempo de duracao: %ds\t Tipo: %s\n", aux->id, aux->prioridade, aux->tmpDuracao, tipo);
	}
	printf("\n\n");
}

FilaProcessoNo* maiorPrioridade(FilaProcesso* fila){
	FilaProcessoNo* aux;
	FilaProcessoNo* noPrio;
	int prioridade = -1;
	
	for(aux = fila->ini; aux != NULL; aux = aux->prox){
		if(aux->prioridade > prioridade){
			prioridade = aux->prioridade;
			noPrio = aux;
		}
	}
	
	return noPrio;
}

void adicionaNoDe(FilaProcesso* fila, FilaProcessoNo* no){
	FilaProcessoNo* novo = (FilaProcessoNo*) malloc(sizeof(FilaProcessoNo));
	novo = no;
	
	if(fila->fim != NULL){
		fila->fim->prox = novo;
	}else{
		fila->ini = novo;
	}
	fila->fim = novo;
}

int verificaExitenciaDoProcesso(FilaProcesso* fila, int id){
	FilaProcessoNo* aux;
	
	for(aux = fila->ini; aux != NULL; aux = aux->prox){
		if(id == aux->id){
			return 1;
		}
	}
	return 0;
}

void imprimeResultadoFinal(FilaProcesso* fila){
	setlocale(LC_ALL, "Portuguese");
	FilaProcessoNo* aux;
	
	printf("\n\n------------------- Resultado dos Processos -------------------");
	for(aux = fila->ini; aux != NULL; aux = aux->prox){
		printf("\nProcesso: %d\t Prioridade: %d\t Quantidade de Vezes da CPU: %d\t Duração do Processo: %ds", aux->id, aux->prioridade, aux->quant, aux->tmpDuracao);
	}
	
	printf("\n\n-----Produzido por Felipe Coelho e Manoel Malon\n");
}

