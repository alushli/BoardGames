#pragma once
#include <exception>
using namespace std;

class GeneralException : public exception {
public:
	/* the function return the explanation of the exception */
	virtual const char* what()const throw() {
		return "General error - ";
	}

	/* GeneralException constructor */
	GeneralException(string messege) :_messege(messege) {}
	
	/* the function return string property*/
	const string& getMessege() const { return _messege; }

protected:
	string _messege;
};
