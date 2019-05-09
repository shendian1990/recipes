#include <bitset>
#include <iostream>

using std::bitset;
using std::cout;
using std::endl;

void print() {

}

template<typename T, typename... Types>
void print(const T& firstArg, const Types& ... args) {
	//注意sizeof...(args)的写法
	cout <<"size of args... "<< sizeof...(args)<<endl;
	cout << firstArg << endl;
	print(args...);
}

int main(int argc, const char* argv) {
	print(7.5, bitset<16>(377), 42);
}