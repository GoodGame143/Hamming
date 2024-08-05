#include <string>
#include <cstring>
#include <sstream>
#include <iostream>
#include <ios>
#include <fstream>


#include <bitset>
#include <iomanip>


#include <vector>
#include <cmath>

using namespace std;

int convert(char character) {
	int ch = character;

	ch = ch << 2;

	int mask = 0b1111111111111000;
	int save = (ch & mask) << 1;

	int mask2 = 0b0000000000000111;
	ch = ch & mask2;

	ch = ch | save;

	int mask3 = 0b1111111110000000;
	save = (ch & mask3) << 1;

	int mask4 = 0b0000000001111111;
	ch = ch & mask4;

	ch = ch | save;

	return ch;
}

int sum(short value, const int* indexes, int n)
{
	int sum = 0;
	for (int i = 0; i < n; i++)
	{
		int index = indexes[i];
		if (index >= 0 && index < 16)
		{
			int bit = (value >> index) & 1;
			sum += bit;
		}
	}
	if (sum % 2 == 0) sum = 0;
	else sum = 1;
	return sum;
}
int deff(int value, const int* indexes, int n)
{
	int def = 0;
	for (int i = 0; i < n; i++)
	{

		int index = indexes[i];
		if (index >= 0 && index < 16)
		{
			int bit = (value >> index) & 1;
			def += bit;
		}
	}
	return def;
}
int hamming16bit(int value) {
	int mask = 0b0000111100001111;
	
	int indexes1[] = { 0,2,4,6,8,10,12,14 };
	int indexes2[] = { 1,2,5,6,9,10,13,14 };
	int indexes4[] = { 3,4,5,6,11,12,13,14 };
	int indexes8[] = { 7,8,9,10,11,12,13,14 };
	int n = sizeof(indexes1) / sizeof(indexes1[0]);
	int n2 = sizeof(indexes2) / sizeof(indexes2[0]);
	int n4 = sizeof(indexes4) / sizeof(indexes4[0]);
	int n8 = sizeof(indexes8) / sizeof(indexes8[0]);
	int bit1 = sum(value, indexes1, n);
	int bit2 = sum(value, indexes2, n2);
	int bit4 = sum(value, indexes4, n4);
	int bit8 = sum(value, indexes8, n8);
	value = value | bit1;
	value = value | (bit2 << 1);
	value = value | (bit4 << 3);
	value = value | (bit8 << 7);
	return value;
}
int decodeHamming16bit(int value)
{
	int indexes1[] = { 2,4,6,8,10,12,14 };
	int indexes2[] = { 2,5,6,9,10,13,14 };
	int indexes4[] = { 4,5,6,11,12,13,14 };
	int indexes8[] = { 8,9,10,11,12,13,14 };
	int n = sizeof(indexes1) / sizeof(indexes1[0]);
	int n2 = sizeof(indexes2) / sizeof(indexes2[0]);
	int n4 = sizeof(indexes4) / sizeof(indexes4[0]);
	int n8 = sizeof(indexes8) / sizeof(indexes8[0]);
	int bit1 = deff(value, indexes1, n);
	int bit2 = deff(value, indexes2, n2);
	int bit4 = deff(value, indexes4, n4);
	int bit8 = deff(value, indexes8, n8);
	int mask0 = 0b0000'0000'0000'0000;
	int mask1 = 0b0000'0000'0000'0001;
	int mask2 = 0b0000'0000'0000'0010;
	int mask4 = 0b0000'0000'0000'1000;
	int mask8 = 0b0000'0000'1000'0000;
	if (bit1 % 2 == 0) {
		bit1 = 0;
	}
	else {
		bit1 = 1;
	}
	if (bit2 % 2 == 0) {
		bit2 = 0;
	}
	else {
		bit2 = 1;
	}
	bit2 = bit2 << 1;
	if (bit4 % 2 == 0) {
		bit4 = 0;
	}
	else {
		bit4 = 1;
	}
	bit4 = bit4 << 3;
	if (bit8 % 2 == 0) {
		bit8 = 0;
	}
	bit8 = bit8 << 7;
	/*if (bit16 % 2 == 0) {
		bit16 = 0;
	}
	else {
		bit16 = 1;
	}*/
	
	int valid1 = value & mask1;
	int valid2 = value & mask2;
	int valid4 = value & mask4;
	int valid8 = value & mask8;
	//int valid16 = value & mask16; //?
	std::bitset<16> v1(valid1);
	std::bitset<16> v2(valid2);
	std::bitset<16> v4(valid4);
	std::bitset<16> v8(valid8);
	std::bitset<16> b1(bit1);
	std::bitset<16> b2(bit2);
	std::bitset<16> b4(bit4);
	std::bitset<16> b8(bit8);
	
	int sumControl = 0;
	if (bit1 != valid1) {
		sumControl += 1;
	}
	if (bit2 != valid2) {
		sumControl += 2;
	}
	if (bit4 != valid4) {
		sumControl += 4;
	}
	if (bit8 != valid8) {
		sumControl += 8;
	}
	/*if (bit16 != valid4) {
		sumControl += bit4;
	}*/

	if (sumControl == 0) {

		value = value >> 2;
		int vmask1 = 0b0000'0000'0000'0001;
		int vmask2 = 0b0000'0000'0001'1100;
		int vmask3 = 0b0111'1111'1100'0000;
		int save1 = (value & vmask1);
		int save2 = (value & vmask2) >> 1;
		int save3 = (value & vmask3) >> 2;
		value = 0b0000000000000000 | save1 ;
		value = value | save2 ;
		value = value | save3 ;
	}
	return value;
}


int distance(const std::vector<std::vector<int>>& code, int n, int k) {
	int minDistance = INT_MAX;

	for (int i = 0; i < n - 1; i++) {
		for (int j = i + 1; j < n; j++) {
			int distance = 0;
			for (int bitIndex = 0; bitIndex < k; bitIndex++) {
				if (code[i][bitIndex] != code[j][bitIndex]) {
					distance++;
				}
			}
			if (distance < minDistance) {
				minDistance = distance;
			}
		}
	}

	return minDistance;
}

int main() {
	/*int n = 4;
	int r = 3;
	int data[] = { 1,0,1,1,1,0,1 }; 
	int encoded[] = { 0 }; */
	int a = 0;
	int b = 1;
	int c = 2;
	int d = 3;
	int e = 0;
	
	const char *character = "Reb Michael KlimRod";
	
	int q = strlen(character);

	for (int i = 0; i < 20 ; i++)
	{
		std::bitset<32> cha(character[i]);
		std::cout << cha << " " << std::hex << uppercase << character << " <- character " << i+1 << std::endl;

		short value = convert(character[i]);
		int hamming = hamming16bit(value);
		std::bitset<32> h(hamming);
		std::cout << h << " <- encode hamming" << std::endl;


		int decode_hamming = decodeHamming16bit(hamming);
		const char t = decode_hamming;
		std::bitset<32> h2(decode_hamming);
		std::cout << h2 << " <- decode hamming" << std::endl;
		std::cout << h2 << " " << t << " <- character" << "\n" << std::endl;

		
	}
	e = a = b = c = d;
	std::cout << a << b << c << d << e << std::endl;
	

	


	


}
