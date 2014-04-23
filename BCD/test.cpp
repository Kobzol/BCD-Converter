#include <iostream>

#include "bcd_converter.h"

using std::cout;
using std::cin;
using std::endl;

int main(int argc, char** argv)
{
	int num = 123;
	int bits = 4;

	std::string dpd = BCDConverter::EncodeDPD(num);

	cout << dpd << endl;
	cout << BCDConverter::DecodeDPD(dpd) << endl;


	system("pause");

	return 0;
}