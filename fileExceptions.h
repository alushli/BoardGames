#pragma once
#include "generalExceptions.h"
#include <iostream>
#include <fstream>
#include <exception>
using namespace std;

class FileException : public GeneralException {
public:
	/* the function return the explanation of the exception */
	virtual const char* what()const throw() {
		return "Error in file handeling - ";
	}

	/* FileException constructor */
	FileException(string messege):GeneralException(messege){}
};
