//BFS+DFS using randomly intialized adjacency matrix

#include "stdio.h"
#include "stdlib.h"
#include "time.h"
#include <utility>
#include <queue>
#include "Heap.h"
#include "GraphEdge.h"

const int SIZE = 15;
const int VisitRegistrySize = SIZE+1;
const int VisitRegistryIdx = VisitRegistrySize-1;

using namespace cgriffis;

void display(EdgelistGraph<VisitRegistrySize,SIZE>& e);

GraphEdge* Prim(int (&adjMtrix)[VisitRegistrySize][SIZE],int startNode=0); //min spanning tree
int Warshall( const int (&adjMtrix)[VisitRegistrySize][SIZE], int (&out)[VisitRegistrySize][SIZE]); //path existence
int Floyd( const int (&adjMtrix)[VisitRegistrySize][SIZE], int (&out)[VisitRegistrySize][SIZE]); //all pair shortest path
void Dijkstra(int (&adjMtrix)[VisitRegistrySize][SIZE],int startNode=0); //single source shortest path
void AStar(int (&adjMtrix)[VisitRegistrySize][SIZE],int startNode=0);
void DFS(int (&adjMatrix)[VisitRegistrySize][SIZE],int startNode=0);
void BFS(int (&adjMatrix)[VisitRegistrySize][SIZE],std::queue<int>& backlog, int startNode=0);


int main()
{
    srand(static_cast<unsigned int>(time(0)));
    int adjMatrix[VisitRegistrySize][SIZE]; //the adjacency matrix

    //initialize and print
    for(int i=0; i<SIZE; i++)
    {
        adjMatrix[VisitRegistryIdx][i]=0;
        for(int j=0; j<SIZE; j++)
        {
            if (i != j)
            {
	            adjMatrix[i][j]=rand()%2*rand()%100;
            } 
            else
            {
                adjMatrix[i][j]=0;
            }
            printf("%d\t",adjMatrix[i][j]);
        }
        printf("\n\n\n");
    }
    printf("\n\nedge list representation\n");

    //edge list rotation
    EdgelistGraph<VisitRegistrySize,SIZE>* e = new EdgelistGraph<VisitRegistrySize,SIZE>(adjMatrix);
    display(*e);
    delete e;

    printf("\n\ndepth first search\n");

    //depth first search
    DFS(adjMatrix);
    printf("\n\nbreadth first search\n");

    for(int i=0; i<SIZE; i++)
    {
        adjMatrix[VisitRegistryIdx][i]=0;
    }

    //breadth first search
    std::queue<int> backlog;
    BFS(adjMatrix,backlog);

    printf("\n\nPrim algorithm\n");
    GraphEdge* primlist = Prim(adjMatrix);
    while(primlist)
    {
        std::cout << primlist->origin_vertex << " | " << primlist->weight << " | " << primlist->terminal_vertex << "\n";
        primlist = primlist->next;
    }

    printf("\n\n\n");
    int out[VisitRegistrySize][SIZE] = {{0}};
    Floyd(adjMatrix,out);
    for(int i=0; i<SIZE; i++)
    {
        for(int j=0; j<SIZE; j++)
        {
            printf("%d\t",out[i][j]);
        }
        printf("\n\n\n");
    }
    printf("\n\nall pair shortest path\n");


    system("pause");
    return 0;
}

void DFS(int (&adjMatrix)[VisitRegistrySize][SIZE],int startNode)
{
    if(adjMatrix[VisitRegistryIdx][startNode] >0) return;

    //visit this node
    ++adjMatrix[VisitRegistryIdx][startNode];
    printf("%d ",startNode);

    //look at all adjacent node to startNode
    for (int nextNode=0; nextNode<SIZE; nextNode++)
    {
        //if this vertex connects startNode to nextNode
        if (adjMatrix[startNode][nextNode] != 0)
        {
            //if target node not adjMatrix[VisitRegistry]
            if (adjMatrix[VisitRegistryIdx][nextNode]==0)
            {
                //go from there
                DFS(adjMatrix,nextNode);
            } 
        }
    }
}

void BFS(int (&adjMatrix)[VisitRegistrySize][SIZE],std::queue<int>& backlog, int startNode)
{
    //initialize the queue
    if (backlog.empty())
    {
        if (!adjMatrix[VisitRegistryIdx][startNode])
        {
            backlog.push(startNode);                
            ++adjMatrix[VisitRegistryIdx][startNode];
        }
        else
        {
            return;
        }
    }
    while(!backlog.empty())
    {
        //dequeue first node
        int thisNode = backlog.front();
        backlog.pop();
        printf("%d ",thisNode);

        //look at all adjacent node to thisNode
        for (int nextNode=0; nextNode<SIZE; nextNode++)
        {
            //if this vertex connects thisNode to nextNode
            if (adjMatrix[thisNode][nextNode] != 0)
            {
                //if target node not adjMatrix[VisitRegistry]
                if (adjMatrix[VisitRegistryIdx][nextNode]==0)
                {
                    //enqueue it
                    backlog.push(nextNode);                
                    ++adjMatrix[VisitRegistryIdx][nextNode];
                } 
            }
        }
    }
}


void display(EdgelistGraph<VisitRegistrySize,SIZE>& e)
{
    for (int i = 0; i < SIZE ; i++)
    {
        GraphEdge* current = e.m_edgelist[i];
        while (current)
        {
            std::cout << current->origin_vertex << " | " << current->weight << " | " << current->terminal_vertex << "\t";
            current = current->next;
        }
        std::cout << "\n";
    }
}

GraphEdge* Prim( int (&adjMtrix)[VisitRegistrySize][SIZE],int startNode/*=0*/ )
{
    for(int i=0; i<SIZE; i++)
    {
        adjMtrix[VisitRegistryIdx][i]=0;
    }

    Heap<GraphEdge> frontier;
    EdgelistGraph<VisitRegistrySize,SIZE> g(adjMtrix);
    GraphEdge* mst=0;

    //get first node
    GraphEdge* current = g.m_edgelist[startNode];
    //initialize frontier
    while(current)
    {
        //if not already in frontier
        if(current->visited < 1)
        {
            //add to frontier
            current->visited = 1;
            frontier.Insert(*current);
        }
        current=current->next;
    }
    GraphEdge* orig = mst = new GraphEdge(frontier.Delete());
    while(!frontier.empty())
    {
        current = g.m_edgelist[orig->terminal_vertex];
        while(current)
        {
            //if not already in frontier
            if(current->visited < 1)
            {
                //add to frontier
                current->visited = 1;
                frontier.Insert(*current);
            }
            current=current->next;
        }
        orig = orig->next = new GraphEdge(frontier.Delete());
    }
    return mst;
}

void Dijkstra( int (&adjMtrix)[VisitRegistrySize][SIZE],int startNode/*=0*/ )
{
    
}

int Warshall( const int (&adjMtrix)[VisitRegistrySize][SIZE] )
{
    return 1;           
}

int Floyd( const int (&adjMtrix)[VisitRegistrySize][SIZE], int (&out)[VisitRegistrySize][SIZE] )
{
    for (int i = 0; i < SIZE ; i++)
    {
    	for (int j = 0; j < SIZE ; j++)
    	{
    		out[i][j] = adjMtrix[i][j];
    	}
    }
    for (int k = 0; k < SIZE ; k++)
    {
    	for (int i = 0; i < SIZE ; i++)
    	{
    		for (int j = 0; j < SIZE ; j++)
    		{
#define makeinf(x) ((x == 0) ? (~0 ^ 1<<31 ^ 1<<30) : x)
#define makezero(x) ((x == (~0 ^ 1<<31 ^ 1<<30)) ? 0 : x)
                out[i][j] = makezero(std::min(
                    makeinf(out[i][j]), 
                    makeinf(out[i][k]) + makeinf(out[k][j])  
                    ));
#undef makeinf
#undef makezero
    		}
    	}
    }
    return 1;
}

