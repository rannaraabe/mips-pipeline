#include "systemc.h"

/*** Componentes que serão utilizados ***/
#include "./components/alu.h"
#include "./components/reg_bank.h"
#include "./components/data_memory.h"
#include "./components/decoder.h"
#include "./components/inst_memory.h"
// TODO: importar os componentes que foram implementados

/*** Constantes que serão utilizadas ***/
#define INST_SIZE 32            // Tamanho da palavra de instrucao
#define WORD_SIZE 32 	        // Tamanho da palavra de dados

#define OPCODE 5		        // Tamanho da papalvra do opcode
#define RS 9                    // Tamanho do primeiro operando
#define RT 9                    // Tamanho do segundo operando
#define RD 9                    // Tamanho do operando de saída

#define ALU_CODE 5 		        // Tamanho da palavra do opcode que chega na ULA

#define REG_SIZE 9 	            // Tamanho dos registradores
#define REG_BANK_SIZE 512 	    // Quantidade de registradores (tamanho do banco de registradores) (2^(REG_SIZE))

#define WORD_ADDRESSING_SIZE 9 	// Tamanho da palavra endereçamento da memória de dados e de instrução
#define MEM_SIZE 512 	        // Quantidade de blocos de memoria (2^(WORD_ADDRESSING_SIZE))

int sc_main(int argc, char* argv[]) {
    /*** Declaração dos componentes que serão utilizados ***/
        
    // TODO: Declarar todos os componentes

    // TODO: Criar sinais da implementacao dedicada (sinais de input e output de cada componente)

    sc_start();
    return 0;
}
