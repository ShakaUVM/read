//read.h - a tiny header-only library to allow input in C++ to be more sensible
//by ShakaUVM
#ifndef READ_SHAKAUVM_H
#define READ_SHAKAUVM_H
#include <iostream> // IWYU pragma: export
#include <fstream>  // IWYU pragma: export
#include <string>   // IWYU pragma: export
#include <cctype>

namespace readlib_detail {
//After a failed extraction, throw away the offending token so the next read can make progress.
//Some failures have already consumed part or all of the bad token (an int that overflows, like
//"99999999999", is consumed completely). If the stream is no longer in the middle of a token,
//discarding another word would throw away the *next*, valid, token, so only discard when needed.
inline void discard_bad_token(std::istream &ins) {
	const int c = ins.peek();
	if (c == std::char_traits<char>::eof()) return; //Nothing left to discard
	if (std::isspace(static_cast<unsigned char>(c))) return; //Bad token was already consumed
	std::string s;
	ins >> s; //Remove the rest of the word that caused the error
}

//Shared implementation of read<T>(), read<T>(ins), and int x = read();
template<class T>
inline T read_impl(std::istream &ins, const std::string &prompt) {
	while (true) {
		if (!prompt.empty()) std::cout << prompt;
		T retval = T();
		if (ins >> retval) return retval; //If this doesn't compile, your type needs an operator>>
		//The read failed. If we ran out of input (end of file, or the user hit ctrl-d) or the stream
		//is broken, give up and return a default value. The failbit is deliberately left set,
		//so the caller can detect this with if (!ins) and loops like while (true) { x = read(ins); if (!ins) break; } terminate.
		if (ins.eof() || ins.bad()) return T(); //Alternatively, we could throw an exception
		//Otherwise it was bad input (like "squirrel" for an int): clear the error, discard it, and try again
		ins.clear();
		discard_bad_token(ins);
	}
}
} //namespace readlib_detail

//Read a T from cin, reprompting if they type in something wrong
//On end of input, returns T() and leaves std::cin in a failed state
//Examples:
//int x = read<int>();
//const int x = read<int>("Please enter an int");
//string s = read<string>("What is your name?");
template<class T>
inline T read(const std::string &prompt = "") {
	return readlib_detail::read_impl<T>(std::cin, prompt);
}

//Read a T from a file, so no prompt
//On end of file, returns T() and leaves the stream in a failed state, so you can check if (!ins)
//Example:
//auto d = read<double>(file);
template<class T>
inline T read(std::istream &ins) {
	return readlib_detail::read_impl<T>(ins, "");
}

//Reads a whole line of text, analogue to getline
//Note: leading whitespace is skipped (this is what makes it safe to mix with read()),
//so blank lines are skipped and the line's leading indentation is removed.
inline std::string readline(const std::string &prompt = "", char delimiter = '\n') {
	//Eliminate a common bug when switching from >> to getline, the >> will leave a newline in the input buffer
	std::string retval;
	std::cout << prompt;
	std::cin >> std::ws;
	std::getline(std::cin,retval,delimiter);
	return retval;
}

//Getline equivalent for reading from a file
//Same whitespace behavior as above: blank lines and leading indentation are skipped
inline std::string readline(std::istream &ins, char delimiter = '\n') {
	std::string retval;
	ins >> std::ws;
	std::getline(ins,retval,delimiter);
	return retval;
}

//To use read_opt requires C++17 and above
#if __cplusplus >= 201703L
#include <optional>
namespace readlib_detail {
template<class T>
inline std::optional<T> read_opt_impl(std::istream &ins, const std::string &prompt) {
	if (!prompt.empty()) std::cout << prompt;
	T retval{};
	if (ins >> retval) return retval;
	//Out of input: leave eofbit and failbit set, so the caller can tell "no more input" (ins.eof())
	//apart from "bad input" (!ins.eof())
	if (ins.eof() || ins.bad()) return std::nullopt;
	ins.clear(); //Bad input: clear the error code, so the caller can try again when they like
	return std::nullopt; //Return that nothing was read
}
} //namespace readlib_detail

//Returns an optional. So if your specified type is not read, the caller will know this rather than silently discarding the error
//Recommended for more advanced programmers than read()
//If nullopt is returned:
//  * If cin.eof() is true, there is no more input, and the stream is left in a failed state
//  * Otherwise the input was bad. The failbit is cleared and the bad input is usually left in the stream,
//    so you can read it as something else. Caveat: some failures consume characters, e.g. an int that
//    overflows ("99999999999") is consumed entirely.
// Example:
// auto a = read_opt<int>(); 
// if (!a) exit(EXIT_FAILURE);
// cout << *a << endl;
template<class T>
inline std::optional<T> read_opt(const std::string &prompt = "") {
	return readlib_detail::read_opt_impl<T>(std::cin, prompt);
}

//This version reads from a file, so no prompt
//Like the other read_opt, returns nullopt if it didn't read what was expected
template<class T>
inline std::optional<T> read_opt(std::istream &ins) {
	return readlib_detail::read_opt_impl<T>(ins, "");
}
//End requiring C++17 and above
#endif

//Simplest read possible: int x = read();
//Credit: /u/9cantthinkofgoodname, modified to support C++98 by Mikadore
//read() returns a proxy object; the actual read happens when the proxy is converted to the type you want
struct ReadLibReader {
	ReadLibReader(std::istream& ins_, const std::string& prompt_) : ins(ins_), prompt(prompt_) {}
#if __cplusplus >= 201103L
	//The && means the conversion only works on the temporary returned by read().
	//This turns "auto r = read(); int a = r; int b = r;" (which would silently read twice) into a compile error.
	template<class T>
		operator T() && { return readlib_detail::read_impl<T>(ins, prompt); }
#else
	template<class T>
		operator T() { return readlib_detail::read_impl<T>(ins, prompt); }
#endif
	std::istream &ins;
	const std::string prompt;
};

inline ReadLibReader read(const std::string &prompt = "") {
	return ReadLibReader(std::cin,prompt);
}
inline ReadLibReader read(std::istream &ins) {
	return ReadLibReader(ins,"");
}
#endif

