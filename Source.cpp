#include "workflow.h";
#include <iostream>
int main() {
	try {
		Workflow* wf = new Workflow("notworkingworkflow.txt");
		wf->run_instruction();
	}
	catch (std::string str) {
		std::cout << "ERROR: " + str + "\n";
	}
	return 0;
}