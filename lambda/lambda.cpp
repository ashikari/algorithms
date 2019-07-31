#include <iostream>

template <typename T>
void print(T arg){
	std::cout<<arg<<std::endl;
}

int sum_change(int a, int &b){
	b+=a;
	return a + b;
}


int main(){
	print(10);
	print("Sup bitch");


	int k =5;
	auto sumk = [&k](int c) {return sum_change(c, k);};

	print(sumk(10));
	print(k);

	return 0;
}