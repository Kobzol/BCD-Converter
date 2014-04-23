#pragma once

#include <string>
#include <sstream>

class BCDConverter
{
public:
	static std::string decToBin(int num, size_t bits);
	static int binToDec(std::string num);

	static bool isSmallDigit(int num);
	static size_t countSmallDigits(int num, size_t digits);

	static bool matchesMask(int num, std::string mask);
	static int getNum(char token);

public:
	static std::string EncodeBCD(int num, size_t bits_per_digit, std::string separator = "");
	static int DecodeBCD(std::string bits, size_t bits_per_digit);

	static std::string EncodeDPD(int num, std::string separator = "");
	static int DecodeDPD(std::string num);
};