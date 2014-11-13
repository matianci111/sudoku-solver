#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>
#include <sstream>
#include <time.h>
#include <iomanip>
#include <vector>
#include <math.h>

using namespace std;

struct node {
    int row;
    int column;
    int possibility;
};

void readsudoku(int allgrid[50][9][9]);
bool solve(int grid[][9], int& backtracking_recall);
void printsudoku(int grid[][9]);
bool checkunassigned(int grid[][9], int& row, int& column);
bool heuristiccheckunassigned(int grid[9][9], int& row, int& column);
bool unviolating(int grid[][9], int row, int column, int number);
bool rowtrue(int grid[][9], int row, int number);
bool columntrue(int grid[][9], int column, int number);
bool boxtrue(int grid[][9], int rowsuffix, int columnsuffix, int number);
int rowposs(int grid[][9], int row);
int columnposs(int grid[][9], int column);
int subgridposs(int grid[][9], int rowsuffix, int columnsuffix);
void assign(int grid[][9], int allgrid[50][9][9], int i);

float backtracking(int grid[][9], int& backtracking_recall);
float heuristic(int grid[][9], int& heuristic_recall);
float preordredlist(int grid, int &preorderedlist_recall);
bool heuristicsolve(int grid[][9], int& heuristic_recall);
bool heuristiccheckunassigned(int grid[9][9], int& row, int& column);
void printchar(vector<int>& bwin);

//preorderedlist
void createlist(int grid[][9], vector<node>& gridlist);
float preorderedlist(int grid[][9], int& preorderedlist_recall);
int findunassignednumber(int grid[][9]);
bool preorderedlistsolve(int grid[][9], vector<node> gridlist, int gridnumber, int& preorderedlist_recall);
bool precheckunassigned(int grid[9][9], int& row, int& column, vector<node> gridlist, int gridnumber);

void frequencyvstime(vector<float> ts, vector<float> t1s, vector<float> t2s);
void recallvstime(vector<int> recall_b, vector<int> recall_h, vector<int> recall_p);



int main(/*int argc, const char * argv[]*/)
{
    int backtracking_recall=0;
    int heuristic_recall=0;
    int preorderedlist_recall=0;
    vector<int> bwin;
    vector<int> hwin;
    vector<int> pwin;
    vector<int> recall_b;
    vector<int> recall_h;
    vector<int> recall_p;
    vector<float> ts;
    vector<float> t1s;
    vector<float> t2s;
    int b=0;
    int h=0;
    int p=0;
    float btotal =0;
    float htotal =0;
    float ptotal =0;
    int grid[9][9];
    int allgrid[50][9][9];
    readsudoku(allgrid);
    int tmp;
    int tmp1;
    int tmp2;
    int recallp=0;
    int recallh=0;
    int recallb=0;
    int limit =250000;
    for(int i=0;i<50;i++){
        //input from below on
//========================
        assign(grid, allgrid, i);
        tmp = backtracking_recall;
    float t = backtracking(grid, backtracking_recall);
        recall_b.push_back(backtracking_recall - tmp);
        if((backtracking_recall - tmp) < limit){
            recallb=(backtracking_recall - tmp)+recallb;
        }
        ts.push_back(t);
//=======================
        assign(grid, allgrid, i);
        tmp1 = heuristic_recall;
    float t1 = heuristic(grid, heuristic_recall);
        t1s.push_back(t1);
        recall_h.push_back(heuristic_recall - tmp1);
        if((heuristic_recall - tmp1) < limit){
            recallh=(heuristic_recall - tmp1)+recallh;
        }
//========================
        assign(grid, allgrid, i);
        tmp2 = preorderedlist_recall;
    float t2 = preorderedlist(grid, preorderedlist_recall);
        t2s.push_back(t2);
        recall_p.push_back(preorderedlist_recall - tmp2);
        if((preorderedlist_recall - tmp2) < limit){
            recallp=(preorderedlist_recall - tmp2)+recallp;
        }
        
        
//=========================
        
        //processing
//=========================
        //who wins
       if(t<t2 && t<t1){
            b++;
            bwin.push_back(i);
        }
        else if(t1<t2 && t1<t2){
            h++;
            hwin.push_back(i);
        }
        else if(t2<t && t2<t1){
            p++;
            pwin.push_back(i);
        }
//=================================
        //total time
        //if(t < 1){
        btotal = btotal +t;
        //}
        //if(t1 < 1){
        htotal = t1 + htotal;
        //}
        //if(t2 < 1){
        ptotal = t2 + ptotal;
        //}
    }
    cout << "backtracking total " << recallb << endl;
    cout << "heuristic total " << recallh << endl;
    cout << "preordered total " << recallp << endl;
    
    cout << "average" << endl;
    cout << recallb/50 << endl;
    cout << recallh/50 << endl;
    cout << recallp/50 << endl;
    cout << "average over" << endl;
    
    //**********************************
    //standard deviation part
    int sdb=0;
    int sdh=0;
    int sdp=0;
    for(int i=0;i<recall_b.size();i++){
        if(recall_b[i]<250000){
            sdb = sdb + (recall_b[i]-(recallb/50)) * (recall_b[i]-(recallb/50));
        }
    }
    for(int i=0;i<recall_h.size();i++){
        if(recall_h[i]<250000){
            sdh = sdh + (recall_h[i]-(recallh/50)) * (recall_h[i]-(recallh/50));
        }
    }
    for(int i=0;i<recall_p.size();i++){
        if(recall_p[i]<250000){
            sdp = sdp + (recall_p[i]-(recallp/50)) * (recall_p[i]-(ptotal/50));
        }
    }
    sdb = sqrt(sdb/50);
    sdh = sqrt(sdh/50);
    sdp = sqrt(sdp/50);
    cout << "standard deviation of backtracking timing is " << sdb << " times" << endl;;
    cout << "standard deviation of heuristic timing is " << sdh << " times" << endl;
    cout << "standard deviation of preordered timing is " << sdp << " seconds" << endl;
    //===============================================
    
//**********************************
//standard deviation part
    float sdbtotal=0;
    float sdhtotal=0;
    float sdptotal=0;
    for(int i=0;i<ts.size();i++){
        if(ts[i]<1){
        sdbtotal = sdbtotal + (ts[i]-(btotal/50)) * (ts[i]-(btotal/50));
        }
    }
    for(int i=0;i<t1s.size();i++){
        if(t1s[i]<1){
        sdhtotal = sdhtotal + (t1s[i]-(htotal/50)) * (t1s[i]-(htotal/50));
        }
    }
    for(int i=0;i<t2s.size();i++){
        if(t2s[i]<1){
            sdptotal = sdptotal + (t2s[i]-(ptotal/50)) * (t2s[i]-(ptotal/50));
        }
    }
    sdbtotal = sqrt(sdbtotal/50);
    sdhtotal = sqrt(sdhtotal/50);
    sdptotal = sqrt(sdptotal/50);
    cout << "standard deviation of backtracking timing is " << sdbtotal << " seconds" << endl;;
    cout << "standard deviation of heuristic timing is " << sdhtotal << " seconds" << endl;
    cout << "standard deviation of preordered timing is " << sdptotal << " seconds" << endl;
//===============================================
 //********printing area*************************

    cout << "Backtracking wins " << b << " times." << " heuristic wins " << h << " times." << " Preordered wins " << p << " times." << endl;
    cout << "Total backtracking time " << btotal << " seconds." << " Total heuristic time " <<htotal<< " seconds." << " Total preordered time " <<ptotal<< " seconds."  << endl;
    cout << "Average backtracking time " << btotal/50 << " seconds." << " Average heuristic time " <<htotal/50<< " seconds." << " Average preordered time " <<ptotal/50<< " seconds." << endl;

    
    
    cout << endl;
    cout << "chart for backtracking" <<endl;
    printchar(bwin);
    cout << "chart for heuristic" <<endl;
    printchar(hwin);
    cout << "chart for preordered" << endl;
    printchar(pwin);
    frequencyvstime(ts, t1s, t2s);
    recallvstime(recall_b, recall_h, recall_p);
    return 0;
    
    
    //**********************************************
    
}

void frequencyvstime(vector<float> ts, vector<float> t1s, vector<float> t2s){
    ofstream outfile;
    outfile.open("/Users/ewan/Desktop/Finalsudoku/frequencyvstime.txt");
    outfile << "number of recalls for backtracking" << endl;
    for(int i=0; i<ts.size();i++){
        outfile <<  ts[i] << endl;
    }
    outfile << endl;
    outfile << endl;
    outfile << "number of recalls for heuristic" << endl;
    for(int i=0; i<t1s.size();i++){
        outfile << t1s[i] <<  endl;
    }
    
    outfile << endl;
    outfile << endl;
    outfile << "number of recalls for preordered" << endl;
    for(int i=0; i<t2s.size();i++){
        outfile << t2s[i] <<  endl;
    }
    
    outfile.close();
}

void recallvstime(vector<int> recall_b, vector<int> recall_h, vector<int> recall_p){
    ofstream outfile;
    outfile.open("/Users/ewan/Desktop/Finalsudoku/frequencyvsrecall.txt");
    outfile << "number of recalls for backtracking" << endl;
    for(int i=0; i<recall_b.size();i++){
        outfile <<  recall_b[i] << endl;
    }
    outfile << endl;
    outfile << endl;
    outfile << "number of recalls for heuristic" << endl;
    for(int i=0; i<recall_h.size();i++){
        outfile << recall_h[i] <<  endl;
    }
    outfile << endl;
    outfile << endl;
    outfile << "number of recalls for preordered" << endl;
    for(int i=0; i<recall_p.size();i++){
        outfile << recall_p[i] <<  endl;
    }
    
    outfile.close();
}

void printchar(vector<int>& bwin){
    int zero=0;
    int ten=0;
    int twen=0;
    int thir=0;
    int fourty=0;
    for(int i=0; i<bwin.size();i++){
        if(bwin[i]<=10 && bwin[i] >=0){
            zero++;
        }
        if(bwin[i]<=20 && bwin[i] >10){
            ten++;
        }
        if(bwin[i]<=30 && bwin[i] >20){
            twen++;
        }
        if(bwin[i]<=40 && bwin[i] >30){
            thir++;
        }
        if(bwin[i]<=50 && bwin[i] >40){
            fourty++;
        }
    }
    cout << "0-10 :";
    for(int i=0; i<zero;i++){
        cout << "*";
    }
    cout << endl;
    cout << "10-20:";
    for(int i=0; i<ten;i++){
        cout << "*";
    }
    cout << endl;
    cout << "20-30:";
    for(int i=0; i<twen;i++){
        cout << "*";
    }
    cout << endl;
    cout << "30-40:";
    for(int i=0; i<thir;i++){
        cout << "*";
    }
    cout << endl;
    cout << "40-50:";
    for(int i=0; i<fourty;i++){
        cout << "*";
    }
    cout << endl;
}
void assign(int grid[][9], int allgrid[50][9][9], int i){
    for(int k=0; k<9; k++){
        for(int j=0; j<9; j++){
            grid[k][j]=allgrid[i][k][j];
        }
    }
}

float backtracking(int grid[][9], int& backtracking_recall){
    float t;
    clock_t t1, t2;
    
    t1 = clock();
    solve(grid, backtracking_recall);
    t2 = clock();
    t = ((float)t2-(float)t1);
    t = t / CLOCKS_PER_SEC;
        
    return t;
}

float heuristic(int grid[][9], int& heuristic_recall){
    float t;
    clock_t t1, t2;
    t1 = clock();
    heuristicsolve(grid, heuristic_recall);
    t2 = clock();
    t = ((float)t2-(float)t1);
    t = t / CLOCKS_PER_SEC;
    
    return t;
}

float preorderedlist(int grid[][9], int& preorderedlist_recall){
    vector<node> gridlist;
    int gridnumber =0;
    float t;
    clock_t t1, t2;
    t1 = clock();
    createlist(grid, gridlist);
    preorderedlistsolve(grid, gridlist, gridnumber, preorderedlist_recall);
    t2 = clock();
    t = ((float)t2-(float)t1);
    t = t / CLOCKS_PER_SEC;
    
    return t;
}

void readsudoku(int allgrid[50][9][9]){
    string s;
    ifstream infile;
    infile.open("/Users/ewan/Desktop/Finalsudoku/commandfile.txt");
    if(!infile.is_open()){
        cout << "file not found" << endl;
        exit(EXIT_FAILURE);
    }
    //get the first
    int i=0;
    int j=0;
    int r=0;
    while(infile >> s){
        if( s == "G"){
            getline(infile, s);
            continue;
        }
        istringstream(s) >> allgrid[r][i][j];
        j++;
        if(i==9&&j==9){
            r++;
            i=0;
            j=0;
        }
        if(j%9==0){
            i++;
            j=0;
        }
    }
    infile.close();
    //read the sudoku into the grid[9][9]
    
}

bool solve(int grid[][9], int& backtracking_recall){
    int row=0, column=0;
    if(!checkunassigned(grid, row, column)){
        return true;
    }
    //check if there is any unassigned position in the sudoku, if any update row and column value to that position
    //if there isn't any, return a true to the previous calling function
    
    for(int i=1; i<=9; i++){
        if(unviolating(grid, row, column, i)){
            //go through 1 to 9 in the unassigned position, try each number to see if it violates the rule
            //if unviolating, put that number in the position, else increment the value by one
            
            grid[row][column] = i;
            
            if(solve(grid, backtracking_recall)){
                return true;
                //if there is no more unassigned position, this means the sudoku is solved,
                //return a true to the previous calling function, and thus will ultimately result a returning
                //of true in the "main function"
            }
            grid[row][column] = 0;
            backtracking_recall++;
            //if all possibilities don't satisfy the sudoku rule, make this position unassigned
            //in case that a further recall of function is required, this position will be 0
            //instead of the previously assigned value
        }
    }
    return false;
    //return a false to the previous function, meaning that all 9 posibilities don't satisfy the sudoku rule
    //and a reconsideration of combination is required
    //this false calls a backtracking
    
    
}

bool heuristicsolve(int grid[][9], int& heuristic_recall){
    int row=0, column=0;
    if(!heuristiccheckunassigned(grid, row, column)){
        return true;
    }
    //check if there is any unassigned position in the sudoku, if any update row and column value to that position
    //if there isn't any, return a true to the previous calling function
    
    for(int i=1; i<=9; i++){
        if(unviolating(grid, row, column, i)){
            //go through 1 to 9 in the unassigned position, try each number to see if it violates the rule
            //if unviolating, put that number in the position, else increment the value by one
            
            grid[row][column] = i;
            
            if(heuristicsolve(grid, heuristic_recall)){
                return true;
                //if there is no more unassigned position, this means the sudoku is solved,
                //return a true to the previous calling function, and thus will ultimately result a returning
                //of true in the "main function"
            }
            grid[row][column] = 0;
            heuristic_recall++;
            //if all possibilities don't satisfy the sudoku rule, make this position unassigned
            //in case that a further recall of function is required, this position will be 0
            //instead of the previously assigned value
        }
    }
    return false;
    //return a false to the previous function, meaning that all 9 posibilities don't satisfy the sudoku rule
    //and a reconsideration of combination is required
    //this false calls a backtracking
    
    
}

bool preorderedlistsolve(int grid[][9], vector<node> gridlist, int gridnumber, int& preorderedlist_recall){
    int row=0, column=0;
    if(!precheckunassigned(grid, row, column, gridlist, gridnumber)){
        return true;
    }
    gridnumber++;
    //check if there is any unassigned position in the sudoku, if any update row and column value to that position
    //if there isn't any, return a true to the previous calling function
    
    for(int i=1; i<=9; i++){
        if(unviolating(grid, row, column, i)){
            //go through 1 to 9 in the unassigned position, try each number to see if it violates the rule
            //if unviolating, put that number in the position, else increment the value by one
            
            grid[row][column] = i;
            
            if(preorderedlistsolve(grid, gridlist, gridnumber, preorderedlist_recall)){
                return true;
                //if there is no more unassigned position, this means the sudoku is solved,
                //return a true to the previous calling function, and thus will ultimately result a returning
                //of true in the "main function"
            }
            grid[row][column] = 0;
            preorderedlist_recall++;
            //if all possibilities don't satisfy the sudoku rule, make this position unassigned
            //in case that a further recall of function is required, this position will be 0
            //instead of the previously assigned value
        }
    }
    return false;
    //return a false to the previous function, meaning that all 9 posibilities don't satisfy the sudoku rule
    //and a reconsideration of combination is required
    //this false calls a backtracking
    
    
}

bool checkunassigned(int grid[9][9], int& row, int& column){
    for(row=0; row<9; row++){
        for(column=0; column<9; column++){
            if(grid[row][column]==0)
                return true;
            //check if there is a unassgined location
            //if there is return true, update the row and column value to that location
            //else print false
        }
    }
    return false;
}

bool heuristiccheckunassigned(int grid[9][9], int& row, int& column){
    int poss=0;
    node lowest;
    lowest.possibility = 50;
    for(row=0; row<9; row++){
        for(column=0; column<9; column++){
            if(grid[row][column]==0){
                poss = rowposs(grid, row) + columnposs(grid, column) + subgridposs(grid, row - row % 3, column - column % 3);
                if(lowest.possibility >= poss){
                    lowest.row = row;
                    lowest.column = column;
                    lowest.possibility = poss;
                }
            }
        }
    }
    
    if(lowest.possibility != 50){
        row = lowest.row;
        column = lowest.column;
        return true;
    }
    else return false;
}

bool precheckunassigned(int grid[9][9], int& row, int& column, vector<node> gridlist, int gridnumber){
    if(gridnumber<gridlist.size()){
        row = gridlist[gridnumber].row;
        column = gridlist[gridnumber].column;
        return true;
    }
    else return false;
}


bool unviolating(int grid[][9], int row, int column, int number){
    return rowtrue(grid, row, number) && columntrue(grid, column, number) && boxtrue(grid, row - (row % 3), column - (column % 3), number);
}

bool rowtrue(int grid[][9], int row, int number){
    for(int i =0; i<9; i++){
        if(grid[row][i]==number){
            return false;
        }
    }
    return true;
    //keep the row number and increment the column number to traverse through the row
    //check if there is a violation of the rule
}

bool columntrue(int grid[][9], int column, int number){
    for(int i =0; i<9; i++){
        if(grid[i][column]==number){
            return false;
        }
    }
    return true;
    //keep the column number and increment the row number to traverse through the row
    //check if there is a violation of the rule
}

bool boxtrue(int grid[][9], int rowsuffix, int columnsuffix, int number){
    
    for(int i=0; i<3; i++){
        for(int j=0; j<3; j++){
            if(grid[i+rowsuffix][j+columnsuffix]==number){
                return false;
            }
        }
    }
    return true;
    //go through that subgrid to check if there is a violation of the rule
}

void printsudoku(int grid[][9]){
    int i=0;
    int j=0;
    
    for(j=0; j<9; j++){
        for(i=0;i<9;i++){
            cout << grid[j][i];
        }
        cout << endl;
    }
    //display the sudoku
}

int rowposs(int grid[][9], int row){
    int p=0;
    for(int i=0; i<9; i++){
        if(grid[row][i]==0){
            p++;
        }
    }
    return p;
}
int columnposs(int grid[][9], int column){
    int p=0;
    for(int i=0; i<9; i++){
        if(grid[i][column]==0){
            p++;
        }
    }
    return p;
    
}
int subgridposs(int grid[][9], int rowsuffix, int columnsuffix){
    int p=0;
    for(int i=0; i<3; i++){
        for(int j=0; i<3; i++){
            if(grid[i+rowsuffix][j+columnsuffix]==0){
                p++;
            }
        }
    }
    return true;
}

void createlist(int grid[][9], vector<node>& gridlist){
    int poss=0;
    node lowest;
    int unassignednumber;
    unassignednumber = findunassignednumber(grid);
    for(unassignednumber; unassignednumber!=0; unassignednumber--){
        lowest.possibility = 50;
        for(int row=0; row<9; row++){
            for(int column=0; column<9; column++){
                if(grid[row][column]==0){
                    poss = rowposs(grid, row) + columnposs(grid, column) + subgridposs(grid, row - row % 3, column - column % 3);
                    if(lowest.possibility >= poss){
                        lowest.row = row;
                        lowest.column = column;
                        lowest.possibility = poss;
                    }
                }
            }
        }
        grid[lowest.row][lowest.column]=10;
        gridlist.push_back(lowest);
    }
    //restore the list to 0;
    for(int row=0; row<9; row++){
        for(int column=0; column<9; column++){
            if(grid[row][column]==10){
                grid[row][column]=0;
            }
        }
    }
    
}

int findunassignednumber(int grid[][9]){
    int number=0;
    for(int row=0; row<9; row++){
        for(int column=0; column<9; column++){
            if(grid[row][column]==0){
                number++;
            }
        }
    }
    return number;
}