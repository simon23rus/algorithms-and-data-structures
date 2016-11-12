#include<iostream>
#include<random>

int main() {
	for(int i = 0; i < 200000; ++i) {
		std::cout << (char)('a' + rand() % 26);
	}
	return 0;
}
