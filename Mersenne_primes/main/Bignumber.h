#define max_word 64//表示能存下最大的数有64字,即2^(64*32)
const int prime[168] = { 2,3,5,7,11,13,17,19,23,29,31,37,41,43,47,53,59,61,67,71,
73,79,83,89,97,101,103,107,109,113,127,131,137,139,149,151,157,163,167,173,
179,181,191,193,197,199,211,223,227,229,233,239,241,251,257,263,269,271,277,
281,283,293,307,311,313,317,331,337,347,349,353,359,367,373,379,383,389,397,
401,409,419,421,431,433,439,443,449,457,461,463,467,479,487,491,499,503,509,
521,523,541,547,557,563,569,571,577,587,593,599,601,607,613,617,619,631,641,
643,647,653,659,661,673,677,683,691,701,709,719,727,733,739,743,751,757,761,
769,773,787,797,809,811,821,823,827,829,839,853,857,859,863,877,881,883,887,
907,911,919,929,937,941,947,953,967,971,977,983,991,997 };
unsigned long long initialization[max_word] = { 0 };//大数初始化
const unsigned long long high_check = 0xffffffff00000000;
const unsigned long long low_check = 0x00000000ffffffff;
const unsigned long long init = 0x00000000ffffffff;//2^n - 1初始化
size_t max(size_t a, size_t b) {
	if (a > b)return a;
	else return b;
}
class Bignumber {
public:
	unsigned long long *number;
	size_t word, bit;
	explicit Bignumber(unsigned long long*num= initialization, size_t a=0, size_t b=0){
		number = new unsigned long long[max_word];
		memmove(number, num, max_word * sizeof(unsigned long long));
		word = a;
		bit = b;
	}//构造函数
	explicit Bignumber(size_t n) {
		number = new unsigned long long[max_word];
		size_t i = n / 32;
		size_t j = n % 32;
		for (size_t k = 0;k < i;k++) {
			number[k] = init;
		}
		number[i] = (1 << j) - 1;
		word = (n - 1) / 32;
		if (n - 1 > 0)bit = n - 1;
		else bit = 0;
	}//构造函数
	Bignumber(const Bignumber &obj)noexcept {
		number = new unsigned long long[max_word];
		memmove(number, obj.number, max_word * sizeof(unsigned long long));
		word = obj.word;
		bit = obj.bit;
	}//拷贝构造函数
	void operator=(const Bignumber &obj) {
		memmove(number, obj.number, max_word * sizeof(unsigned long long));
		word = obj.word;
		bit = obj.bit;
	}//=重载
	bool operator==(const Bignumber &obj) {
		size_t i;
		if (obj.word != word)return false;
		else {
			for (i = 0;i <= obj.word;i++)
			{
				if (obj.number[i] != number[i])return false;
			}
		}
		return true;
	}//==重载
	~Bignumber(){
		delete[] number;
	}//析构函数
	void Set_Bit_and_Word();
	void Set_Bit_and_Word(size_t new_size);
};
void Bignumber::Set_Bit_and_Word() {//更新Bignumber的bit数和word(字)数
	int i = max_word - 1, j = 31;
	unsigned long long check = (1 << 31);
	while (i>=0) {
		if (number[i] != 0)break;
		i--;
	}
	if (i == -1) {
		bit = 0;
		word = 0;
	}
	else {
		word = i;
		while (j >= 0) {
			if ((number[i] & check) > 0)break;
			check = check >> 1;
			j--;
		}
		bit = j + 32 * i;
	}
}
void Bignumber::Set_Bit_and_Word(size_t new_size) {     //n=a.bit()+b.bit()+1，更新Bignumber的bit数和word(字)数
	size_t i = new_size / 32, j = new_size % 32;
	size_t check = (1 << j);
	if ((number[i] & check) > 0) {
		bit = new_size;
		word = bit / 32;
	}
	else {
		bit = new_size - 1;
		word = bit / 32;
	}
}
Bignumber Add(Bignumber addend_0, Bignumber addend_1) {//加法
	unsigned long long carry = 0;
	Bignumber ans;
	for (size_t i = 0;i <= max(addend_0.word, addend_1.word);i++) {
		ans.number[i] = addend_0.number[i] + addend_1.number[i] + carry;
		carry = (ans.number[i] & high_check)>>32;
		ans.number[i] = ans.number[i] & low_check;
	}
	if (carry > 0){
		ans.number[max(addend_0.word, addend_1.word) + 1] = carry;
		ans.word = max(addend_0.word, addend_1.word) + 1;
	}
	else {
		ans.word = max(addend_0.word, addend_1.word);
	}
	return ans;
}
Bignumber Multiplication(Bignumber multiplier_0, Bignumber multiplier_1) {//乘法
	unsigned long long carry = 0;
	Bignumber ans;
	Bignumber *intermediate;
	intermediate = new Bignumber[multiplier_1.word + 1];
	size_t i;
	size_t j;
	for (i = 0;i <= multiplier_1.word;i++) {
		for (j = 0;j <= multiplier_0.word; j++) {
			intermediate[i].number[j + i] += multiplier_1.number[i] * multiplier_0.number[j];
		}
		intermediate[i].word = multiplier_0.word + i;
	}
	for (i = 0;i <= multiplier_1.word;i++) {
		ans = Add(ans, intermediate[i]);
	}
	return ans;
}
Bignumber Mod(Bignumber Num, size_t n) {//对Num取模(2^n-1)
	Bignumber target(n);
	Bignumber division, mod, ans;
	size_t i = n / 32;
	size_t j = n % 32;
	if (Num.word < target.word) {
		return Num;
	}
	if (Num.word == target.word) {
		if (Num.number[Num.word] <= target.number[Num.word])return Num;
	}
	unsigned long long int L_check, H_check;
	L_check = (1 << j) - 1;
	H_check = ~L_check;
	for (size_t k = 0;k < i;k++) {
		mod.number[k] = Num.number[k];
	}
	mod.number[i] = Num.number[i] & L_check;
	mod.word = (n - 1) / 32;
	division.word = -1;
	for (size_t k = i;k <= Num.word;k++) {
		division.number[k - i] += ((Num.number[k] & H_check) >> j);
		division.word++;
		if (k != Num.word) {
			division.number[k - i] += ((Num.number[k + 1] & L_check) << (32 - j));
		}
		else break;
	}
	ans = Add(division, mod);
	while (ans.number[ans.word] == 0) {
		ans.word--;
	}
	return ans;
}