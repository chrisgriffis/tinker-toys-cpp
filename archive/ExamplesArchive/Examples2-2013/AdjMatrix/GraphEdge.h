#pragma once

namespace cgriffis
{
    typedef int position;
    typedef int Weight;
    typedef int vertex;
    struct GraphEdge
    {
        GraphEdge():origin_vertex(0),terminal_vertex(0),weight(0),next(0),visited(0){}
        GraphEdge(const GraphEdge& e):origin_vertex(e.origin_vertex),terminal_vertex(e.terminal_vertex),weight(e.weight),next(e.next),visited(e.visited){}
        GraphEdge(Weight w, vertex o, vertex t):weight(w),origin_vertex(o),terminal_vertex(t),next(0),visited(0){}
        bool operator<(GraphEdge& rhs){return -weight<-rhs.weight;}
        bool operator>(GraphEdge& rhs){return -weight>-rhs.weight;}
        bool operator<=(GraphEdge& rhs){return -weight<=-rhs.weight;}
        bool operator>=(GraphEdge& rhs){return -weight>=-rhs.weight;}
        bool operator==(GraphEdge& rhs){return weight==rhs.weight;}
        GraphEdge& operator=(const GraphEdge& e)
        {
            origin_vertex = e.origin_vertex;
            terminal_vertex = e.terminal_vertex;
            weight = e.weight;
            next = e.next;
            visited = e.visited;
            return *this;
        }
        vertex origin_vertex;
        vertex terminal_vertex;
        Weight weight;
        GraphEdge* next;
        int visited;
    };

    template<size_t ADJ_MAT_DIM_1 = 1, size_t ADJ_MAT_DIM_2 = ADJ_MAT_DIM_1>
    struct EdgelistGraph
    {
        int edgecount;
        int vertexcount;
        EdgelistGraph( int (&adjMtrix)[ADJ_MAT_DIM_1][ADJ_MAT_DIM_2]):edgecount(0),vertexcount(0)
        {
            m_edgelist = new GraphEdge*[ADJ_MAT_DIM_2];
            for (int i = 0; i < ADJ_MAT_DIM_2 ; i++)
            {
                m_edgelist[i] = NULL;
                vertexcount++;
            }
            for (int row = 0; row < ADJ_MAT_DIM_2 ; row++)
            {
                for (int col = 0; col < ADJ_MAT_DIM_2 ; col++)
                {
                    if (adjMtrix[row][col] > 0)
                    {
                        if (m_edgelist[row])
                        {
                            //value belongs in graph
                            GraphEdge* temp = m_edgelist[row];
                            m_edgelist[row] = new GraphEdge(adjMtrix[row][col],row,col);
                            m_edgelist[row]->next = temp;
                        } 
                        else
                        {
                            m_edgelist[row] = new GraphEdge(adjMtrix[row][col],row,col);
                        }
                        edgecount++;
                    }   	    	
                }
            }
        }
        virtual ~EdgelistGraph()
        {
            //dont delete nodes that have been created
            delete[] m_edgelist;
        }
        GraphEdge** m_edgelist;
    };

}
