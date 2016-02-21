//#include <iostream>
//#include <cmath>
#include "Object.h"
#include "Field.h"


#ifndef GF
#define GF 1 


class Particle:public Object
{
private:
	float mass = 1;
	float acce[3];
	void initacce(){
		for (int i = 0; i < 3; i++)
		{
			acce[i] = 0;
		}
	}
public:
	~Particle(){}
	Particle(){ initacce(); }
	Particle(const float M){ 
		mass = M;
		initacce();
	}

	Particle(const float M, const float inPosi[])
		:Object(inPosi),mass(0){
		initacce();
	}

	Particle(const float inPosi[])
		:Object(inPosi){
		initacce();
	}

	Particle(const float inPosi[], const float inVelo[])
		:Object(inPosi, inVelo){
		initacce();
	}

	Particle(const float M, const float inPosi[], const float inVelo[])
		:Object(inPosi, inVelo), mass(M){
		initacce();
	}

	virtual void Show() const {
		std::cout << "Mass: " << mass << "\n";
		std::cout << "Posotion: " << position[0] << ", " << position[1] << ", " << position[2] << "\n";
		std::cout << "Velocity: " << velocity[0] << ", " << velocity[1] << ", " << velocity[2] << "\n";
		std::cout << "Accerlation: " << acce[0] << ", " << acce[1] << ", " << acce[2] << "\n\n";
	}

	void SetFieldAccel(Field *f, int n){
		for (int i = 0; i < n; i++)
		{
			f[i].GetAccer(acce);
		}
	}

	void SetMass(const float m){
		mass = m;
	}

	void SetFieldAccel(Field f){
		f.GetAccer(acce);
	}

	void SetAccer(float inAccer[]){
		for (int j = 0; j < 3; j++)
		{
			acce[j] = inAccer[j];
		}
	}

	void GetAccer(float *inAccer) {
		inAccer[0] = acce[0];
		inAccer[1] = acce[1];
		inAccer[2] = acce[2];
	}

	void calGAccerVec(const Particle& par, float *vec){
		float Dist = sqrtf(powf(position[0] - par.position[0], 2)
			+ powf(position[1] - par.position[1], 2) + powf(position[2] - par.position[2], 2));
		if (Dist < 0.01f){
			Dist = 0.01f;
		}		
		float mmforce = -GF*par.mass/(Dist*Dist*Dist);
		vec[0] = mmforce*(position[0] - par.position[0]);
		vec[1] = mmforce*(position[1] - par.position[1]);
		vec[2] = mmforce*(position[2] - par.position[2]);
	}

	void calsSphereFieldAccerVec(Field& field, float *vec){
		float fpos[3];
		field.GetPosi(fpos);

		float Dist = sqrtf(powf(position[0] - fpos[0], 2)
			+ powf(position[1] - fpos[1], 2) + powf(position[2] - fpos[2], 2));
		if (Dist < 0.01f){
			Dist = 0.01f;
		}
		float mmforce = field.GetAccerSp() /Dist;
		vec[0] = mmforce*(position[0] - fpos[0]);
		vec[1] = mmforce*(position[1] - fpos[1]);
		vec[2] = mmforce*(position[2] - fpos[2]);
	}

	void calsSphereGravityFieldAccerVec(Field& field, float *vec, bool PoM){
		float fpos[3];
		field.GetPosi(fpos);
		float Dist = sqrtf(pow(position[0] - fpos[0], 2)
			+ pow(position[1] - fpos[1], 2) + pow(position[2] - fpos[2], 2));
		if (Dist < 0.01f){
			Dist = 0.01f;
		}
		float mmforce = field.GetAccerSp() / (Dist*Dist*Dist);
		if (!PoM){
			mmforce = -mmforce;
		}
		vec[0] = mmforce*(position[0] - fpos[0]);
		vec[1] = mmforce*(position[1] - fpos[1]);
		vec[2] = mmforce*(position[2] - fpos[2]);
	}
};
#endif