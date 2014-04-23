#include "bcd_converter.h"

std::string BCDConverter::decToBin(int num, size_t bits)
{
	std::string result;

	for (size_t i = 0; i < bits; ++i)
	{
		char digit = !!(num & (1 << i)) + '0';
		result = digit + result;
	}

	return result;
}
int BCDConverter::binToDec(std::string num)
{
	int converted = 0;

	size_t size = num.size();
	for (size_t i = 0; i < size; ++i)
	{
		converted |= (BCDConverter::getNum(num.at(size - i - 1)) << i);
	}

	return converted;
}

bool BCDConverter::isSmallDigit(int num)
{
	return num < 8;
}

size_t BCDConverter::countSmallDigits(int num, size_t digits)
{
	size_t small_digits = 0;

	if (num == 0)
	{
		small_digits = digits;
	}
	else
	{
		for (size_t i = 0; i < digits; ++i)
		{
			int digit = num % 10;

			if (BCDConverter::isSmallDigit(digit))
			{
				small_digits++;
			}

			num /= 10;
		}
	}



	return small_digits;
}

bool BCDConverter::matchesMask(int num, std::string mask) 
{
	for (int i = mask.size() - 1; i >= 0; --i)
	{
		int digit = num % 10;
		bool small_digit = BCDConverter::isSmallDigit(digit);

		char token = mask.at(i);

		if ((int) small_digit == (token - '0'))
		{
			return false;
		}

		num /= 10;
	}

	return true;
}

int BCDConverter::getNum(char token)
{
	return token - '0';
}

std::string BCDConverter::EncodeBCD(int num, size_t bits_per_digit, std::string separator)
{
	std::string result;

	if (num == 0)
	{
		result.append("0", bits_per_digit);
	}
	else
	{
		while (num > 0)
		{
			int digit = num % 10;

			result = BCDConverter::decToBin(digit, bits_per_digit) + separator + result;

			num /= 10;
		}
	}

	return result;
}
int BCDConverter::DecodeBCD(std::string bits, size_t bits_per_digit)
{
	std::stringstream ss;

	int remainder = bits.size() % bits_per_digit;

	if (remainder != 0)
	{
		bits = std::string(remainder, '0') + bits;
	}

	for (size_t i = 0; i < bits.size(); i += bits_per_digit)
	{
		std::string nibble = bits.substr(i, bits_per_digit);
		ss << binToDec(nibble);
	}

	int result;
	ss >> result;

	return result;
}

std::string BCDConverter::EncodeDPD(int num, std::string separator)
{
	std::string result;

	if (num == 0)
	{
		result.append("0", 10);
	}
	else
	{
		while (num > 0)
		{
			int tercet = num % 1000;

			std::string bcd_str = BCDConverter::EncodeBCD(tercet, 4);
			int bcd[12];

			for (int i = 0; i < 12; ++i)
			{
				bcd[i] = BCDConverter::getNum(bcd_str.at(i));
			}

			std::string bits(10, '0');

			bits[0] = bcd[1] | (bcd[0] & bcd[9]) | (bcd[0] & bcd[5] & bcd[8]);
			bits[1] = bcd[2] | (bcd[0] & bcd[10]) | (bcd[0] & bcd[6] & bcd[8]);
			bits[2] = bcd[3];
			bits[3] = (bcd[5] & (~bcd[0] | ~bcd[8])) | (~bcd[0] & bcd[4] & bcd[9]) | (bcd[4] & bcd[8]);
			bits[4] = bcd[6] | (~bcd[0] & bcd[4] & bcd[9]) | (bcd[0] & bcd[8]);
			bits[5] = bcd[7];
			bits[6] = bcd[0] | bcd[4] | bcd[8];
			bits[7] = bcd[0] | (bcd[4] & bcd[8]) | (~bcd[4] & bcd[9]);
			bits[8] = bcd[4] | (bcd[4] & bcd[8]) | (~bcd[0] & bcd[10]);
			bits[9] = bcd[11];

			for (int i = 0; i < 10; ++i)
			{
				bits[i] = bits[i] + '0';
			}

			result = bits + separator + result;

			num /= 1000;
		}
	}

	return result;
}
int BCDConverter::DecodeDPD(std::string num)
{
	std::stringstream ss;

	for (size_t i = 0; i < num.size(); i += 10)
	{
		std::string declet = num.substr(i, 10);

		int bits[10];

		for (int i = 0; i < 10; ++i)
		{
			bits[i] = BCDConverter::getNum(declet.at(i));
		}

		std::string bcd(12, '0');
		
		bcd[0] = (bits[6] & bits[7]) & (~bits[3] | bits[4] | ~bits[8]);
		bcd[1] = bits[0] & (~bits[6] | ~bits[7] | (bits[3] & ~bits[4] & bits[8]));
		bcd[2] = bits[1] & (~bits[6] | ~bits[7] | (bits[3] & ~bits[4] & bits[8]));
		bcd[3] = bits[2];
		bcd[4] = bits[6] & ((~bits[7] & bits[8]) | (~bits[4] & bits[8]) | (bits[3] & bits[8]));
		bcd[5] = (bits[3] & (~bits[6] | ~bits[8])) | (bits[0] & ~bits[3] & bits[4] & bits[6] & bits[7] & bits[8]);
		bcd[6] = (bits[4] & (~bits[6] | ~bits[8])) | (bits[1] & ~bits[3] & bits[4] & bits[7]);
		bcd[7] = bits[5];
		bcd[8] = bits[6] & ((~bits[7] & ~bits[8]) | (bits[7] & bits[8] & (bits[3] | bits[4]))); // v & ((\w & \x) | (w & x & (s | t)))
		bcd[9] = (~bits[6] & bits[7]) | (bits[3] & bits[6] & ~bits[7] & bits[8]) | (bits[0] & bits[7] & (~bits[8] | (~bits[3] & ~bits[4])));	// (\v & w) | (s & v & \w & x) | (p & w & (\x | (\s & \t)))
		bcd[10] = (~bits[6] & bits[8]) | (bits[4] & ~bits[7] & bits[8]) | (bits[1] & bits[6] & bits[7] & (~bits[8] | (~bits[3] & ~bits[4])));	// (\v & x) | (t & \w & x) | (q & v & w & (\x | (\s & \t)))
		bcd[11] = bits[9];

		for (int i = 0; i < 12; ++i)
		{
			bcd[i] = bcd[i] + '0';
		}

		ss << BCDConverter::DecodeBCD(bcd, 4);
	}

	int result;
	ss >> result;

	return result;
}