#ifndef OBJ_H
#define OBJ_H


//#include <iostream>
//#include <cmath>

class Object
{
protected:
	float position[3];
	float velocity[3];
public:
	Object(){
		for (int i = 0; i < 3; i++)
		{
			position[i] = 0;
			velocity[i] = 0;
		}
	}
	Object(const float inPosi[]){
		for (int i = 0; i < 3; i++)
		{
			position[i] = inPosi[i];
		}
	}
	Object(const float *inPosi, const float *inVelo){
		for (int i = 0; i < 3; i++)
		{
			position[i] = inPosi[i];
			velocity[i] = inVelo[i];
		}
	}

	void Set(const float *inPosi, const float *inVelo) {
		for (int i = 0; i < 3; i++)
		{
			position[i] = inPosi[i];
			velocity[i] = inVelo[i];
		}
	}

	void GetPosi(float* posi){
		posi[0] = position[0];
		posi[1] = position[1];
		posi[2] = position[2];
	}

	void GetVelo(float* velo){
		velo[0] = velocity[0];
		velo[1] = velocity[1];
		velo[2] = velocity[2];
	}

	
	virtual ~Object(){};
	virtual void Show() const = 0;
};

#endif