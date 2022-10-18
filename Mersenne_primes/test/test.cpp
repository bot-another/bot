#include<iostream>
#include<String>
#include"Bignumber.h"
#include<time.h>
#include<cstdlib>
#include<vector>
#include<fstream>
using namespace std;
#define Max_number 1024
//用于大数计算
extern Bignumber Add(Bignumber addend_0, Bignumber addend_1);
extern Bignumber Multiplication(Bignumber multiplier_0, Bignumber multiplier_1);
extern Bignumber Mod(Bignumber multiplier_0, size_t n);
//用于计算输出的结果
extern string String_Square(const char* num1);
extern string String_Multiplication(const char* num1, const char*num2);
extern string String_Pow(size_t n);
extern string String_Sub(string a);
using namespace std;
Bignumber ram_num1, ram_num2;
Bignumber result_Add, result_Mul;
unsigned int rand_32()
{
	return (rand() & 0x3) << 30 | rand() << 15 | rand();
}
void output_file()
{
	unsigned int data;
	ofstream outfile;
	outfile.open("data.txt", ios::out | ios::trunc);
	srand(time(NULL));
	for (size_t i = 0;i < 32;i++) {
		data = rand_32();
		ram_num1.number[i] = data;
		outfile << data << endl;
	}
	for (size_t i = 0;i < 32;i++) {
		data = rand_32();
		ram_num2.number[i] = data;
		outfile << data << endl;
	}
	ram_num1.word = 31;
	ram_num2.word = 31;
	result_Add = Add(ram_num1, ram_num2);
	result_Mul = Multiplication(ram_num1, ram_num2);
	outfile << result_Add.word << endl;
	for (size_t i = 0;i <= result_Add.word;i++) {
		outfile << result_Add.number[i] << endl;
	}
	outfile << result_Mul.word << endl;
	for (size_t i = 0;i <= result_Mul.word;i++) {
		outfile << result_Mul.number[i] << endl;
	}
	size_t mod_num = (rand() % 1000) + 1;
	outfile << mod_num << endl;
	Bignumber mod_bignum(mod_num);
	while (result_Mul.word > mod_bignum.word || result_Mul.word == mod_bignum.word&&result_Mul.number[result_Mul.word] > mod_bignum.number[mod_bignum.word]) {
		result_Mul = Mod(result_Mul, mod_num);
	}
	outfile << result_Mul.word << endl;
	for (size_t i = 0;i <= result_Mul.word;i++) {
		outfile << result_Mul.number[i] << endl;
	}
	for (size_t i = 1;i <= 1000;i++) {
		outfile << String_Sub(String_Pow(i)) << endl;
	}
	outfile.close();
}
int main()
{
	output_file();
}