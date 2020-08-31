# read
A small header-online library to make input in C++ sensible

Using cin >> to read in input from the keyboard is problematic for a few different reasons:
1) New programmers chain together multiple cin statements, leading to the inability to tell where an error occurred in the input
2) Cannot be combined with const
3) Cannot be combined with auto
4) Cannot be combined with cout statements
5) Cannot be used at the same time a variable is declared
6) Breaks if you mix and match cin >> statements with getlines

This short (~150LOC) header-only library provides an alternative to cin that fixes all six of those issues. It provides four functions for reading from the standard input or from files. Here are some example of the functions in action:

```int x = read();``` This reads an int from the keyboard and stores it in x. If the user types something not an int, it discard it and keeps reading until an int is read. Using cin, you'd need two lines to do this: ```int x; cin >> x;``` which is annoying and awkward. And many more lines of code if you wanted it to handle errors.

```int age = read("Please enter your age:\n");``` This will prompt the user to enter their age. If they don't type in a valid int, it will prompt them to do so again until the user successfully gives them an int.

```const double SIZE = read();``` Because the input is something that can appear on the right hand side of an assignment operation, this opens up a lot of things that can't be done with cin, such as creating a variable and initializing it at the same time, as well as using input to initialize constants and combining with auto.

```auto x = read<char>();``` Reads a char from cin and returns it into x (which becomes a char). All of the read functions are templated by the type of data to read, but it can be omitted if the left hand size has a type that we can use to deduce what sort of read should happen.

```const string NAME = readline("Please enter your name: ");``` Read is the equivalent to "cin >>". Readline is the equivalent of getline(cin,s), except it combines cleanly with read, whereas it is buggy to use cin >> and getline without being very careful with trailing newlines. This line here will prompt the user for their name and read until a newline, and then return the result in a string.

In addition to read and readline, there are two more functions also named read and readline that read from files instead of cin:

```ifstream ins("file.txt"); cout << read<int>(ins) << endl;``` The first line opens a file for reading, the second line directly outputs the first int found in that file. This would be awkward to write using cin and cout.

```string s = readline(ins);``` This reads a line of text from the file ins and stores the result in s. Like with the other readline, it plays nicely with read.

It also works just fine with any class that has a default constructor and an operator>> overloaded for it. The overload can even use read for the primitive types, as such:

```struct Tester{ int x; float f; };```

```istream &operator>>(istream &ins, Tester &t) { t.x = read<int>(ins); t.f = read<float>(ins); return ins; }```

And then in main:

```Tester t = read<Tester>("Please enter an int and a float: \n");```

If you want more of a traditional I/O experience, you can choose to be notified when a read error occurs (such as you wanting to read an int, but the user types "squirrel") via the read_opt() function, which returns an optional:

```
auto temp = read_opt<unsigned int>("Please enter your age:\n");
if (!temp or temp > 130) {
	std::cout << "You failed to enter a valid age.\n";
	exit(EXIT_FAILURE);
}
unsigned int age = *temp;
std::cout << "You should have " << age * 10'000 << " dollars saved up right now.\n";
```

Testing the performance on a million ints, it appears to be equivalent to the old way, though this was not written with performance in mind. It was written to make input easier for new C++ programmers, who frequently get tripped up on input, especially when vetting their input for errors and when switching between >> and getline.

Installation instructions:

1. Clone the directory and put read.h wherever you would like it to be. /usr/include/ if you want it accessible to your whole system, for example, or just in a local project directory.
2. All the other files are just for showing how to use the library and can be deleted after you've taken a look at them. The sample run shows what it looks like to run main when the user types in some bad input.
3. If you want to build the executable and test the performance of read.h versus cin, then "make tester" and "make oldway", then run tester and run oldway. They'll output to the screen how long it teach each approach to read a million ints from a file. The new way is slower with the optimizer off and ASAN on, but with the optimizer on and ASAN off, I got equivalent times.

Requires C++11, with more options available if you have C++14 (a simpler syntax using auto) and C++17 (optionals support). The example main shows the difference in using the library with the different versions of the C++ standard.
