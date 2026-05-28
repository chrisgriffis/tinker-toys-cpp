//A Knight's Tour
// by Chris Griffis, Jan 2014

// adjust this value and recompile for different board sizes. Best results @ [5..9]
const int BoardDimension = 8;
const int NumGameBoardCells = BoardDimension*BoardDimension;

extern "C" int* getClosedTour();
extern "C" int* getOpenTour();