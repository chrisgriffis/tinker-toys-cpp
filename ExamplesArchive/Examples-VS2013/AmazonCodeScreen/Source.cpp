//time complexity of solution
//T(n) = O((|V| + |E|) * lgN)
// space complexity of solution
////S(n) = O(n) (for set container)


#include <cstdlib>
#include <stdio.h>
#include <vector>
#include <set>
#include <iostream>
#include <queue>

using std::vector;
using std::set;

class Movie {
private:
    int movieId;
    float rating;
    vector<Movie*> similarMovies; // Similarity is bidirectional

    struct CompareByRating
    {
        bool operator() (Movie* const x, Movie* const y) 
        {
            if(NULL == x || NULL == y)return false;
            return x->getRating()<y->getRating();
        }
    };

public:
    Movie(int movieId, float rating) {
        this->movieId = movieId;
        this->rating = rating;
    }

    int getId() {
        return movieId;
    }

    float getRating() {
        return rating;
    }

    void addSimilarMovie(Movie* movie) {
        similarMovies.push_back(movie);
        movie->similarMovies.push_back(this);
    }

    vector<Movie *>& getSimilarMovies() {
        return similarMovies;
    }

    /*
    * Implement a function to return top rated movies in the network of movies
    * reachable from the current movie
    * eg:            A(Rating 1.2)
    *               /   \
    *            B(2.4)  C(3.6)
    *              \     /
    *               D(4.8)
    * In the above example edges represent similarity and the number is rating.
    * getMovieRecommendations(A,2) should return C and D (sorting order doesn't matter so it can also return D and C)
    * getMovieRecommendations(A,4) should return A, B, C, D (it can also return these in any order eg: B,C,D,A)
    * getMovieRecommendations(A,1) should return D. Note distance from A to D doesn't matter, 
    *                             return the highest rated.
    *
    *     @param movie
    *     @param numTopRatedSimilarMovies
    *                      number of movies we want to return
    *     @return List of top rated similar movies
    */
    static void getMovieRecommendations(Movie& movie, int numTopRatedSimilarMovies, vector<Movie *>& recommendedMovies) {
        // Implement me
        //S(n) = O(n) for set container
        std::set<Movie*,CompareByRating> similarMovieNetwork;
        std::queue<Movie*> unprocessedMovies;
        unprocessedMovies.push(&movie);
        traverseAndInsert(similarMovieNetwork, unprocessedMovies);
        //time complexity for this dominated by traverse and insert
        for (std::set<Movie*,CompareByRating>::reverse_iterator it = similarMovieNetwork.rbegin(); it != similarMovieNetwork.rend(); it++)
        {
            if (numTopRatedSimilarMovies-- > 0)
            {
                recommendedMovies.push_back(*it);
            }
            else
            {
                return;
            }
        }
        return;
    }

    //T(n) = O(|V| + |E|) traversal
    static void traverseAndInsert( std::set<Movie*,CompareByRating>& distinctMovies, std::queue<Movie*>& unprocessedMovies )
    {
        using namespace std;
        while(!unprocessedMovies.empty())
        {
            Movie* n = unprocessedMovies.front();
            unprocessedMovies.pop();
            if(NULL != n)
            {
                if (distinctMovies.insert(n).second)
                {
                    for (auto similarMovie : n->getSimilarMovies())
                    {
                        unprocessedMovies.push(similarMovie);
                    }
                }
            }
        }
        return;
    }

};


int main()
{
    const static int MOVIE_LIBRARY_SIZE = 100000;
    vector<Movie*> mvec;
    mvec.reserve(MOVIE_LIBRARY_SIZE);
    srand(2047);
    for (int i = 0; i < MOVIE_LIBRARY_SIZE ; i++)
    {
        float rating = 5.0f * static_cast <float> (rand()) / (static_cast <float> (RAND_MAX));
        mvec.push_back(new Movie(rand(),rating));
    }

    //for each movie
    for (int i = 0; i < MOVIE_LIBRARY_SIZE ; i++)
    {
        //pick a random number of similar movies
        //keeping similar movie count low ('3') keeps similarity network from ending up
        //being the same as the whole network
        int numSimilarTitles = rand()%4;
        for (int j = 0; j < numSimilarTitles ; j++)
        {
            //make random titles similar
        	mvec[i]->addSimilarMovie(mvec[rand()%mvec.size()]);
        }
    	
    }

    std::vector<Movie*> recs;
    const static int NumberOfRecs = 5;
    int randomTitle = rand()%mvec.size();
    Movie::getMovieRecommendations(*mvec[randomTitle],NumberOfRecs,recs);
    for (unsigned int i = 0; i < recs.size() ; i++)
    {
        std::cout << recs[i]->getId() << " ";
    }
    std::cout << std::endl;
}