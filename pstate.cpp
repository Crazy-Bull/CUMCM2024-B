#include <bits/stdc++.h>
#include "pstate.h"
using namespace std;

pstate::pstate()
{
	pstate(0,0);
}

pstate::pstate(double p_A,double p_B)
{
	p[0][0]=p_A*p_B;
	p[0][1]=p_A*(1-p_B);
	p[1][0]=(1-p_A)*p_B;
	p[1][1]=(1-p_A)*(1-p_B);
	EV=default_ev;
	for(int i=0;i<=6;i++) nextIds[i]=0;
}

pstate::pstate(double p_00,double p_01,double p_10,double p_11)
{
	p[0][0]=p_00;
	p[0][1]=p_01;
	p[1][0]=p_10;
	p[1][1]=p_11;
	EV=default_ev; 
	for(int i=0;i<=6;i++) nextIds[i]=0;
}

bool pstate::operator== (pstate other)
{
	for(int i=0;i<=1;i++)
		for(int j=0;j<=1;j++)
			if(abs(p[i][j]-other.p[i][j])>eps) return 0;
	return 1;
}

double pstate::p_rjA()
{
	return p[0][0]+p[0][1];
}

double pstate::p_rjB()
{
	return p[0][0]+p[1][0];
}

pstate pstate::xchgA()
{
	return pstate(p_A,p_rjB());
}

pstate pstate::xchgB()
{
	return pstate(p_rjA(),p_B);
}

pstate pstate::fxchgA()
{
	return (pstate(p[0][0]/p_rjA(),p[0][1]/p_rjA(),0,0)).xchgA();
}

pstate pstate::fxchgB()
{
	return (pstate(p[0][0]/p_rjB(),0,p[1][0]/p_rjB(),0)).xchgB();
}

pstate pstate::sucA()
{
	return pstate(0,0,p[1][0]/(1-p_rjA()),p[1][1]/(1-p_rjA()));
}

pstate pstate::sucB()
{
	return pstate(0,p[0][1]/(1-p_rjB()),0,p[1][1]/(1-p_rjB()));
}

pstate pstate::reject()
{
	double p_rj=1-p[1][1]*(1-p_C);
	return pstate(p[0][0]/p_rj,p[0][1]/p_rj,p[1][0]/p_rj,p[1][1]*p_C/p_rj);
}

unsigned long long pstate::hashId()
{
	const unsigned long long M=1ull<<15;
	unsigned long long mult=1,ret=0;
	for(int i=0;i<=1;i++)
		for(int j=0;j<=1;j++)
		{
			ret+=round(p[i][j]*M)*mult;
			mult*=M;
		}		
	return ret;
}

void pstate::disp()
{
	cout<<"Id:"<<hashId()<<endl;
	cout<<"Ev:"<<EV<<endl;
	cout<<"p:";
	for(int i=0;i<=1;i++)
		for(int j=0;j<=1;j++)
			cout<<p[i][j]<<" ";
	cout<<endl;
	
	cout<<"NextIds:";
	for(int i=0;i<=6;i++) cout<<nextIds[i]<<" ";
	cout<<endl<<endl; 
}

void pstate::disp_p()
{
	cout<<"[ ";
	for(int i=0;i<=1;i++)
		for(int j=0;j<=1;j++)
			cout<<p[i][j]<<" ";
	cout<<"]";
}
