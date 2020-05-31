//read.h - a tiny header-only library to allow input in C++ to be more sensible
//by ShakaUVM
#include <iostream>
#include <fstream>
#include <string>

//This will read a T from cin, reprompting if they type in something wrong
//Examples:
//int x = read<int>();
//const int x = read<int>("Please enter an int");
//string s = read<string>("What is your name?");
template<class T>
T read(const std::string prompt = "") {
	while (true) {
		T retval{};
		std::cout << prompt;
		std::cin >> retval;
		if (std::cin.eof()) //We reached the end of file, or the user hit ctrl-d
			return {}; //Alternatively, we could throw an exception
		if (!std::cin) {
			std::cin.clear(); //Clear error code
			std::string s;
			std::cin >> s; //Remove the word that caused the error
			continue;
		}
		return retval;
	}
}

//This version reads from a file, so no prompt
//Example:
//auto d = read<double>(file);
template<class T>
T read(std::istream &ins) {
	while (true) {
		T retval{};
		ins >> retval;
		if (ins.eof()) //We reached the end of file, or the user hit ctrl-d
			return {};
		if (!ins) {
			ins.clear(); //Clear error code
			std::string s;
			ins >> s; //Remove the word that caused the error
			continue;
		}
		return retval;
	}
}

//To read a whole line of text, analogue to getline
std::string readline(const std::string prompt = "") {
	//Eliminate a common bug when switching from >> to getline, the >> will leave a newline in the input buffer
	std::string retval;
	std::cout << prompt;
	std::cin >> std::ws;
	std::getline(std::cin,retval);
	if (std::cin.eof()) //We reached the end of file, or the user hit ctrl-d
		return {};
	if (!std::cin)
		throw std::runtime_error("Error within the readline function.");
	return retval;
}

//Readline for reading from a file
std::string readline(std::istream &ins) {
	std::string retval;
	ins >> std::ws;
	std::getline(ins,retval);
	if (ins.eof()) //We reached the end of file, or the user hit ctrl-d
		return {};
	if (!ins)
		throw std::runtime_error("Error within the readline function.");
	return retval;
}
