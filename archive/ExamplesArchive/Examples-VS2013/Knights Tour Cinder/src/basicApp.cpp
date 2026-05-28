#include "../vc11/Win32Project1/Header.h"

#include "cinder/app/AppBasic.h"
#include <list>

using namespace ci;
using namespace ci::app;
using namespace std;




// We'll create a new Cinder Application by deriving from the AppBasic class
class BasicApp : public AppBasic {
  public:
      BasicApp();
	void mouseDrag( MouseEvent event );
	void keyDown( KeyEvent event );
	void draw();

	// This will maintain a list of points which we will draw line segments between
	list<Vec2f>		mPoints;
    list<Vec2f>		mTourPoints;
    int* m_tour;
    float Bounds;
    bool m_isClosed;
};

BasicApp::BasicApp():m_tour(nullptr),Bounds(50.0f),m_isClosed(false)
{
    mPoints.clear();
    for (int i = 0; i <= BoardDimension ; i++)
    {
        if (i%2==0)
        {
            for (int j = 0; j <= BoardDimension ; j++)
            {
                mPoints.push_back(*(new cinder::Vec2f(i*Bounds,j*Bounds)));
            }
        } 
        else
        {
            for (int j = BoardDimension; j >= 0 ; j--)
            {
                mPoints.push_back(*(new cinder::Vec2f(i*Bounds,j*Bounds)));	
            }
        }
    }
    for (int j = 0; j <= BoardDimension ; j++)
    {
        if (j%2==1)
        {
            for (int i = 0; i <= BoardDimension ; i++)
            {
                mPoints.push_back(*(new cinder::Vec2f(i*Bounds,j*Bounds)));	
            }
        } 
        else
        {
            for (int i = BoardDimension; i >= 0 ; i--)
            {
                mPoints.push_back(*(new cinder::Vec2f(i*Bounds,j*Bounds)));	
            }
        }
    }
}

void BasicApp::mouseDrag( MouseEvent event )
{
}

void BasicApp::keyDown( KeyEvent event )
{
	if( event.getChar() == 'f' )
		setFullScreen( ! isFullScreen() );
    else if(event.getChar() == 'c' )
    {
        if(m_tour!=nullptr)delete m_tour;
        m_tour = getClosedTour();
        m_isClosed = true;
    }
    else if (event.getChar() == 'o' )
    {
        if(m_tour!=nullptr)delete m_tour;
        m_tour = getOpenTour();
        m_isClosed = false;
    }
    else exit(0);
    if(m_tour!=nullptr)
    {
        mTourPoints.clear();
        for (int i = 0; i < NumGameBoardCells ; i++)
        {
            int col = m_tour[i]%BoardDimension;
            int row = m_tour[i]/BoardDimension;
            mTourPoints.push_back(*(new cinder::Vec2f(col*Bounds + (Bounds/2),row*Bounds + (Bounds/2))));
        }
        if(m_isClosed)mTourPoints.push_back(*(new cinder::Vec2f((m_tour[0]%BoardDimension)*Bounds + (Bounds/2),(m_tour[0]/BoardDimension)*Bounds + (Bounds/2))));
    }
}

void BasicApp::draw()
{
	gl::clear( Color( 0.1f, 0.1f, 0.15f ) );
	gl::color( 1.0f, 0.5f, 0.25f );	
	gl::begin( GL_LINE_STRIP );
	for( auto pointIter : mPoints ) {
		gl::vertex( pointIter );
	}
	gl::end();

    gl::color( 3.0f, 3.5f, 3.25f );	
    gl::begin( GL_LINE_STRIP );
    for( auto pointIter : mTourPoints ) {
        gl::vertex( pointIter );
    }
    gl::end();


}

// This line tells Cinder to actually create the application
CINDER_APP_BASIC( BasicApp, RendererGl )
