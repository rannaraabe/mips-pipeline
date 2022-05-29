# MIPS Pipeline - SystemC

Constantes que serão utilizadas:
 - INST_SIZE 32:     Tamanho da palavra de instrucao
 - WORD_SIZE 32: 	 Tamanho da palavra de dados

 - OPCODE 5:		 Tamanho da papalvra do opcode
 - RS 9:             Tamanho do primeiro operando
 - RT 9:             Tamanho do segundo operando
 - RD 9:             Tamanho do operando de saída

 - ALU_CODE 5: 		 Tamanho da palavra do opcode que chega na ULA

 - REG_SIZE 9: 	     Tamanho dos registradores
 - REG_BANK_SIZE 512: 	  Quantidade de registradores (tamanho do banco de registradores) (2^(REG_SIZE))

 - WORD_ADDRESSING_SIZE 9:  Tamanho da palavra endereçamento da memória de dados e de instrução
 - MEM_SIZE 512: 	 Quantidade de blocos de memoria (2^(WORD_ADDRESSING_SIZE))


## To compile

g++ -I. -I$SYSTEMC_HOME/include -L. -L$SYSTEMC_HOME/lib-linux64 main.cpp -o main -lsystemc -lm -std=c++11


Desenvolvido por @camiladuartes_ e @rannaraabe, 2022.
