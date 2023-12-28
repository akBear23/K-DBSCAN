#include <iostream>
#include "readerCSV.h"
#include <vector>
#include <string>
#include <ostream>
#include <fstream>
#include <sstream>
#include <boost/algorithm/string.hpp>


/*
* Parses through csv file line by line and returns the data
* in vector of vector of strings.
*/
std::vector<std::vector<std::string> > CSVReader::getData()
{
	std::ifstream file(fileName);
	if (!file) {
        std::cerr << "Unable to open file: " << fileName << std::endl;
        // Return an empty vector
        return std::vector<std::vector<std::string>>();
    }
	std::vector<std::vector<std::string> > dataList;

	std::string line = "";
	// Iterate through each line and split the content using delimeter
	while (getline(file, line))
	{
		std::vector<std::string> vec;
		boost::algorithm::split(vec, line, boost::is_any_of(delimeter));
		dataList.push_back(vec);
	}
	// Close the File
	file.close();

	return dataList;
}
// int main(){
// 	CSVReader reader("/home/ankhanh/learning/data_mining/data/sample_dataset.csv"); // Creating an object of CSVReader/CSVWriter	
// 	std::vector<std::vector<std::string>> dataList = reader.getData(); // Get the data from CSV File	
// 	// dataList.erase(dataList.begin());
// 	// for (std::vector<std::vector<std::string> >::iterator it = dataList.begin(); it != dataList.end(); ++it) {
// 	// 	for (std::vector<std::string>::iterator it2 = it->begin(); it2 != it->end(); ++it2) {
// 	// 		std::cout << *it2 << " ";
// 	// 	}
// 	// 	std::cout << std::endl;
// 	// }
    
//     // Check if dataList is empty
//     if (dataList.empty()) {
//         std::cout << "No data was read from the file." << std::endl;
//         return 1;
//     }

//     for (std::vector<std::vector<std::string> >::iterator it = dataList.begin(); it != dataList.end(); ++it) {
//         for (std::vector<std::string>::iterator it2 = it->begin(); it2 != it->end(); ++it2) {
//             std::cout << *it2 << " ";
//         }
//         std::cout << std::endl;
//     }

// 	std::cout << "Test completed\n";
// 	return 0;
// }