#ifndef DATA_MEMORY
#define DATA_MEMORY

#include "systemc.h"
#include <bits/stdc++.h>

using namespace std;

SC_MODULE(data_memory){
    // *** Signals ***
    sc_in<bool> clock;
    sc_in<bool> enable;
    sc_in<bool> wr;

    sc_in<sc_int<9>> address;
    sc_in<sc_int<32>> rs;
    sc_out<sc_int<32>> mem_out;

    // *** Methods ***
    void operate() {
        while(true){
            wait();
            if(enable.read()){
                if(wr.read()){
                    mem_data[address.read()] = rs.read();
                } else {
                    mem_out.write(mem_data[address.read()]);
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
    SC_HAS_PROCESS(data_memory);
    data_memory (sc_module_name smn) : sc_module {smn} {
        cout << ">> Data memory " << smn << endl;
        SC_METHOD(operate);
        sensitive << clock.pos();

        mem_data = new sc_signal<sc_int<32>>[512];
    }

    private: 
        sc_signal<sc_int<32>> *mem_data; 
};

#endif