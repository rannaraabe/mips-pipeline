#include "systemc.h"

/*** Componentes que serão utilizados ***/
#include "./components/and.h"
// TODO: importar os componentes que foram implementados

/*** Constantes que serão utilizadas ***/
#define TAM_INSTRUCAO 32    // Tamanho de instrucao
#define TAM_PALAVRA 32 	    // Tamanho da palavra de dado FIXME:
#define OPCODE 5		    // Tamanho da palavra de opcode
#define ULA_CODE 3 		    // Tamanho da palavra de ULA opcode
#define ENDEREC_REG 5 	    // Tamanho da palavra de endereçamento
#define TAM_BANCO_REG 32 	// Quantidade de registradores (2^(ENDEREC_REG))
#define TAM_PALAVRA_ENDEREC 7 	    // Tamanho da palavra de endereçamento
#define TAM_MEM_INST 128 	    // Quantidade de blocos de memoria (2^(ENDEREC_MEM))

int sc_main(int argc, char* argv[]) {
    /*** Declaração dos componentes que serão utilizados ***/

    // TODO: adicionar clock
    // PC
	PC pc("PC");
    // Memoria de intrucao
    MemoriaInstrucao memoriaInst("MemoriaInstrucao");
    // Decodificador
    Decodificador decodificador("Decodificador");
    // Parte de Controle
    Controle controle("Controle");
	// Banco de registradores
	BancoRegistradores bancoReg("BancoRegistradores");
	// Unidade Logica Aritmetica
	ULA ula("ULA");
	// Memoria de dados
	MemoriaDados memoriaDados("MemoriaDados");
	// Multiplexadores
	Multiplexador mux1("Multiplexador1");
	Multiplexador mux2("Multiplexador2");
	Multiplexador mux3("Multiplexador3");
	Multiplexador mux4("Multiplexador4");
    // Shift-left
    ShiftLeft sl("ShiftLeft");
    // Extensor de 16 bits para 32 bits
    Ext ext("Ext");
    // Somador
    Somador4 somador1("Somador1");
    Somador32 somador2("Somador2");
    // Branch
    And portaAnd("AND");

    // TODO: Criar sinais da implementacao dedicada (sinais de input e output de cada componente)


    sc_start();
    return 0;
}
