#ifndef CONTROL
#define CONTROL

#include "systemc.h"

SC_MODULE(control){
    // *** Signals ***
    sc_in_clk clock;
    sc_in<sc_uint<5>> op;
    sc_in<bool> neg;
    sc_in<bool> zero;
    sc_in<sc_uint<9>> rs;	
    sc_in<sc_uint<9>> rt;
    sc_in<sc_uint<9>> rd;	

    sc_out<sc_uint<2>> selector_mux_read;
    sc_out<sc_uint<2>> selector_mux_write;

    // PC
    sc_out<bool> enable_pc;
    sc_out<bool> wr_pc;
    sc_out<bool> reset_pc;
    sc_out<sc_unit<9>> jump_pc;
    // Instruction Memory
    sc_out<bool> enable_instr_memory;
    sc_out<bool> wr_instr_memory;
    // Data memory 
    sc_out<bool> enable_data_mem;
    sc_out<bool> wr_data_mem;
    // Registes bank
    sc_out<bool> enable_reg_bank;
    sc_out<bool> wr_reg_bank;
    // Pipeline
    sc_out<bool> enable_pipeline;
    sc_out<bool> wr_pipeline;
    sc_out<bool> reset_pipeline;    
    // ALU
    sc_out<bool> reset_z_n;
    // FIXME: cadê o alu_op q vai pra ula????????????

    // *** Methods ***
    void operate() {
        // TODO:
    }

    // *** Constructor ***
    SC_CTOR(control) {
        cout << ">> Control" << endl;
        SC_METHOD(operate);
        sensitive << clock.pos();
    }
}

#endif