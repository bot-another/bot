#include <iostream>
#include <time.h>
#include "Bignumber.h"
using namespace std;
//用于大数计算
extern size_t max(size_t a, size_t b);
extern Bignumber Add(Bignumber addend_0, Bignumber addend_1);
extern Bignumber Multiplication(Bignumber multiplier_0, Bignumber multiplier_1);
extern Bignumber Mod(Bignumber multiplier_0, size_t n);
//用于计算输出的结果
extern string String_Square(const char* num1);
extern string String_Multiplication(const char* num1,const char*num2);
extern string String_Pow(size_t n);
extern string String_Sub(string a);
bool IspLucas_Lehmer_primalityrime(size_t n)
{
	if (n == 2)return true;
	Bignumber target(n);
	Bignumber Lucas_Lehmer_list;
	Lucas_Lehmer_list.number[0] = 4;
	for (size_t i = 0;i < n - 2;i++) {
		Lucas_Lehmer_list = Multiplication(Lucas_Lehmer_list, Lucas_Lehmer_list);
		if (Lucas_Lehmer_list.number[0] < 2) {    //进行-2操作
			for (size_t j = 1;j < 64;j++)
			{
				if (Lucas_Lehmer_list.number[j] != 0) {
					Lucas_Lehmer_list.number[j]--;
					break;
				}
				else Lucas_Lehmer_list.number[j] += 4294967295;//2^32-1
			}
			Lucas_Lehmer_list.number[0] += 4294967296;//2^32
		}
		Lucas_Lehmer_list.number[0] = Lucas_Lehmer_list.number[0] - 2;
		while (Lucas_Lehmer_list.word > target.word|| 
			Lucas_Lehmer_list.word == target.word&&Lucas_Lehmer_list.number[Lucas_Lehmer_list.word] > target.number[Lucas_Lehmer_list.word]) {
			Lucas_Lehmer_list = Mod(Lucas_Lehmer_list, n);//不断取mod
		}
	}
	if (Lucas_Lehmer_list == target)return true;
	else return false;
}
int main()
{
	for (size_t i = 0;i < 168;i++) {
		if (IspLucas_Lehmer_primalityrime(prime[i])) {
			cout << prime[i] << ("\n") << String_Sub(String_Pow(prime[i])) << endl;
		}
	}
    std::cout << "The run time is: " <<(double)clock() / CLOCKS_PER_SEC << "s" << std::endl;
}