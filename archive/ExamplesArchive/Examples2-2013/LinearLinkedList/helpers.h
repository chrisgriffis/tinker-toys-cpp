template<class List, class MapNode>
int to_array(List* head, MapNode*& arr, int depth = 0 )
{
	if(head)
	{
		arr [ to_array(head->next(), arr, depth+1) ] = head->m_records[0];
		return depth -1;
	}
	//reached bottom
	else
	{
		arr = new MapNode[depth];
		//initialize array somehow
		return depth-1;
	}
}
