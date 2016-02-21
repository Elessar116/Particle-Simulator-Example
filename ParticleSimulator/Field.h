//#include <iostream>
//#include <cmath>
#include "Object.h"

#ifndef SF
#define SF 1 


class Field:public Object
{
private:
	float shAccer = -SF;
	float proAccer[3];
	void initproAccer(){
		proAccer[0] = 0;
		proAccer[1] = 0;
		proAccer[2] = -SF;
	}
public:
	Field(){ initproAccer(); }

	Field(const float inPosi[])
		:Object(inPosi){
		initproAccer();
	}

	Field(const float inPosi[], const float inAccer)
		:Object(inPosi){
		shAccer = inAccer;
	}

	Field(const float inPosi[], const float inAccer[])
		:Object(inPosi){
		for (int i = 0; i < 3; i++)
		{
			proAccer[i] = inAccer[i];
		}
	}

	Field(const float inPosi[], const float inVelo[], const float inAccer[])
		:Object(inPosi, inVelo){
		for (int i = 0; i < 3; i++)
		{
			proAccer[i] = inAccer[i];
		}
	}

	Field(const float inPosi[], const float inVelo[], const float inAccer)
		:Object(inPosi, inVelo){
		shAccer = inAccer;
	}

	void GetAccer(float* inAccer){
		inAccer[0] = proAccer[0];
		inAccer[1] = proAccer[1];
		inAccer[2] = proAccer[2];
	}

	float GetAccerSp() const{
		return this->shAccer;
	}

	void SetAccer(const float inAccer){
		shAccer = inAccer;
	}

	void SetAccer(const float inAccer[]){
		for (int i = 0; i < 3; i++)
		{
			proAccer[i] = inAccer[i];
		}
	}

	virtual void Show() const {
		std::cout << "Posotion: " << position[0] << ", " << position[1] << ", " << position[2] << "\n";
		std::cout << "Velocity: " << velocity[0] << ", " << velocity[1] << ", " << velocity[2] << "\n";
		std::cout << "Accerlation: " << proAccer[0] << ", " << proAccer[1] << ", " << proAccer[2] << "\n";
	}
	~Field(){}
};

#endif
