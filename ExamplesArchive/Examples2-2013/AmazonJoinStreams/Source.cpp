#include <xutility>
#include "string"

using namespace std;
using uint unsigned int;
using SId = uint;
using Card = long int;
using Ref = long int;
using Person = string;
using Address = string;
using Stream1 = iterator < forward_iterator_tag, tuple<SId, Card, Ref> >;
using Stream2 = iterator < forward_iterator_tag, tuple<SId, Person, Address> > ;
using Table = tuple<SId, Card, Ref, Person, Address>;

template<typename Stream>
void merge(Stream s, Table table); //gen case

template<>
void merge<Stream1>(Stream1 s1, Table& table)
{

}
template<>
void merge<Stream2>(Stream2 s2, Table& table)
{

}

//two streams, common join. real time populate a table with the join

int main()
{
	//populate inputs or otherwise atart streaming
	//output table
	return 0;
}
