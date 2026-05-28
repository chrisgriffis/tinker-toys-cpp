// map/bst, WIP

#include <cstdio>
#include <vector>

namespace cgriffis
{

    class ComparisonException{};

    template<typename T>
    struct Compare2
    {
        static int compare(const T& i1, const T& i2)
        {
            if (i1<i2)
            {
                return -1;
            } 
            else if (i1>i2)
            {
                return 1;
            }
            else if (i1==i2)
            {
                return 0;
            }
            else throw ComparisonException();
        }
    };

    template< class T1,class T2>
    struct pair 
    {
        pair():first(),second(){}
        pair(T1& t1,T2& t2):first(t1),second(t2){}
        T1 first;
        T2 second;
    };

    template< class T1,class T2>
    cgriffis::pair<T1,T2> make_pair(T1& t1,T2& t2)
    {
        return pair<T1,T2>(t1,t2);
    }

    template <class T1,class T2, class Comp = Compare2<T1> >
    class Map 
    {
    public:
        struct MapNode
        {
            MapNode():val(cgriffis::pair<T1,T2>()),Left(0),Right(0){}
            cgriffis::pair<T1,T2> val;
            MapNode* Left;
            MapNode* Right;
        };

        Map():m_head(0){}

        T2& operator[](const T1& t1)
        {
            return retrieve(m_head,t1);
        }
    private:
        T2& retrieve(MapNode*&,T1);
        MapNode* m_head;
    };

    template <class T1,class T2, class Comp>
    T2& cgriffis::Map<T1,T2,Comp>::retrieve(MapNode*& n, T1 t1)
    {
        if (n)
        {
            if (Comp::compare(t1 , n->val.first) < 0)
            {
                return retrieve(n->Left,t1);
            } 
            else if (Comp::compare(t1 , n->val.first) > 0)
            {
                return retrieve(n->Right,t1);
            }
            else
            {
                return n->val.second;
            }
        } 
        else
        {
            T2 t2;
            n = new MapNode;
            n->val = cgriffis::make_pair(t1,t2);
            return n->val.second;
        }
    }
}

int main()
{
    using namespace cgriffis;
    Map<int,const char*> mymap;
    Map<const char*,const char*> mymap2;
    Map<int,std::vector<int> > mymap3;

    std::vector<int> v;
    v.push_back(1);

    mymap[1] = "first";
    mymap[2] = "second";
    mymap[3] = "third";

    mymap2["entry"] = "first";
    mymap2["id"] = "chrisg";
    mymap2["month"] = "jan";

    mymap3[-8] = v;

    printf ("%s\n",mymap[2]);
    printf ("%s\n",mymap2["id"]);
    printf ("%d\n",mymap3[-8][0]);
    system("pause");
    return 0;
}
