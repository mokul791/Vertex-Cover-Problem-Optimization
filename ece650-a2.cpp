#include<iostream>
#include<bits/stdc++.h>

using namespace std;

#define APPLE 0
#define ORANGE 1
#define BANANA 2
#define SIZE 100

int adj[SIZE][SIZE];
int fruit[SIZE];
int parent[SIZE];
int dist[SIZE];
int n1,n2;
int V, row, col, s, r;
char text,t;



void TakeV();
void TakeE();
void source();
void BFS(int adj[][SIZE] , int V , int sNode, int eNode);
void PrintP(int sNode , int eNode);
void MatrixInit();
void SPath();

void TakeV()
{
    MatrixInit();
    cin>>V;
}

void TakeE()
{
    cin>>t;
    while(t!= '}')
    {
        cin>>t>>n1>>t>>n2>>t>>t;
        if (n1<V && n2<V)
        {
            adj[n1][n2]=adj[n2][n1]=1;
        }
        else
        {
            cout<<"Error: Vertex does not exist. "<<endl;
            continue;
        }
    }
}

void source()
{
    cin >> s >> r;
}

void MatrixInit()
{
    for(row=0;row<100;row++)
    {
        for(col=0;col<100;col++)
        {
            adj[row][col] = 0;
        }
    }
}

void BFS(int adj[][SIZE] , int V , int sNode, int eNode)
{
    for(int i = 0 ; i < V ; i++)
    {
        if(i!= sNode)
        {
            fruit[i] = APPLE;
            dist[i] = INT_MIN;
            parent[i] = -1;
        }
    }

    fruit[sNode] = ORANGE;
    dist[sNode] = 0;
    parent[sNode] = -1;
    queue <int> BFSQueue;
    BFSQueue.push(sNode);
    while(!BFSQueue.empty())
    {
        int u = BFSQueue.front();
        BFSQueue.pop();
        for(int i = 0 ; i < V ; i++)
        {
            if(adj[u][i]!=0)
            {
                int v = i;
                if(fruit[v] == APPLE)
                {
                    fruit[v] = ORANGE;
                    dist[v] = dist[u]+1;
                    parent[v] = u;
                    BFSQueue.push(v);
                    if(v==eNode)
                    {
                        return;
                    }
                }
            }
        }
        fruit[u] = BANANA;
    }
}

void SPath()
{
    source();

    if (s<V && r<V)
    {
        BFS(adj , V , s, r);
        PrintP(s,r);
    }
    else
    {
        cout<<"Error: Invalid value of start or end point detected."<<endl;
    }
}

void PrintP(int sNode , int eNode)
{
    int path[SIZE];
    int  par = parent[eNode];
    int i = 0;
    par = eNode;
    if(parent[eNode]==-1)
    {
        cout << "Error: No path exits" << endl;
        return;
    }

    while(par != -1)
    {
        path[i++] = par;
        par = parent[par];
    }

    while(i>0)
    {
        cout << path[i-1];
        if(i==1)
        {
            cout << endl;
        }
        else
        {
            cout << "-";
        }
        i--;
    }
}

int main()
{
    char text;
    while (cin>>text)
    {
        if(text=='V' || text=='v')
            TakeV();
        else if (text=='E' || text=='e')
            TakeE();
        else if (text=='S'||text=='s')
            SPath();
        else
            cout<<"Error: Invalid Input."<<endl;
    }
}

