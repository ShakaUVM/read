//read.h - a tiny header-only library to allow input in C++ to be more sensible
//by ShakaUVM
#include <iostream>
#include <fstream>
#include <string>
#include <optional>

//Simplest Version, Designed for Newbies
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

//Returns an optional. So if your specified type is not read, the caller will know this rather than silently discarding the error
//Recommended for more advanced programmers than read()
//If an error occurs, it does not affect the input stream at all and clears the failbit
// Example:
// auto a = read_opt<int>(); 
// if (!a) exit(EXIT_FAILURE);
// cout << *a << endl;
template<class T>
std::optional<T> read_opt(const std::string prompt = "") {
	T retval{};
	std::cout << prompt;
	std::cin >> retval;
	if (std::cin.eof()) //We reached the end of file, or the user hit ctrl-d
		return std::nullopt;  //Return that nothing was read
	if (!std::cin) {
		std::cin.clear(); //Clear error code, so the user can try again when they like
		return std::nullopt;  //Return that nothing was read
	}
	return retval;
}

//This version reads from a file, so no prompt
//Like the other read_opt, returns nullopt if it didn't read what was expected
template<class T>
std::optional<T> read_opt(std::istream &ins) {
	T retval{};
	ins >> retval;
	if (ins.eof()) //We reached the end of file, or the user hit ctrl-d
		return std::nullopt;  //Return that nothing was read
	if (!ins) {
		ins.clear(); //Clear error code, so the user can try again when they like
		return std::nullopt;  //Return that nothing was read
	}
	return retval;
}

//Simplest read possible: int x = read();
//Credit: /u/9cantthinkofgoodname
//https://old.reddit.com/r/cpp/comments/gtzsnm/we_need_to_do_better_than_cin_for_new_programmers/fsx6z7x/
//However, int x = read(ins) is about 20% slower than using read<int>(ins), though
//There's probably some template tricks we can use to eliminate the while loop when reading from a file
struct Reader {
	template<class T>
		operator T() {
			while(true) {
				T retval{};
				std::cout << prompt;
				ins >> retval;
				if(ins.eof() ) //We reached the end of file, or the user hit ctrl-d
					return {}; //Alternatively, we could throw an exception
				if(!ins) {
					ins.clear(); //Clear error code
					std::string s;
					ins >> s; //Remove the word that caused the error
					continue;
				}
				return retval;
			}
		}
	std::istream &ins;
	const std::string prompt;
};

auto read(const std::string prompt = "") {
	return Reader{std::cin,prompt};
}
auto read(std::istream &ins) {
	return Reader{ins,""};
}
