#include "WordGenerator.h"
#include <iomanip>
#include <fstream>

#include "CL/cl.h"

using namespace std;

void printHex(unsigned int *hash)
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < sizeof(int); j++)
		{
			unsigned int temp = hash[i] >> (j * 8);
			cout << setfill('0') << setw(2) << hex << (temp & 0xff);
		}
	}
}

void printHexInFile(ofstream &stream, unsigned int *hash)
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < sizeof(int); j++)
		{
			unsigned int temp = hash[i] >> (j * 8);
			stream << setfill('0') << setw(2) << hex << (temp & 0xff);
		}
	}
}

struct data_s {
	vector<password_s> passwords;
	vector<hash_s> hashes;
	size_t passwordsTotalSize = 0;
};

void checkError(cl_int error)
{
	if (error != CL_SUCCESS)
	{
		if (error == CL_OUT_OF_RESOURCES)
		{
			cerr << "Failure to allocate resources required by the OpenCL implementation on the device." << endl;
		}
		else
		{ 
		cerr << "OpenCL call failed with error " << error << endl;
		}
		exit(error);
	}
	
}

string getPlatformName(cl_platform_id id)
{
	size_t size = 0;
	clGetPlatformInfo(id, CL_PLATFORM_NAME, 0, nullptr, &size);
	string result;
	result.resize(size);
	clGetPlatformInfo(id, CL_PLATFORM_NAME, size, 
		const_cast<char*> (result.data()), nullptr);
	return result;
}

string getDeviceName(cl_device_id id)
{
	size_t size = 0;
	clGetDeviceInfo(id, CL_DEVICE_NAME, 0, nullptr, &size);
	string result;
	result.resize(size);
	clGetDeviceInfo(id, CL_DEVICE_NAME, size,
		const_cast<char*> (result.data()), nullptr);
	return result;
}

string loadKernel(const char* name)
{
	ifstream iFile(name);
	string result((istreambuf_iterator<char>(iFile)), istreambuf_iterator<char>());
	return result;
}

cl_program createProgram(const string& source, cl_context context)
{
	size_t lengths[1] = { source.size() };
	const char* sources[1] = { source.data() };
	
	cl_int error = 0;
	cl_program program = clCreateProgramWithSource(context, 1, sources, lengths, &error);
	checkError(error);
	
	return program;
}

int main(int argc, char** argv)
{
	string mask = "**\\(const0\\)**";
	const size_t maxInArray = 1000;
	cl_context context;
	cl_int error = CL_SUCCESS;

	cl_uint platformIdCount = 0;
	clGetPlatformIDs(2, nullptr, &platformIdCount);
	if (!platformIdCount)
	{
		cerr << "No OpenCL platform found" << endl;
		return 1;
	}
	else
	{
		cout << "Found " << platformIdCount << " platform(s)" << endl;
	}
	vector<cl_platform_id> platformIds(platformIdCount);
	clGetPlatformIDs(platformIdCount, platformIds.data(), nullptr);
	for (cl_uint i = 0; i < platformIdCount; i++)
	{
		cout << "\t[" << i + 1 << "] : " << 
			getPlatformName(platformIds[i]).c_str() << endl;
	}
	unsigned int userInput;
	cout << "Choose platform (0 - exit): ";
	cin >> userInput;
	if (!userInput)
	{
		exit(0);
	}
	cl_uint deviceIdCount = 0;
	clGetDeviceIDs(platformIds[userInput - 1], CL_DEVICE_TYPE_ALL, 0, nullptr,
		&deviceIdCount);
	if (!deviceIdCount)
	{
		cerr << "No OpenCL devices found" << endl;
		return 1;
	}
	else
	{
		cout << "Found " << deviceIdCount << " device(s)" << endl;
	}
	vector<cl_device_id> deviceIds(deviceIdCount);
	clGetDeviceIDs(platformIds[0], CL_DEVICE_TYPE_ALL, deviceIdCount,
		deviceIds.data(), nullptr);
	for (cl_uint i = 0; i < deviceIdCount; i++)
	{
		cout << "\t[" << (i + 1) << "] : " << getDeviceName(deviceIds[i]).c_str() << endl;
	}
	const cl_context_properties contextProperties[] =
	{
		CL_CONTEXT_PLATFORM,
		reinterpret_cast<cl_context_properties> (platformIds[0]), 0, 0
	};

	context = clCreateContext(contextProperties, deviceIdCount,
		deviceIds.data(), nullptr, nullptr, &error);
	checkError(error);
	
	cout << "Context created" << endl;

	cl_program program = createProgram(loadKernel("md5.cl"), context);
	error = clBuildProgram(program, deviceIdCount, deviceIds.data(), nullptr, nullptr, nullptr);
	if (error == CL_BUILD_PROGRAM_FAILURE) {
		// Determine the size of the log
		size_t log_size;
		clGetProgramBuildInfo(program, deviceIds[0], CL_PROGRAM_BUILD_LOG, 0, NULL, &log_size);

		// Allocate memory for the log
		char *log = (char *)malloc(log_size);

		// Get the log
		clGetProgramBuildInfo(program, deviceIds[0], CL_PROGRAM_BUILD_LOG, log_size, log, NULL);

		// Print the log
		printf("%s\n", log);
	}
	checkError(error);

	cl_kernel kernel = clCreateKernel(program, "calcHashes", &error);
	checkError(error);
	cl_command_queue queue = clCreateCommandQueueWithProperties(context, deviceIds[0], 0, &error);
	//Preparing data
	WordGenerator wordGenerator(mask.data(), maxInArray);
	hash_s *hashes = new hash_s[maxInArray];
	cl_mem iBuffer = clCreateBuffer(context, CL_MEM_READ_ONLY, 
		sizeof(password_s) * maxInArray, nullptr, &error);
	checkError(error);
	cl_mem oBuffer = clCreateBuffer(context, CL_MEM_READ_WRITE,
		sizeof(hash_s) * maxInArray, hashes, &error);
	checkError(error);
	ofstream outFile;
	outFile.open("test.txt", ios_base::app);
	while (!wordGenerator.isOver())
	{
		wordGenerator.generateWords();
		size_t resultSize = wordGenerator.resultSize();
		vector<password_s> pas = wordGenerator.getResult();
		unsigned int wordSize = wordGenerator.wordSize();
		checkError(clEnqueueWriteBuffer(queue, iBuffer, CL_TRUE, 0, 
			sizeof(password_s) * resultSize, pas.data(), 0, nullptr, nullptr));
		clSetKernelArg(kernel, 0, sizeof(cl_mem), &iBuffer);
		clSetKernelArg(kernel, 1, sizeof(int), &wordSize);
		clSetKernelArg(kernel, 2, sizeof(cl_mem), &oBuffer);
		const size_t globalWorkSize[] = {resultSize, 0, 0 };
		checkError(clEnqueueNDRangeKernel(queue, kernel, 1,
			nullptr, globalWorkSize, nullptr, 0, nullptr, nullptr));
		checkError(clEnqueueReadBuffer(queue, oBuffer, CL_TRUE, 0,
			sizeof(hash_s) * resultSize, hashes, 0, nullptr, nullptr));
		for (unsigned int i = 0; i < resultSize; i++)
		{
			outFile << pas[i].password << "\t";
			printHexInFile(outFile, hashes[i].h);
			outFile << endl;
		}
		wordGenerator.clearResult();
	}

	clReleaseCommandQueue(queue);

	clReleaseMemObject(iBuffer);
	clReleaseMemObject(oBuffer);

	clReleaseKernel(kernel);
	clReleaseProgram(program);

	clReleaseContext(context);
    return 0;
}