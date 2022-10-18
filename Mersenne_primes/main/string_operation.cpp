#include <iostream>
#include<String>
using namespace std;
string String_Square(const char* num1)//用于计算平方               
{
	const char*num2 = num1;
	int length1 = strlen(num1);
	int length2 = length1;
	int i, l;
	char *res = (char *)malloc(sizeof(char)*(length1 + length2)); //开辟相应内存
	memset(res, 0, sizeof(char)*(length1 + length2));
	for (i = length1 - 1; i >= 0; i--)
		for (l = length2 - 1; l >= 0; l--)
		{
			res[i + l + 1] += (num1[i] - '0')*(num2[l] - '0'); //类乘完累加
				res[i + l] += res[i + l + 1] / 10;    //马上进行进位
			res[i + l + 1] %= 10;
		}
	int count = 0;
	while (res[count] == 0)  //由于保存的结果是从右向左的，所以要消除左部分的0；
	{
		count++;
	}
	char* ret = (char *)malloc(sizeof(char)*(length1 + length2 + 2));
	memset(ret, 0, sizeof(char)*(length1 + length2 + 2));
	for (l = 0, i = count; i < length1 + length2; l++, i++)  //非0部分赋给ret
	{
		ret[l] = res[i] + '0';
	}
	string ans(ret);
	free(ret);
	free(res);
	return ans;
}
string String_Multiplication(const char* num1,const char*num2)//普通乘法
{
	int length1 = strlen(num1);
	int length2 = strlen(num2);
	int i, l;
	char *res = (char *)malloc(sizeof(char)*(length1 + length2)); //开辟相应内存
	memset(res, 0, sizeof(char)*(length1 + length2));
	for (i = length1 - 1; i >= 0; i--)
		for (l = length2 - 1; l >= 0; l--)
		{
			res[i + l + 1] += (num1[i] - '0')*(num2[l] - '0'); //类乘完累加
			res[i + l] += res[i + l + 1] / 10;    //马上进行进位
			res[i + l + 1] %= 10;
		}
	int count = 0;
	while (res[count] == 0)  //由于保存的结果是从右向左的，所以要消除左部分的0；
	{
		count++;
	}
	char* ret = (char *)malloc(sizeof(char)*(length1 + length2 + 2));
	memset(ret, 0, sizeof(char)*(length1 + length2 + 2));
	for (l = 0, i = count; i < length1 + length2; l++, i++)  //非0部分赋给ret
	{
		ret[l] = res[i] + '0';
	}
	string ans(ret);
	free(ret);
	free(res);
	return ans;
}

string String_Pow(size_t n)//计算2的幂(递归方案)
{
	string ans;
	if (n == 1)return "2";
	else if (n % 2 == 1)ans = String_Multiplication(String_Square(String_Pow(n / 2).c_str()).c_str(), "2");
	else ans = String_Square(String_Pow(n / 2).c_str());
	return ans;
}
string String_Sub(string a) //专门用来给字符串减一
{
	int size = a.size();
	for (int i = size - 1;i >= 0;i++) {
		if (a[i] != '0') {
			a[i]--;
			break;
		}
		else {
			a[i] = '9';
		}
	}
	return a;
}