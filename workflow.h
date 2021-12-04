#include <map>
#include <vector>
#include <string>


class Command {
private:
	
public:
	std::vector <std::string> operands;
	virtual std::vector<std::string> run(std::vector<std::string> text);
};

class readfile : public Command {
public:
	readfile(std::vector <std::string> v);
	virtual std::vector<std::string> run(std::vector<std::string> text);
};

class writefile : public Command {
public:
	writefile(std::vector <std::string> v);
	virtual std::vector<std::string> run(std::vector<std::string> text);
};

class grep : public Command {
public:
	grep(std::vector <std::string> v);
	virtual std::vector<std::string> run(std::vector<std::string> text);
};

class sort : public Command {
public:
	sort(std::vector <std::string> v);
	virtual std::vector<std::string> run(std::vector<std::string> text);
};

class replace : public Command {
public:
	replace(std::vector <std::string> v);
	virtual std::vector<std::string> run(std::vector<std::string> text);
};

class dump : public Command {
public:
	dump(std::vector <std::string> v);
	virtual std::vector<std::string> run(std::vector<std::string> text);
};

class Workflow {
private:
	std::map <unsigned int, Command*> interpretations;
	std::vector <unsigned int> instruction;
public:
	Workflow(std::string file_in);
	int run_instruction();



};
