#include <bits/stdc++.h>
using namespace std;
#define ll long long
#define NO_OF_NODES 5000
#define ITERATION_COUNT 1200

struct Edge
{
    ll startNode;
    ll endNode;
    ll weight;
};

struct Result
{
    //<grasp iterationNo,max-cut result>
    vector<pair<ll, ll>> randomizedPartitionResults;
    //<grasp iterationNo,<alpha value,max-cut result>>
    vector<pair<ll, pair<double, ll>>> semiGreedyPartitionResults;
    /// greedy max-cut result
    ll greedyPartitionResult;
    // <local search type,<iterationCount,max-cut result>>
    // O for randomized partitions, 1 for greedy partitions, 2 for semi-greedy partitions.....
    vector<pair<ll, pair<ll, ll>>> localSearchResults[3];
};

int nodePartitionTrack[NO_OF_NODES]; // 0 if not partitioned, 1 if partition 1, 2 if partition 2.......
int bestNodePartition[NO_OF_NODES];
ll bestAns = -1e18;
int nodes;
int edgeNo;
ll maxWeight = -1e18,ansItr=-1;
ll minWeight = 1e18;
vector<pair<ll, ll>> edges[NO_OF_NODES];

Result result;
int currentIteration;
int currentPartitionType;

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
    // cout << "*SEMI GREEDY PARTITION IS DONE FOR THIS ITERATION*************************************" << endl;

    double alpha = randomAlphaGeneration();

    resetPartition();
    createSemiGreedyPartition(alpha);

    ll cur = calculateRecentPartition();
    if (cur > bestAns)
    {
        savePartition();
        bestAns = cur;
    }
    result.semiGreedyPartitionResults.push_back({currentIteration, {alpha, cur}});

    // cout << "max cut: " << cur << "  alpha:  " << alpha << endl;

    // cout << endl;
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
    // cout << "*GREEDY PARTITION IS DONE FOR THIS ITERATION*************************************" << endl;

    resetPartition();
    createGreedyPartition();
    ll cur = calculateRecentPartition();
    if (cur > bestAns)
    {
        savePartition();
        bestAns = cur;
    }
    // cout << "max cut: " << cur << endl;
    result.greedyPartitionResult = max(result.greedyPartitionResult, cur);

    // cout << endl;
}

// random partition**************************************************************************************

void createRandomPartition()
{
    for (int i = 1; i <= nodes; i++)
        nodePartitionTrack[i] = randomlyChooseZeroAndOne() + 1;
}
void random_partition_simulation()
{
    // cout << "*RANDOM PARTITION IS DONE FOR THIS ITERATION*************************************" << endl;

    resetPartition();
    createRandomPartition();
    ll cur = calculateRecentPartition();
    if (cur > bestAns)
    {
        savePartition();
        bestAns = cur;
    }
    // cout << "max cut: " << cur << endl;
    result.randomizedPartitionResults.push_back({currentIteration, cur});

    // cout << endl;
}

void greedyStage()
{
    int ran = randomInRange(0, 20);
    if (ran <= 11)
    {
        currentPartitionType = 2;
        semi_greedy_partition_simulation();
    }
    else if (ran <= 17)
    {
        currentPartitionType = 0;
        random_partition_simulation();
    }
    else
    {
        currentPartitionType = 1;
        greedy_partition_simulation();
    }
}

// ###################################LOCAL SEARCH STAGE#####################################################

// way 1.......................................................
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

// way 2...............................................

pair<ll, ll> firstNodeToExchange() // return the best node to move and the gain from it.
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
                break;
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
                break;
            }
        }
    }

    return {node, best};
}

void localSearchOnPartition()
{
    // cout << "*LOCAL SEARCH IS STARTING**********************************" << endl;
    bool better = true;
    ll bestAnswer = bestAns;
    int total_iteration = 0;
    int c = randomlyChooseZeroAndOne() + 1;

    while (better)
    {
        better = false;
        total_iteration++;
        pair<ll, ll> p;
        if (c == 1)
            p = bestNodeToExchange();
        else
            p = firstNodeToExchange();

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
    result.localSearchResults[currentPartitionType].push_back({c, {total_iteration, bestAnswer}});

    // cout << "max cut: " << bestAns << endl;
}

void oneIteration()
{
    greedyStage();
    localSearchOnPartition();
    
}

void printResult()
{
    cout << "SEMI GREEDY PARTITION RESULTS------------------------------>"
         << endl;
    cout << setw(20) << left << "iteration no" << setw(20) << left << "alpha" << setw(20) << left << "max-cut" << endl;

    double averageAlpha = 0;
    double averageSemiGreedyMaxCut = 0;
    for (int i = 0; i < result.semiGreedyPartitionResults.size(); i++)
    {
        averageAlpha += result.semiGreedyPartitionResults[i].second.first;
        averageSemiGreedyMaxCut += result.semiGreedyPartitionResults[i].second.second;
        cout << setw(20) << left << result.semiGreedyPartitionResults[i].first
             << setw(20) << left << result.semiGreedyPartitionResults[i].second.first
             << setw(20) << left << result.semiGreedyPartitionResults[i].second.second << endl;
    }
    averageAlpha = 1.0 * averageAlpha / result.semiGreedyPartitionResults.size();
    averageSemiGreedyMaxCut = 1.0 * averageSemiGreedyMaxCut / result.semiGreedyPartitionResults.size();
    cout << endl
         << endl;

    cout << "RANDOM PARTITION RESULTS------------------------------>"
         << endl;
    cout << setw(20) << left << "iteration no" << setw(20) << left << "max-cut" << endl;
    double averageRandomMaxCut = 0;
    for (int i = 0; i < result.randomizedPartitionResults.size(); i++)
    {
        averageRandomMaxCut += result.randomizedPartitionResults[i].second;
        cout << setw(20) << left << result.randomizedPartitionResults[i].first
             << setw(20) << left << result.randomizedPartitionResults[i].second << endl;
    }
    averageRandomMaxCut = 1.0 * averageRandomMaxCut / result.randomizedPartitionResults.size();
    cout << endl
         << endl;

    cout << "LOCAL SEARCH RESULTS------------------------------>"
         << endl;
    cout << setw(20) << left << "local search type" << setw(20) << left << "iteration count" << setw(20) << left << "max-cut" << endl;
    int vp = 0, firstType = 0, secondType = 0,partitionType[3]={0};
    double FirstTypeItrCount[3] = {0}, firstTypeMaxCut[3] = {0};
    double SecondTypeItrCount[3] = {0}, secondTypeMaxCut[3] = {0};

    for (int t = 0; t < 3; t++)
    {
        for (int i = 0; i < result.localSearchResults[t].size(); i++)
        {
            vp++;
            if (result.localSearchResults[t][i].first == 1)
            {
                firstType++;
                FirstTypeItrCount[t] += result.localSearchResults[t][i].second.first;
                firstTypeMaxCut[t] += result.localSearchResults[t][i].second.second;
            }
            else
            {
                secondType++;
                SecondTypeItrCount[t] += result.localSearchResults[t][i].second.first;
                secondTypeMaxCut[t] += result.localSearchResults[t][i].second.second;
            }
            cout << setw(20) << left << result.localSearchResults[t][i].first
                 << setw(20) << left << result.localSearchResults[t][i].second.first
                 << setw(20) << left << result.localSearchResults[t][i].second.second << endl;
        }
    }

    double avgFirstTypeItr=0,avgSecondTypeItr=0,avgFirstTypeMaxCut=0,avgSecondTypeMaxCut=0;
    for(int i=0;i<3;i++)
    {
        avgFirstTypeItr+=FirstTypeItrCount[i];
        avgSecondTypeItr+=SecondTypeItrCount[i];
        avgFirstTypeMaxCut+=firstTypeMaxCut[i];
        avgSecondTypeMaxCut+=secondTypeMaxCut[i];
    }
    


    

    cout << endl
         << endl;

    FILE *file = freopen("outputs/partitionOutput.csv", "a", stdout);

    // PARTITION RESULTS.....................................

    cout << nodes << "," << edgeNo << "," << minWeight << "," << maxWeight << ",";
    // cout<<"Average alpha for semi greedy: ";
    cout << averageAlpha << ",";
    // cout<<"Average max cut for semi greedy: ";
    cout << averageSemiGreedyMaxCut << ",";
    // cout<<"Average max cut  for random: ";
    cout << averageRandomMaxCut << ",";
    // average maxcut for greedy
    cout << result.greedyPartitionResult;
    cout << endl;
    fclose(file);


    file = freopen("outputs/localSearchVsPartitionType.csv", "a", stdout);

    cout << nodes << "," << edgeNo << "," << minWeight << "," << maxWeight << ",";
    //for randomized
    cout<<1.0*(FirstTypeItrCount[0]+SecondTypeItrCount[0])/result.localSearchResults[0].size()<<","<<1.0*(firstTypeMaxCut[0]+secondTypeMaxCut[0])/result.localSearchResults[0].size()<<",";
    //for greedy
    cout<<1.0*(FirstTypeItrCount[1]+SecondTypeItrCount[1])/result.localSearchResults[1].size()<<","<<1.0*(firstTypeMaxCut[1]+secondTypeMaxCut[1])/result.localSearchResults[1].size()<<",";
    //for semi_greedy
    cout<<1.0*(FirstTypeItrCount[2]+SecondTypeItrCount[2])/result.localSearchResults[2].size()<<","<<1.0*(firstTypeMaxCut[2]+secondTypeMaxCut[2])/result.localSearchResults[2].size();
    cout<<endl;
    fclose(file);



    file = freopen("outputs/localSearchComp.csv", "a", stdout);

    ///LOCAL SEARCH COMPARISON...............
    cout << nodes << "," << edgeNo << "," << minWeight << "," << maxWeight << ",";
    // cout<<"Average iteration count for best chose local search: ";
    cout << 1.0*avgFirstTypeItr/firstType<< ",";
    // cout<<"Average max cut for best chose local search: ";
    cout << 1.0*avgFirstTypeMaxCut/firstType << ",";
    // cout<<"Average iteration count for first chose local search: ";
    cout << 1.0*avgSecondTypeItr/secondType<< ",";
    // cout<<"Average max cut for first chose local search: ";
    cout << 1.0*avgSecondTypeMaxCut/secondType<< ",";
    // GRASP RESULTS....................
    cout <<ITERATION_COUNT<<","<< bestAns<<","<<ansItr<<","<<currentPartitionType;
    cout << endl;

    fclose(file);
}

int main(int argc, char *argv[])
{
    freopen(argv[1], "r", stdin);

    cin >> nodes >> edgeNo;
    for (int i = 0; i < edgeNo; i++)
    {
        ll l, r, w;
        cin >> l >> r >> w;
        minWeight = min(minWeight, w);
        maxWeight = max(maxWeight, w);
        edges[l].push_back({r, w});
        edges[r].push_back({l, w});
    }
    ll ans = -1e18;
    for (int i = 1; i <= ITERATION_COUNT; i++)
    {
        currentIteration++;
        // cout << "****************************ITERATION " << i << "*********************************" << endl;
        bestAns = -1e18;
        oneIteration();
        if (bestAns > ans)
        {
            ans = bestAns;
            ansItr=i;
        }
        // cout << endl
        //      << endl
        //      << endl
        //      << endl
        //      << endl;
    }
    bestAns = ans;
    printResult();
    cout << "BEST ANS AFTER ALL: " << ans << endl;
}
