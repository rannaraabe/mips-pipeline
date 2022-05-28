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

    // PC
    sc_out<bool> enable_pc;
    sc_out<bool> wr_pc;
    sc_out<bool> reset_pc;
    sc_out<sc_unit<9>> jump_pc;
    // Instruction Memory
    sc_out<bool> enable_instr_memory;
    sc_out<bool> wr_instr_memory;
    // Instruction register
    sc_out<bool> enable_ir;
    sc_out<bool> wr_ir;
    sc_out<sc_uint<9>> immed_r;
    sc_out<sc_uint<9>> immed_rd;
    // Registes bank
    sc_out<bool> enable_reg_bank;
    sc_out<bool> wr_reg_bank;
    // Pipeline
    sc_out<bool> enable_pipeline;
    sc_out<bool> wr_pipeline;
    sc_out<bool> reset_pipeline;    
    // ALU
    sc_out<bool> reset_z_n;
    // Multiplexers
    sc_out<sc_uint<2>> mux_read;
    sc_out<sc_uint<2>> mux_write;
    // Data memory 
    sc_out<bool> enable_data_mem;
    sc_out<bool> wr_data_mem;

    // *** Constructor ***
    SC_CTOR(control) {
        cout << ">> Control" << endl;
        SC_METHOD(operate);
        sensitive << clock.pos();
    }

    // *** Methods ***
    private:
        // Local variables
        int curr_state = 0;
        bool restart_pipe = false;

        void turnInstAvailable() {
            enable_pc.write(1);
            enable_instr_memory.write(1);
            wr_instr_memory.write(0);
        }

        void prepareStoreInstInIR() {
            enable_pc.write(0);
            enable_instr_memory.write(0);
            enable_ir.write(1);
            wr_ir.write(1);
        }

        void prepareDecodedInstInIRtoPipe() {
            enable_ir.write(0);
            enable_pipeline.write(1);
            wr_pipeline.write(1);
        }

        void waitCycleAfterWriteIR() {
            enable_ir.write(0);
        }

        void operate() {
            switch(curr_state) {
                case 0:
                    turnInstAvailable();
                    curr_state = 1;
                    break;
                // Write instruction into the IR
                case 1:
                    prepareStoreInstInIR();
                    curr_state = 2;
                    break;
                // Write at pipeline decodeed
                case 2:
                    if(restart_pipe) {
                        curr_state = 0;
                        restart_pipe = false;
                    } else {
                        prepareDecodedInstInIRtoPipe();
                        curr_state = 3;
                    }
                    break;
                // Have written in pipeline
                case 3:
                    enable_pipeline.write(0);
                    curr_state = 4;
                    // Take a new instruction:
                    turnInstAvailable();
                    break;
                case 4:
                    /*
                     * Opcode:
                     * 1 -> 7: ULA
                     * 8: LD
                     * 9: ST
                     * 10: J
                     * 11: JN
                     * 12: JZ
                    */
                    prepareStoreInstInIR();
                    // LD operation (load)
                    if(op.read() == 8) {
                        mux_read.write(1);
                        mux_write.write(1);
                        enable_reg_bank.write(1);
                        wr_ref_bank.write(1);
                        enable_data_mem.write(1);
                        wr_data_mem.write(0);
                        curr_state = 9;
                    }
                    // ST operation (store)
                    else if(op.read() == 9) {
                        enable_reg_bank.write(1);
                        wr_ref_bank.write(0);
                        mux_write.write(0);
                        curr_state = 6;
                    }
                    // J operation
                    else if(op.read == 10) {
                        enable_pc.write(0);
                        wr_pc.write(1);
                        jump_pc.write(rd);
                        restart_pipe = true;
                        curr_state = 7;
                    }
                    // JN operation
                    else if(op.read() == 11) {
                        if(neg.read()) {
                            jump_pc.write(rd);
                            enable_pc.write(0);
                            wr_pc.write(1);
                            reset_z_n.write(true);
                            restart_pipe = true;
                        }
                    }
                    //JZ operation
                    else if(op.read == 12) {
                        if(zero.read()) {
                            jump_pc.write(rd);
                            enable_pc.write(0);
                            wr_pc.write(1);
                            reset_z_n.write(true);
                            restart_pipe = true;
                        }
                    }
                    // ALU operations
                    else if(op.read() != 0) {
                        mux_read.write(0);
                        enable_reg_bank.write(1);
                        wr_reg_bank.write(0);
                        curr_state = 5;
                    }
                    else if(op.read() == 0) {
                        sc_stop();
                    }
                    break;
                // ALU operations execution
                case 5:
                    enable_reg_bank.write(1);
                    wr_reg_bank.write(1);
                    curr_state = 8;
                    // Stop propagation of pipeline:
                    waitCycleAfterWriteIR();
                    break;
                // ST operation execution
                case 6:
                    enable_data_mem.write(1);
                    wr_data_mem.write(1);
                    curr_state = 8;
                    // Stop propagation of pipeline:
                    waitCycleAfterWriteIR();
                    break;
                // Jumps execution
                case 7:
                    wr_pc.write(0);
                    curr_state = 2;
                    break;
                // Storing ALU operations results
                case 8:
                    enable_reg_bank.write(0);
                    enable_data_mem.write(0);
                    curr_state = 2;
                    // Stop propagation of pipeline:
                    waitCycleAfterWriteIR();
                    break;
                // LD operation execution
                case 9:
                    enable_reg_bank.write(1);
                    wr_reg_bank.write(1);
                    curr_state = 8;
                    break;
            }
        }
}

#endif