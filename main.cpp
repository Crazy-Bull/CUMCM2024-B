#include <bits/stdc++.h>
#include "pstate.h"

using namespace std;

/* run this program using the console pauser or add your own getch, system("pause") or input loop */
queue<pstate> bfs_queue;
map<unsigned long long, pstate> state_map;
map<unsigned long long, int> strat;
set<unsigned long long> vis;

const int MAX_STATES = 500;
const int ITERATE_TIMES = 50;

void disp_strat_tree(unsigned long long start, int case_depth);

pstate phi0(p_A,p_B);

int main(int argc, char** argv) {

	double max_E=((c_A/(1-p_A)+c_B/(1-p_B))+c_C)/(1-p_C);
	double suc_rate_limit=(c_C+min(c_xchg, c_tC))/max_E;

	//bfs
	while(!bfs_queue.empty()) bfs_queue.pop();
	bfs_queue.push(phi0);
	
	while(!bfs_queue.empty()&&state_map.size()<=MAX_STATES)
	{
		pstate phi=bfs_queue.front();
		bfs_queue.pop();
		unsigned long long hashid=phi.hashId();
		if(!state_map.count(hashid))
		{
			if(abs(phi.p_rjA())>eps)
			{
				pstate nxt(0,0);
				if(abs(phi.p_rjA()-p_A)>eps)
				{
					nxt=phi.xchgA();
					bfs_queue.push(nxt);
					phi.nextIds[0]=nxt.hashId();
				}
				nxt=phi.fxchgA();
				bfs_queue.push(nxt);
				phi.nextIds[2]=nxt.hashId();
				nxt=phi.sucA();
				bfs_queue.push(nxt);
				phi.nextIds[4]=nxt.hashId();
			}
			if(abs(phi.p_rjB())>eps)
			{
				pstate nxt(0,0);
				if(abs(phi.p_rjB()-p_B)>eps)
				{
					nxt=phi.xchgB();
					bfs_queue.push(nxt);
					phi.nextIds[1]=nxt.hashId();
				}
				nxt=phi.fxchgB();
				bfs_queue.push(nxt);
				phi.nextIds[3]=nxt.hashId();
				nxt=phi.sucB();
				bfs_queue.push(nxt);
				phi.nextIds[5]=nxt.hashId();
			}
			if(phi.p[1][1]*(1-p_C)>suc_rate_limit)
			{
				pstate nxt=phi.reject();
				bfs_queue.push(nxt);
				phi.nextIds[6]=nxt.hashId();
			}
			state_map[hashid]=phi;
		}
	}

	for(int i=1;i<=ITERATE_TIMES;i++)
	{
		for(auto iter=state_map.begin();iter!=state_map.end();iter++)
		{
			pstate p=iter->second;	
			double new_EV=p.EV,tmp_EV;
			
			if(state_map.count(p.nextIds[0]))
			{
				tmp_EV=c_A+state_map[p.nextIds[0]].EV;
				if(tmp_EV<new_EV)
				{
					new_EV=tmp_EV;
					strat[iter->first]=1;
				}
			}
			if(state_map.count(p.nextIds[1]))
			{
				tmp_EV=c_B+state_map[p.nextIds[1]].EV;
				if(tmp_EV<new_EV)
				{
					new_EV=tmp_EV;
					strat[iter->first]=2;
				}
			}
			if(state_map.count(p.nextIds[2])&&state_map.count(p.nextIds[4]))
			{
				tmp_EV=min(new_EV,c_tA+p.p_rjA()*(c_A+state_map[p.nextIds[2]].EV)
						+(1-p.p_rjA())*state_map[p.nextIds[4]].EV); 
				if(tmp_EV<new_EV)
				{
					new_EV=tmp_EV;
					strat[iter->first]=3;
				}
			}
			if(state_map.count(p.nextIds[3])&&state_map.count(p.nextIds[5]))
			{
				tmp_EV=min(new_EV,c_tB+p.p_rjB()*(c_B+state_map[p.nextIds[3]].EV)
						+(1-p.p_rjB())*state_map[p.nextIds[5]].EV);
				if(tmp_EV<new_EV)
				{
					new_EV=tmp_EV;
					strat[iter->first]=4;
				}
			}
			if(state_map.count(p.nextIds[6]))
			{
				double p_rj=1-p.p[1][1]*(1-p_C);
				tmp_EV=min(new_EV,c_C+min(p_rj*c_xchg,c_tC)+
				p_rj*min(c_dcmp+state_map[p.nextIds[6]].EV,c_A+c_B+state_map[phi0.hashId()].EV)); 
				if(tmp_EV<new_EV)
				{
					new_EV=tmp_EV;
					strat[iter->first]=5;
				}
			}
			iter->second.EV=new_EV;
		}
		cout<<"Round:"<<i<<"  EV:"<<state_map[phi0.hashId()].EV<<endl;
	}
	cout<<endl<<"策略树："<<endl;
	disp_strat_tree(phi0.hashId(),0);
	return 0;
}

void tabs(int k)
{
	for(int i=1;i<=4*k;i++) cout<<" ";
}
void disp_strat_tree(unsigned long long start, int case_depth)
{
	if(vis.count(start)) return;
	if(case_depth>5) return;
	vis.insert(start);
	int s=strat[start];
	tabs(case_depth); 
	cout<<"概率状态 " ;
	pstate ps=state_map[start];
	ps.disp_p();
	cout<<" ，选择策略：" ;
	if(s==1)
	{
		cout<<"更换零件A，结果为 ";
		pstate nxt=state_map[ps.nextIds[0]];
		nxt.disp_p();
		cout<<endl;
		disp_strat_tree(ps.nextIds[0],case_depth);
	}
	if(s==2)
	{
		cout<<"更换零件B，结果为 ";
		pstate nxt=state_map[ps.nextIds[1]];
		nxt.disp_p();
		cout<<endl;
		disp_strat_tree(ps.nextIds[1],case_depth);
	}
	if(s==3)
	{
		cout<<"检测零件A"<<endl;
		tabs(case_depth);
		cout<<"若合格，结果为 ";
		pstate nxt1=state_map[ps.nextIds[4]];
		nxt1.disp_p();
		cout<<endl;
		disp_strat_tree(ps.nextIds[4],case_depth+1);
		cout<<endl; 
		tabs(case_depth);
		cout<<"若不合格，更换零件A，结果为 ";
		pstate nxt2=state_map[ps.nextIds[2]];
		nxt2.disp_p();
		cout<<endl;
		disp_strat_tree(ps.nextIds[2],case_depth+1);
	}
	if(s==4)
	{
		cout<<"检测零件B"<<endl;
		tabs(case_depth);
		cout<<"若合格，结果为 ";
		pstate nxt1=state_map[ps.nextIds[5]];
		nxt1.disp_p();
		cout<<endl;
		disp_strat_tree(ps.nextIds[5],case_depth+1);
		cout<<endl; 
		tabs(case_depth);
		cout<<"若不合格，更换零件B，结果为 ";
		pstate nxt2=state_map[ps.nextIds[3]];
		nxt2.disp_p();
		cout<<endl;
		disp_strat_tree(ps.nextIds[3],case_depth+1);
	}
	if(s==5)
	{
		cout<<"进行装配，成品";
		double p_rj=1-ps.p[1][1]*(1-p_C);
		if(p_rj*c_xchg<c_tC) cout<<"不";
		cout<<"检测"<<endl;
		tabs(case_depth);
		cout<<"若不合格/被退回，";
		if(c_dcmp+state_map[ps.nextIds[6]].EV<c_A+c_B+state_map[phi0.hashId()].EV)
		{
			cout<<"拆解，结果为 ";
			pstate nxt=state_map[ps.nextIds[6]];
			nxt.disp_p();
			cout<<endl;
			disp_strat_tree(ps.nextIds[6],case_depth);
		}
		else
		{
			cout<<"不拆解"<<endl; 
		}
	} 
}
