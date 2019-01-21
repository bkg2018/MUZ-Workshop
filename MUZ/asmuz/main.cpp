//
//  main.cpp
//  asmuz
//
//  Created by Francis Pierot on 20/01/2019.
//  Copyright © 2019 Francis Pierot. All rights reserved.
//
#include "pch.h"

using std::string;

void nextParam(int & arg, int argc, const char* argv[])
{
	if (arg+1 == argc) {
		printf("Error 1: missing parameter after %s\n", argv[arg]);
		exit(1);
	}
	arg += 1;
}

int main(int argc, const char * argv[]) {

	MUZ::Assembler as;
	MUZ::ErrorList msg;
	string inputFile;
	int arg = 1;
	while (arg < argc) {
		if ((strcmp(argv[arg], "--outputdir")==0) || (strcmp(argv[arg], "-od")==0)) {
			nextParam(arg, argc, argv);
			as.SetOutputDirectory(argv[arg]);
		} else if ((strcmp(argv[arg], "--listing")==0) || (strcmp(argv[arg], "-l")==0)) {
			nextParam(arg, argc, argv);
			as.SetListingFilename(argv[arg]);
		} else if ((strcmp(argv[arg], "--memory")==0) || (strcmp(argv[arg], "-m")==0)) {
			nextParam(arg, argc, argv);
			as.SetMemoryFilename(argv[arg]);
		} else if ((strcmp(argv[arg], "--hex")==0) || (strcmp(argv[arg], "-h")==0)) {
			nextParam(arg, argc, argv);
			as.SetIntelHexFilename(argv[arg]);
		} else if ((strcmp(argv[arg], "--log")==0)) {
			nextParam(arg, argc, argv);
			as.SetLogFilename(argv[arg]);
		} else {
			if ((strcmp(argv[arg], "--inputfile")==0) || (strcmp(argv[arg], "-f")==0)) {
				nextParam(arg, argc, argv);
			}
			inputFile = argv[arg];
		}
		arg += 1;
	}

	// do assembling
#ifdef __APPLE__
	CFTimeInterval startTime = CACurrentMediaTime();
#elif _WIN32
	DWORD startTime = timeGetTime();
#endif
	if ( ! ExistFile(inputFile) ) {
		printf("Error 2: missing file %s\n", inputFile.c_str());
		exit(2);
	}
	if (! inputFile.empty())
	try {
			as.AssembleFile(inputFile, msg);
		} catch (std::exception &e) {
			perror(e.what());
		}

#ifdef __APPLE__
	CFTimeInterval elapsedTime = CACurrentMediaTime() - startTime;
#elif _WIN32
	double elapsedTime = ((timeGetTime() * 1.0) - startTime) / 1000.0;
#endif
	printf("Assembling took %lf seconds\n", elapsedTime);

	return 0;
}