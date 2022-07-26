
typedef enum tipo Tipo;

typedef struct filaprocessono FilaProcessoNo;

typedef struct filaprocesso FilaProcesso;

FilaProcesso* fila_cria();

FilaProcessoNo* fila_retira(FilaProcesso* fila);

void fila_libera(FilaProcesso* fila);

void fila_imprime(FilaProcesso* fila);

void escalonaProcesso(FilaProcesso* pronto);         

void listaProcessos(FilaProcesso* fila);

FilaProcessoNo* retiraPorId(FilaProcesso* fila, int id);
         
void fila_insere_inicio(FilaProcesso* f, int quant, int id, int tempoDuracao, int prioridade, int tipo);

void fila_insere_final(FilaProcesso* fila, int quant, int id, int tempoDuracao, int prioridade, int tipo);

FilaProcessoNo* maiorPrioridade(FilaProcesso* fila);

void adicionaNoDe(FilaProcesso* fila, FilaProcessoNo* no);

int verificaExitenciaDoProcesso(FilaProcesso* fila, int id);

void imprimeResultadoFinal(FilaProcesso* fila);                                                                                                                                                                                                                                                                                                                                                                                                                                                                                          
