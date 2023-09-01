#include <bits/stdc++.h>
using namespace std;


struct DataPoint
{
    int buyingPriceClass;
    int maintPriceClass;
    int doorNoClass;
    int personNoClass;
    int bootSizeClass;
    int safetyClass;
    int resultClass;
};
struct Node
{
    bool isLeaf = false;
    int resultClass = -1; // result for leaf, plurality for internal nodes.....
    int parentPlurality = -1;
    vector<Node *> childs;
    vector<DataPoint *> datas;
    int choosenAttr=-1;
    double entropy;
};



vector<DataPoint *> dataSet;
vector<DataPoint*> testSet;
vector<DataPoint*> trainSet;

// creating data set...........................................................

int classifyPrice(string s)
{
    if (s == "vhigh")
        return 1;
    if (s == "high")
        return 2;
    if (s == "med")
        return 3;
    if (s == "low")
        return 4;
    return -1;
}

int classifyPersonNo(string s)
{
    if (s == "2")
        return 1;
    if (s == "4")
        return 2;
    if (s == "more")
        return 3;
    return -1;
}

int classifyDoorNo(string s)
{
    if (s == "2")
        return 1;
    if (s == "3")
        return 2;
    if (s == "4")
        return 3;
    if (s == "5more")
        return 4;
    return -1;
}

int classifyBootSize(string s)
{
    if (s == "big")
        return 1;
    if (s == "med")
        return 2;
    if (s == "small")
        return 3;
    return -1;
}

int classifySafety(string s)
{
    if (s == "high")
        return 1;
    if (s == "med")
        return 2;
    if (s == "low")
        return 3;
    return -1;
}

int classifyResult(string s)
{
    if (s == "unacc")
        return 1;
    if (s == "acc")
        return 2;
    if (s == "good")
        return 3;
    if (s == "vgood")
        return 4;
    return -1;
}

void makeDataSet()
{
    ifstream input("dataset/car.data");

    string line;
    while (getline(input, line))
    {
        istringstream s(line);
        string token;
        vector<string> tokens;

        char delimiter = ',';

        while (getline(s, token, delimiter))
        {
            tokens.push_back(token);
        }

        // for (auto token : tokens)
        // {
        //     cout << token << " ";
        // }
        // cout << endl;

        struct DataPoint *data = new DataPoint;

        data->buyingPriceClass = classifyPrice(tokens[0]);
        data->maintPriceClass = classifyPrice(tokens[1]);
        data->doorNoClass = classifyDoorNo(tokens[2]);
        data->personNoClass = classifyPersonNo(tokens[3]);
        data->bootSizeClass = classifyBootSize(tokens[4]);
        data->safetyClass = classifySafety(tokens[5]);
        data->resultClass = classifyResult(tokens[6]);

        if (data->buyingPriceClass == -1 || data->maintPriceClass == -1 ||
            data->doorNoClass == -1 || data->personNoClass == -1 ||
            data->safetyClass == -1 || data->bootSizeClass == -1 ||
            data->resultClass == -1)
            cout << "err in data" << endl;

        dataSet.push_back(data);
    }

    input.close();
}

void splitDataSet(double splitRatio)
{
    random_device rd;
    mt19937 g(rd());
    shuffle(dataSet.begin(), dataSet.end(), g);

    int split_index = static_cast<int>(dataSet.size() *splitRatio);

    
    for(int i=0;i<split_index;i++)
    {
        trainSet.push_back(dataSet[i]);
    }
    for(int i=split_index;i<dataSet.size();i++)
    {
        testSet.push_back(dataSet[i]);
    }
}


// calculation gain and choose attribute...................................
double reminderCalculation(vector<DataPoint *> currDatas, int attributeNo)
{
    double ans;
    if (attributeNo == 1) // buying price;
    {
        int a[4] = {0}, b[4] = {0}, c[4] = {0}, d[4] = {0}; // 4 classes in buying price;
        int n = currDatas.size();
        for (auto it : currDatas)
        {
            if (it->buyingPriceClass == 1)
            {
                a[it->resultClass - 1]++;
            }
            else if (it->buyingPriceClass == 2)
                b[it->resultClass - 1]++;
            else if (it->buyingPriceClass == 3)
                c[it->resultClass - 1]++;
            else
                d[it->resultClass - 1]++;
        }
        double fora = 0, forb = 0, forc = 0, ford = 0;
        for (int i = 0; i < 4; i++)
        {
            if(a[i]>0)
            fora += 1.0 * a[i] * log2(1.0 * (a[0] + a[1] + a[2] + a[3]) / a[i]);
            if(b[i]>0)
            forb += 1.0 * b[i] * log2(1.0 * (b[0] + b[1] + b[2] + b[3]) / b[i]);
            if(c[i]>0)
            forc += 1.0 * c[i] * log2(1.0 * (c[0] + c[1] + c[2] + c[3]) / c[i]);
            if(d[i]>0)
            ford += 1.0 * d[i] * log2(1.0 * (d[0] + d[1] + d[2] + d[3]) / d[i]);
        }
        ans = 1.0 * (fora + forb + forc + ford) / n;
        return ans;
    }

    else if (attributeNo == 2) // maint price;
    {
        int a[4] = {0}, b[4] = {0}, c[4] = {0}, d[4] = {0}; // 4 classes in maint price;
        int n = currDatas.size();
        for (auto it : currDatas)
        {
            if (it->maintPriceClass == 1)
            {
                a[it->resultClass - 1]++;
            }
            else if (it->maintPriceClass == 2)
                b[it->resultClass - 1]++;
            else if (it->maintPriceClass == 3)
                c[it->resultClass - 1]++;
            else
                d[it->resultClass - 1]++;
        }
        double fora = 0, forb = 0, forc = 0, ford = 0;
        for (int i = 0; i < 4; i++)
        {
            if(a[i]>0)
            fora += 1.0 * a[i] * log2(1.0 * (a[0] + a[1] + a[2] + a[3]) / a[i]);
            if(b[i]>0)
            forb += 1.0 * b[i] * log2(1.0 * (b[0] + b[1] + b[2] + b[3]) / b[i]);
            if(c[i]>0)
            forc += 1.0 * c[i] * log2(1.0 * (c[0] + c[1] + c[2] + c[3]) / c[i]);
            if(d[i]>0)
            ford += 1.0 * d[i] * log2(1.0 * (d[0] + d[1] + d[2] + d[3]) / d[i]);
        }
        ans = 1.0 * (fora + forb + forc + ford) / n;
        return ans;
    }

    else if (attributeNo == 3) // door no
    {
        int a[4] = {0}, b[4] = {0}, c[4] = {0}, d[4] = {0}; // 4 classes in door no
        int n = currDatas.size();
        for (auto it : currDatas)
        {
            if (it->doorNoClass == 1)
            {
                a[it->resultClass - 1]++;
            }
            else if (it->doorNoClass == 2)
                b[it->resultClass - 1]++;
            else if (it->doorNoClass == 3)
                c[it->resultClass - 1]++;
            else
                d[it->resultClass - 1]++;
        }
        double fora = 0, forb = 0, forc = 0, ford = 0;
        for (int i = 0; i < 4; i++)
        {
            if(a[i]>0)
            fora += 1.0 * a[i] * log2(1.0 * (a[0] + a[1] + a[2] + a[3]) / a[i]);
            if(b[i]>0)
            forb += 1.0 * b[i] * log2(1.0 * (b[0] + b[1] + b[2] + b[3]) / b[i]);
            if(c[i]>0)
            forc += 1.0 * c[i] * log2(1.0 * (c[0] + c[1] + c[2] + c[3]) / c[i]);
            if(d[i]>0)
            ford += 1.0 * d[i] * log2(1.0 * (d[0] + d[1] + d[2] + d[3]) / d[i]);
        }
        ans = 1.0 * (fora + forb + forc + ford) / n;
        //cout<<ans<<endl;
        return ans;
    }

    else if (attributeNo == 4) // person no
    {
        int a[4] = {0}, b[4] = {0}, c[4] = {0}; // 3 classes in person no
        int n = currDatas.size();
        for (auto it : currDatas)
        {
            if (it->personNoClass == 1)
            {
                a[it->resultClass - 1]++;
            }
            else if (it->personNoClass == 2)
                b[it->resultClass - 1]++;
            else if (it->personNoClass == 3)
                c[it->resultClass - 1]++;
        }
        double fora = 0, forb = 0, forc = 0;
        for (int i = 0; i < 4; i++)
        {
            if(a[i]>0)
            fora += 1.0 * a[i] * log2(1.0 * (a[0] + a[1] + a[2] + a[3]) / a[i]);
            if(b[i]>0)
            forb += 1.0 * b[i] * log2(1.0 * (b[0] + b[1] + b[2] + b[3]) / b[i]);
            if(c[i]>0)
            forc += 1.0 * c[i] * log2(1.0 * (c[0] + c[1] + c[2] + c[3]) / c[i]);
        }
        ans = 1.0 * (fora + forb + forc) / n;
        return ans;
    }

    else if (attributeNo == 5) // boot size
    {
        int a[4] = {0}, b[4] = {0}, c[4] = {0}; // 3 classes in boot size
        int n = currDatas.size();
        for (auto it : currDatas)
        {
            if (it->bootSizeClass == 1)
            {
                a[it->resultClass - 1]++;
            }
            else if (it->bootSizeClass == 2)
                b[it->resultClass - 1]++;
            else if (it->bootSizeClass == 3)
                c[it->resultClass - 1]++;
        }
        double fora = 0, forb = 0, forc = 0;
        for (int i = 0; i < 4; i++)
        {
            if(a[i]>0)
            fora += 1.0 * a[i] * log2(1.0 * (a[0] + a[1] + a[2] + a[3]) / a[i]);
            if(b[i]>0)
            forb += 1.0 * b[i] * log2(1.0 * (b[0] + b[1] + b[2] + b[3]) / b[i]);
            if(c[i]>0)
            forc += 1.0 * c[i] * log2(1.0 * (c[0] + c[1] + c[2] + c[3]) / c[i]);
        }
        ans = 1.0 * (fora + forb + forc) / n;
        return ans;
    }

    else if (attributeNo == 6) // safety
    {
        int a[4] = {0}, b[4] = {0}, c[4] = {0}; // 3 classes in safety
        int n = currDatas.size();
        for (auto it : currDatas)
        {
            if (it->safetyClass == 1)
            {
                a[it->resultClass - 1]++;
            }
            else if (it->safetyClass == 2)
                b[it->resultClass - 1]++;
            else if (it->safetyClass == 3)
                c[it->resultClass - 1]++;
        }
        double fora = 0, forb = 0, forc = 0;
        for (int i = 0; i < 4; i++)
        {
            if(a[i]>0)
            fora += 1.0 * a[i] * log2(1.0 * (a[0] + a[1] + a[2] + a[3]) / a[i]);
            if(b[i]>0)
            forb += 1.0 * b[i] * log2(1.0 * (b[0] + b[1] + b[2] + b[3]) / b[i]);
            if(c[i]>0)
            forc += 1.0 * c[i] * log2(1.0 * (c[0] + c[1] + c[2] + c[3]) / c[i]);
        }
        ans = 1.0 * (fora + forb + forc) / n;
        return ans;
    }

    return -1;
}

int chooseAttribute(Node* currNode,int* arr)
{
    //cout<<"hey\n"<<endl;
    double mxGain=-1e9,attributeNo=-1;
    for(int i=1;i<=6;i++)
    {
        if(arr[i-1]==0)continue;
        
        double reminder=reminderCalculation(currNode->datas,i);
        // if(currNode->entropy<reminder)
        //     cout<<"err"<<endl;
        //cout<<currNode->entropy<<" "<<reminder<<endl;
        if(currNode->entropy-reminder>mxGain)
        {
            mxGain=currNode->entropy-reminder;
            attributeNo=i;
        }

    }
    //cout<<attributeNo<<endl;
    return attributeNo;
    
}

//calculate plurality and entropy...........
void nodeCalculation(Node* currNode)
{
    int a[4] = {0};
    for (auto it : currNode->datas)
    {
        a[it->resultClass - 1]++;
    }
    double f = 0;
    int mx=-1e9,plurality=-1;
    for (int i = 0; i < 4; i++)
    {
        if(a[i]>=mx) // we are giving last result class more importance
        {
            mx=a[i];
            plurality=i+1;
        }
        if(a[i]>0)
        f += 1.0 * a[i] * log2(1.0 * (a[0] + a[1] + a[2] + a[3]) / a[i]) / (a[0] + a[1] + a[2] + a[3]);
    }
    currNode->entropy=f;
    currNode->resultClass=plurality;
    //cout<<currNode->entropy<<" "<<currNode->resultClass<<endl;
    
}

// child node calculation..............

void childNodeCreation(Node *parent, int attributeNo)
{

    if (attributeNo == 1) // buying price;
    {
        // create the childs and pass down the plurality...
        for (int i = 0; i < 4; i++)
        {
            Node *child = new Node;
            parent->childs.push_back(child);
            child->parentPlurality = parent->resultClass;
        }
        // pass data to childs...........
        for (auto it : parent->datas)
        {
            parent->childs[it->buyingPriceClass - 1]->datas.push_back(it);
        }
    }
    if (attributeNo == 2) // maint price;
    {
        // create the childs and pass down the plurality...
        for (int i = 0; i < 4; i++)
        {
            Node *child = new Node;
            parent->childs.push_back(child);
            child->parentPlurality = parent->resultClass;
        }
        // pass data to childs...........
        for (auto it : parent->datas)
        {
            parent->childs[it->maintPriceClass - 1]->datas.push_back(it);
        }
    }
    if (attributeNo == 3) // door no
    {
        // create the childs and pass down the plurality...
        for (int i = 0; i < 4; i++)
        {
            Node *child = new Node;
            parent->childs.push_back(child);
            child->parentPlurality = parent->resultClass;
        }
        // pass data to childs...........
        for (auto it : parent->datas)
        {
            parent->childs[it->doorNoClass - 1]->datas.push_back(it);
        }
    }
    if (attributeNo == 4) // persons
    {
        // create the childs and pass down the plurality...
        for (int i = 0; i < 3; i++)
        {
            Node *child = new Node;
            parent->childs.push_back(child);
            child->parentPlurality = parent->resultClass;
        }
        // pass data to childs...........
        for (auto it : parent->datas)
        {
            parent->childs[it->personNoClass - 1]->datas.push_back(it);
        }
    }
    if (attributeNo == 5) // boot size
    {
        // create the childs and pass down the plurality...
        for (int i = 0; i < 3; i++)
        {
            Node *child = new Node;
            parent->childs.push_back(child);
            child->parentPlurality = parent->resultClass;
        }
        // pass data to childs...........
        for (auto it : parent->datas)
        {
            parent->childs[it->bootSizeClass - 1]->datas.push_back(it);
        }
    }
    if (attributeNo == 6) // safety
    {
        // create the childs and pass down the plurality...
        for (int i = 0; i < 3; i++)
        {
            Node *child = new Node;
            parent->childs.push_back(child);
            child->parentPlurality = parent->resultClass;
        }
        // pass data to childs...........
        for (auto it : parent->datas)
        {
            parent->childs[it->safetyClass - 1]->datas.push_back(it);
        }
    }
}


void treeConstruction(Node* rootNode,int* arr)
{
    //check if there is no data...............
    if(rootNode->datas.size()==0)
    {
        rootNode->isLeaf=true;
        rootNode->resultClass=rootNode->parentPlurality;
        return;
    }
    //check if it is pure node............
    nodeCalculation(rootNode);
    if(abs(rootNode->entropy-0.0)<0.00001)
    {
        rootNode->isLeaf=true;
        return;
    }
    //check if there is no attribute..................
    bool noAttr=true;
    for(int i=0;i<6;i++)
    {
        if(arr[i]==1)
            noAttr=false;
    }
    if(noAttr)
    {
        rootNode->isLeaf=true;
        return;
    }


    int attribute=chooseAttribute(rootNode,arr);
    
    rootNode->choosenAttr=attribute;

    //remaining attributes.................
    int* newarr=new int[6];
    for(int i=0;i<6;i++)
    {
        //cout<<arr[i]<<" ";
        if(arr[i]==1 && i+1!=attribute)
            newarr[i]=1;
        else
            newarr[i]=0;
    }
    //cout<<endl;
    childNodeCreation(rootNode,attribute);
    //cout<<rootNode->childs.size()<<endl;
    for(auto it:rootNode->childs)
    {
        treeConstruction(it,newarr);
        //cout<<it->datas.size()<<endl;
    }
}

Node* modelTraining()
{
    Node* root=new Node;
    root->datas=trainSet;
    //cout<<trainSet.size()<<endl;
    int* arr=new int[6];
    for(int i=0;i<6;i++)
        arr[i]=1;
    treeConstruction(root,arr);
    return root;

}

void testing(Node* root)
{

    int right=0;
    Node* currNode=new Node;
    currNode=root;
    for(auto it:testSet)
    {
        while(1)
        {
            //cout<<currNode->choosenAttr<<" ";
            if(currNode->isLeaf==true)
            {
                if(currNode->resultClass==it->resultClass)
                    right++;
                break;
            }
            if(currNode->choosenAttr==1)
            {
                currNode=currNode->childs[it->buyingPriceClass-1];
            }
            if(currNode->choosenAttr==2)
            {
                currNode=currNode->childs[it->maintPriceClass-1];
            }
            if(currNode->choosenAttr==3)
            {
                currNode=currNode->childs[it->doorNoClass-1];
            }
            if(currNode->choosenAttr==4)
            {
                currNode=currNode->childs[it->personNoClass-1];
            }
            if(currNode->choosenAttr==5)
            {
                currNode=currNode->childs[it->bootSizeClass-1];
            }
            if(currNode->choosenAttr==6)
            {
                currNode=currNode->childs[it->safetyClass-1];
            }
        }
        //cout<<endl;
        currNode=root;


    }

    cout<<1.0*right/testSet.size()<<endl;

}





int main(int argc, char* argv[]) {
    if (argc != 2) {
        cerr << "Usage: " << argv[0] << " <number>" << endl;
        return 1;
    }

    // Convert the command line argument to an integer
    int number = atoi(argv[1]);
    double splitRatio=1.0*number/100;
    makeDataSet();
    splitDataSet(splitRatio);
    Node* root=modelTraining();
    testing(root);

}