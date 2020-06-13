//main.cc - A short driver full of examples of how to use the read.h header library
//There is an explanation of what each line does and the benefits of it
//by ShakaUVM
#include "read.h"
#include <chrono>
#include <vector>
#include <cstdlib>
#include <cmath>
#include <iomanip>
using namespace std;
using namespace chrono;
using hrc = high_resolution_clock;

//The library works on user defined types with operator>> as well as all primitive types
struct Tester{ int x; float f; };
istream &operator>>(istream &ins, Tester &t) { t.x = read<int>(ins); t.f = read<float>(ins); return ins; }

int main() {

	//Example 1 - reading using a function instead of cin >>
	//Will clear errors and reprompt if the user doesn't type in an int
	int green_apples = read("Please enter how many green and red apples you want to buy: "); //Reads an int from the standard in

	//You can specify the type in angle brackets.
	//The prompt is optional, in which case it works just like cin >> but can appear on the right hand side
	auto red_apples = read<int>(); //Reads an int from the standard in

	//By making input on the right hand side, you can use const and/or auto with input
	//Const and auto are popular these days, so it makes sense for input to be assignable
	const auto price = read<double>("Please enter the price per apple: "); //Reads a double, stores it in a const
	cout << "Your total bill is " << (red_apples + green_apples) * price << endl;


	//Example 2 - can mix and match cin >> and getline without issue
	//This solves a persistent problem with iostreams
	//On the downside, you can't just hit enter to enter an empty string now
	string filename = readline("Please enter a file to read ints from (shuf.txt is the default): ");
	cout << "Attempting to open " << filename << " now...\n";


	//Example 3 - Works with files as well
	//Note there is no prompt when reading from a file, since that doesn't make sense
	ifstream ins(filename); //Shuf.txt Holds the numbers from 1 to 1M, shuffled
	if (!ins) {
		cout << "Error: Couldn't open " << filename << endl;
		exit(EXIT_FAILURE);
	}
	cout << "The first int in the file is: " << read<int>(ins) << endl; // You can embed input in output


	//Example 4 - time how long it takes to read 1M numbers from a file
	vector<int> vec; 
	vec.reserve(1'000'000);
	hrc::time_point start = hrc::now(); //Start timer
	while (true) {
#ifdef ORIG
		//Compile with -DORIG to time the old way
		int x = 0;
		ins >> x;
		if (!ins) break;
		vec.emplace_back(x);
#else
		//The new way is more compact and sensible
		int x = read<int>(ins);
		if (!ins) break;
		vec.emplace_back(x);
#endif
	}
    hrc::time_point end = hrc::now(); //End timer
    cerr << "Time to read 1M ints: " << fixed << setprecision(3) << duration_cast<duration<double>>(end - start).count() << "s\n";


	//Example 5 - The library works with any type for which there is a default constructor and operator>> defined
	//So anything that you could cin >> before you can read() now
	Tester t = read("Please enter an int and a float:\n");
	cout << "t.x = " << t.x << " t.f = " << t.f << endl;


	//Example 6 - If you don't want to silently discard errors, use read_opt instead, which will allow you to see if the read was successful
	//It returns an optional, which you can check to see if it actually has a value
	optional<unsigned int> height_cm = read_opt<unsigned int>("Please enter your height (in cm):\n");
	if (!height_cm) {
		cout << "You did not enter an unsigned int when asked for your height. Quitting...\n";
		exit(EXIT_FAILURE);
	}
	//I prefer auto for brevity
	auto weight_kg = read_opt<unsigned int>("Please enter your weight (in kg):\n");
	if (!weight_kg) {
		cout << "You did not enter an unsigned int when asked for your weight. Quitting...\n";
		exit(EXIT_FAILURE);
	}
	//Optionals work like pointers, kinda, so you get the value from them like this -
	double height_m = *height_cm / 100.0; //Convert from cm to m

	//Compute BMI (as a side note, BMI is kind of nonsense)
	cout << "Your BMI is: " << *weight_kg / (height_m*height_m) << endl;

	//Example 7 - read_opt from a file, count how many bad reads there are
	cout << "Opening foo.txt...\n";
	ins.clear();
	ins.close();
	ins.open("foo.txt");
	if (!ins) {
		cout << "Couldn't open file.\n";
		exit(EXIT_FAILURE);
	}
	int valid_count = 0, invalid_count = 0;
	while (true) {
		auto num = read_opt<int>(ins);
		if (ins.eof()) break;
		if (!num) {
			invalid_count++;
			string s = read<string>(ins); 
			cout << s << endl;
		}
		else {
			cout << *num << endl;
			valid_count++;
		}
	}
	cout << "There were " << valid_count << " valid ints in the file and " << invalid_count << " invalid tokens in the file.\n";
}
