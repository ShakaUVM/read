//main.cc - A short driver full of examples of how to use the read.h header library
//by ShakaUVM
#include "read.h"
#include <chrono>
#include <vector>
using namespace std;
using namespace chrono;
using hrc = high_resolution_clock;

//The library works on user defined types as well as all primitive types
struct Tester{ int x; float f; };
istream &operator>>(istream &ins, Tester &t) { t.x = read<int>(ins); t.f = read<float>(ins); return ins; }

int main() {

	//Example 1 - reading using a function instead of cin >>
	//Will clear errors and reprompt if the user doesn't type in an int
	int green_apples = read<int>("Please enter how many green and red apples you want to buy: "); //Reads an int from the standard in
	//The prompt is optional. It also allows you to use const with input from the keyboard
	const int red_apples = read<int>(); //Reads an int from the standard in
	//Works fine with const and auto for people who are into that kind of stuff
	const auto price = read<double>("Please enter the price per apple: "); //Reads a double, stores it in a const
	cout << "Your total bill is " << (red_apples + green_apples) * price << endl;


	//Example 2 - can mix and match cin >> and getline without issue
	string name = readline("Please enter your name: ");
	cout << "Hello " << name << ", how are you?\n";

	
	//Example 3 - Works with files as well
	//Note there is no prompt when reading from a file, since that doesn't make sense
	ifstream ins("shuf.txt"); //Holds the numbers from 1 to 1M, shuffled
	cout << read<int>(ins) << endl; // You can embed input in output


	//Example 4 - time how long it takes to read 1M numbers from a file
	//Start timer
	vector<int> vec; 
	vec.reserve(1'000'000);
	hrc::time_point start = hrc::now();
	while (true) {
#ifdef ORIG
		//Compile with -DORIG to time the old way
		int x = 0;
		ins >> x;
		if (!ins) break;
		vec.push_back(x);
#else
		//The new way is a bit more compact and sensible
		int x = read<int>(ins);
		if (!ins) break;
		vec.push_back(x);
#endif
	}
    hrc::time_point end = hrc::now();
    cerr << "Time to read 1M ints: " << fixed << duration_cast<duration<double>>(end - start).count() << endl;


	//Example 5 - The library works with any type for which there is a default constructor and operator>> defined
	Tester t = read<Tester>("Please enter an int and a float: \n");
	cout << "t.x = " << t.x << " t.f = " << t.f << endl;
}
