#include <bits/stdc++.h>
using namespace std;
#define ll long long
#define NO_OF_NODES 5000
#define ITERATION_COUNT 100
struct Edge
{
    ll startNode;
    ll endNode;
    ll weight;
};

int nodePartitionTrack[NO_OF_NODES]; // 0 if not partitioned, 1 if partition 1, 2 if partition 2.......
int nodes;

vector<pair<ll, ll>> edges[NO_OF_NODES];

void resetPartition()
{
    for(int i=0;i<=nodes;i++)
    {
        nodePartitionTrack[i]=true;
    }
}

ll randomInRange(ll l, ll r) /// l inclusive, but r is exclusive
{
    return random() % (r - l + 1) + l;
}

ll sigmaFunction(int node, int partitionNo) // node is in partitionNo...
{
    ll w = 0;
    for (auto it : edges[node])
    {
        if (nodePartitionTrack[it.first] != 0 && nodePartitionTrack[it.first] != partitionNo)
            w += it.second;
    }
    return w;
}

Edge *randomPartitionStart(double alpha) // returns the first edge selected for the partitions.......
{

    vector<Edge *> SelectedEdge;
    ll maximumWeight = -1e18, minimumWeight = 1e18;

    for (int i = 1; i <= nodes; i++)
    {
        for (auto it : edges[i])
        {
            if (it.second > maximumWeight)
                maximumWeight = it.second;
            if (it.second < minimumWeight)
                minimumWeight = it.second;
        }
    }

    ll threshold = minimumWeight + alpha * (maximumWeight - minimumWeight);
    for (int i = 1; i <= nodes; i++)
    {
        for (auto it : edges[i])
        {
            if (it.second >= threshold)
            {
                Edge *edge = new Edge;
                edge->startNode = i;
                edge->endNode = it.first;
                edge->weight = it.second;
                SelectedEdge.push_back(edge);
            }
        }
    }

    int ran = randomInRange(0, SelectedEdge.size());

    return SelectedEdge[ran];
}

void createPartition(double alpha)
{
    // entering the first pair of nodes.................
    Edge *edge = randomPartitionStart(alpha);
    nodePartitionTrack[edge->startNode] = 1;
    nodePartitionTrack[edge->endNode] = 2;

    // this loops will go on until all nodes are partitioned...............
    while (1)
    {
        //finding all non partitioned nodes...................
        vector<ll> nonCatagorizedNodes;
        for (ll i = 1; i <= nodes; i++)
        {
            if (nodePartitionTrack[i] == 0)
                nonCatagorizedNodes.push_back(i);
        }
        // if no node is remaining non partitioned, break this loop
        if(nonCatagorizedNodes.size()==0)
            break;

        // find the value a node could gain if added to any of the two partitions............
        vector<pair<ll, ll>> sigmaValuePair;
        ll minSigmaValue = 1e18, maxSigmaValue = -1e18;
        for (auto it : nonCatagorizedNodes)
        {
            ll w1 = sigmaFunction(it, 1);
            ll w2 = sigmaFunction(it, 2);
            sigmaValuePair.push_back({w1, w2});
            minSigmaValue = min(minSigmaValue, min(w1, w2));
            maxSigmaValue = max(maxSigmaValue, max(w1, w2));
        }

        /// select best nodes for random selection....................
        ll threshold = minSigmaValue + alpha * (maxSigmaValue - minSigmaValue);
        vector<ll> selectedNodes;
        int i = 0;
        for (auto it : sigmaValuePair)
        {
            if (threshold <= max(it.first, it.second))
            {
                selectedNodes.push_back(nonCatagorizedNodes[i]);
            }
            i++;
        }

        /// selecting a node and adding it to the best suited partition...............
        int selectedIndex=randomInRange(0,selectedNodes.size());
        if(sigmaValuePair[selectedIndex].first>=sigmaValuePair[selectedIndex].second)
            nodePartitionTrack[nonCatagorizedNodes[selectedIndex]]=1;
        else
            nodePartitionTrack[nonCatagorizedNodes[selectedIndex]]=2;

    }
}

ll calculationAfterCreatingPartition()
{
    ll ans=0;
    for(int i=1;i<=nodes;i++)
    {
        if(nodePartitionTrack[i]==1)
        {
            for(auto it:edges[i])
            {
                if(nodePartitionTrack[it.first]==2)
                    ans+=it.second;
            }
        }
    }
    return ans;

}


pair<ll,ll> bestNodeToExchange() // return the best node to move and the gain from it.
{
    ll  best=-1e18,node=-1;
    for(int i=1;i<=nodes;i++)
    {
        if(nodePartitionTrack[i]==1)
        {
            ll partition1_value=sigmaFunction(i,1);
            ll partition2_value=sigmaFunction(i,2);
            if(partition2_value-partition1_value>best)
            {
                best=partition2_value-partition1_value;
                node=i;
            }
        }
        if(nodePartitionTrack[i]==2)
        {
            ll partition1_value=sigmaFunction(i,1);
            ll partition2_value=sigmaFunction(i,2);
            if(partition1_value-partition2_value>best)
            {
                best=partition1_value-partition2_value;
                node=i;
            }
        }
    }

    return {node,best};

}

ll localSearchOnPartition(int maxTillNow) 
{
    bool better=true;
    ll bestAnswer=maxTillNow;
    while(better)
    {
        better=false;
        pair<ll,ll> p=bestNodeToExchange();
        if(p.second>0) // a gain!!!
        {
            bestAnswer+=p.second;
            if(nodePartitionTrack[p.first]==1)
            {
                nodePartitionTrack[p.first]=2;
            }
            else
                nodePartitionTrack[p.first]=1;
            
            better=true;
        }
    }
    return bestAnswer;
}


ll oneIteration()
{
    double alpha=0.8;
    resetPartition();
    createPartition(alpha);
    ll maxTillNow=calculationAfterCreatingPartition();
    ll bestAnswer=localSearchOnPartition(maxTillNow);
    return bestAnswer;

}




int main()
{
    freopen("input.txt", "r", stdin);
    int m;
    cin>>nodes>>m;
    for(int i=0;i<m;i++)
    {
        ll l,r,w;
        cin>>l>>r>>w;
        edges[l].push_back({r,w});
        edges[r].push_back({l,w});
    }
    ll ans=0;
    for(int i=1;i<=ITERATION_COUNT;i++)
    {

        ll p=oneIteration();
        if(p>ans)
        {
            ans=p;
            cout<<"Iteration "<<i<<": "<<ans<<endl;
        }
        
    }
    
}
