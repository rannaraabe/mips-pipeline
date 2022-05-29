#ifndef DATA_MEMORY
#define DATA_MEMORY

#include "systemc.h"
#include <vector>

SC_MODULE(data_memory){
    // *** Signals ***
    sc_in<bool> clock;
    sc_in<bool> enable;
    sc_in<bool> wr;

    sc_in<sc_uint<9>> address;
    sc_in<32> rs;
    sc_out<32> mem_out;

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

    void update_memory(const vector<32> &d){
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

        mem_data = new sc_signal<32>[512];
    }

    private: 
        sc_signal<32> *mem_data; 
};

#endif