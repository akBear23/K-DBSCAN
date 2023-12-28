
/* This code implements the algorithm IDS that is published in 2019
* Bao, X. and Wang, L., 2019. A clique-based approach for co-location pattern mining. Information Sciences, 490, pp.244-264.
* 
* Writer: vanha tran
* Date: 1.9.2020
*/


#include <iostream>
#include <ctime>
#include <map>
#include <set>
#include <vector>
#include <algorithm>
#include <queue>

// For tree
#include "tree.hh"

// For collecting memory usage
// #include <Windows.h>
#include <stdio.h>
// #include <Psapi.h>
// #pragma comment(lib, "psapi.lib")

// Import my function
#include "Object.h"
#include "printFunctions.h"
#include "readerCSV.h"
#include "toolFunctions.h"


using namespace std;


int main()
{
	// 1. Set time
	clock_t startTime, endTime; // start and end time
	clock_t timeMaterializeNeighbor; // time for materializing star neighborhoods
	clock_t timeGenerateCliques;	
	clock_t timeFilPCPs;

	double time_taken; // count the total execution time
	double totalTimeMatStarNei = 0.0f; // total time for materializing star neighborhoods
	double totalTimeGenerateCliques = 0.0f;	
	double totalTimeFilPCPs = 0.0f;

	startTime = clock(); // Begin count time
		
	// 2. Set parameters
	float dist_thres = 2.5;  // set the distance threshold
	float prev_thres = 0.2f; // set the prevalence

	// 3. Load datasets
	int choice = 2; // 1 for IDS, 2 for NDS
	int pr_ = 1; // 1 for cout, 0 for not

	// string file_name = "./Data/random_dataset_50_4.csv";
	string file_name = "/home/ankhanh/learning/data_mining/data/sample_dataset.csv";
	if (choice == 1)	freopen("./Result/sample_dataset_IDS.txt", "w", stdout);
	else				freopen("./Result/sample_dataset_NDS.txt", "w", stdout);
	
	CSVReader reader(file_name); // Creating an object of CSVReader/CSVWriter	
	std::vector<std::vector<std::string>> dataList = reader.getData(); // Get the data from CSV File	
	dataList.erase(dataList.begin()); // Delete the first line

	// 4. Count the instance number of each feature
	countNumberInstance(dataList);	

	// 5. Make grid and find neighbors
	// 5.1 Make grid
	std::map<std::pair<int, int>, std::vector<ObjWithCoord>> grid = makeGrid(dataList, dist_thres);

	// 5.2 Find neighbors, generate A1: <B.1, C.1>
	std::pair<std::map<ObjWithoutCoord, std::set<ObjWithoutCoord>>, std::map<ObjWithoutCoord, std::set<ObjWithoutCoord>>> nbs = genStarNeighborhoods1(grid, dist_thres);
	std::map<ObjWithoutCoord, std::set<ObjWithoutCoord>> SNs = nbs.first, BNs = nbs.second;

	timeMaterializeNeighbor = clock();
	totalTimeMatStarNei = double(timeMaterializeNeighbor - startTime);		

	// 6. Construct I-tree, get H-cliques and construct the chash structure
	if (choice == 1) {
		//generateCliques2_IDS(SN);
		generateCliques_IDS(BNs);
	}
	else {
		generateCliques_NDS(SNs, BNs);
	}
	
	timeGenerateCliques = clock();
	totalTimeGenerateCliques = double(timeGenerateCliques - timeMaterializeNeighbor);

	if (pr_) {
		// 7. Calculate PIs and filter prevalent patterns
		timeFilPCPs = clock();
		std::map<std::string, float> PkAll; // store all prevalent patterns
		PkAll = CalPIandFilterPatterns2(prev_thres);
		std::cout << "Number of candidates: " << numOfCands << endl;

		totalTimeFilPCPs = double(clock() - timeFilPCPs) - totalTimeCalPIs;
		std::cout << "The number of prevalent pattern is: " << PkAll.size() << endl << endl;

		for (auto const& ele : PkAll) {
			std::cout << ele.first << ": " << ele.second << endl;
		}

		// 8. Print the running time
		std::cout << "Execution time for materizaling starneighbors:\t" << totalTimeMatStarNei / CLOCKS_PER_SEC << "s" << endl;
		std::cout << "Execution time for generating cliques:\t\t\t" << (totalTimeGenerateCliques - totalTimeCreateChash) / CLOCKS_PER_SEC << "s" << endl;
		std::cout << "Execution time for creating chash structure:\t" << totalTimeCreateChash / CLOCKS_PER_SEC << "s" << endl;
		std::cout << "Execution time for calculating PIs:\t\t\t\t" << totalTimeCalPIs / CLOCKS_PER_SEC << "s" << endl;
		std::cout << "Execution time for filtering PCPs:\t\t\t\t" << totalTimeFilPCPs / CLOCKS_PER_SEC << "s" << endl << endl;

		endTime = clock();
		time_taken = double(endTime - startTime);
		std::cout << "Time taken by the program is (no neighboring):\t" << (time_taken - totalTimeMatStarNei) / CLOCKS_PER_SEC << "s" << endl;
		std::cout << "Time taken by the program is (all):\t\t\t\t" << time_taken / CLOCKS_PER_SEC << "s" << endl << endl;

		// 9. Show memory usage
		// HANDLE handle = GetCurrentProcess();
		// PROCESS_MEMORY_COUNTERS memCounter;
		// GetProcessMemoryInfo(handle, &memCounter, sizeof(memCounter));
		// SIZE_T physMemUsedByMe = memCounter.WorkingSetSize;
		// SIZE_T physPeackMemUsedByMe = memCounter.PeakWorkingSetSize;
		// std::cout << "Memory usage:\t\t" << physMemUsedByMe / 1024 << " KB" << endl;
		// std::cout << "Peak memory usage:\t" << physPeackMemUsedByMe / 1024 / 1024 << " MB" << endl << endl;

		// 10. Get the maximal size of patterns
		std::vector<int> sizePats;
		std::map<std::string, float>::iterator itAllPats = PkAll.begin();
		while (itAllPats != PkAll.end()) {
			sizePats.push_back(itAllPats->first.size());
			++itAllPats;
		}
		int maxSize = *std::max_element(sizePats.begin(), sizePats.end());
		std::cout << "The maximal size of patterns is: " << maxSize << endl;	

		//11. Classify pattern by sizes
		std::map<int, std::vector<std::string>> classPattBySize;
		classifyPattsBySize(PkAll, classPattBySize);
		printNumberPattsBySize(classPattBySize);
	}

	// The next code in here
	// std::cout << "hello world";
	return 0;
}








