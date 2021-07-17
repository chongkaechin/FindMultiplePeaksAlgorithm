#include "stdafx.h"
#include "FindMultiplePeaksAlgorithm.h"

FindMultiplePeaksAlgorithm::FindMultiplePeaksAlgorithm(std::vector<int> &insertVector) 
{
	for (int i = 0; i < insertVector.size(); i++)
	{
		Data tempData;
		tempData.index = i;
		tempData.value = insertVector[i];
		tempData.type = NO_TYPE;

		dataSet[i] = tempData;
	}

	dataSetFullRange[RANGE_START] = dataSet.begin()->second;
	dataSetFullRange[RANGE_END] = (--dataSet.end())->second;

	findPeakRange[RANGE_START] = dataSetFullRange[RANGE_START];
	findPeakRange[RANGE_END] = dataSetFullRange[RANGE_END];

}

std::map<int, Data> &FindMultiplePeaksAlgorithm::findMultiplePeaksFunction(int peakNumber)
{
	int tempPeakNumber = peakNumber + 1;
	std::map<int, Data> tempDataSet;

	while (tempPeakNumber > peakNumber) 
	{
		tempDataSet = findMultiplePeaksLoop(dataSet, findPeakRange, dataSetFullRange);

		tempPeakNumber = 0;
		for (std::map<int, Data>::iterator index = tempDataSet.begin(); index != tempDataSet.end(); index++) 
		{
			if (tempDataSet[index->first].type == NO_TYPE) 
			{
				tempPeakNumber = peakNumber + 1;
				break;
			}
			if (tempDataSet[index->first].type == PEAK)
			{
				tempPeakNumber += 1;
			}
		}

		if (tempPeakNumber > peakNumber)
		{
			dataSetFullRange[RANGE_START] = tempDataSet.begin()->second;
			dataSetFullRange[RANGE_END] = (--tempDataSet.end())->second;

			findPeakRange[RANGE_START] = dataSetFullRange[RANGE_START];
			findPeakRange[RANGE_END] = dataSetFullRange[RANGE_END];

			dataSet.clear();
			dataSet = tempDataSet;
			tempDataSet.clear();
		}
	}
	int i = 0;
	for (std::map<int, Data>::iterator index = tempDataSet.begin(); index != tempDataSet.end(); index++)
	{
		if (tempDataSet[index->first].type == PEAK) 
		{
			result[i] = tempDataSet[index->first];
			i++;
		}
	}

	return result;
}

std::map<int, Data> &FindMultiplePeaksAlgorithm::findMultiplePeaksLoop(std::map<int, Data> &currentDataSet, std::map<int, Data> &currentFindPeakRange, std::map<int, Data> currentDataSetFullRange)
{
	std::map<int, Data>::iterator start;
	std::map<int, Data>::iterator stop;

	std::stack<Snapshot> snapshotStack;

	Snapshot currentSnapshot;
	currentSnapshot.snapshotFindPeakRange = currentFindPeakRange;
	currentSnapshot.maxValue = -1;
	currentSnapshot.maxValueIndex = -1;
	currentSnapshot.stage = 0;

	snapshotStack.push(currentSnapshot);

	while (!snapshotStack.empty())
	{
		currentSnapshot = snapshotStack.top();
		snapshotStack.pop();

		switch (currentSnapshot.stage)
		{
		case 0: 
		{
			if (currentSnapshot.snapshotFindPeakRange[RANGE_START].index == currentDataSetFullRange[RANGE_START].index &&
				currentSnapshot.snapshotFindPeakRange[RANGE_END].index == currentDataSetFullRange[RANGE_END].index)
			{
				for (std::map<int, Data>::iterator index = currentDataSet.begin(); index != currentDataSet.end(); index++)
				{
					if (currentDataSet[index->first].value > currentSnapshot.maxValue)
					{
						currentSnapshot.maxValue = currentDataSet[index->first].value;
						currentSnapshot.maxValueIndex = index->first;
					}
				}

				currentDataSet[currentSnapshot.maxValueIndex].type = PEAK;
				
				Snapshot newSnapshot;

				newSnapshot.snapshotFindPeakRange[RANGE_START] = currentSnapshot.snapshotFindPeakRange[RANGE_START];
				newSnapshot.snapshotFindPeakRange[RANGE_END] = currentDataSet[currentSnapshot.maxValueIndex];
				newSnapshot.maxValue = -1;
				newSnapshot.maxValueIndex = -1;
				newSnapshot.stage = 0;

				currentSnapshot.snapshotFindPeakRange[RANGE_START] = currentDataSet[currentSnapshot.maxValueIndex];
				currentSnapshot.snapshotFindPeakRange[RANGE_END] = currentSnapshot.snapshotFindPeakRange[RANGE_END];
				currentSnapshot.maxValue = -1;
				currentSnapshot.maxValueIndex = -1;
				currentSnapshot.stage = 0;

				snapshotStack.push(currentSnapshot);
				snapshotStack.push(newSnapshot);
			}
			else 
			{
				currentSnapshot.stage = 1;
				snapshotStack.push(currentSnapshot);
			}
			break;
		}
		case 1: 
		{
			for (std::map<int, Data>::iterator index = currentDataSet.begin(); index != currentDataSet.end(); index++)
			{
				if (index->first == currentSnapshot.snapshotFindPeakRange[RANGE_START].index) 
				{
					start = index;
					start++;
				}
				else if (index->first == currentSnapshot.snapshotFindPeakRange[RANGE_END].index) 
				{
					stop = index;
					stop--;
				}
			}
			for (std::map<int, Data>::iterator index = currentDataSet.begin(); index != currentDataSet.end(); index++)
			{
				if (index->first >= start->first && index->first <= stop->first) 
				{
					if (currentDataSet[index->first].value > currentSnapshot.maxValue)
					{
						currentSnapshot.maxValue = currentDataSet[index->first].value;
						currentSnapshot.maxValueIndex = index->first;
					}
				}
			}

			start--;
			stop++;

			int sideLeft = 0;
			int sideRight = 0;

			for (std::map<int, Data>::iterator index = currentDataSet.begin(); index != currentDataSet.end(); index++)
			{
				if (index->first >= start->first && index->first <= stop->first) 
				{
					if (index->first <= currentSnapshot.maxValueIndex)
					{
						sideLeft += 1;
					}
					if (index->first >= currentSnapshot.maxValueIndex)
					{
						sideRight += 1;
					}
				}
			}

			if (sideLeft == sideRight && sideLeft == 2)
			{
				if (currentSnapshot.snapshotFindPeakRange[RANGE_START].index == currentDataSetFullRange[RANGE_START].index ||
					currentSnapshot.snapshotFindPeakRange[RANGE_END].index == currentDataSetFullRange[RANGE_END].index)
				{
					currentDataSet[currentSnapshot.maxValueIndex].type = SLOPE;

					if (currentSnapshot.snapshotFindPeakRange[RANGE_START].index == currentDataSetFullRange[RANGE_START].index)
					{
						currentDataSet[currentSnapshot.snapshotFindPeakRange[RANGE_START].index].type = SLOPE;
					}
					else if (currentSnapshot.snapshotFindPeakRange[RANGE_END].index == currentDataSetFullRange[RANGE_END].index)
					{
						currentDataSet[currentSnapshot.snapshotFindPeakRange[RANGE_END].index].type = SLOPE;
					}
				}
				else
				{
					currentDataSet[currentSnapshot.maxValueIndex].type = VALLEY;
					currentDataSet.erase(currentSnapshot.maxValueIndex);
				}
			}
			else 
			{
				Snapshot newSnapshot;

				newSnapshot.snapshotFindPeakRange[RANGE_START] = currentSnapshot.snapshotFindPeakRange[RANGE_START];
				newSnapshot.snapshotFindPeakRange[RANGE_END] = currentDataSet[currentSnapshot.maxValueIndex];

				currentSnapshot.snapshotFindPeakRange[RANGE_START] = currentDataSet[currentSnapshot.maxValueIndex];
				currentSnapshot.snapshotFindPeakRange[RANGE_END] = currentSnapshot.snapshotFindPeakRange[RANGE_END];

				if (sideLeft >= sideRight)
				{
					//long side
					newSnapshot.maxValue = -1;
					newSnapshot.maxValueIndex = -1;
					newSnapshot.stage = 0;

					//short side
					currentSnapshot.stage = 2;

					snapshotStack.push(currentSnapshot);
					snapshotStack.push(newSnapshot);

				}
				else if (sideLeft < sideRight)
				{
					//short side
					newSnapshot.maxValue = currentSnapshot.maxValue;
					newSnapshot.maxValueIndex = currentSnapshot.maxValueIndex;
					newSnapshot.stage = 2;

					//long side
					currentSnapshot.maxValue = -1;
					currentSnapshot.maxValueIndex = -1;
					currentSnapshot.stage = 0;

					snapshotStack.push(currentSnapshot);
					snapshotStack.push(newSnapshot);

				}
			}
			break;
		}
		case 2: 
		{
			int dataNumber = 0;
			for (std::map<int, Data>::iterator index = currentDataSet.begin(); index != currentDataSet.end(); index++)
			{
				if (index->first == currentSnapshot.snapshotFindPeakRange[RANGE_START].index)
				{
					start = index;
				}
				else if (index->first == currentSnapshot.snapshotFindPeakRange[RANGE_END].index)
				{
					stop = index;
				}
			}
			for (std::map<int, Data>::iterator index = currentDataSet.begin(); index != currentDataSet.end(); index++)
			{
				if (index->first >= start->first && index->first <= stop->first) 
				{
					dataNumber ++;
				}
			}

			if (dataNumber == 2)
			{
				if (currentSnapshot.snapshotFindPeakRange[RANGE_START].index == currentDataSetFullRange[RANGE_START].index ||
					currentSnapshot.snapshotFindPeakRange[RANGE_END].index == currentDataSetFullRange[RANGE_END].index)
				{
					currentDataSet[currentSnapshot.maxValueIndex].type = PEAK;

					if (currentSnapshot.snapshotFindPeakRange[RANGE_START].index == currentDataSetFullRange[RANGE_START].index)
					{
						currentDataSet[currentSnapshot.snapshotFindPeakRange[RANGE_START].index].type = SLOPE;
					}
					else if (currentSnapshot.snapshotFindPeakRange[RANGE_END].index == currentDataSetFullRange[RANGE_END].index)
					{
						currentDataSet[currentSnapshot.snapshotFindPeakRange[RANGE_END].index].type = SLOPE;
					}
				}
				else
				{
					currentDataSet[currentSnapshot.maxValueIndex].type = SLOPE;
				}
			}
			else if (dataNumber == 3)
			{
				currentDataSet[currentSnapshot.maxValueIndex].type = PEAK;
				
				for (std::map<int, Data>::iterator index = currentDataSet.begin(); index != currentDataSet.end(); index++)
				{
					if (index->first > start->first && index->first < stop->first) 
					{
						currentDataSet[index->first].type = VALLEY;
						currentDataSet.erase(index->first);
						break;
					}
				}
			}
			else if (dataNumber >= 4)
			{
				currentDataSet[currentSnapshot.maxValueIndex].type = PEAK;

				currentSnapshot.maxValue = -1;
				currentSnapshot.maxValueIndex = -1;
				currentSnapshot.stage = 0;
				snapshotStack.push(currentSnapshot);
			}
			break;
		}
		default:
			break;
		}
	}
	return currentDataSet;

}
