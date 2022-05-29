#include "systemc.h"
#include <components/*>
#include <bits/stdc++.h>
using namespace std;

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

SC_MODULE(processor) {
    vector<sc_uint<32>> instructions;

    /*** Initializing Components ***/
    control CONTROL{"CONTROL"};
    pc PC{"PC"};
    inst_memory INST_MEMORY{"INST_MEMORY"};
    inst_register INST_REGISTER{"INST_REGISTER"};
    decoder DECODER{"DECODER"};
    pipeline_reg PIPELINE_REG{"PIPELINE_REG"};
    reg_bank REG_BANK{"REG_BANK"};
    alu ALU{"ALU"};
    data_memory DATA_MEMORY{"DATA_MEMORY"};
    mux_read MUX_READ{"MUX_READ"};
    mux_write MUX_WRITE{"MUX_WRITE"};

    /*** Initializing signals ***/
    sc_in<bool> clock;

    //** Control signals
    // PC
    sc_signal<bool> enable_pc_sig;
    sc_signal<bool> load_pc_sig;
    sc_signal<bool> reset_pc_sig;
    sc_signal<bool> jump_pc_sig;
    // Instruction Memory
    sc_signal<bool> enable_instr_memory_sig;
    sc_signal<bool> wr_instr_memory_sig;
    // Data memory 
    sc_signal<bool> enable_data_mem_sig;
    sc_signal<bool> wr_data_mem_sig;
    // Instruction register
    sc_signal<bool> enable_ir_sig;
    sc_signal<bool> wr_ir_sig;
    sc_signal<sc_uint<8>> immed_rd_sig;
	sc_signal<sc_uint<9>> immed_r_sig;
    // Registes bank
    sc_signal<bool> enable_reg_bank_sig;
    sc_signal<bool> wr_reg_bank_sig;
    // Pipeline
    sc_signal<bool> enable_pipeline_sig;
    sc_signal<bool> wr_pipeline_sig;
    sc_signal<bool> reset_pipeline_sig;
    // ALU
    sc_signal<bool> reset_z_n_sig;
    // Multiplexers
    sc_signal<sc_uint<2>> mux_read_sig;
    sc_signal<sc_uint<2>> mux_write_sig;
    
    //** Other signals
    sc_signal<sc_uint<5>> op_sig;	    // Opcode
	sc_signal<sc_uint<9>> rs_sig;		// Primeiro operando
	sc_signal<sc_uint<9>> rt_sig;		// Segundo operando
	sc_signal<sc_uint<9>> rd_sig;		// Operando destino
	
    sc_signal<sc_uint<5>> op_sig_pipe;	// Opcode
	sc_signal<sc_uint<9>> rs_sig_pipe;	// Primeiro operando
	sc_signal<sc_uint<9>> rt_sig_pipe;	// Segundo operando
	sc_signal<sc_uint<9>> rd_sig_pipe;	// Operando destino
	
    sc_signal<bool> zero_sig;
    sc_signal<bool> neg_sig;

	sc_signal<sc_uint<9>> pc_out_sig;
	sc_signal<sc_uint<32>> inst_sig;

    sc_signal<sc_uint<32>> inst_ir_sig;

	sc_signal<sc_uint<9>> rs_dm_sig;
	sc_signal<sc_int<32>> mem_out_sig;

	sc_signal<sc_uint<9>> next_inst_sig;

	sc_signal<sc_int<32>> alu_out_sig;
	sc_signal<sc_int<32>> rs_alu_sig;
	sc_signal<sc_int<32>> rt_alu_sig;

	sc_signal<sc_int<32>> write_reg_sig;

	sc_signal<sc_uint<9>> write_mem_sig;

    // *** Constructor ***
    processor(sc_module_name _name, const char * instructionsPath) : sc_module{_name} {
        sleep(1);
        cout << "\n>> MIPS_PIPE Processor ir ready! <<" << endl;
        sleep(1);

        /*** Connecting ***/
        //** Control
        // In
        CONTROL.clock(clock);
        CONTROL.neg(neg_sig);
        CONTROL.zero(zero_sig);
        CONTROL.op(op_sig_pipe);
        CONTROL.rs(rs_sig_pipe);
        CONTROL.rt(rt_sig_pipe);
        CONTROL.rd(rd_sig_pipe);
        // Out
        CONTROL.enable_pc(enable_pc_sig);
        CONTROL.load_pc(load_pc_sig);
        CONTROL.reset_pc(reset_pc_sig);
        CONTROL.jump_pc(jump_pc_sig);
        
        CONTROL.enable_instr_memory(enable_instr_memory_sig);
        CONTROL.wr_instr_memory(wr_instr_memory_sig);
        
        CONTROL.enable_ir(enable_ir_sig);
        CONTROL.wr_ir(wr_ir_sig);
        CONTROL.immed_r(immed_r_sig);
        CONTROL.immed_rd(immed_rd_sig);

        CONTROL.enable_reg_bank(enable_reg_bank_sig);
        CONTROL.wr_reg_bank(wr_reg_bank_sig);

        CONTROL.enable_pipeline(enable_pipeline_sig);
        CONTROL.wr_pipeline(wr_pipeline_sig);
        CONTROL.reset_pipeline(reset_pipeline_sig);    

        CONTROL.reset_z_n(reset_z_n_sig);

        CONTROL.mux_read(mux_read_sig);
        CONTROL.mux_write(mux_write_sig);

        CONTROL.enable_data_mem(enable_data_mem_sig);
        CONTROL.wr_data_mem(wr_data_mem_sig);

        //** Instruction Memory
        // In
        INST_MEMORY.clock(clock);
        INST_MEMORY.enable(enable_ir_sig);
        INST_MEMORY.wr(wr_ir_sig);
        INST_MEMORY.inst_pc(pc_out_sig);
        // Out
        INST_MEMORY.inst(inst_sig);

        // Instruction register
        // In
        INST_REGISTER.clock(clock);
        INST_REGISTER.enable(enable_ir_sig);
        INST_REGISTER.wr(wr_ir_sig);
        INST_REGISTER.inst_in(inst_ir_sig);
        // Out
        INST_REGISTER.inst_out(inst_sig);

        //** Data Memory
        // In
        DATA_MEMORY.clock(clock);
        DATA_MEMORY.enable(enable_data_mem_sig);
        DATA_MEMORY.wr(wr_data_mem_sig);
        DATA_MEMORY.address(write_mem_sig);
        DATA_MEMORY.rs(rs_alu_sig);
        // Out
        DATA_MEMORY.mem_out(mem_out_sig);

        //** Decoder
        // In
        DECODER.inst(inst_sig);
        // Out
        DECODER.op(op_sig);
        DECODER.rs(rs_sig);
        DECODER.rt(rt_sig);
        DECODER.rd(rd_sig);

        //** PC
        // In
        PC.clock(clock);
        PC.enable(enable_pc_sig);
        PC.load(load_pc_sig);
        PC.reset(reset_pc_sig);
        PC.next_inst(next_inst_sig);
        // Out
        PC.pc_out(pc_out_sig);

        //** Pipeline register
        // In
        PIPELINE_REG.clock(clock);
        PIPELINE_REG.enable(enable_pipeline_sig);
        PIPELINE_REG.wr(wr_pipeline_sig);
        PIPELINE_REG.op_in(op_sig);
        PIPELINE_REG.rs_in(rs_sig);
        PIPELINE_REG.rt_in(rt_sig);	
        PIPELINE_REG.rd_in(rd_sig);

        // Out
        PIPELINE_REG.op_out(op_sig_pipe);	
        PIPELINE_REG.rs_out(rs_sig_pipe);
        PIPELINE_REG.rt_out(rt_sig_pipe);
        PIPELINE_REG.rd_out(rd_sig_pipe);	

        //** ALU
        // In
        ALU.rs(rs_alu_sig);
        ALU.rt(rt_alu_sig);
        ALU.alu_op(op_sig_pipe);
        ALU.reset_z_n(reset_z_n_sig); 
        // Out
        ALU.alu_out(alu_out_sig); 
        ALU.zero(zero_sig);
        ALU.neg(neg_sig);

        //** Register bank
        // In
        REG_BANK.clock(clock);
        REG_BANK.enable(enable_reg_bank_sig);
        REG_BANK.wr(wr_reg_bank_sig);
        REG_BANK.rs(rs_sig_pipe);
        REG_BANK.rt(rt_sig_pipe);
        REG_BANK.rd(rd_sig_pipe);
        REG_BANK.write_reg(write_reg_sig);
        // Out
        REG_BANK.op_1(rs_alu_sig);
        REG_BANK.op_2(rt_alu_sig);

        //** Mux_read (write reg)
        // In
        MUX_READ.selector(write_reg_sig);
        MUX_READ.mem_out(mem_out_sig);
        MUX_READ.alu_out(alu_out_sig);
        MUX_READ.rs(immed_rd_sig);
        // Out
        MUX_READ.write_reg(write_reg_sig);

        //** Mux_write (write mem)
        // In
        MUX_WRITE.selector(write_mem_sig);
        MUX_WRITE.rs(rs_sig_pipe);
        MUX_WRITE.rd(rd_sig_pipe);
        // Out
        MUX_WRITE.write_mem(write_mem_sig);

        /*** Sensitivity List ***/
        sensitive << clock.pos();

        /*** Reading instructions from file ***/
        ifstream ifStream;
		string inst;
		int size = 0;
		ifStream.open(instructionsPath, ifstream::in);

        cout << "\t=============================" << endl;
        cout << "\n*** Trying to find instructions... ***\n";

        if (ifStream.is_open()) {
			cout << " >> Instructions found! Processing them..." << endl;
			sleep(1);

			while(getline(ifStream, inst)) {
				size_t found = inst.find("#");

				if(found != string::npos && found != 0){
					instructions.push_back(instStringToInt(inst.substr(0, found)));
					size++;
				} else if(found == string::npos){
					instructions.push_back(instStringToInt(inst));
					size++;
				}
			}

			cout << "> Loading into instruction memory..." << endl;
			sleep(1);

			IM.updateMemory(instructions); // Update memory
			cout << "\t> Done! Instructions to be processed: <"> << endl;
			for (int i = 0; i < instructions.size(); ++i) {
				cout << instructions[i] << endl;
			}
		} else {
			cout << ">> Error! Could not find the file with the instructions..." << endl;
		}

        cout << "\t=============================" << endl;
        sleep(1);
    }
};
