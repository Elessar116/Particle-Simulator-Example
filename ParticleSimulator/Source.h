#include <iostream>
#include <cstdlib>
#include "Object.h"
#include "Particle.h"
#include "Field.h"

//#define useParallel

//#ifdef useParallel
//#include <ppl.h>
//using namespace concurrency;
//#endif

#ifndef IN_VEL 
#define IN_VEL 1

class Source :public Object
{
private:
	int n;
	Particle * particles;
	float velocityScale = IN_VEL;
	float veloVec[3];
	void initVec(){
		veloVec[0] = 1 * IN_VEL;
		veloVec[1] = 0;
		veloVec[2] = 0;
	}
public:
	Source()
	{
		particles = new Particle[n];
		initVec();
	}

	Source(const Source & src){
		if (particles != nullptr)
		{
			delete[] particles;
		}
		particles = new Particle[src.n];
		initVec();
	}

	Source & operator=(const Source& src){
		if (particles != nullptr)
		{
			delete[] particles;
		}
		particles = new Particle[src.n];

		veloVec[0] = src.veloVec[0];
		veloVec[1] = src.veloVec[1];
		veloVec[2] = src.veloVec[2];
	}

	~Source(){
		if (particles)
		{
			delete[] particles;//delete crash
		}
	}

	Source(int Num, const float inMag, const float* inVeloVec, const float* inPosi)
		:Object(inPosi){
		n = Num;
		particles = new Particle[n+100];//testing
		float norm = inMag / sqrtf(inVeloVec[0] * inVeloVec[0] + inVeloVec[1] * inVeloVec[1] + inVeloVec[2] * inVeloVec[2]);
		for (int i = 0; i < 3; i++)
		{
			veloVec[i] = inVeloVec[i] * norm;
		}
	}

	Source(int Num, const float inMag, const float* inVeloVec, const float* inPosi, const float* inVelo)
		:Object(inPosi, inVelo){
		n = Num;
		particles = new Particle[n];
		float norm = inMag / sqrtf(inVeloVec[0] * inVeloVec[0] + inVeloVec[1] * inVeloVec[1] + inVeloVec[2] * inVeloVec[2]);
		for (int i = 0; i < 3; i++)
		{
			veloVec[i] = inVeloVec[i] * norm;
		}
	}

	void SetVeloNMag(const float inMag, const float* inVeloVec){
		float norm = inMag / sqrtf(inVeloVec[0] * inVeloVec[0] + inVeloVec[1] * inVeloVec[1] + inVeloVec[2] * inVeloVec[2]);
		for (int i = 0; i < 3; i++)
		{
			veloVec[i] = inVeloVec[i] * norm;
		}
	}

	void Launch(int timer){
		float tempVelo[3];
		if (timer >= n)
		{
			//timer = timer % n;
			timer = n;
		}

		for (int i = 0; i < 3; i++)
		{
			tempVelo[i] = veloVec[i];
			float r = 0.0;
			r = (static_cast <float> (rand()) / static_cast <float> (RAND_MAX))*0.8 - 0.4;
			tempVelo[i] += r;
		}

		if (timer < n)//¨¾¤î¶W¬É
		{
			particles[timer].Set(position, tempVelo);
		}
		else
		{
			particles[timer-1].Set(position, tempVelo);
		}

		float posi[3], velo[3], acce[3];
#ifndef useParallel
		for (int i = 0; i < timer; i++)
		{
			particles[i].GetAccer(acce);
			particles[i].GetVelo(velo);
			particles[i].GetPosi(posi);
			for (int j = 0; j < 3; j++)
			{
				posi[j] = posi[j] + velo[j];
				velo[j] = velo[j] + acce[j];
			}
			particles[i].Set(posi, velo);
		}
#else
		parallel_for(0, n, [&](unsigned int i)
		{
			particles[i].GetAccer(acce);
			particles[i].GetVelo(velo);
			particles[i].GetPosi(posi);
			for (int j = 0; j < 3; j++)
			{
				posi[j] = posi[j] + velo[j];
				velo[j] = velo[j] + acce[j];
			}
			particles[i].Set(posi, velo);		
		});
#endif
	}

	void GetParPosi(int nnn, float *inPosi){
		particles[nnn].GetPosi(inPosi);
	}

	float GetAbsParAcce(int nnn){
		float a[3];
		particles[nnn].GetAccer(a);

		return sqrtf(a[0] * a[0] + a[1] * a[1] + a[2] * a[2]);
	}

	float GetAbsParVelo(int nnn){
		float a[3];
		particles[nnn].GetVelo(a);

		return sqrtf(a[0] * a[0] + a[1] * a[1] + a[2] * a[2]);
	}

	void gravityReset(){
		for (int i = 0; i < n; i++)
		{
			float zz[3] = { 0, 0, 0 };
			particles[i].SetAccer(zz);
		}
	}

	void AllReset(){
		for (int i = 0; i < n; i++)
		{
			float zz[3] = { 0, 0, 0 };
			particles[i].SetAccer(zz);
			particles[i].Set(zz, zz);
		}
	}

	void gravityByGravityField(Field & field, bool PoM){
		float accer[3], accer0[3];
		for (int i = 0; i < n; i++)
		{
			particles[i].calsSphereGravityFieldAccerVec(field, accer, PoM);
			particles[i].GetAccer(accer0);
			for (int j = 0; j < 3; j++)
			{
				accer0[j] = accer0[j] + accer[j];
			}
			particles[i].SetAccer(accer0);
		}
	}

	void gravityFlat(){
		Field Fg;
		for (int i = 0; i < n; i++)
		{
			particles[i].SetFieldAccel(Fg);
		}
	}

	void gravityFlat(Field & Fg){
		for (int i = 0; i < n; i++)
		{
			particles[i].SetFieldAccel(Fg);
		}
	}

	virtual void Show() const {
		std::cout << "Num: " << n << "\n";
		std::cout << "Pos: " << position[0] << ", " << position[1] << ", " << position[2] << "\n";
		std::cout << "Vec: " << veloVec[0] << ", " << veloVec[1] << ", " << veloVec[2] << "\n";
		std::cout << "Vec Mag: " << velocityScale << "\n";
	}
};
#endif