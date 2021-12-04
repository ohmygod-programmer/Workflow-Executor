#include "workflow.h"
#include <algorithm>
#include <fstream>

std::vector<std::string> Command::run(std::vector<std::string> text) {
	return text;
}

readfile::readfile(std::vector <std::string> v) {
	operands = v;
}

std::vector<std::string> readfile::run(std::vector<std::string> none) {
	std::vector <std::string> text;
	std::ifstream fin(operands[0]);
	if (!fin.is_open()) {
		throw "File is not opened";
	}
	size_t i = 0;
	std::string line;
	while (!fin.eof()) {
		getline(fin, line);
		text.push_back(line);
		i++;
	}

	return text;
}

writefile::writefile(std::vector <std::string> v) {
	operands = v;
}

std::vector<std::string> writefile::run(std::vector<std::string> text) {
	std::ofstream fout(operands[0]);
	size_t size = text.size();
	for (int i = 0; i < (int)size-1; i++) {
		fout << text[i] << "\n";
	}
	if (size > 0) {
		fout << text.back();
	}
	std::vector<std::string> none;
	return none;
}

grep::grep(std::vector <std::string> v) {
	operands = v;
}

std::vector<std::string> grep::run(std::vector<std::string> text) {
	std::vector<std::string> text_out;
	size_t size = text.size();
	for (size_t i = 0; i < size; i++) {
		std::vector<std::string> words; //Вектор слов
		size_t words_count;
		std::string word;
		size_t len = text[i].length();
		for (size_t j = 0; j < len; j++) {
			if (text[i][j] == ' ') {
				words.push_back(word);
				word = "";
			}
			else {
				word.push_back(tolower(text[i][j]));
			}
		}
		if (word.size() > 0) {
			words.push_back(word);
		}
		words_count = words.size();
		int flag = 0;
		for (size_t j = 0; j < words_count; j++) {
			if (words[j] == operands[0]) {
				flag = 1;
			}
		}
		if (flag) {
			text_out.push_back(text[i]);
		}
	};
	return text_out;
}

sort::sort(std::vector <std::string> v) {
	operands = v;
}

bool comparator(std::string s1, std::string s2) {
	return s1.compare(s2)<0;
}

std::vector<std::string> sort::run(std::vector<std::string> text) {
	std::sort(text.begin(), text.end());
	return text;
}

replace::replace(std::vector <std::string> v) {
	operands = v;
}

std::vector<std::string> replace::run(std::vector<std::string> text) {
	std::vector<std::string> text_out;
	size_t size = text.size();
	for (size_t i = 0; i < size; i++) {
		std::vector<std::string> words; //Вектор слов
		size_t words_count;
		std::string word;
		size_t len = text[i].length();
		for (size_t j = 0; j < len; j++) {//Разбиваю строку на слова
			if (text[i][j] == ' ') {
				words.push_back(word);
				word = "";
			}
			else {
				word.push_back(tolower(text[i][j]));
			}
		}
		if (word.size() > 0) {
			words.push_back(word);
		}
		words_count = words.size();
		for (size_t j = 0; j < words_count; j++) { //Произвожу замену слов
			if (words[j] == operands[0]) {
				words[j] = operands[1];
			}
		}
		std::string str = "";//Собираю строку обратно
		for (size_t j = 0; j < words_count-1; j++) {
			str.append(words[j]);
			str.push_back(' ');
		}
		str.append(words[words_count - 1]);
		text_out.push_back(str);
	};
	return text_out;
}


dump::dump(std::vector <std::string> v) {
	operands = v;
}

std::vector<std::string> dump::run(std::vector<std::string> text) {
	std::ofstream fout(operands[0]);
	size_t size = text.size();
	for (int i = 0; i < (int)size - 1; i++) {
		fout << text[i] << "\n";
	}
	if (size > 0) {
		fout << text.back();
	}
	std::vector<std::string> none;
	return text;
}



Command* do_command(std::vector <std::string> v) {
	std::vector <std::string> operands(v.begin()+3, v.end());
	if (v[2] == "readfile") {
		if (operands.size() < 1) {
			throw "Not enough args for command #" + v[0];
		}
		return new readfile(operands);
	}
	if (v[2] == "writefile") {
		if (operands.size() < 1) {
			throw "Not enough args for command #" + v[0];
		}
		return new writefile(operands);
	}
	if (v[2] == "grep") {
		if (operands.size() < 1) {
			throw "Not enough args for command #" + v[0];
		}
		return new grep(operands);
	}
	if (v[2] == "sort") {
		return new sort(operands);
	}
	if (v[2] == "replace") {
		if (operands.size() < 2) {
			throw "Not enough args for command #" + v[0];
		}
		return new replace(operands);
	}
	if (v[2] == "dump") {
		if (operands.size() < 1) {
			throw "Not enough args for command #" + v[0];
		}
		return new dump(operands);
	}
}

Workflow::Workflow(std::string file_in) {
	std::ifstream fin(file_in);
	std::string str;
	std::vector <std::string> str_el;
	
	getline(fin, str);
	if (str != "desc") {
		throw (std::string)"Incorrect file format";
	}

	while (getline(fin, str) && str != "csed") {
		std::string el;
		size_t len = str.length();
		for (int i = 0; i < len; i++) {
			if (str[i] == ' ') {
				str_el.push_back(el);
				el = "";
			}
			else {
				el.push_back(str[i]);
			}
		}
		str_el.push_back(el);
		if (!isdigit(str_el[0][0])) {
			throw "Incorrect number of command: " + str;
		}
		interpretations.insert(std::make_pair(stoi(str_el[0]), do_command(str_el)));
		str_el.clear();
	}

	if (str != "csed") {
		throw (std::string)"Incorrect file format";
	}

	getline(fin, str);
	size_t len = str.length();
	unsigned int el = 0;
	for (int i = 0; i < len; i++) {
		if (isdigit(str[i])) {
			el = el * 10 + str[i]-48;
		}
		if (el != 0 && !isdigit(str[i])) {
			instruction.push_back(el);
			el = 0;
		}
	}
	if (el != 0) {
		instruction.push_back(el);
	}
}

int Workflow::run_instruction() {
	size_t size = instruction.size();
	std::vector<std::string> text;
		for (size_t i = 0; i < size; i++) {
			if (interpretations.find(instruction[i]) == interpretations.end()) {
				throw (std::string)"Incorrect instruction";
			}
			text = interpretations[instruction[i]]->run(text);
		};
	return 0;
}