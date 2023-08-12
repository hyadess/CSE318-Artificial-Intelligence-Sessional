#include <bits/stdc++.h>
using namespace std;
#define ll long long
#define NO_OF_NODES 5000
#define ITERATION_COUNT 200

struct Edge
{
    ll startNode;
    ll endNode;
    ll weight;
};

int nodePartitionTrack[NO_OF_NODES]; // 0 if not partitioned, 1 if partition 1, 2 if partition 2.......
int bestNodePartition[NO_OF_NODES];
ll bestAns = -1e18;
int nodes;
vector<pair<ll, ll>> edges[NO_OF_NODES];

// random generations******************************************************************
double randomAlphaGeneration()
{
    // Seed the random number generator
    std::random_device rd;
    std::mt19937 gen(rd());

    // Create a uniform distribution between 0 and 1
    std::uniform_real_distribution<double> dis(0.0, 1.0);

    // Generate a random double between 0 and 1
    double randomValue = dis(gen);
    return randomValue;
}

ll randomInRange(ll l, ll r) /// l inclusive, but r is exclusive
{
    std::random_device rd;
    std::mt19937_64 gen(rd());

    // Create a uniform distribution for the specified range
    std::uniform_int_distribution<ll> dis(l, r - 1);

    // Generate a random integer within the specified range
    ll randomValue = dis(gen);
    return randomValue;
}

ll randomlyChooseZeroAndOne()
{
    // Seed the random number generator
    std::random_device rd;
    std::mt19937 gen(rd());

    // Create a Bernoulli distribution with probability p
    double p = 0.5; // Probability of success (1)
    std::bernoulli_distribution dis(p);

    // Generate random choices between 0 and 1
    int randomChoice = dis(gen);

    return randomChoice;
}

// partition Management for next steps************************************************
void resetPartition()
{
    for (int i = 0; i <= nodes; i++)
    {
        nodePartitionTrack[i] = 0;
    }
}

void savePartition()
{
    for (int i = 0; i <= nodes; i++)
    {
        bestNodePartition[i] = nodePartitionTrack[i];
    }
}

ll calculateRecentPartition()
{
    ll ans = 0;
    for (int i = 1; i <= nodes; i++)
    {
        if (nodePartitionTrack[i] == 1)
        {
            for (auto it : edges[i])
            {
                if (nodePartitionTrack[it.first] == 2)
                    ans += it.second;
            }
        }
    }
    return ans;
}

ll calculateBestPartition()
{
    ll ans = 0;
    for (int i = 1; i <= nodes; i++)
    {
        if (bestNodePartition[i] == 1)
        {
            for (auto it : edges[i])
            {
                if (bestNodePartition[it.first] == 2)
                    ans += it.second;
            }
        }
    }
    return ans;
}

// semi greedy partition*********************************************************************
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

Edge *semiGreedyPartitionStart(double alpha) // returns the first edge selected for the partitions.......
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
            if (it.second >= threshold && i < it.first)
            {
                Edge *edge = new Edge;
                edge->startNode = i;
                edge->endNode = it.first;
                edge->weight = it.second;
                SelectedEdge.push_back(edge);
            }
        }
    }
    // cout<<"done"<<endl;
    ll ran = randomInRange(0, SelectedEdge.size());
    // cout<<ran<<endl;
    return SelectedEdge[ran];
}

void createSemiGreedyPartition(double alpha)
{
    // entering the first pair of nodes.................
    Edge *edge = semiGreedyPartitionStart(alpha);
    nodePartitionTrack[edge->startNode] = 1;
    nodePartitionTrack[edge->endNode] = 2;

    // this loops will go on until all nodes are partitioned...............
    while (1)
    {
        // finding all non partitioned nodes...................
        vector<ll> nonCatagorizedNodes;
        for (ll i = 1; i <= nodes; i++)
        {
            if (nodePartitionTrack[i] == 0)
                nonCatagorizedNodes.push_back(i);
        }
        // if no node is remaining non partitioned, break this loop
        if (nonCatagorizedNodes.size() == 0)
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
        int selectedIndex = randomInRange(0, selectedNodes.size());
        if (sigmaValuePair[selectedIndex].first >= sigmaValuePair[selectedIndex].second)
            nodePartitionTrack[nonCatagorizedNodes[selectedIndex]] = 1;
        else
            nodePartitionTrack[nonCatagorizedNodes[selectedIndex]] = 2;
    }
}

void semi_greedy_partition_simulation()
{
    cout << "*SEMI GREEDY PARTITION IS DONE FOR THIS ITERATION*************************************" << endl;

    double alpha = randomAlphaGeneration();

    resetPartition();
    createSemiGreedyPartition(alpha);

    ll cur = calculateRecentPartition();
    if (cur > bestAns)
    {
        savePartition();
        bestAns = cur;
    }

    cout << "max cut: " << cur << "  alpha:  " << alpha << endl;

    cout << endl;
}

// greedy partition**************************************************************************************

Edge *GreedyPartitionStart() // returns the first edge selected for the partitions.......
{

    ll maximumWeight = -1e18, startNode = -1, endNode = -1, weight = -1e18;

    for (int i = 1; i <= nodes; i++)
    {
        for (auto it : edges[i])
        {
            if (it.second > maximumWeight)
            {
                startNode = i;
                endNode = it.first;
                weight = it.second;
                maximumWeight = it.second;
            }
        }
    }

    Edge *edge = new Edge;
    edge->startNode = startNode;
    edge->endNode = endNode;
    edge->weight = weight;

    return edge;
}

void createGreedyPartition()
{
    // entering the first pair of nodes.................
    Edge *edge = GreedyPartitionStart();
    nodePartitionTrack[edge->startNode] = 1;
    nodePartitionTrack[edge->endNode] = 2;

    // this loops will go on until all nodes are partitioned...............
    while (1)
    {
        // finding all non partitioned nodes...................
        vector<ll> nonCatagorizedNodes;
        for (ll i = 1; i <= nodes; i++)
        {
            if (nodePartitionTrack[i] == 0)
                nonCatagorizedNodes.push_back(i);
        }
        // if no node is remaining non partitioned, break this loop
        if (nonCatagorizedNodes.size() == 0)
            break;

        // find the best value a node could gain if added to any of the two partitions............

        ll maxSigmaValue = -1e18, nodeNo = -1, partitionNo = -1;
        for (auto it : nonCatagorizedNodes)
        {
            ll w1 = sigmaFunction(it, 1);
            ll w2 = sigmaFunction(it, 2);
            if (w1 > maxSigmaValue)
            {
                nodeNo = it;
                partitionNo = 1;
                maxSigmaValue = w1;
            }
            if (w2 > maxSigmaValue)
            {
                nodeNo = it;
                partitionNo = 2;
                maxSigmaValue = w2;
            }
        }

        nodePartitionTrack[nodeNo] = partitionNo;
    }
}

void greedy_partition_simulation()
{
    cout << "*GREEDY PARTITION IS DONE FOR THIS ITERATION*************************************" << endl;

    resetPartition();
    createGreedyPartition();
    ll cur = calculateRecentPartition();
    if (cur > bestAns)
    {
        savePartition();
        bestAns = cur;
    }
    cout << "max cut: " << cur << endl;

    cout << endl;
}

//random partition**************************************************************************************

void createRandomPartition()
{
    for(int i=1;i<=nodes;i++)
        nodePartitionTrack[i]=randomlyChooseZeroAndOne()+1;
}
void random_partition_simulation()
{
    cout << "*RANDOM PARTITION IS DONE FOR THIS ITERATION*************************************" << endl;

    resetPartition();
    createRandomPartition();
    ll cur = calculateRecentPartition();
    if (cur > bestAns)
    {
        savePartition();
        bestAns = cur;
    }
    cout << "max cut: " << cur << endl;

    cout << endl;
}

void greedyStage()
{
    int ran=randomInRange(0,10);
    if(ran<=5)
        semi_greedy_partition_simulation();
    else if(ran<=8)
        random_partition_simulation();
    else 
        greedy_partition_simulation();
}

pair<ll, ll> bestNodeToExchange() // return the best node to move and the gain from it.
{
    ll best = -1e18, node = -1;
    for (int i = 1; i <= nodes; i++)
    {
        if (nodePartitionTrack[i] == 1)
        {
            ll partition1_value = sigmaFunction(i, 1);
            ll partition2_value = sigmaFunction(i, 2);
            if (partition2_value - partition1_value > best)
            {
                best = partition2_value - partition1_value;
                node = i;
            }
        }
        if (nodePartitionTrack[i] == 2)
        {
            ll partition1_value = sigmaFunction(i, 1);
            ll partition2_value = sigmaFunction(i, 2);
            if (partition1_value - partition2_value > best)
            {
                best = partition1_value - partition2_value;
                node = i;
            }
        }
    }

    return {node, best};
}

void localSearchOnPartition()
{
    cout << "*LOCAL SEARCH IS STARTING**********************************" << endl;
    bool better = true;
    ll bestAnswer = bestAns;
    while (better)
    {
        better = false;
        pair<ll, ll> p = bestNodeToExchange();
        if (p.second > 0) // a gain!!!
        {
            bestAnswer += p.second;
            if (nodePartitionTrack[p.first] == 1)
            {
                nodePartitionTrack[p.first] = 2;
            }
            else
                nodePartitionTrack[p.first] = 1;

            better = true;
        }
    }
    bestAns = bestAnswer;

    cout << "max cut: " << bestAns << endl;
}

ll oneIteration()
{
    greedyStage();
    localSearchOnPartition();
    return bestAns;
}

int main()
{
    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);
    int m;
    cin >> nodes >> m;
    for (int i = 0; i < m; i++)
    {
        ll l, r, w;
        cin >> l >> r >> w;
        edges[l].push_back({r, w});
        edges[r].push_back({l, w});
    }
    ll ans = -1e18;
    for (int i = 1; i <= ITERATION_COUNT; i++)
    {
        cout << "****************************ITERATION " << i << "*********************************" << endl;
        bestAns = -1e18;
        ll p = oneIteration();
        if (p > ans)
        {
            ans = p;
        }
        cout << endl
             << endl
             << endl
             << endl
             << endl;
    }

    cout << "BEST ANS AFTER ALL: " << ans << endl;
}
