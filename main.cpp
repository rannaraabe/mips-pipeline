#include "systemc.h"
#include "processor.cpp"
#include <bits/stdc++.h>

int sc_main(int argc, char* argv[]) {
    
	cout << ">> Initializing processor..." << endl;
	sleep(1);
	processor mips_pipe("processor", argc[1]);
	sc_signal<bool> clock;
	mips_pipe.clock(clock);
	int cycles = 0;

	while(not sc_end_of_simulation_invoked()) {
		clock = 0;
		sc_start(1, SC_NS);
		clock = 1;
		sc_start(1, SC_NS);
		cycles++;
	}
	
	cout << "\n>> Finished after " << cycles - 4 << " cycles." << endl;	

    cout << ">> Final state: "
	cout << "> Registers:" << endl;
	mips_pipe.REGISTERS.print(10);

	cout << "> Memory:" << endl;
	mips_pipe.DM.print(10);
    return 0;
}
