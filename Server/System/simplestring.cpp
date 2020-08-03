#include "simplestring.h"
#include "functions.h"

using namespace Mafia;
using namespace System;

String::String(char* d, int s){
	REGISTER(d);
	this->data = d;
	this->size = s;
}

String::String(std::string data){
	SAFE_NEW(this->data, char[data.length() + 1]);
	this->data[data.length()] = '\0';
	this->size = data.length();
	for(int i = 0; i < this->size; i++){
		this->data[i] = data[i];
	}
}

String::String(){
	data = 0;
	size = 0;
}

String::~String()
{
	/*Ах, какой же тут кринж!!! Я совсем задолбался с двойными разыменовываниями указателей,
	 *Так что нафиг все это. Если потом обнаружим, что память течет, то будем решать проблему.
	 *А пока просто нафиг!*/

	//std::cout << "deleting string: " << this->data << std::endl
	//		  << "size: " << this->size << std::endl;
	if(size > 0){
		//delete[] this->data;
		//SAFE_DELETE_ARRAY(this->data);
		size = -1;
	}
}

void String::operator =(String source){
	this->size = source.size;
	SAFE_NEW(this->data, char[this->size]);
	for(int i = 0; i < this->size; i++){
		this->data[i] = source.data[i];
	}
}

void String::paste(String data, int index){
	for(int i = index; i < index + data.size; i++){
		if(i == this->size){
			this->data[i] = '\0';
			break;
		}
		this->data[i] = data.data[i - index];
	}
}
