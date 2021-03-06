#ifndef PIPELINE
#define PIPELINE

#include "systemc.h"
using namespace std;

SC_MODULE(pipeline_reg) {
    // *** Signals ***
    sc_in_clk clock;
	sc_in<bool> enable;
	sc_in<bool> wr;

	sc_in<sc_int<5>> op_in;
	sc_in<sc_int<9>> rs_in;
	sc_in<sc_int<9>> rt_in;	
	sc_in<sc_int<9>> rd_in;

	sc_out<sc_int<5>> op_out;	
	sc_out<sc_int<9>> rs_out;
	sc_out<sc_int<9>> rt_out;
	sc_out<sc_int<9>> rd_out;	

    // *** Methods ***
    void operate() {
        while(true) {
            wait();
            if(enable.read()) {
                if(wr.read()) {
                    op = op_in.read();
                    rd = rd_in.read();
                    rs = rs_in.read();
                    rt = rt_in.read();
                    
                    // Write immediately
                    op_out.write(op);
                    rs_out.write(rs);
                    rt_out.write(rt);
                    rd_out.write(rd);
                } else {
                    op_out.write(op);
                    rs_out.write(rs);
                    rt_out.write(rt);
                    rd_out.write(rd);
                }
            }
        }
	}

    // *** Constructor ***
    SC_HAS_PROCESS(pipeline_reg);
	pipeline_reg (sc_module_name smn) : sc_module { smn } {
        cout << ">> Pipeline Register" << endl;
        SC_METHOD(operate);
        sensitive << clock.pos();
    };

	private:
	    int op, rs, rt, rd;

};

#endif