// main.cpp : 定義主控台應用程式的進入點。
//

#include "stdafx.h"
#include <iostream>
#include "FindMultiplePeaksAlgorithm.h"

int main()
{
	std::vector<int> dataVector = { 0,10,6,14,7,5,6,7,11,42,57,71,21,72,61,661,606,584,267,538,323,255,183,62,179,195,176,173,49,157,122,111,109,123,94,110,43,114,109,95,110,50,115,110,111,101,99,32,118,95,119,97,104,110,40,84,107,118,116,115,42,95,111,104,104,139,145,62,155,67,148,191,128,173,135,77,156,146,87,131,148,136,129,79,128,131,120,128,131,84,117,106,122,73,118,114,103,123,75,109,90,74,59,77,64,66,61,76,48,64,58,59,48,46,66,51,52,62,57,56,49,54,64,50,44,45,50,44,51,53,42,67,34,78,63,71,76,76,97,114,123,104,85,86,82,92,102,73,54,87,72,85,69,73,52,75,58,71,51,71,56,60,61,43,51,44,36,58,52,53,66,50,51,44,50,55,55,57,51,55,47,33,56,38,20,0,0,1,0 };
	FindMultiplePeaksAlgorithm FMP_algorithm(dataVector);

	std::map<int, Data> result = FMP_algorithm.findMultiplePeaksFunction(3);

	for (int i = 0; i < result.size(); i++)
	{
		std::cout <<"index:"<< result[i].index << std::endl;
		std::cout <<"value:"<< result[i].value << std::endl;
		std::cout <<"type:"<< result[i].type << std::endl;
		std::cout << std::endl;

	}
	system("pause");
	return 0;
}
