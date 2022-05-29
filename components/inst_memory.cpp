#include "systemc.h"
#include <bits/stdc++.h>
using namespace std;

SC_MODULE(inst_memory){
    // *** Signals ***
	sc_in<bool> clock;
	sc_in<bool> enable;
	sc_in<bool> wr;
    sc_in<sc_int<9>> address;
    sc_in<sc_int<32>> inst_pc;
	sc_out<sc_int<32>> inst;

    // *** Methods ***
    void operate() {
        while(true){
            wait();
            if(enable.read()){
                if(wr.read()){
                    mem_data[address.read()] = inst_pc.read();
                } else {
                    inst.write(mem_data[address.read()]);
                }
            }
        }
    }

    void update_memory(const vector<sc_int<32>> &d){
        for(int i=0; i<d.size(); i++){
            mem_data[i] = d[i];
        }
    }

    // *** Constructor ***
    SC_HAS_PROCESS(inst_memory);
    inst_memory (sc_module_name smn) : sc_module {smn} {
        cout << ">> Instruction Memory" << endl;
        SC_METHOD(operate);
        sensitive << clock.pos();

        mem_data = new sc_signal<sc_int<32>>[512];
    }

     private: 
        sc_signal<sc_int<32>> *mem_data;
};