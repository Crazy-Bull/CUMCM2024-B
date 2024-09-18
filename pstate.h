#ifndef PSTATE_H
#define PSTATE_H


const double p_A=0.1,p_B=0.1,p_C=0.1;
const double c_A=4,c_B=18,c_C=6,c_tA=2,c_tB=3,c_tC=3,c_xchg=6,c_dcmp=5;
const double eps=1e-8;
const double default_ev=56;

class pstate
{
public:
	double p[2][2];
	pstate();
	pstate(double p_A,double p_B);
	pstate(double p_00,double p_01,double p_10,double p_11);
	bool operator== (pstate other);
	double p_rjA();
	double p_rjB();
	pstate xchgA();
	pstate xchgB();
	pstate fxchgA();
	pstate fxchgB();
	pstate sucA();
	pstate sucB();
	pstate reject();
	unsigned long long hashId();
	//若允许通过某函数到达下个状态，则为其id，否则为0 
	unsigned long long nextIds[7];
	double EV;
	void disp();
	void disp_p();
};


#endif


