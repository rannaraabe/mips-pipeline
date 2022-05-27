#ifndef CONTROL
#define CONTROL

#include "systemc.h"

SC_MODULE(control){
    // *** Signals ***
    // TODO:

    // *** Methods ***
    void operate() {
        // TODO:
    }

    // *** Constructor ***
    SC_CTOR(control) {
        cout << ">> Control" << endl;
        SC_METHOD(operate);
        // TODO: sensitive
    }
}

#endif