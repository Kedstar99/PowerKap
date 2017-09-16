#include "procfs.h"

Procfs::Procfs(bool* profilerState)
	   :adapterPresent(false),
	    profilerState(profilerState)
{
	prevNetworkInfo = NULL;
}

bool Procfs::checkAdapter(int pid) 
{
	adapterPresent = detectNetworkInterface(pid, VETH_A);
	return adapterPresent;
}

bool Procfs::detectNetworkInterface(int pid, std::string adapter)
{
	bool result = false;

	std::string dir = PROC_STRING + NET_STRING;
	if (*profilerState && pid > 0) 
	{
		dir = PROC_STRING + "/" + std::to_string(pid) + NET_STRING;
	}
	std::string line;
	std::ifstream infile(dir);
	
	if (!infile.fail()) 
	{
		while(std::getline(infile, line)) 
		{
			std::size_t found = line.find(':');
			if (found != std::string::npos) 
			{
				if (line.substr(0, found) == adapter) 
				{
					result = true;
					break;
				}
			}

		}

	}
	return result;
}

// Returns CPU information such as model type, package info, physical cores etc...
void Procfs::detectCPU(void) 
{
	std::string GENUINEINTEL("GenuineIntel");
	std::string VENDORID("vendor_id");
	std::string CPUFAMILY("cpu family");
	std::string MODEL("model");
	std::string MODELNAME("model name");
	std::string CORES("cpu cores");
	std::string modelName("");
	std::string line;

	std::ifstream infile(CPU_INFO_PATH);
	int modelNumber = 0;
	if (!infile.fail()) 
	{
		
		while(std::getline(infile, line)) 
		{
			if (!line.compare(0, VENDORID.size(), VENDORID))
			{
				if (!line.find(GENUINEINTEL)) 
				{
					std::cout << "NOT GENUINE INTEL"<< std::endl;
				} 
			}
			if (!line.compare(0, CPUFAMILY.size(), CPUFAMILY))
			{
				std::string family = line.substr(line.find(": ") + 2);
				int familyNumber = std::stoi(family);
				if (familyNumber != 6) 
				{
					std::cout << "NOT GENUINE INTEL"<< std::endl;
				}
			}
			if (!line.compare(0, MODEL.size(), MODEL)) 
			{
				if (line.compare(0, MODELNAME.size(), MODELNAME)) 
				{
					std::string modelStr = line.substr(line.find(": ") + 2);
					modelNumber = std::stoi(modelStr);
				} 
				else 
				{
					modelName = line.substr(line.find(": ") + 2);
				}
			}
			if (!line.compare(0, CORES.size(), CORES))
			{
				int noCores = std::stoi(line.substr(line.find(": ") + 2));
			}	
		}
		infile.close();
	}

	if (modelNumber > 0) 
	{
		std::cout << "FOUND " << printCPUModel(modelNumber) << std::endl;
		if (modelName.empty()) 
		{
			std::cout << modelName << std::endl;
		}

	}
	return;
}

const DiskInfo* Procfs::getDiskInfo(int pid) 
{
	const DiskInfo* result = NULL;
	std::string dir = PROC_STRING + "/" + std::to_string(pid) + IO_STRING;
	std::ifstream infile(dir);
	std::string line;
	long long readBytes = -1;
	long long writeBytes = -1;
	if (!infile.fail()) 
	{
		while(std::getline(infile, line)) 
		{
			if (!line.compare(0, READ_BYTES.size(), READ_BYTES))
			{
				std::string value = line.substr(line.find(": ") + 2);
				readBytes = std::stoll(value);
			}
			if (!line.compare(0, WRITE_BYTES.size(), WRITE_BYTES))
			{
				std::string value = line.substr(line.find(": ") + 2);
				writeBytes = std::stoll(value);
			}
		}
		infile.close();
	}
	if (profilerState && !(*profilerState) && pid > 0)
	{
		if (readBytes != -1 && writeBytes != -1) 
		{
			result = new DiskInfo(readBytes, writeBytes);
			//if the result was good, store it
			prevDiskInfo = result;
		} else {
			if (prevDiskInfo) 
			{
				//otherwise, restore from previous result
				result = prevDiskInfo->clone();
			}
		}
	}
	return result;
}

const NetworkInfo* Procfs::getNetworkInfo(int pid)
{
	const NetworkInfo* result = NULL;
	if (adapterPresent) 
	{
		std::string dir = PROC_STRING + "/" + std::to_string(pid) + NET_STRING;
		std::string line;
		std::ifstream infile(dir);
		if (!infile.fail()) 
		{
			while(std::getline(infile, line)) 
			{
				result = this->parseNetString(line);
				if (result != NULL) 
				{
					break;
				}
			}
			infile.close();
		}
		if (result != NULL) 
		{
			//if there was a failure to read, restore from a previous result
			this->prevNetworkInfo = result;
		} else {
			if (profilerState && !(*profilerState) && pid > 0) 
			{
				if (detectNetworkInterface(pid, VETH_A)) 
				{
					if (prevNetworkInfo != NULL) 
					{
						//If the result was good, store the old value
						result = this->prevNetworkInfo->clone();
					}
				} else {
					adapterPresent = false;
					std::cerr << "Virtual adapter is missing (likely because the process died), stopping networkAnalysis" <<  "\n";
				}
			}
		}
	}
	return result;
}


//Simple file to map cpu model number to cpu family
std::string Procfs::printCPUModel(int cpu_model_code) 
{
	std::string result;
	switch(cpu_model_code) 
	{
		case CPU_SANDYBRIDGE:
			result = "Sandybridge";
			break;
		case CPU_SANDYBRIDGE_EP:
			result = "Sandybridge-EP";
			break;
		case CPU_IVYBRIDGE:
			result = "Ivybridge";
			break;
		case CPU_IVYBRIDGE_EP:
			result = "Ivybridge-EP";
			break;
		case CPU_HASWELL:
			result = "Haswell";
			break;
		case CPU_HASWELL_EP:
			result = "Haswell-EP";
			break;
		case CPU_BROADWELL:
			result = "Broadwell";
			break;
		case CPU_KABYLAKE:
			result = "Kabylake";
			break;
		default:
			result = "Unsupported model " + std::to_string(cpu_model_code);
			break;
	}
	return result;
}

const NetworkInfo* Procfs::parseNetString(std::string interfaceString)
{
	const NetworkInfo* result = NULL;
	std::size_t found = interfaceString.find(':');
	if (found != std::string::npos) 
	{
		std::string interfaceName = interfaceString.substr(0, found);
		if (interfaceName == VETH_A) 
		{
			std::string restOfString = interfaceString.substr(found + 1, interfaceString.length() - found);
			std::stringstream iss( restOfString );
			std::vector< long long > myNumbers;
			long long number;
			while( iss >> number) 
			{
				myNumbers.push_back(number);
			}
			if (myNumbers.size() == 16) 
			{
				long long receivedBytes = myNumbers.at(0);
				long long sentBytes = myNumbers.at(8);
				if (receivedBytes >= 0 && sentBytes >= 0) 
				{
					if (profilerState && !(*profilerState))
					{
						result = new NetworkInfo(receivedBytes, sentBytes);
					}
				}
			}

		}
	}
	return result;
}

Procfs::~Procfs() {}