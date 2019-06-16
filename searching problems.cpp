#include<bits/stdc++.h>

using namespace std;

#define State vector<int>
const int INF=INT_MAX;
enum Move{Up,Down,Left,Right};

class Problem{
public:
	Problem(){ }
	Problem(State inist){
		for (int i=0;i<9;++i){
			initial.push_back(inist[i]);
			goal.push_back((i+1)%9);
		}
	}
	State initialState(){ return initial; }
	State goalState(){ return goal; }
	int funcH(State curst){
		int ans=0;
		for (int i=0;i<9;++i){
			if(curst[i]==0)
				continue;
			int cnt=curst[i]-1;
			ans+=abs(cnt/3-i/3)+abs(cnt%3-i%3);
		}
		return ans;
	}
	vector<Move> actions(State curst){
		int ind;
		for (int i=0;i<9;++i)
			if(curst[i]==0)
				ind=i;
		vector<Move> act;
		if(ind/3>0)
			act.push_back(Up);
		if(ind/3<2)
			act.push_back(Down);
		if(ind%3>0)
			act.push_back(Left);
		if(ind%3<2)
			act.push_back(Right);
		return act;
	}
	State result(State curst,Move mv){
		int ind;
		for (int i=0;i<9;++i)
			if(curst[i]==0)
				ind=i;
		if(mv==Up)
			swap(curst[ind],curst[ind-3]);
		if(mv==Down)
			swap(curst[ind],curst[ind+3]);
		if(mv==Left)
			swap(curst[ind],curst[ind-1]);
		if(mv==Right)
			swap(curst[ind],curst[ind+1]);
		return curst;
	}
	bool goalTest(State st){
		for (int i=0;i<8;++i)
			if(st[i]!=i+1)
				return false;
		return true;
	}
private:
	State initial;
	State goal;
};

class Algorithm{
public:
	Algorithm(Problem prob){ this->prob=prob; }
	bool BreadthFirst(){
		allnd=mxsz=0;
		path.clear();
		queue<State> que;
		map<State,int> mark;
		map<State,int> dist;
		map<State,State> par;

		State init=prob.initialState();
		que.push(init);
		mark[init]=1;
		dist[init]=0;
		par[init]=init;
		++allnd;
		mxsz=1;
		while(!que.empty()){
			State cur=que.front();
			que.pop();
			mark[cur]=2;
			vector<Move> mv=prob.actions(cur);
			for (int i=0;i<mv.size();++i){
				State nx=prob.result(cur,mv[i]);
				if(mark.find(nx)==mark.end()){
					mark[nx]=1;
					que.push(nx);
					par[nx]=cur;
					dist[nx]=dist[cur]+1;
					++allnd;
					mxsz=max(mxsz,(int)que.size());
					if(prob.goalTest(nx)){
						State it=nx;
						while(it!=par[it]){
							path.push_back(it);
							it=par[it];
						}
						path.push_back(it);
						distgoal=path.size()-1;
						return true;
					}
				}
			}
		}
		distgoal=-1;
		return false;
	}
	bool DepthLimited(int maxL){
		allnd=mxsz=0;
		path.clear();
		stack<State> stk;
		map<State,int> mark;
		map<State,int> dist;
		map<State,State> par;

		State init=prob.initialState();
		stk.push(init);
		mark[init]=1;
		dist[init]=0;
		par[init]=init;
		++allnd;
		mxsz=1;
		while(!stk.empty()){
			State cur=stk.top();
			if(mark[cur]==2){
				stk.pop();
				continue;
			}
			mark[cur]=2;
			if(dist[cur]==maxL)
				continue;
			vector<Move> mv=prob.actions(cur);
			for (int i=0;i<mv.size();++i){
				State nx=prob.result(cur,mv[i]);
				if(mark.find(nx)==mark.end() or dist[nx]>dist[cur]){
					stk.push(nx);
					mark[nx]=1;
					dist[nx]=dist[cur]+1;
					par[nx]=cur;
					++allnd;
					mxsz=max(mxsz,(int)stk.size());
					if(prob.goalTest(nx)){
						State it=nx;
						while(it!=par[it]){
							path.push_back(it);
							it=par[it];
						}
						path.push_back(it);
						distgoal=path.size()-1;
						return true;
					}
				}
			}
		}
		distgoal=-1;
		return false;
	}
	bool DepthFirst(){
		return DepthLimited(INF);
	}
	bool IterativeDeepening(){
		if (DepthFirst()==false)
			return false;
		int sumnd=0,ttlmx=0;
		for(int i=1; ;++i){
			bool done=DepthLimited(i);
			sumnd+=allnd;
			ttlmx=max(ttlmx,mxsz);
			if(done){
				allnd=sumnd;
				mxsz=ttlmx;
				return true;
			}
		}
	}
	bool Bidirectional(){
		if(BreadthFirst()==false){
			distgoal=-1;
			return false;
		}

		allnd=mxsz=0;
		path.clear();
		queue<State> qini,qgl;
		map<State,int> mark;
		map<State,State> parini,pargl;

		State init=prob.initialState();
		State goal=prob.goalState();
		mark[init]=1;
		mark[goal]=3;
		parini[init]=init;
		pargl[goal]=goal;
		qini.push(init);
		qgl.push(goal);
		allnd+=2;
		mxsz=2;
		while(true){
			State curini=qini.front();
			qini.pop();
			mark[curini]=2;
			vector<Move> mvini=prob.actions(curini);
			for (int i=0;i<mvini.size();++i){
				State nx=prob.result(curini,mvini[i]);
				if(mark.find(nx)==mark.end()){
					mark[nx]=1;
					qini.push(nx);
					parini[nx]=curini;
					++allnd;
					mxsz=max(mxsz,(int)qini.size()+(int)qgl.size());
				}
				if(mark[nx]==3){
					parini[nx]=curini;
					vector<State> pci,pcg;
					State it=nx;
					while(it!=parini[it]){
						pci.push_back(it);
						it=parini[it];
					}
					pci.push_back(it);
					it=nx;
					while(it!=pargl[it]){
						pcg.push_back(it);
						it=pargl[it];
					}
					pcg.push_back(it);
					for (int j=pcg.size()-1;j>=0;--j)
						path.push_back(pcg[j]);
					for (int j=1;j<pci.size();++j)
						path.push_back(pci[j]);
					distgoal=path.size()-1;
					return true;
				}
			}

			State curgl=qgl.front();
			qgl.pop();
			mark[curgl]=4;
			vector<Move> mvgl=prob.actions(curgl);
			for (int i=0;i<mvgl.size();++i){
				State nx=prob.result(curgl,mvgl[i]);
				if(mark.find(nx)==mark.end()){
					mark[nx]=3;
					qgl.push(nx);
					pargl[nx]=curgl;
					++allnd;
					mxsz=max(mxsz,(int)qini.size()+(int)qgl.size());
				}
				if(mark[nx]==1){
					pargl[nx]=curgl;
					vector<State> pci,pcg;
					State it=nx;
					while(it!=parini[it]){
						pci.push_back(it);
						it=parini[it];
					}
					pci.push_back(it);
					it=nx;
					while(it!=pargl[it]){
						pcg.push_back(it);
						it=pargl[it];
					}
					pcg.push_back(it);
					for (int j=pcg.size()-1;j>=0;--j)
						path.push_back(pcg[j]);
					for (int j=1;j<pci.size();++j)
						path.push_back(pci[j]);
					distgoal=path.size()-1;
					return true;
				}
			}			
		}
	}
	bool UnifromCost(){
		allnd=mxsz=0;
		path.clear();
		set<pair<int,State> > st;
		map<State,int> mark;
		map<State,int> dist;
		map<State,State> par;

		State init=prob.initialState();
		mark[init]=1;
		dist[init]=0;
		par[init]=init;
		st.insert(make_pair(0,init));
		++allnd;
		mxsz=1;
		while(!st.empty()){
			auto it=st.begin();
			State cur=it->second;
			st.erase(it);
			mark[cur]=2;
			if(prob.goalTest(cur)){
				State trj=cur;
				while(trj!=par[trj]){
					path.push_back(trj);
					trj=par[trj];
				}
				path.push_back(trj);
				distgoal=dist[cur];
				return true;
			}
			vector<Move> mv=prob.actions(cur);
			for (int i=0;i<mv.size();++i){
				State nx=prob.result(cur,mv[i]);
				if(mark.find(nx)==mark.end() or mark[nx]==1){
					mark[nx]=1;
					if(dist.find(nx)==dist.end()){
						dist[nx]=dist[cur]+1;
						par[nx]=cur;
						++allnd;
					}
					if(dist[nx]>dist[cur]+1){
						st.erase(make_pair(dist[nx],nx));
						dist[nx]=dist[cur]+1;
						par[nx]=cur;
					}
					st.insert(make_pair(dist[nx],nx));
					mxsz=max(mxsz,(int)st.size());
				}
			}
		}
		distgoal=-1;
		return false;
	}
	bool AStar(){
		allnd=mxsz=0;
		path.clear();
		set<pair<int,State> > st;
		map<State,int> mark;
		map<State,int> dist;
		map<State,State> par;

		State init=prob.initialState();
		mark[init]=1;
		dist[init]=0;
		par[init]=init;
		st.insert(make_pair(prob.funcH(init),init));
		++allnd;
		mxsz=1;
		while(!st.empty()){
			auto it=st.begin();
			State cur=it->second;
			st.erase(it);
			mark[cur]=2;
			if(prob.goalTest(cur)){
				State it=cur;
				while(it!=par[it]){
					path.push_back(it);
					it=par[it];
				}
				path.push_back(it);
				distgoal=dist[cur];
				return true;
			}
			vector<Move> mv=prob.actions(cur);
			for (int i=0;i<mv.size();++i){
				State nx=prob.result(cur,mv[i]);
				if(mark.find(nx)==mark.end() or mark[nx]==1){
					mark[nx]=1;
					if(dist.find(nx)==dist.end()){
						dist[nx]=dist[cur]+1;
						par[nx]=cur;
						++allnd;
					}
					if(dist[nx]>dist[cur]+1){
						st.erase(make_pair(dist[nx]+prob.funcH(nx),nx));
						dist[nx]=dist[cur]+1;
						par[nx]=cur;
					}
					st.insert(make_pair(dist[nx]+prob.funcH(nx),nx));
					mxsz=max(mxsz,(int)st.size());	
				}
			}
		}
		distgoal=-1;
		return false;
	}
	void printPath(string algo){
		cout << endl << algo << " : " << endl;
		cout << "All Nodes:" << allnd << "   ";
		cout << "Goal Distance:" << distgoal << "   ";
		cout << "Max Memory:" << mxsz << endl;
		if (algo=="Depth First Search")
			return;
		for (int i=path.size()-1;i>=0;--i){
			for (int j=0;j<9;++j)
				cout << path[i][j];
			cout << endl;
		}
	}
private:
	int allnd,mxsz,distgoal;
	Problem prob;
	vector<State> path;
};

int main(){
	vector<int> init;
	for (int i=0;i<9;++i){
		int x;
		cin >> x;
		init.push_back(x);
	}
	Problem prob(init);
	Algorithm algo(prob);

	algo.BreadthFirst();
	algo.printPath("Breadth First Search");
	algo.DepthLimited(10);
	algo.printPath("Depth Limited Search(10)");
	algo.DepthFirst();
	algo.printPath("Depth First Search");
	algo.IterativeDeepening();
	algo.printPath("Iterative Deepening Search");
	algo.Bidirectional();
	algo.printPath("Bidirectional Search");
	algo.UnifromCost();
	algo.printPath("UnifromCost Seach");
	algo.AStar();
	algo.printPath("AStar Search");
}
