#include "systemc.h"
#include <bits/stdc++.h>
#include <unistd.h>

#include "./components/alu.h"
#include "./components/control.cpp"
#include "./components/data_memory.h"
#include "./components/decoder.cpp"
#include "./components/inst_memory.cpp"
#include "./components/inst_register.cpp"
#include "./components/mux_read.cpp"
#include "./components/mux_write.cpp"
#include "./components/pc.cpp"
#include "./components/pipeline_reg.h"
#include "./components/reg_bank.h"

using namespace std;

map<std::string, int> op_map = {
    {"AND",1},
    {"OR", 2},
    {"XOR",3},
    {"NOT",4},
    {"CMP",5},
    {"ADD",6},
    {"SUB",7},
    {"LD", 8},
    {"ST", 9},
    {"J",  10},
    {"JN", 11},
    {"JZ", 12},
};

int txtToInst(string op, int rs, int rt, int rd){
    int op = op_map[op];
    return op + (rs<<5) + (rt<<14) + (rd<<23);
}

SC_MODULE(processor) {
    vector<sc_int<32>> instructions;

    /* Initializing Components */
    control CONTROL{"CONTROL"};
    pc PC{"PC"};
    inst_memory INST_MEMORY{"INST_MEMORY"};
    inst_register INST_REGISTER{"INST_REGISTER"};
    decoder DECODER{"DECODER"};
    pipeline_reg PIPELINE_REG{"PIPELINE_REG"};
    reg_bank REG_BANK_{"REG_BANK"};
    alu ALU_{"ALU"};
    data_memory DATA_MEMORY_{"DATA_MEMORY"};
    mux_read MUX_READ{"MUX_READ"};
    mux_write MUX_WRITE{"MUX_WRITE"};

    /* Initializing signals */
    sc_in<bool> clock;

    /* Control signals */
    // PC
    sc_signal<bool> enable_pc_sig;
    sc_signal<bool> load_pc_sig;
    sc_signal<bool> reset_pc_sig;
    sc_signal<bool> jump_pc_sig;
    // Instruction Memory
    sc_signal<bool> enable_instr_memory_sig;
    sc_signal<bool> wr_instr_memory_sig;
    sc_signal<sc_int<32>> in_instr_memory_sig;
    // Data memory 
    sc_signal<bool> enable_data_mem_sig;
    sc_signal<bool> wr_data_mem_sig;
    // Instruction register
    sc_signal<bool> enable_ir_sig;
    sc_signal<bool> wr_ir_sig;
    sc_signal<sc_int<9>> immed_rd_sig;
	sc_signal<sc_int<9>> immed_r_sig;
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
    sc_signal<sc_int<2>> mux_read_sig;
    sc_signal<sc_int<2>> mux_write_sig;
    
    /* Other signals */
    sc_signal<sc_int<5>> op_sig;	    // Opcode
	sc_signal<sc_int<9>> rs_sig;		// Primeiro operando
	sc_signal<sc_int<9>> rt_sig;		// Segundo operando
	sc_signal<sc_int<9>> rd_sig;		// Operando destino
	
    sc_signal<sc_int<5>> op_sig_pipe;	// Opcode
	sc_signal<sc_int<9>> rs_sig_pipe;	// Primeiro operando
	sc_signal<sc_int<9>> rt_sig_pipe;	// Segundo operando
	sc_signal<sc_int<9>> rd_sig_pipe;	// Operando destino
	
    sc_signal<bool> zero_sig;
    sc_signal<bool> neg_sig;

	sc_signal<sc_int<9>> pc_out_sig;
	sc_signal<sc_int<32>> inst_sig;

    sc_signal<sc_int<32>> inst_ir_sig;

	sc_signal<sc_int<9>> rs_dm_sig;
	sc_signal<sc_int<32>> mem_out_sig;

	sc_signal<sc_int<9>> next_inst_sig;

	sc_signal<sc_int<32>> alu_out_sig;
	sc_signal<sc_int<32>> rs_alu_sig;
	sc_signal<sc_int<32>> rt_alu_sig;

	sc_signal<sc_int<32>> write_reg_sig;
	sc_signal<sc_int<9>> write_mem_sig;

    /* Constructor */
    processor(sc_module_name smn, const char * instructionsPath) : sc_module{smn} {
        sleep(1);
        cout << "\n>> MIPS_PIPE Processor ir ready! <<" << endl;
        sleep(1);

        /* Connecting */
        // Control
        CONTROL.clock(clock);
        CONTROL.neg(neg_sig);
        CONTROL.zero(zero_sig);
        CONTROL.op(op_sig_pipe);
        CONTROL.rs(rs_sig_pipe);
        CONTROL.rt(rt_sig_pipe);
        CONTROL.rd(rd_sig_pipe);
        
        CONTROL.enable_pc(enable_pc_sig);
        CONTROL.load_pc(load_pc_sig);
        CONTROL.reset_pc(reset_pc_sig);
        CONTROL.jump_pc(next_inst_sig); // era jump_pc_sig
        
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

        /* Instruction Memory */
        INST_MEMORY.clock(clock);
        INST_MEMORY.enable(enable_instr_memory_sig);
        INST_MEMORY.wr(wr_instr_memory_sig);
        INST_MEMORY.address(pc_out_sig);
        INST_MEMORY.inst_pc(in_instr_memory_sig);
        
        INST_MEMORY.inst(inst_ir_sig); // FIXME: inst_ir_sig

        /* Instruction register */
        INST_REGISTER.clock(clock);
        INST_REGISTER.enable(enable_ir_sig);
        INST_REGISTER.wr(wr_ir_sig);
        INST_REGISTER.inst_in(inst_ir_sig);
        
        INST_REGISTER.inst_out(inst_sig); // FIXME:

        /* Data Memory */
        DATA_MEMORY_.clock(clock);
        DATA_MEMORY_.enable(enable_data_mem_sig);
        DATA_MEMORY_.wr(wr_data_mem_sig);
        DATA_MEMORY_.address(write_mem_sig);
        DATA_MEMORY_.rs(rs_alu_sig);
        
        DATA_MEMORY_.mem_out(mem_out_sig);

        /* Decoder */
        DECODER.inst(inst_sig);
        
        DECODER.op(op_sig);
        DECODER.rs(rs_sig);
        DECODER.rt(rt_sig);
        DECODER.rd(rd_sig);

        /* PC */
        PC.clock(clock);
        PC.enable(enable_pc_sig);
        PC.load(load_pc_sig);
        PC.reset(reset_pc_sig);
        PC.next_inst(next_inst_sig);
        
        PC.pc_out(pc_out_sig);

        /* Pipeline register */
        PIPELINE_REG.clock(clock);
        PIPELINE_REG.enable(enable_pipeline_sig);
        PIPELINE_REG.wr(wr_pipeline_sig);
        PIPELINE_REG.op_in(op_sig);
        PIPELINE_REG.rs_in(rs_sig);
        PIPELINE_REG.rt_in(rt_sig);	
        PIPELINE_REG.rd_in(rd_sig);

        PIPELINE_REG.op_out(op_sig_pipe);	
        PIPELINE_REG.rs_out(rs_sig_pipe);
        PIPELINE_REG.rt_out(rt_sig_pipe);
        PIPELINE_REG.rd_out(rd_sig_pipe);	

        /* ALU */
        ALU_.rs(rs_alu_sig);
        ALU_.rt(rt_alu_sig);
        ALU_.alu_op(op_sig_pipe);
        ALU_.reset_z_n(reset_z_n_sig); 
        
        ALU_.alu_out(alu_out_sig); 
        ALU_.zero(zero_sig);
        ALU_.neg(neg_sig);

        /* Register bank */
        REG_BANK_.clock(clock);
        REG_BANK_.enable(enable_reg_bank_sig);
        REG_BANK_.wr(wr_reg_bank_sig);
        REG_BANK_.rs(rs_sig_pipe);
        REG_BANK_.rt(rt_sig_pipe);
        REG_BANK_.rd(rd_sig_pipe);
        REG_BANK_.write_reg(write_reg_sig);
        
        REG_BANK_.op_1(rs_alu_sig);
        REG_BANK_.op_2(rt_alu_sig);

        /* Mux (write_reg) */
        MUX_READ.selector(mux_read_sig); // era: write_reg_sig
        MUX_READ.mem_out(mem_out_sig);
        MUX_READ.alu_out(alu_out_sig);
        MUX_READ.rs(immed_rd_sig); 

        MUX_READ.write_reg(write_reg_sig);

        /* Mux (write_mem) */
        MUX_WRITE.selector(mux_write_sig); // era: write_mem_sig
        MUX_WRITE.rs(rs_sig_pipe);
        MUX_WRITE.rd(rd_sig_pipe);
        
        MUX_WRITE.write_mem(write_mem_sig);

        /* Sensitivity List */
        sensitive << clock.pos();

        /* Reading instructions from file */
        ifstream ifStream;
		string inst;
		int size = 0;
		ifStream.open(instructionsPath, ifstream::in);

        cout << "\t=============================" << endl;
        cout << "\n** Trying to find instructions... **\n";
	    sleep(1);
        string inst;
		int size = 0;
		while(getline (cin, inst)){
            stringstream ss(inst); 
            string op;
            ss >> operation;

            vector<int> rs_rt_rd;
            int data;
            while(ss >> data){
                rs_rt_rd.push_back(data);
            }

            while(rs_rt_rd.size() < 3){
                rs_rt_rd.push_back(0);
            }

            if(op != "Memset"){
                sc_int<32> instruction = txtToInst(op, rs_rt_rd[0], rs_rt_rd[1], rs_rt_rd[2]);
                instructions.push_back(instruction);
            }
            else{
                int pos = rs_rt_rd[0];
                sc_int<32> data = rs_rt_rd[1];
                DATA_MEMORY_.update_memory_at_pos(data, pos);
            }
            
        }

        cout << "> Loading instructions into instruction memory..." << endl;
		sleep(1);
        INST_MEMORY.update_memory(instructions);
        cout << "\t> Done! Instructions to be processed: <" << endl;
        for (int i = 0; i < instructions.size(); ++i) {
            cout << instructions[i] << endl;
        }
        cout << "\t=============================" << endl;
        sleep(1);
    }
};