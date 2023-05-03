#include<bits/stdc++.h>
using namespace std;
// for prototype testing
#define ADMIN 0 

// for adj mat view in each step
#define LIST_VIEW 0 

// for my testing
#define ADMIN2 0 


struct dsu{
    vector<int> parent,sz;
    int n;
    dsu(int len){
        n = len;
        parent.resize(n);
        sz.resize(n,1);
        iota(parent.begin(),parent.end(),0);
    }
    int find(int v){
        //find parent of vertex v
        if (parent[v]==v) return v;
        else {
            while(parent[v] != v){
                parent[v] = parent[parent[v]];
                v = parent[v];
            }
            return parent[v];
        }
    }
    void merge(int u, int v){
        int pu = find(u);
        int pv = find(v);
        if (pu!=pv)  {
        //if not already connected
            if (sz[pu] < sz[pv]) swap(pu,pv);
            //pu is larger tree and pv is smainter tree : pv -> pu
            sz[pu] += sz[pv];sz[pv]=0;
            parent[pv] = pu;
        }
    }
    int getsz(int v) {return sz[v];}
};


void solve(){
    int N,M;
    int nN,nM;
    int n,m; 
    int cutx,cuty;
    int joinx, joiny;
    int GOALx,GOALy;

    // cin >> n >> m;

    n = 5; m = 8;

    N = n; M = m;
    nN = 0; nM = 0; 

    vector<pair<int,int>> EGs = {
        {1,2}, {2,3}, {3,4},{4,5}, {1,5}, {1,4}, {1,3}, {2,4}
    };

    GOALx = 2;
    GOALy = 5;
    

    set<int> notNode;
    vector<vector<int>> adj(n+5, vector<int>(n+5,0));

    int V = n;
    int E = n - 1;

    for(int i=0;i<m;i++){
        int a,b; 
        a = EGs[i].first;
        b = EGs[i].second;
        adj[a][b]++;
        adj[b][a]++;
    }



    int tree_found = 0;
    int t1,t2;

    function<int()> WIN = [&]{
        adj[cutx][cuty]--;
        adj[cuty][cutx]--;

        int WIN_BY_JOIN = 0;

        if(adj[GOALx][GOALy]>0 && adj[GOALy][GOALx]>0)
            WIN_BY_JOIN = 1;

        adj[cutx][cuty]++;
        adj[cuty][cutx]++;

        return WIN_BY_JOIN;
    };


    function<void()> print = [&](){
        int PP = 15;
        for(int i=0;i<PP;i++) cout << "-" ; cout << "\n";
        for(int i=1;i<=N;i++){
            cout << "node : " << i << "  ==>  ";
            if(notNode.count(i)){
                cout << "NOT FOUND!\n";
                continue;
            }
            for(int j=1;j<=N;j++)
                if(adj[i][j] > 0)
                    cout << "( "<<  j<<", " << adj[i][j] << " )   ->";
            cout << "\n"; 
        }
        for(int i=0;i<PP;i++) cout << "-" ; cout << "\n";
    };


    function<int()> SPAN = [&](){
        int ec  = 0, vc = 0;
        vector<pair<int,int>> list;
        vector<vector< int >> all_tree ;

        for(int i=1;i<=N;i++){
            if(notNode.count(i))
                continue;

            for(int j=1;j<=i;j++){
                if( adj[i][j]>0 ){
                    ec += adj[i][j];
                    int parellel = adj[i][j];
                    while(parellel--)
                        list.push_back({i,j});
                }
            }
            vc++;
        }

        // vector<int> idx(ec);
        // iota(idx.begin(),idx.end(),0);
        // ec -> vc-1 ( select )

        vector<int> dsa;

        function<void(int)> dfs = [&](int idx){
            if(idx == ec){
                if(dsa.size()==vc-1){
                    // check that the tree is connected or not
                    dsu d(N+1);
                    for(int id=0;id<vc-1;id++){
                        auto pp = list[ dsa[id] ];
                        d.merge(pp.first, pp.second);
                    }
                    set<int> ccs; // connected components
                    for(int nd=1;nd<=N;nd++){
                        if(notNode.count(nd))
                            continue;
                        
                        ccs.insert( d.find(nd)  );
                    }
                    if(ccs.size()==1) // this is spanning tree
                        all_tree.push_back(dsa);
                }
                return;
            }

            dsa.push_back(idx);
            dfs(idx+1);

            dsa.pop_back();
            dfs(idx+1);
        };


        dfs(0);


        function<int(int,int)> dis = [&](int a, int b){
            // distance between all_tree[a] and all_tree[b]

            vector<int> T1 = all_tree[a];
            vector<int> T2 = all_tree[b];

            // check for this 2 spanning tree

            multiset<int> T_;
            for(auto &x : T1)
                T_.insert(x);

            int dd = 0;

            for(auto &x : T2)
                if( T_.count(x) )
                    dd++;

            return dd;
        };


        // now check for distance
        int total_tree = all_tree.size();
        vector<pair<int,int>> zero,one,more;

        for(int i=0;i<total_tree;i++){
            for(int j=i+1;j<total_tree;j++){
                int distance = dis(i,j);
                if(distance == 0)
                    zero.push_back({i,j});
                else if(distance == 1)
                    one.push_back({i,j});
                else
                    more.push_back({i,j});
            }
        }

        if( ADMIN ){
            cout << "** EDGE MAPPING \n";
            for(int i=0;i<ec;i++){
                cout << i << " => ";
                cout << list[i].first << " to " << list[i].second << "\n";
            }
        }

        if( ADMIN2 ){
            cout << " *** LIST OF SPANNING TREE \n";
            for(int i=0;i<total_tree;i++){
                cout << i << "-> ";
                for(auto &x : all_tree[i])
                    cout <<x <<" "; cout << "\n";
            }

            cout << " *** ZERO DISTANCE \n";
            for(auto &p : zero)
                cout << p.first << " " << p.second << "\n";
        }

        // cout << " *** ONE DISTANCE \n";
        // for(auto &p : one)
        //     cout << p.first << " " << p.second << "\n";

        // cout << " *** MORE DISTANCE \n";
        // for(auto &p : more)
        //     cout << p.first << " " << p.second << "\n";

        // WORKING CORRECT  :: BHAI BHAI BHAI BHAI !!!


        // if(zero.size() > 0){
        //     t1 = zero[0].first;
        //     t2 = zero[0].second;
        // }

        if(zero.empty()){
            cout << "NOT DISJOINT : CASE NOT CURRENTLY AVAILABLE\n";
            return 0;
        }
        

        // t1 and t2 are 2 spanning tree
        pair<int,int> cut_edge1 = {cutx, cuty};
        pair<int,int> cut_edge2 = {cutx, cuty};

        int cut_edge;

        // find the index of this 
        // CUT a je edge kadhi eni ID find kari (0 to 7 ) mathi

        for(int id=0;id<ec;id++){
            if(cut_edge1==list[id] || cut_edge2==list[id]){
                cut_edge = id;
                break;
            }
        }

        // T1 - T2 selection is based on what edge is removed by CUT

        // from zero distance 
        // find such pair of (t1,t2) such that 
        // koi ek ma CUT a je kadhi a hoi


        for(auto &zp : zero){
            int _t1 = zp.first;
            int _t2 = zp.second;

            vector<int> _T1 = all_tree[_t1];
            vector<int> _T2 = all_tree[_t2];
            
            int mali = 0 ;
            for(auto &x : _T1)
                if(x == cut_edge)
                    mali = 1;
            for(auto &x : _T2)
                if(x == cut_edge)
                    mali = 1;

            if(mali){
                t1 = _t1;
                t2 = _t2;
                break;
            }
        }


        // now I know the edge which is removed by the player CUT

        // now chech that edge in T1 or T2

        bool inT1 = 0;
        for( auto &pp : all_tree[t1] ){
            if(pp == cut_edge)
                inT1 = 1;
        }

        if(!inT1)
            swap(t1,t2);

        // t1 and t2 a all_tree in index che
        vector<int>  T1 = all_tree[t1];
        vector<int>  T2 = all_tree[t2];

        if(ADMIN){
            cout << "edge id in T1 # ";
            for(auto &x : T1)
                cout << x << " "; cout << "\n";
            cout << "edge id in T2 # ";
            for(auto &x : T2)
                cout << x <<" "; cout << "\n";
        }

        // now the cut_edge in t1 only

        // SPLIT T1 in R and S by removing the edge

        dsu d(N+1);
        for( auto &id : all_tree[t1]){
            if(cut_edge == id)
                continue;
            
            pair<int,int> pp = list[id];

            d.merge(pp.first,pp.second);
        }

        map<int,vector<int>> ccs; // connected componet must be 2
        // R and S
        // R and S na head ne lai lau


        for(int i=1;i<=N;i++){
            if(notNode.count(i))
                continue;
            ccs[d.find(i)].push_back(i);
        }

        vector<int> R,S;

        int flp = 1;
        for(auto &pp : ccs){
            if(flp)
                R = pp.second;
            else
                S = pp.second;
            flp ^= 1;
        }   

        if(ADMIN){
            cout << "vertex in R # ";
            for(auto &x : R)
                cout << x << " "; cout << "\n";
            cout << "vertex in S # ";
            for(auto &x : S)
                cout << x <<" "; cout << "\n";
        }

        for(int rrr=0;rrr<R.size();rrr++){
            for(int sss=0;sss<S.size();sss++){
                pair<int,int> dum1 = { R[rrr], S[sss] };
                pair<int,int> dum2 = { S[sss], R[rrr] };

                for(auto &ppp : list){
                    if(ppp == dum1 || ppp == dum2){
                        // JOIN ne edge mali gai
                        joinx = ppp.first;
                        joiny = ppp.second;
                        break;
                    }
                }
            }
        }

        for(auto &A : R){
            for(auto &B : S){
                pair<int,int> AB = {A,B};
                pair<int,int> BA = {B,A};

                int e_id; // A - B ni id mali gai graph ma
                
                for(int ei=0;ei<list.size();ei++){
                    pair<int,int> P = list[ei];

                    if(AB == P || BA == P){
                        e_id = ei;
                        break;
                    }
                }

                // check that aa eid in T2 or not ( such that I can merge those vertex )

                for(auto &e2 : all_tree[t2])
                    if(e2 == e_id){  // aa edge T2 ma che to ane merge kari sakay
                        joinx = list[e2].first;
                        joiny = list[e2].second;
                        break;
                    }
            }
        }

        // CUT will cut : cutx-cuty
        // JOIN will join : joinx-joiny

        return 1;
    };

    /*
        NOTE PELA T1 and T2 find karvana pachi ek jode 
        CUT NU step and JOIN nu step karvanu !!

        PELA CUT a je kadhi a remove nai karvani
        naitar G temper thai jase T1 and T2 find thaya pehala 
    */

    // cout << "HELLO WORLD\n";

    

    int TESTCASE = 4; // THIS IS IN ADMIN1 MODE

    cout << " ***** START THE GAME ***** \n\n"; 
    // print();

    while( N - notNode.size() > 2){

        if(ADMIN2){
            cout << "Not Nodes are :- " ;
            for(auto &x : notNode){
                cout << x << " ";
            }
            cout << "\n";
        }
        if( LIST_VIEW )
        print();

        // CUT MOVE
        cout << " **** PLAYER CUT (Human) **** \n";
        cout << " ** Enter your edge : ";
        cin >> cutx >> cuty ;

        if(WIN()){
            adj[GOALx][GOALy]--;
            adj[GOALy][GOALx]--;
            cout << "**** PLAYER JOIN (Computer) ****\n";
            cout << "** Reinforced edge : ";
            cout << GOALx << " " << GOALy << "\n";
            cout << "--------- HURRRAY!! JOIN WIN THE GAME ---------\n";
            return;
        }

        int possible = SPAN(); // joinx-joiny find karse

        if(!possible){
            return;
        }

        // cout << joinx <<" " << joiny << "\n";

        // continue;   

        // REMOVE EDGE cutx-cuty
        adj[cutx][cuty]--;
        adj[cuty][cutx]--;

        if( LIST_VIEW )
        print();

        if(joinx == GOALx || joinx == GOALy){ 
            
            for(int i=1;i<=N;i++)
                swap(adj[joinx][i],adj[joiny][i]);

            for(int i=1;i<=N;i++)
                swap(adj[i][joinx],adj[i][joiny]);

            swap(joinx,joiny);
        }

        // REINFORCE joinx-joiny

        // joinx kadhvanu
        // joiny rakhvanu

        adj[joinx][joiny]=0;
        adj[joiny][joinx]=0;

        vector<int> kids;// x na kids

        for(int i=1;i<=N;i++){
            if(notNode.count(i))
                continue;
            if(adj[joinx][i]){
                adj[joinx][i] = 0;
                adj[i][joinx] = 0;
                kids.push_back(i);
            }
        }
        
        for(auto &j : kids){
            adj[joiny][j]++;
            adj[j][joiny]++;
        }

        notNode.insert(joinx); // join x is now no longer a node

        // BHAI ++ and -- JOISE !!!

        // JOIN MOVE
        
        // print();

        // UPDATE THE GOAL IN CASE OF Merging Vertex of Goal 
        // Goal Vertices might become notNode

        // 1 & 5 join thase to 5 -> 1 thai jase

        // 2 5
        // 1 5
        // if(joinx == GOALx){

        // }else if(joinx == GOALy){

        // }
        
        // if(joiny == GOALx){

        // }else if(joiny == GOALy){

        // }


        cout << "**** PLAYER JOIN (Computer) ****\n";
        cout << "** Reinforced edge : ";
        cout << joinx << " " << joiny << "\n";

        // reinforce logic

        // TESTCASE--; // ADMIN2 MODE
        // print();
    }

    cout << "-- CUT(Human) WIN -- THIS ALGORITHM NOT WORK --\n";
}




void run(){
    #ifndef ONLINE_JUDGE 
    freopen("in.txt","r",stdin);
    freopen("out.txt","w",stdout);
    #endif
    // cout << fixed << setprecision(10);
}

int32_t main(){
    // run();
    solve();
}


/*

5 8
1 2
2 3
3 4
4 5
1 5
1 4
1 3
2 4

*/