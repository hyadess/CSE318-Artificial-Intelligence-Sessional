#include<bits/stdc++.h>
using namespace std;

int explored=0;
int expanded=0;

class board
{
    int n;
    vector<vector<int>>v;
    board* parent;


public:
    board(int n)
    {
        this->n=n;
        parent=NULL;
    }
    board(vector<vector<int>> v, int n)
    {
        this->n=n;
        this->v=v;
        parent=NULL;
    }

    void setParent(board* parent)
    {
        this->parent=parent;

    }
    board* getParent()
    {
        return parent;
    }


    int getDimension()
    {
        return n;
    }
    vector<vector<int>> getArray()
    {
        return v;
    }



    void printBoard()
    {
        for(int i=0; i<n; i++)
        {
            for(int j=0; j<n; j++)
            {
                cout<<v[i][j]<<" ";
            }
            cout<<endl;
        }
        cout<<endl;
    }











};



class n_puzzle
{
    board* initial_board;
    int n;
    vector<board*>path;
    set<vector<vector<int>>> visited;


    bool check_visited(board* current)
    {
        vector<vector<int>> v=current->getArray();
        bool ans=true;

        if(visited.count(v)==0)
            ans=false;
        return ans;
    }

    void make_visited(board* current)
    {
        vector<vector<int>> v=current->getArray();

        visited.insert(v);

    }


    /// for solvability test................


    vector<int> convert_to_1d_array(board* currentBoard)
    {

        vector<vector<int>> arr=currentBoard->getArray();
        int n=currentBoard->getDimension();


        vector<int>v;
        for(int i=0; i<n; i++)
        {
            for(int j=0; j<n; j++)
            {
                if(arr[i][j]!=0)
                    v.push_back(arr[i][j]);
            }
        }
        return v;
    }

    int inversion_count()
    {
        vector<int>arr=convert_to_1d_array(initial_board);
        int n=arr.size();
        int ans=0;
        for(int i=0; i<n; i++)
        {
            for(int j=i+1; j<n; j++)
            {
                if(arr[j]<arr[i])
                    ans++;
            }
        }
        return ans;
    }

    int height_of_zero(board* currentBoard)
    {
        vector<vector<int>> arr=currentBoard->getArray();
        int n=currentBoard->getDimension();

        int f;
        for(int i=0; i<n; i++)
        {
            for(int j=0; j<n; j++)
            {
                if(arr[i][j]==0)
                    f=i;
            }
        }
        return n-1-f;
    }



    /// heuristic...................

    int manhattan_distance(board* currentBoard)
    {
        int ans=0;
        vector<vector<int>> arr=currentBoard->getArray();
        int n=currentBoard->getDimension();



        for(int i=0; i<n; i++)
        {
            for(int j=0; j<n; j++)
            {
                if(arr[i][j]==0)
                    continue;
                int ideal_row=(arr[i][j]-1)/n;
                int ideal_column=(arr[i][j]-1)%n;
                ans+=abs(ideal_row-i)+abs(ideal_column-j);

            }

        }
        return ans;

    }


    int hamming_distance(board* currentBoard)
    {
        int ans=0;
        vector<vector<int>> arr=currentBoard->getArray();
        int n=currentBoard->getDimension();



        for(int i=0; i<n; i++)
        {
            for(int j=0; j<n; j++)
            {
                if(arr[i][j]==0)
                    continue;
                int ideal_row=(arr[i][j]-1)/n;
                int ideal_column=(arr[i][j]-1)%n;
                if(arr[ideal_row][ideal_column]!=arr[i][j])
                    ans++;

            }

        }
        return ans;
    }


    /// helper functions to solve puzzle..........

    vector<board*> nextBoards(board* currentBoard)
    {
        vector<board*> b;

        vector<vector<int>> arr=currentBoard->getArray();
        int n=currentBoard->getDimension();


        for(int i=0; i<n; i++)
        {
            for(int j=0; j<n; j++)
            {
                if(arr[i][j]==0)
                {
                    if(i>0)
                    {
                        vector<vector<int>> newarr1( n , vector<int> (n));

                        for(int ii=0; ii<n; ii++)
                        {

                            for(int jj=0; jj<n; jj++)
                            {
                                newarr1[ii][jj]=arr[ii][jj];
                            }
                        }
                        newarr1[i][j]=arr[i-1][j];
                        newarr1[i-1][j]=0;



                        board* b1=new board(newarr1,n);
                        b.push_back(b1);
                    }
                    if(j>0)
                    {
                        vector<vector<int>> newarr2( n , vector<int> (n));

                        for(int ii=0; ii<n; ii++)
                        {

                            for(int jj=0; jj<n; jj++)
                            {

                                newarr2[ii][jj]=arr[ii][jj];
                            }
                        }
                        newarr2[i][j]=arr[i][j-1];
                        newarr2[i][j-1]=0;
                        board* b2=new board(newarr2,n);
                        b.push_back(b2);
                    }
                    if(i<n-1)
                    {
                       vector<vector<int>> newarr3( n , vector<int> (n));

                        for(int ii=0; ii<n; ii++)
                        {

                            for(int jj=0; jj<n; jj++)
                            {
                                newarr3[ii][jj]=arr[ii][jj];
                            }
                        }
                        newarr3[i][j]=arr[i+1][j];
                        newarr3[i+1][j]=0;
                        board* b3=new board(newarr3,n);
                        b.push_back(b3);
                    }
                    if(j<n-1)
                    {
                        vector<vector<int>> newarr4( n , vector<int> (n));

                        for(int ii=0; ii<n; ii++)
                        {

                            for(int jj=0; jj<n; jj++)
                            {
                                newarr4[ii][jj]=arr[ii][jj];
                            }
                        }
                        newarr4[i][j]=arr[i][j+1];
                        newarr4[i][j+1]=0;
                        board* b4=new board(newarr4,n);
                        b.push_back(b4);
                    }



                }
            }
        }

        return b;
    }





public:


    n_puzzle(int n)
    {
        this->initial_board=NULL;
        this->n=n;

    }

    void initial_board_input()
    {
        vector<vector<int>> arr( n , vector<int> (n));


        for(int i=0; i<n; i++)
        {

            for(int j=0; j<n; j++)
            {
                cin>>arr[i][j];
            }
        }
        initial_board=new board(arr,n);

    }


    bool check_solvability()
    {
        if(initial_board->getDimension()%2==1)
        {
            if(inversion_count()%2==1)
                return false;
            else
                return true;
        }
        else
        {
            if(inversion_count()%2==1)
            {
                if(height_of_zero(initial_board)%2==1)
                    return true;
                else
                    return false;
            }
            else
            {
                if(height_of_zero(initial_board)%2==0)
                    return true;
                else
                    return false;
            }
        }

    }



    int find_distance(board* currentBoard)
    {
        return manhattan_distance(currentBoard);
        //return hamming_distance(currentBoard);
    }



    int solve_puzzle()
    {
        priority_queue<pair<int,board*>,vector<pair<int,board*>>,greater<pair<int,board*>>> pq;
        int distance=find_distance(initial_board)+0;
        pq.push({distance,initial_board});

        board* final_board;
        int ans=-1;
        int cur=0;
        while(!pq.empty())
        {
            pair<int,board*>p=pq.top();
            pq.pop();
            expanded++;
            make_visited(p.second);

            cur=p.first-find_distance(p.second);
            if(find_distance(p.second)==0)
            {
                final_board=p.second;
                ans=cur;
                break;

            }



            vector<board*> v=nextBoards(p.second);


            //cout<<cur<<endl;
            for(auto it: v)
            {

                int dis=cur+1+find_distance(it);
                if(check_visited(it)==false)
                {
                    explored++;
                    pq.push({dis,it});
                    it->setParent(p.second);

                }





            }


        }
        find_path(final_board);
        return ans;

    }


    void find_path(board* final_board)
    {
        while(final_board->getParent()!=NULL)
        {
            path.push_back(final_board);
            final_board=final_board->getParent();
        }

    }

    void print_path()
    {
        for(int i=path.size()-1; i>=0; i--)
        {
            path[i]->printBoard();
        }
    }




};


int main()
{
    int n;
    cin>>n;
    n_puzzle* puzzle=new n_puzzle(n);
    puzzle->initial_board_input();
    if(puzzle->check_solvability()!=true)
    {
        cout<<"NOT SOLVABLE"<<endl;
        return 0;
    }
    cout<<"minimum moves: "<<puzzle->solve_puzzle()<<endl;
    cout<<"explored node: "<<explored<<endl;
    cout<<"expanded node: "<<expanded<<endl;
    cout<<endl;
    puzzle->print_path();

    return 0;



}
