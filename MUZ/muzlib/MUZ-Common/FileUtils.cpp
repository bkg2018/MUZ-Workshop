//
//  FileUtils.cpp
//  MUZ-Workshop
//
//  Created by Francis Pierot on 28/12/2018.
//  Copyright © 2018 Francis Pierot. All rights reserved.
//
#include "pch.h"
#include "FileUtils.h"
#include <stdio.h>

bool ExistFile(std::string file)
{
	FILE* f = fopen(file.c_str(), "r");
	if (f) {
		fclose(f);
		return true;
	}
	return false;
}
