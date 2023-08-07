#include <bits/stdc++.h>
using namespace std;



class Board
{
    int enemyBins[7] = {0};
    int ownBins[7] = {0};
    int ownStorage;
    int enemyStorage;
    int heuristic = 0;
    bool parentSame = false;
    int movePlace = -1;
    int extraMoveCount=0;

    void printNum(int num)
    {
        if (num < 10)
            cout << "0";
        cout << num;
    }

public:
    Board(int *ownBins, int *enemyBins, int ownStorage, int enemyStorage)
    {
        for (int i = 1; i <= 6; i++)
        {
            this->enemyBins[i] = enemyBins[i];
            this->ownBins[i] = ownBins[i];
        }
        this->ownStorage = ownStorage;
        this->enemyStorage = enemyStorage;
    }

    Board(Board *prevBoard)
    {
        for (int i = 1; i <= 6; i++)
        {
            this->enemyBins[i] = prevBoard->stoneInEnemyBin(i);
            this->ownBins[i] = prevBoard->stoneInOwnBin(i);
        }
        this->ownStorage = prevBoard->stoneInOwnStorage();
        this->enemyStorage = prevBoard->stoneInEnemyStorage();
        this->extraMoveCount=prevBoard->getExtraMoveCount();
    }

    /// get functions.........................

    int stoneInEnemyBin(int bin)
    {
        return enemyBins[bin];
    }
    int stoneInOwnBin(int bin)
    {
        return ownBins[bin];
    }

    int stoneInEnemyStorage()
    {
        return enemyStorage;
    }
    int stoneInOwnStorage()
    {
        return ownStorage;
    }

    int getHeuristic()
    {
        return heuristic;
    }
    bool ifParentSame()
    {
        return parentSame;
    }
    int getMovePlace()
    {
        return movePlace;
    }
    int getExtraMoveCount()
    {
        return extraMoveCount;
    }

    /// set functions........................

    void updateStoneInEnemyBin(int bin, int newStone)
    {
        enemyBins[bin] = newStone;
    }
    void updateStoneInOwnBin(int bin, int newStone)
    {
        ownBins[bin] = newStone;
    }

    void updateEnemyStorage(int newStone)
    {
        this->enemyStorage = newStone;
    }
    void updateOwnStorage(int newStone)
    {
        this->ownStorage = newStone;
    }

    void setHeuristic(int heuristic)
    {
        this->heuristic = heuristic;
    }
    void setparentSame(bool parentSame)
    {
        this->parentSame = parentSame;
    }
    void setMovePlace(int place)
    {
        this->movePlace = place;
    }
    void setExtraMoveCount(int e)
    {
        this->extraMoveCount=e;
    }

    /// part of heuristic.................
    int boardSituation(int weight1, int weight2,int htype)
    {
        int StoneOnEnemyBins = 0, StoneOnMybins = 0;
        int suitableBin=0;

        for (int i = 1; i <= 6; i++)
        {
            StoneOnEnemyBins += enemyBins[i];
            StoneOnMybins += ownBins[i];
        }
        for(int i=1; i<=6; i++)
        {
            int y=ownBins[i]%14;
            if(7-i==y)
                suitableBin++;
        }
        if(htype==1)
            return weight1 * (ownStorage - enemyStorage);
        else if(htype==2)
            return weight1 * (ownStorage - enemyStorage) + weight2 * (StoneOnMybins - StoneOnEnemyBins);
        else if(htype==5)
            return weight1 * (ownStorage - enemyStorage) + weight2 * (StoneOnMybins - StoneOnEnemyBins)+suitableBin;
        else
            return weight1 * (ownStorage - enemyStorage) + weight2 * (StoneOnMybins - StoneOnEnemyBins);
    }

    void printboard()
    {
        cout << "-------------------------" << endl;
        cout << "|";
        printNum(enemyStorage);
        cout << "|";
        for (int i = 6; i >=1; i--)
        {
            printNum(enemyBins[i]);
            cout << "|";
        }
        printNum(ownStorage);
        cout << "|" << endl;
        cout << "-  -------------------  -" << endl;
        cout << "|  |";
        for (int i = 1; i <= 6; i++)
        {
            printNum(ownBins[i]);
            cout << "|";
        }
        cout << "  |" << endl;
        cout << "-------------------------" << endl;
        cout << endl;
    }


    bool result()
    {
        int own=0,enemy=0;
        for(int i=1; i<=6; i++)
        {
            own+=ownBins[i];
            enemy+=enemyBins[i];

        }
        if(own==0 || enemy==0)
        {
            own+=ownStorage;
            enemy+=enemyStorage;
            this->printboard();
            cout<<"YOU: "<<own<<endl;
            cout<<"AI: "<<enemy<<endl;
            if(own>enemy)
                cout<<"YOU WIN!!!"<<endl;
            else
                cout<<"ENEMY WINS!!!"<<endl;
            return true;
        }
        return false;

    }
    bool AI_vs_AI_result()
    {
        int own=0,enemy=0;
        for(int i=1; i<=6; i++)
        {
            own+=ownBins[i];
            enemy+=enemyBins[i];

        }
        if(own==0 || enemy==0)
        {
            own+=ownStorage;
            enemy+=enemyStorage;
            this->printboard();
            cout<<"AI-1: "<<own<<endl;
            cout<<"AI-2: "<<enemy<<endl;
            if(own>enemy)
                cout<<"AI-1 WINS!!!"<<endl;
            else
                cout<<"AI-2 WINS!!!"<<endl;
            return true;
        }
        return false;

    }

    bool ifEmptySide(bool ifMax)
    {
        if(ifMax)
        {
            int u=0;
            for(int i=1; i<=6; i++)
            {
                u+=ownBins[i];
            }
            if(u==0)
                return true;
            else
                return false;
        }
        else
        {
            int u=0;
            for(int i=1; i<=6; i++)
            {
                u+=enemyBins[i];
            }
            if(u==0)
                return true;
            else
                return false;
        }

    }



};

class Move_and_heuristic
{

    int storageSituationWeight;
    int binSituationWeight;
    int extraMoveWeight;
    int capturedWeight;

    void printBoard(Board *printBoard)
    {
        cout << "--------------------heuristic: " << printBoard->getHeuristic() << "----------------------" << endl;
        cout << "Total Stones on enemy Mancala: " << printBoard->stoneInEnemyStorage() << endl;
        cout << "enemy bins: ";
        for (int i = 6; i >= 1; i--)
        {
            cout << printBoard->stoneInEnemyBin(i) << " ";
        }
        cout << endl;
        cout << "My bins: ";
        for (int i = 1; i <= 6; i++)
        {
            cout << printBoard->stoneInOwnBin(i) << " ";
        }
        cout << endl;
        cout << "Total Stones on My Mancala: " << printBoard->stoneInOwnStorage() << endl;
        cout << endl;
    }

    void calculateHeuristic(Board *newBoard, bool myTurn, int captured,int htype)
    {
        int heuristic;
        int p;
        if (myTurn)
            p = 1;
        else
            p = -1;


        if (newBoard == NULL)
            cout << 1 << endl;
        if(htype==1 || htype==2)
            heuristic = newBoard->boardSituation(storageSituationWeight, binSituationWeight,htype);
        else if(htype==3)
            heuristic = newBoard->boardSituation(storageSituationWeight, binSituationWeight,htype) + p * extraMoveWeight*newBoard->getExtraMoveCount();
        else if(htype==4 | htype==5)
            heuristic = newBoard->boardSituation(storageSituationWeight, binSituationWeight,htype) + p * extraMoveWeight*newBoard->getExtraMoveCount() + p * captured * capturedWeight;



        newBoard->setHeuristic(heuristic);

        // printBoard(newBoard);
    }

public:
    Move_and_heuristic(int storageSituationWeight, int binSituationWeight, int extraMoveWeight, int capturedWeight)
    {
        this->storageSituationWeight = storageSituationWeight;
        this->binSituationWeight = binSituationWeight;
        this->extraMoveWeight = extraMoveWeight;
        this->capturedWeight = capturedWeight;
    }

    vector<Board *> *calculation(Board *currBoard, bool myTurn, bool needHeuristic,int htype)
    {

        vector<Board *> *nextBoards = new vector<Board *>();

        for (int i = 1; i <= 6; i++)
        {
            int captured = 0;


            int u;
            if (myTurn)
                u = currBoard->stoneInOwnBin(i);
            else
                u = currBoard->stoneInEnemyBin(i);
            if (u == 0) /// current bin is empty, no stone to move
                continue;

            bool isMySide = myTurn;
            int currPoint = i;

            Board *newBoard = new Board(currBoard);

            if (myTurn)
                newBoard->updateStoneInOwnBin(i, 0); /// all stone from current bin is lifted
            else
                newBoard->updateStoneInEnemyBin(i, 0);


            while (u > 0)
            {
                currPoint++;
                if (currPoint == 7) /// fill the storage and change the side .................
                {
                    if (isMySide)
                    {
                        newBoard->updateOwnStorage(newBoard->stoneInOwnStorage() + 1);
                        isMySide = false;
                        currPoint = 0;
                    }
                    else
                    {
                        newBoard->updateEnemyStorage(newBoard->stoneInEnemyStorage() + 1);
                        isMySide = true;
                        currPoint = 0;
                    }
                }
                else /// fill the current bin
                {
                    if (isMySide)
                    {
                        newBoard->updateStoneInOwnBin(currPoint, newBoard->stoneInOwnBin(currPoint) + 1);
                    }
                    else
                    {
                        newBoard->updateStoneInEnemyBin(currPoint, newBoard->stoneInEnemyBin(currPoint) + 1);
                    }
                }

                u--;

                /// check the last fillup..............
                if (u == 0)
                {
                    if (myTurn != isMySide && currPoint == 0) // recently changed side, that means an extra move
                    {
                        newBoard->setExtraMoveCount(currBoard->getExtraMoveCount()+1);
                        newBoard->setparentSame(true);
                    }

                    /// look for recent fillup.................

                    if (currPoint != 0 && isMySide == myTurn) /// last move was not in mancala and in the player's bin...
                    {
                        if (myTurn == true) /// is my turn
                        {
                            // current bin was previously empty but the opposite bin is nonempty...
                            if (newBoard->stoneInOwnBin(currPoint) == 1 && newBoard->stoneInEnemyBin(7-currPoint) > 0)
                            {

                                captured = 1 + newBoard->stoneInEnemyBin(7-currPoint);
                                newBoard->updateStoneInOwnBin(currPoint, 0);
                                newBoard->updateStoneInEnemyBin(7-currPoint, 0);
                                newBoard->updateOwnStorage(newBoard->stoneInOwnStorage() + captured);
                            }
                        }
                        else if (myTurn == false) /// is enemy's turn
                        {
                            // current bin was previously empty but the opposite bin is nonempty...
                            if (newBoard->stoneInEnemyBin(currPoint) == 1 && newBoard->stoneInOwnBin(7-currPoint) > 0)
                            {
                                captured = 1 + newBoard->stoneInOwnBin(7-currPoint);
                                newBoard->updateStoneInOwnBin(7-currPoint, 0);
                                newBoard->updateStoneInEnemyBin(currPoint, 0);
                                newBoard->updateEnemyStorage(newBoard->stoneInEnemyStorage() + captured);
                            }
                        }
                    }
                }
            }

            newBoard->setMovePlace(i);
            if (needHeuristic)
                calculateHeuristic(newBoard, myTurn,captured,htype);

            nextBoards->push_back(newBoard);
        }
        return nextBoards;
    }
};

class Adversial_search
{

    int recentMove; /// stores recently which bin is played by AI...........
    Move_and_heuristic *move;

    int selectBestMove(Board *currBoard, int depth, int alpha, int beta, bool ifMax, bool parentSame,int htype)
    {
        if(currBoard->ifEmptySide(ifMax))
        {
            return currBoard->boardSituation(2,1,htype);

        }
        if (depth == 1)
        {
            vector<Board *> *nextBoards = move->calculation(currBoard, ifMax, true,htype);

            int u;
            if (ifMax)
                u = -1000000000;
            else
                u = 1000000000;

            for (auto it : *nextBoards)
            {
                if (ifMax == true)
                    u = max(u, it->getHeuristic());
                else
                    u = min(u, it->getHeuristic());
            }
            for(auto it: *nextBoards)
                delete it;
            return u;
        }
        else
        {
            vector<Board *> *nextBoards = move->calculation(currBoard, ifMax, false,htype); /// find the children, but don't calculate their heuristic

            if (ifMax) /// current node is max
            {
                int u = -1000000000;
                for (auto it : *nextBoards)
                {
                    bool samechild = it->ifParentSame(); /// if extra move is gained, then the child will also be max

                    bool nextMax;
                    if (samechild)
                        nextMax = ifMax;
                    else
                        nextMax = !ifMax;

                    int r = selectBestMove(it, depth - 1, alpha, beta, nextMax, samechild,htype);
                    alpha = max(alpha, r);
                    u = max(u, r);
                    if (!parentSame && alpha >= beta) /// if prev node is also max, then we can't prune.........
                        break;
                }
                for(auto it: *nextBoards)
                    delete it;
                return u;
            }
            else
            {
                int u = 1000000000;
                for (auto it : *nextBoards)
                {
                    bool samechild = it->ifParentSame();
                    bool nextMax;
                    if (samechild)
                        nextMax = ifMax;
                    else
                        nextMax = !ifMax;
                    int r = selectBestMove(it, depth - 1, alpha, beta, nextMax, samechild,htype);
                    beta = min(beta, r);
                    u = min(u, r);
                    if (!parentSame && alpha >= beta)
                        break;
                }
                 for(auto it: *nextBoards)
                    delete it;
                return u;
            }
        }
    }

public:
    Adversial_search()
    {
        recentMove = -1;
        move = new Move_and_heuristic(2, 1, 4, 3);
    }
    int returnMove(Board *currBoard, int depth,int htype)
    {
        vector<Board *> *nextBoards = move->calculation(currBoard, true, false,htype); /// find the children, but don't calculate their heuristic

        int u = -1000000000;
        int ans;
        for (auto it : *nextBoards)
        {

            bool samechild = it->ifParentSame(); /// if extra move is gained, then the child will also be max

            bool nextMax;
            if (samechild)
                nextMax = true;
            else
                nextMax = false;

            int r = selectBestMove(it, depth, -1000000000, 1000000000, nextMax, samechild,htype);
            if (r > u)
            {
                u = r;
                ans = it->getMovePlace();
            }
        }
        for(auto it: *nextBoards)
            delete it;
        return ans;
    }
    Board *rotateBoard(Board *input) /// now AI is playing, so we need to rotate the board
    {
        int mySide[7], enemySide[7];
        for (int i = 1; i <= 6; i++)
        {
            mySide[i] = input->stoneInEnemyBin(i);
            enemySide[i] = input->stoneInOwnBin(i);
        }
        Board *rotatedBoard = new Board(mySide, enemySide, input->stoneInEnemyStorage(), input->stoneInOwnStorage());
        delete input;
        return rotatedBoard;
    }
};

class Game_simulator
{
    Adversial_search *search;
    int depth;

public:
    Game_simulator(Adversial_search *search, int depth)
    {
        this->search = search;
        this->depth = depth;
    }

    void play_game(Board *cur_board, int player,int htype)
    {
        if(cur_board->result())
            return;
        if (player == 1) // YOU...........
        {

            cout << "CURRENT BOARD:" << endl;
            cur_board->printboard();
            cout << "SELECT A BIN TO PLAY............." << endl;

            int o;

            while (1)
            {
                cin >> o;
                if (cur_board->stoneInOwnBin(o) == 0)
                    cout << "NO STONE IN THAT BIN. SELECT A PROPER ONE" << endl;
                else
                    break;
            }

            int captured = 0;
            bool extraMove = false;

            int u = cur_board->stoneInOwnBin(o);

            Board *newBoard = new Board(cur_board);

            int currPoint = o;
            bool isMySide = true;
            newBoard->updateStoneInOwnBin(o, 0); /// all stone from current bin is lifted

            while (u > 0)
            {
                currPoint++;
                if (currPoint == 7) /// fill the storage and change the side .................
                {
                    if (isMySide)
                    {
                        newBoard->updateOwnStorage(newBoard->stoneInOwnStorage() + 1);
                        isMySide = false;
                        currPoint = 0;
                    }
                    else
                    {
                        newBoard->updateEnemyStorage(newBoard->stoneInEnemyStorage() + 1);
                        isMySide = true;
                        currPoint = 0;
                    }
                }
                else /// fill the current bin
                {
                    if (isMySide)
                    {
                        newBoard->updateStoneInOwnBin(currPoint, newBoard->stoneInOwnBin(currPoint) + 1);
                    }
                    else
                    {
                        newBoard->updateStoneInEnemyBin(currPoint, newBoard->stoneInEnemyBin(currPoint) + 1);
                    }
                }

                u--;

                /// check the last fillup..............
                if (u == 0)
                {
                    /// look for recent fillup.................

                    if (currPoint != 0 && isMySide == true) /// last move was not in mancala and in the player's bin...
                    {

                        // current bin was previously empty but the opposite bin is nonempty...
                        if (newBoard->stoneInOwnBin(currPoint) == 1 && newBoard->stoneInEnemyBin(7-currPoint) > 0)
                        {
                            captured = 1 + newBoard->stoneInEnemyBin(7-currPoint);
                            newBoard->updateStoneInOwnBin(currPoint, 0);
                            newBoard->updateStoneInEnemyBin(7-currPoint, 0);
                            newBoard->updateOwnStorage(newBoard->stoneInOwnStorage() + captured);
                            cout << "YOU CAPTURED " << captured << " STONES FROM ENEMY SIDE!!!!" << endl;
                        }
                    }
                    if (isMySide == false && currPoint == 0) // recently changed side, that means an extra move
                    {
                        extraMove = true;
                        cout << "YOU GOT AN EXTRA MOVE!!!!" << endl;
                    }
                }
            }

            delete cur_board;
            if (extraMove)
            {
                play_game(newBoard, 1,htype);
            }
            else
            {
                play_game(newBoard, 2,htype);
            }
        }

        else if (player == 2) // AI...........
        {

            cout << "CURRENT BOARD:" << endl;
            cur_board->printboard();
            cout << "AI IS PLAYING............." << endl;

            cur_board = search->rotateBoard(cur_board);
            int o = search->returnMove(cur_board, depth,htype);

            int captured = 0;
            bool extraMove = false;

            int u = cur_board->stoneInOwnBin(o);

            Board *newBoard = new Board(cur_board);

            int currPoint = o;
            bool isMySide = true;
            newBoard->updateStoneInOwnBin(o, 0); /// all stone from current bin is lifted

            while (u > 0)
            {
                currPoint++;
                if (currPoint == 7) /// fill the storage and change the side .................
                {
                    if (isMySide)
                    {
                        newBoard->updateOwnStorage(newBoard->stoneInOwnStorage() + 1);
                        isMySide = false;
                        currPoint = 0;
                    }
                    else
                    {
                        newBoard->updateEnemyStorage(newBoard->stoneInEnemyStorage() + 1);
                        isMySide = true;
                        currPoint = 0;
                    }
                }
                else /// fill the current bin
                {
                    if (isMySide)
                    {
                        newBoard->updateStoneInOwnBin(currPoint, newBoard->stoneInOwnBin(currPoint) + 1);
                    }
                    else
                    {
                        newBoard->updateStoneInEnemyBin(currPoint, newBoard->stoneInEnemyBin(currPoint) + 1);
                    }
                }

                u--;

                /// check the last fillup..............
                if (u == 0)
                {
                    /// look for recent fillup.................

                    if (currPoint != 0 && isMySide == true) /// last move was not in mancala and in the player's bin...
                    {

                        // current bin was previously empty but the opposite bin is nonempty...
                        if (newBoard->stoneInOwnBin(currPoint) == 1 && newBoard->stoneInEnemyBin(7-currPoint) > 0)
                        {
                            captured = 1 + newBoard->stoneInEnemyBin(7-currPoint);
                            newBoard->updateStoneInOwnBin(currPoint, 0);
                            newBoard->updateStoneInEnemyBin(7-currPoint, 0);
                            newBoard->updateOwnStorage(newBoard->stoneInOwnStorage() + captured);
                            cout << "ENEMY CAPTURED " << captured << " STONES FROM YOUR SIDE!!!!" << endl;
                        }
                    }
                    if (isMySide == false && currPoint == 0) // recently changed side, that means an extra move
                    {
                        extraMove = true;
                        cout << "ENEMY GOT AN EXTRA MOVE!!!!" << endl;
                    }
                }
            }

            delete cur_board;
            newBoard = search->rotateBoard(newBoard);
            if (extraMove)
            {
                play_game(newBoard, 2,htype);
            }
            else
            {
                play_game(newBoard, 1,htype);
            }
        }
    }

    void AI_vs_AI(Board *cur_board, int player,int depth1,int depth2,int htype1,int htype2)
    {
        if(cur_board->AI_vs_AI_result())
            return;
        else if (player == 1) // AI_1...........
        {

            cout << "CURRENT BOARD:" << endl;
            cur_board->printboard();
            cout << "AI-1 IS PLAYING............." << endl;


            int o = search->returnMove(cur_board, depth1,htype1);

            int captured = 0;
            bool extraMove = false;

            int u = cur_board->stoneInOwnBin(o);

            Board *newBoard = new Board(cur_board);

            int currPoint = o;
            bool isMySide = true;
            newBoard->updateStoneInOwnBin(o, 0); /// all stone from current bin is lifted

            while (u > 0)
            {
                currPoint++;
                if (currPoint == 7) /// fill the storage and change the side .................
                {
                    if (isMySide)
                    {
                        newBoard->updateOwnStorage(newBoard->stoneInOwnStorage() + 1);
                        isMySide = false;
                        currPoint = 0;
                    }
                    else
                    {
                        newBoard->updateEnemyStorage(newBoard->stoneInEnemyStorage() + 1);
                        isMySide = true;
                        currPoint = 0;
                    }
                }
                else /// fill the current bin
                {
                    if (isMySide)
                    {
                        newBoard->updateStoneInOwnBin(currPoint, newBoard->stoneInOwnBin(currPoint) + 1);
                    }
                    else
                    {
                        newBoard->updateStoneInEnemyBin(currPoint, newBoard->stoneInEnemyBin(currPoint) + 1);
                    }
                }

                u--;

                /// check the last fillup..............
                if (u == 0)
                {
                    /// look for recent fillup.................

                    if (currPoint != 0 && isMySide == true) /// last move was not in mancala and in the player's bin...
                    {

                        // current bin was previously empty but the opposite bin is nonempty...
                        if (newBoard->stoneInOwnBin(currPoint) == 1 && newBoard->stoneInEnemyBin(7-currPoint) > 0)
                        {
                            captured = 1 + newBoard->stoneInEnemyBin(7-currPoint);
                            newBoard->updateStoneInOwnBin(currPoint, 0);
                            newBoard->updateStoneInEnemyBin(7-currPoint, 0);
                            newBoard->updateOwnStorage(newBoard->stoneInOwnStorage() + captured);
                            cout << "AI-1 CAPTURED " << captured << " STONES FROM AI-2 SIDE!!!!" << endl;
                        }
                    }
                    if (isMySide == false && currPoint == 0) // recently changed side, that means an extra move
                    {
                        extraMove = true;
                        cout << "AI-1 GOT AN EXTRA MOVE!!!!" << endl;
                    }
                }
            }

            delete cur_board;

            if (extraMove)
            {
                AI_vs_AI(newBoard, 1,depth1,depth2,htype1,htype2);
            }
            else
            {
                AI_vs_AI(newBoard, 2,depth1,depth2,htype1,htype2);
            }
        }

        else if (player == 2) // AI_2...........
        {

            cout << "CURRENT BOARD:" << endl;
            cur_board->printboard();
            cout << "AI-2 IS PLAYING............." << endl;

            cur_board = search->rotateBoard(cur_board);
            int o = search->returnMove(cur_board, depth2,htype2);

            int captured = 0;
            bool extraMove = false;

            int u = cur_board->stoneInOwnBin(o);

            Board *newBoard = new Board(cur_board);

            int currPoint = o;
            bool isMySide = true;
            newBoard->updateStoneInOwnBin(o, 0); /// all stone from current bin is lifted

            while (u > 0)
            {
                currPoint++;
                if (currPoint == 7) /// fill the storage and change the side .................
                {
                    if (isMySide)
                    {
                        newBoard->updateOwnStorage(newBoard->stoneInOwnStorage() + 1);
                        isMySide = false;
                        currPoint = 0;
                    }
                    else
                    {
                        newBoard->updateEnemyStorage(newBoard->stoneInEnemyStorage() + 1);
                        isMySide = true;
                        currPoint = 0;
                    }
                }
                else /// fill the current bin
                {
                    if (isMySide)
                    {
                        newBoard->updateStoneInOwnBin(currPoint, newBoard->stoneInOwnBin(currPoint) + 1);
                    }
                    else
                    {
                        newBoard->updateStoneInEnemyBin(currPoint, newBoard->stoneInEnemyBin(currPoint) + 1);
                    }
                }

                u--;

                /// check the last fillup..............
                if (u == 0)
                {
                    /// look for recent fillup.................

                    if (currPoint != 0 && isMySide == true) /// last move was not in mancala and in the player's bin...
                    {

                        // current bin was previously empty but the opposite bin is nonempty...
                        if (newBoard->stoneInOwnBin(currPoint) == 1 && newBoard->stoneInEnemyBin(7-currPoint) > 0)
                        {
                            captured = 1 + newBoard->stoneInEnemyBin(7-currPoint);
                            newBoard->updateStoneInOwnBin(currPoint, 0);
                            newBoard->updateStoneInEnemyBin(7-currPoint, 0);
                            newBoard->updateOwnStorage(newBoard->stoneInOwnStorage() + captured);
                            cout << "AI-2  CAPTURED " << captured << " STONES FROM AI-1 SIDE!!!!" << endl;
                        }
                    }
                    if (isMySide == false && currPoint == 0) // recently changed side, that means an extra move
                    {
                        extraMove = true;
                        cout << "AI-2 GOT AN EXTRA MOVE!!!!" << endl;
                    }
                }
            }

            delete cur_board;
            newBoard = search->rotateBoard(newBoard);
            if (extraMove)
            {
                AI_vs_AI(newBoard, 2,depth1,depth2,htype1,htype2);
            }
            else
            {
                AI_vs_AI(newBoard, 1,depth1,depth2,htype1,htype2);
            }
        }



    }





};

int main()
{
    int u[] = {0, 4, 4, 4, 4, 4, 4};
    Board *startingBoard = new Board(u, u, 0, 0);
    // startingBoard->printboard();
    Adversial_search *search = new Adversial_search();
    Game_simulator *simulator = new Game_simulator(search, 6);
    //simulator->play_game(startingBoard, 1);
    simulator->AI_vs_AI(startingBoard,1,5,5,4,5);
}
