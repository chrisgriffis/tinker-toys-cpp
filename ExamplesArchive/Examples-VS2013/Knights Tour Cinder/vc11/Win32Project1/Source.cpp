#include "Header.h"
#include "stdio.h" // for printf
#include "stdlib.h" // for qsort
#include <utility> //for standard pair
#include "time.h" // to initialize srand

const int VisitationLookup = NumGameBoardCells;
const int VisitationLookup_Size = NumGameBoardCells+1;
const int NumNeighborsLookup= NumGameBoardCells;
const int NumNeighborsLookup_Size= NumGameBoardCells+1;

const bool printingflag = false; //make true to see backup behavior

const int NUMOFFSETS = 8;
enum label {label_row=0,label_col,labels_size};

//an array of position offsets in (row,col)
//for valid move a knight ('K') can make from a given cell
const int knight_offsets[NUMOFFSETS][labels_size]  = 
{
        {-2,-1},{-2,1},
    {-1,-2},/* |/ */{-1,2},
    { 1,-2},/* |\ */{ 1,2},
        {2,-1}, {2,1}
};


// utilities to aid in the demonstration
void BuildMatrix(int (&matrix)[VisitationLookup_Size][NumNeighborsLookup_Size]);
void PrintMatrix( int (&matrix)[VisitationLookup_Size][NumNeighborsLookup_Size]);
void PrintTour(int (&tour)[NumGameBoardCells]);

// mechanism used to sort cells by their number of neighbors
int CompareStdPairBySecondValue(const void* lhs,const void* rhs);

// core algorithm that conducts the traversal
int KnightsTourWarnsdorff( int (&matrix)[VisitationLookup_Size][NumNeighborsLookup_Size],int (&tour)[NumGameBoardCells], int startNode = 0,int depth = 0);
int KnightsTourClosed( int (&matrix)[VisitationLookup_Size][NumNeighborsLookup_Size],int (&tour)[NumGameBoardCells],int startingPoint=0);


int* getClosedTour()
{
    // initializing variables
    srand(time(0));
    int Matrix[VisitationLookup_Size][NumNeighborsLookup_Size] = {{0}};

    // construct an adjacency matrix of possible move
    BuildMatrix(Matrix);

    // display the matrix in terms of number of neighbors
    //    PrintMatrix(Matrix);

    int tour[NumGameBoardCells] = {0};
    int val =0;
    // start from a random position in the first half of the options available
    if((val = KnightsTourClosed(Matrix, tour,rand()%(NumGameBoardCells / 2))) >= 0) 
    {
        printf("\nThis represents an closed tour, as an ordered sequence of movements");
        printf(", for board %d x %d starts at position (%d,%d).\n\n",
            BoardDimension,BoardDimension,val/BoardDimension,val%BoardDimension);
        PrintTour(tour);
    }
    else
    {
        printf("\n\n Sorry, this board size doesn't support closed circuits.\n");
    }

    //system("pause");
    int* retVal = new int[NumGameBoardCells];
    memcpy(retVal,&tour,NumGameBoardCells*sizeof(int));
    return retVal;
}

int* getOpenTour()
{
    srand(time(0));
    int Matrix[VisitationLookup_Size][NumNeighborsLookup_Size] = {{0}};

    // construct an adjacency matrix of possible move
    BuildMatrix(Matrix);

    // display the matrix in terms of number of neighbors
    //     PrintMatrix(Matrix);

    int tour[NumGameBoardCells] = {0};
    int val =0;
    // start from a random position in the first half of the options available
    if((val = KnightsTourWarnsdorff(Matrix, tour,rand()%(NumGameBoardCells / 2))) >= 0) 
    {
        printf("\nThis represents an open tour, as an ordered sequence of movements");
        printf(", for board %d x %d starts at position (%d,%d).\n\n",
            BoardDimension,BoardDimension,val/BoardDimension,val%BoardDimension);
        PrintTour(tour);
    }
    else
    {
        printf("\n\n Sorry, this board size doesn't support closed circuits.\n");
    }

    //system("pause");
    int* retVal = new int[NumGameBoardCells];
    memcpy(retVal,&tour,NumGameBoardCells*sizeof(int));
    return retVal;
}


void BuildMatrix( int (&matrix)[VisitationLookup_Size][NumNeighborsLookup_Size] )
{
//constructs an adjacency matrix of possible moves.
// if a knight on node A can legally moved to node B, the matrix at (A,B) is set to 1.
// because the nodes are symmetric along the column headings and row headings,
// some extra metadata is stored in matrix padding.
// 1. the matrix is padded with an extra row at the bottom to store visitation
// 2. the matrix is padded with an extra column at the right to store the neighbor count for that node
/*   

a depiction of the array being traversed

nodes  _a_...._n_
     a|          | neighbor
     :| Adjacency| count
     :| Matrix   | col
     n|__________|___
       visitation|unused
          row    |

*/
    //for all board positions, look at a given node
    for (int currentCell = 0; currentCell < NumGameBoardCells ; currentCell++)
    {
        // look at all of the next positions relative to this node
        for (int offset_index = 0; offset_index < NUMOFFSETS; offset_index++)
        {
            //convert to row col for convenience
            int adjacent_node_row = currentCell / BoardDimension  + knight_offsets[offset_index][label_row];
            int adjacent_node_col = currentCell % BoardDimension + knight_offsets[offset_index][label_col];
            
            // determine if this candidate is a valid node
        	if (adjacent_node_row>= 0 &&  adjacent_node_row< BoardDimension &&
                adjacent_node_col>= 0 &&  adjacent_node_col< BoardDimension)
        	{
                //increment neighbor node count
                ++matrix[currentCell][NumNeighborsLookup];
                //connect the two nodes, flatten ordered pair using row major
                matrix[currentCell][adjacent_node_row*BoardDimension+adjacent_node_col] = 1;
        	}
        }

    }
}

void PrintMatrix( int (&matrix)[VisitationLookup_Size][NumNeighborsLookup_Size]) 
{
    printf("Each entry in this matrix represents the number of possible legal moves a knight can make starting from that position.\n\n");
    for (int i = 0; i < NumGameBoardCells; i++)
    {
        printf("%d ", matrix[i][NumNeighborsLookup]);
        if(i%BoardDimension==BoardDimension-1)printf("\n");
    }
}

void PrintTour( int (&tour)[NumGameBoardCells] )
{
    printf("\n\n");
    int outarr[NumGameBoardCells] = {0};
    for (int index = 0; index < NumGameBoardCells ; index++)
    {
        // label the cells of a chessboard according to the tour
    	outarr[tour[index]] = index;
    }
    for (int index = 0; index < NumGameBoardCells ; index++)
    {
    	printf("\t%d ",outarr[index]);
        if (index%BoardDimension==BoardDimension-1)printf("\n\n\n\n");
    }
}

// to support the Warnsdorff heuristic, need to prefer the move that
// has the least neighbors, but you don't know how many you may need to visit,
// therefore you can't just find the smallest, you need to sort.
// but you need to sort the positions based on their neighbors to be able to
// figure out which position corresponds to the neighbors count.
// I'm using this method signature to conform to the qsort interface.
int CompareStdPairBySecondValue(const void* lhs,const void* rhs)
{
    const std::pair<int,int>* _lhs = (const std::pair<int,int>*)lhs;
    const std::pair<int,int>* _rhs = (const std::pair<int,int>*)rhs;
    if ((*_lhs).second < (*_rhs).second)
    {
        return -1;
    } 
    else if((*_lhs).second > (*_rhs).second)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

// Perform a depth first search of the adjacency matrix.
// Mark each cell upon visitation. if in the course of searching,
// a path is found that yields as many nodes as there are on the board,
// then you've got it. Otherwise, keep searching. If you hit a dead-end,
// backup.
// The only difference between this version which uses the heuristic
// and the version that's just a brute force depth first search,
// is that when iterating across all of a given node's adjacent neighbors,
// you're just iterating across a set that has been sorted based on the number of
// neighbors that a candidate target has.
int KnightsTourWarnsdorff( int (&matrix)[VisitationLookup_Size][NumNeighborsLookup_Size],int (&tour)[NumGameBoardCells], int currentNode /*= 0*/,int depth /*= 0*/ )
{
    // recursion stopping condition
    //if the depth is the same as the number of nodes return the starting point
    if (depth == NumGameBoardCells-1)
    {
        //return this starting point
        if(printingflag)printf("\nReached End ");
        return currentNode;
    } 
    //else 
    else
    {
        // notes get marked as visited only if you've arrived there from another node.
        // need to handle the special case of the starting node explicitly.
        if(depth == 0) 
        {
            ++matrix[VisitationLookup][currentNode];
            tour[depth] = currentNode;
        }

        //initialization stuff for setting up the 'sort by neighbors'
        int numberOfPairsInArray = 0;
        std::pair<int,int> pairarray[NUMOFFSETS]; // there's a known upper bound on the number of possible moves a knight can make
        for (int i = 0; i < NUMOFFSETS ; i++) pairarray[i] = std::make_pair(0,0);

        // populate an array that can be sorted.
        //retain the pairing between a physician and its number of neighbors.
        for (int possiblyAdjacentNode = 0; possiblyAdjacentNode < NumGameBoardCells ; possiblyAdjacentNode++)
        {
            if(matrix[currentNode][possiblyAdjacentNode] > 0 ) 
            {
                pairarray[numberOfPairsInArray++] = std::make_pair(possiblyAdjacentNode,matrix[possiblyAdjacentNode][NumNeighborsLookup]);
            }
        }

        // sort the array.
        qsort(&pairarray,NUMOFFSETS,sizeof(std::pair<int,int>),CompareStdPairBySecondValue);

        // the first value in the array is the cell that has the least number of neighbors.
        for (int adjacentNode = 0; adjacentNode < NUMOFFSETS ; adjacentNode++)
        {
	        // (if im an adjacent node) && (I have neighbors eligible for visitation)
	        if (matrix[VisitationLookup][pairarray[adjacentNode].first] == 0 && pairarray[adjacentNode].second != 0)
	        {
	            //Mark current node visited
	            ++matrix[VisitationLookup][currentNode];
	            // make recursive call with that node
	            if(printingflag)printf("\n\tdeeper");
	            int val = KnightsTourWarnsdorff(matrix,tour,pairarray[adjacentNode].first,depth+1);
	            //if it's a valid value, 
	            if (val != -1)
	            {
	                // add it to the array and return this starting point
                    // this code will only be reached if the stopping condition is met.
                    // this is part of the call stack unrolling that actually populates the
                    // tour out var
                    // the flipside of this is that if all of the possibilities have been attempted
                    // without ever having found a solution, this body will never be executed
                    // and the 'tour' out var will be empty
	                tour[depth+1] = val;
	                if(printingflag)printf("\nshallower ");
	                return currentNode;
	            }
                // otherwise it's not a valid value
	            // that call yielded a dead end so im a dead end, un-visit me and 
                // try the next option in the loop
	            --matrix[VisitationLookup][currentNode];
	            if(printingflag)printf("\t\tdeadend");
	        } 
        }

        //if this gets reached, all of the nodes adjacent to this current node
        // resulted in dead ends.
        if(printingflag)printf("\nshallower ");
        return -1;
    }
}

// finds a closed tour solution by brute force.
// iterates across the Board positions, looking for valid open tours.
// stops when it finds the first open tour that meets the closed 
// condition (the final cell has a neighbor that's the initial cell)
int KnightsTourClosed( int (&matrix)[VisitationLookup_Size][NumNeighborsLookup_Size],int (&tour)[NumGameBoardCells],int startingPoint /*= 0*/)
{
    for (; startingPoint < NumGameBoardCells ; startingPoint++)
    {
        if (KnightsTourWarnsdorff(matrix, tour, startingPoint) >= 0)
        {
            // we have a valid tour starting at starting point
            //test to see if the final node on the resulting tour is adjacent to this starting point
            if (matrix[tour[NumGameBoardCells-1]][startingPoint] > 0)
            {
                //return the starting point that created the circuit
                return startingPoint;
            }
        }
        //this starting point didn't produce a circuit
        //set clear the visiting registry and move to the next starting point
        for (int i = 0; i < NumGameBoardCells ; i++)
        {
            matrix[NumNeighborsLookup][i] = 0;
        }
    }
    return -1;
}



