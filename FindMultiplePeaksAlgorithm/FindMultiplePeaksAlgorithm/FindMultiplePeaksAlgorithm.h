#pragma once

#ifndef _FIND_MULTIPLE_PEAKS_ALGORITHM_H_
#define _FIND_MULTIPLE_PEAKS_ALGORITHM_H_

#include <map>
#include <stack>
#include <vector>

struct Data 
{
	int index;
	int value;
	char type;
};

struct Snapshot 
{
	std::map<int, Data> snapshotFindPeakRange;
	int maxValue;
	int maxValueIndex;
	int stage;
};

class FindMultiplePeaksAlgorithm 
{
public:
	FindMultiplePeaksAlgorithm(std::vector<int> &insertVector);
	std::map<int, Data> &findMultiplePeaksFunction(int peakNumber);

private:
	std::map<int, Data> dataSet;
	std::map<int, Data> dataSetFullRange;
	std::map<int, Data> findPeakRange;
	std::map<int, Data> result;

	enum Definition { NO_TYPE = 'n', VALLEY = 'v', PEAK = 'p', SLOPE = 's', RANGE_START = 0, RANGE_END = 1 };

	std::map<int, Data> &findMultiplePeaksLoop(std::map<int, Data> &currentDataSet, std::map<int, Data> &currentFindPeakRange, std::map<int, Data> currentDataSetFullRange);

};

#endif // !_FIND_MULTIPLE_PEAKS_ALGORITHM_H_



