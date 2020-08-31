#include "/public/read.h"

int main() {
    auto temp = read_opt<unsigned int>("Please enter your age:\n");
	if (!temp or temp > 130) {
		std::cout << "You failed to enter a valid age.\n";
		exit(EXIT_FAILURE);
	}
	unsigned int age = *temp;
	std::cout << "You should have " << age * 10'000 << " dollars saved up right now.\n";
}
