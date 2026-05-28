#include <iostream>
#include <vector>
#include <queue>
#include <unordered_map>
#include <functional>
#include <iterator>
#include <utility>
#include <algorithm>
// This is the text editor interface. 
// Anything you type or change here will be seen by the other person in real time.



//// API begin ////
//in the header
namespace PullReq
{
	using ResultCode = int;
	using UserId = int;
	using PRId = int;

	// User API
	ResultCode request(UserId, PRId); //adds to review queue
	ResultCode remove(UserId, PRId); //remove from review queue

									 // Reviewer API
									 //assumes supplied PRid has been pulled from the awaitingreview queue
	PRId getNextPRAwaitingReview(); //return AwaitingReview.front(); 
	ResultCode voteup(UserId); //if(!AwaitingReview.empty()) ++AwaitingReview.front().votebalance;
	ResultCode votedown(UserId); //if(!AwaitingReview.empty()) --AwaitingReview.front().votebalance;
	ResultCode setAwaitingAsReviewed(UserId); //AwaitingApproval.push(AwaitingReview.front()); AwaitingReview.pop();

											  // approver API
											  //assumes supplied PRid has been pulled from the AwaitingApproval queue
	PRId getNextPRAwaitingApproval();//return AwaitingApproval.front(); 
	ResultCode approvePRAwaitingApproval(UserId); ////AwaitingMerge.push(AwaitingApproval.front()); AwaitingApproval.pop();
	ResultCode rejectPRAwaitingApproval(UserId); //handling of PRId TBD... placed in a "rejection queue"?

	struct PR
	{
		int id;
		int origin; //e.g. branch-1
		int target; //e.g. master, thus requesting branch-1 -> master
		int votebalance; //upvaote -> ++votebalance; downvote -> --votebalance
	};
}
///API end ///

//in impl.cpp, 
namespace
{
	std::queue<PullReq::PR> AwaitingReview;
	std::queue<PullReq::PR> AwaitingApproval;
	std::queue<PullReq::PR> AwaitingMerge;
}



using namespace std;
using namespace PullReq;


//returns the PRs in order of how they should be merged
//any PR will have all of its prereqs met by the time its gets merged if 
//the PRs are merged in the order given in this vector
vector<PR> process(queue<PR>& awaitingMerge)
{
	//DependencyLookup is a hash map that, using an originating branch id as key,
	//will give the list of PRs that need to be merged from this branch into the 
	//target common to all PRs in the collection. it assumes that multiple branches 
	//can merge into a single branch, but 
	//a single branch cannot merge into multiple branch. this constrains and 
	//simplifies the problem to allow originating branch to act as a unique key
	//value type is a pair, the second member being the list of PRs originating 
	//from this branch, and the first member being a marker that keeps track of 
	//which originating branches dont have any incoming merges. by marking this branch
	//when it shows up as a target, only dependent-free branches
	//will stay unmarked, allowing them to be chosen correctly when iterating for DAG roots. 
	using DependencyLookup = std::unordered_map<int, pair<bool, vector<PR>>>;
	using DLIter = DependencyLookup::iterator;

	DependencyLookup PRs_by_originating_branch;
	vector<PR> mergeOrdering; //return value on stack, possibly elided

							  //define a DFS topo sort lambda
							  //create a locally scoped ad hoc recursion lambda instead of a named function.
							  //(i.e. why pollute the broader namespace with a named function that has only one-time 
							  //use when it can be avoided? also this keeps the logic local to where it's used and makes 
							  //it easier to reason about)
	std::function<void(DLIter)> DFS =
		[&DFS, &PRs_by_originating_branch, &mergeOrdering](DLIter entry)
	{
		//copy entry locally on stack
		auto prs_originating_from_this_branch = entry->second.second;
		//mark "visited" by removing corresponding entry from candidate pool (a.k.a Lookup Table)
		PRs_by_originating_branch.erase(entry);
		//figure out dependency via lookup
		if (!prs_originating_from_this_branch.empty())
		{
			auto next = PRs_by_originating_branch.find(prs_originating_from_this_branch[0].target);
			if (next != PRs_by_originating_branch.end())
				DFS(next);
		}
		//finished DFS, append this batch of PRs to the list
		copy(prs_originating_from_this_branch.begin(),
			prs_originating_from_this_branch.end(),
			back_inserter(mergeOrdering));
	}; //lambda defined but not yet called

	   //now that setup is done, evaluate whats in the queue
	while (!awaitingMerge.empty())
	{
		//populate the lookup table based on originating branch for a given PR
		//supports multiple PRs from a single originating branch
		auto& curr = awaitingMerge.front();
		if (PRs_by_originating_branch.find(curr.origin) == PRs_by_originating_branch.end())
			PRs_by_originating_branch[curr.origin].first = false; //initialize this branch as being untargeted
		PRs_by_originating_branch[curr.origin].second.emplace_back(curr); //add this PR to group originating from this branch
		PRs_by_originating_branch[curr.target].first = true; //register this occurrence of target as being someone's target branch
		awaitingMerge.pop();
	}

	//for all roots in the DAG forest...
	auto entry = PRs_by_originating_branch.begin();
	while (entry != PRs_by_originating_branch.end())
	{
		// if this entry represents an originating branch that isn't some PRs target branch...
		if (!entry->second.first)
		{
			//topo sort using it as the root of a DAG
			DFS(entry);
			//DFS modifies the table so start from beginning again
			entry = PRs_by_originating_branch.begin();
		}
		else ++entry;
	}

	//topo sort discovers dependencies in reverse order.
	//instead of inefficient vector prepend or inefficient link list (pointer-chasing)
	//just append values to a vector and reverse the list when its populated
	//alternative is to return a stack, but vector has more natural collection semantics
	//for these purposes (traversable, has sub-ranges, etc)
	reverse(mergeOrdering.begin(), mergeOrdering.end());
	return mergeOrdering;
}


#include <iostream>


//master is branch 0
//branches are labeled as ints
//PRs are labeled as ints

/*

PRs		PRs		PRs
1,2,3	4,5,6	7,8,9
Br1	Br2		Br3
\		|		|
PR10	  PR11
Br4	  Br5
|	   /
Master Br0
*/
int main()
{
	vector<PR> inputset
	{
		// 		PRs
		// 		1,2,3
		PR{ 1,1,4,0 }, PR{ 2,1,4,0 }, PR{ 3,1,4,0 },
		// 		PRs
		// 		4,5,6
		PR{ 4,2,4,0 }, PR{ 5,2,4,0 }, PR{ 6,2,4,0 },
		// 		PRs
		// 		7,8,9
		PR{ 7,3,5,0 }, PR{ 8,3,5,0 }, PR{ 9,3,5,0 },
		// 		PR10
		PR{ 10,4,0,0 },
		// 		PR11
		PR{ 11,5,0,0 }
	};

	for (auto& pr : inputset)
		AwaitingMerge.emplace(pr);
	auto result = process(AwaitingMerge);
	for_each(result.begin(), result.end(), [](auto& pr) {
		cout <<
			"(id: " << pr.id <<
			", origin: " << pr.origin <<
			", target: " << pr.target <<
			") " << endl;
	});
	return 0;
}

