//********************************************
// Student Name			:
// Student ID			:
// Student Email Address:
//********************************************
//
// Prof. Sai-Keung WONG
// Email:	cswingo@cs.nctu.edu.tw
//			wingo.wong@gmail.com
//
// National Chiao Tung University, Taiwan
// Computer Science
// Date: 2019/04/09
//
#include "mySystem_2048.h"
#include <iostream>

using namespace std;

static unsigned char specialKeys[] = {
    GLUT_KEY_UP,
    GLUT_KEY_DOWN,
    GLUT_KEY_LEFT,
    GLUT_KEY_RIGHT
};

int numSpecialKeys = sizeof(specialKeys)/sizeof(unsigned char);

MY_2048::MY_2048( )
{
    mNX = 4;
    mNZ = 4;
    reset( );

    mPositionX = 0;
    mPositionZ = 0;
    mFlgFocus = true;

    flg_AutoPlay = false;
}

MY_2048::MY_2048( int nx, int nz )
{
    mNX = nx;
    mNZ = nz;
    reset( );

    mPositionX = 0;
    mPositionZ = 0;
    mFlgFocus = true;
}

void MY_2048::setDimension(int nx, int nz)
{
    mFlgFocus = true;
    ///////////////////////////////////
    mNX = nx;
    mNZ = nz;
    reset( );

    //mPositionX = 0;
    //mPositionZ = 0;
    
}


void MY_2048::setFocus( bool flg )
{
    mFlgFocus = flg;
}

void MY_2048::setPosition( int x, int z )
{
    mPositionX = x;
    mPositionZ = z;
}

MY_2048 &MY_2048::operator=(const MY_2048 &m)
{
    this->copy(&m);
    return (*this);
}

//
//copy the data of the object pointed by m to this object.
//
void MY_2048::copy( const BASE_SYSTEM *m )
{
    for ( int j = 0; j < mNZ; ++j ) {
        for ( int i = 0; i < mNX; ++i ) {
            mBoard[ j ][ i ] = ((MY_2048*)m)->mBoard[ j ][ i ];
        }
    }
}

//
// Copy the previous board to m
//
void MY_2048::copyPrevious( const BASE_SYSTEM *m )
{
    for ( int j = 0; j < mNZ; ++j ) {
        for ( int i = 0; i < mNX; ++i ) {
            mBoard[ j ][ i ] = ((MY_2048*)m)->mPreviousBoard[ j ][ i ];
        }
    }
}


//
//reset the game board
//
void MY_2048::reset( )
{
    mEvent = 0;
    ///////////////////////////////////////
    for ( int j = 0; j < mNZ; ++j ) {
        for ( int i = 0; i < mNX; ++i ) {
            mBoard[ j ][ i ] = 0;
        }
    }
    mNumFreeCells = mNX * mNZ;
    
    copy(mBoard, mPreviousBoard);
}

//
// show messages and ask for input (if any)
//
void MY_2048::askForInput( )
{
    cout << "MY_2048" << endl;
    cout << "Key usage:" << endl;
    cout << "1: generate randomly the numbers 2 and 4 for all the cells" << endl;
    cout << "2: generate randomly the numbers 2, 4 and 8 for all the cells" << endl;
    cout << "3: generate randomly the numbers for all the cells" << endl;
    cout << "r: clear all the cells" << endl;
    cout << " " << endl;
    cout << "UP, DOWN, LEFT, RIGHT: move the numbers to the respective side" << endl;
}

int MY_2048::countFreeCells( ) const
{
    int count = 0;
    for ( int j = 0; j < mNZ; ++j ) { // row
        for ( int i = 0; i < mNX; ++i ) { // column
            if ( mBoard[ j ][ i ] == 0 ) {
                ++count;
            }
        }
    }
    return count;
}

//
// randomly generate a new number
//
void MY_2048::generateNumber( )
{
    if (  mNumFreeCells == 0 ) return;
    //int index = (rand( )%mNumFreeCells);
    int r = rand()%2;
    int v = 2;
    switch( r ) {
    case 0:
        break;
    case 1:
        v = 4;
        break;
    }
    bool flgDone = false;
 
    vector<vector<pair<int,int>>> freeArr(mNZ);

    for ( int j = 0; j < mNZ; ++j ) { // row
        for ( int i = 0; i < mNX; ++i ) { // column
            int z = mNZ - j - 1;
            if ( mBoard[ z ][ i ] == 0 ) {
                //mBoard[ z ][ i ] = v;
                freeArr[j].push_back(pair<int,int>(z,i));
            }
        }
        //if ( flgDone ) break;
    }

    for ( int j = 0; j < mNZ; ++j ) {
        if ( freeArr[j].size() ==0) continue;
        int index = rand()%freeArr[j].size();
        int z = freeArr[j][index].first;
        int x = freeArr[j][index].second;

        mBoard[ z ][ x ] = v;
        break;
    }

}

int MY_2048::findItem( int startIndexZ, int i ) const {
    int z = startIndexZ;
    while ( z < mNZ ) {
        if ( mBoard[z][i] == 0) {
            ++z;
            continue;
        }
        break;
    }
    return z;
}

void MY_2048::moveItem( int zIndex, int targetIndex, int i )
{
    if ( targetIndex == zIndex ) return;
    mBoard[targetIndex][i] = mBoard[zIndex][i];
    mBoard[zIndex][i] = 0;
}

void MY_2048::moveDown_Column( int i ) {
    int z0 = 0;
    int base = 0;
    int flgMerge = false;
    int startZIndex = 0;
    //

    while ( true ) {
        z0 = findItem( startZIndex, i );
        if ( z0 >= mNZ ) break; // no item found
        moveItem( z0, base, i );

        int z1 = findItem( z0 + 1, i );
        //
        //cout << "i:" << i << "\t" << z0 << "\t" << z1 << endl;
        //cout << "startZIndex:" << startZIndex << endl;
        //cout << "base:" << base << endl;

        if ( z1 >= mNZ ) {
            // no item found
            break;
        }

        //can merge?
        if ( mBoard[z1][i] == mBoard[base][i] ) {
            mBoard[base][i] *= 2;
            mBoard[z1][i] = 0;
            ++base;
            startZIndex = z1 + 1;

        } else {
            ++base;
            startZIndex = z1;
        }
    }

}

void MY_2048::copy( int src[][MAX_2048_NX], int tgt[][MAX_2048_NX])
{
    for (int j = 0; j < mNZ; ++j ) {
        for (int i = 0; i < mNX; ++i ) {
            tgt[j][i] = src[j][i];
        }
    }
}

void MY_2048::rotate_CounterClockwise( )
{
    for (int j = 0; j < mNZ; ++j ) {
        for (int i = 0; i < mNX; ++i ) {
            int jx;
            int ix;
            ix = mNZ - j - 1;
            jx = i;
            mTmpBoard[jx][ix] = mBoard[j][i];
        }
    }
    copy( mTmpBoard, mBoard );
}

void MY_2048::rotate_Clockwise( )
{
    for (int j = 0; j < mNZ; ++j ) {
        for (int i = 0; i < mNX; ++i ) {
            int jx;
            int ix;
            ix = j;
            jx = mNX - i - 1;
            mTmpBoard[jx][ix] = mBoard[j][i];
        }
    }
    copy( mTmpBoard, mBoard );
}

void MY_2048::moveDown( )
{
    int z = 0;
    for ( int i = 0; i < mNX; ++i ) {
        moveDown_Column( i );
    }
}

void MY_2048::moveUp( )
{
    rotate_CounterClockwise( );
    rotate_CounterClockwise( );
    moveDown( );
    rotate_Clockwise( );
    rotate_Clockwise( );

}

void MY_2048::moveLeft( )
{
    rotate_CounterClockwise( );
    moveDown( );
    rotate_Clockwise( );
}

void MY_2048::moveRight( )
{
    rotate_Clockwise( );
    moveDown( );
    rotate_CounterClockwise( );
}

void MY_2048::generateNumbers_All_2to4( )
{
    reset( );


    for ( int j = 0; j < mNZ; ++j ) {
        for ( int i = 0; i < mNX; ++i ) {
            int r = rand()%2;

            int v;
            if ( r == 0 ) v = 2;
            else v = 4;
            mBoard[ j ][ i ] = v;
        }
    }
}

void MY_2048::generateNumbers_All_2to8( )
{
    reset( );
    for ( int j = 0; j < mNZ; ++j ) {
        for ( int i = 0; i < mNX; ++i ) {
            int r = rand()%3;

            int v;
            if ( r == 0 ) v = 2;
            else if ( r == 1 ) v = 4;
            else v = 8;
            mBoard[ j ][ i ] = v;
        }
    }
}

void MY_2048::generateNumbers_All( )
{
    reset( );
    for ( int j = 0; j < mNZ; ++j ) {
        for ( int i = 0; i < mNX; ++i ) {
            int r = rand()%11;

            int v = 2;
            v = v << r;
            mBoard[ j ][ i ] = v;
        }
    }
}

void MY_2048::handleKeyPressedEvent( unsigned char key )
{
    switch( key ) {
    case 'r':
    case 'R':
        reset( );
        break;
    case '1':
        generateNumbers_All_2to4( );
        break;
    case '2':
        generateNumbers_All_2to8( );
        break;
    case '3':
        generateNumbers_All( );
        break;
    case ' ':
        flg_AutoPlay = !flg_AutoPlay;

        break;
    case 'a':
        case 'A':
            flg_AutoPlay = true;
            update( );
            flg_AutoPlay = false;
        break;
    }
}

bool MY_2048::performAction( unsigned char key )
{
        bool flg_action = false;

    //copy(mBoard, mPreviousBoard);

    switch( key ) {
    case GLUT_KEY_UP:
        flg_action = true;

        moveUp( );
        mEvent = GLUT_KEY_UP;
        break;
    case GLUT_KEY_DOWN:
        flg_action = true;
        moveDown( );
        mEvent = GLUT_KEY_DOWN;

        break;
    case GLUT_KEY_LEFT:
        flg_action = true;
        moveLeft( );
        mEvent = GLUT_KEY_LEFT;

        break;
    case GLUT_KEY_RIGHT:
        flg_action = true;
        moveRight( );
        mEvent = GLUT_KEY_RIGHT;

        break;

    }

    if ( flg_action ) {
        generateNumber( );
    }

    return flg_action;
}


void MY_2048::handleSpecialKeyPressedEvent( 
    unsigned char key 
    )
{
    //cout << "MY_2048::handleSpecialKeyPressedEvent:" << key << endl;

    int initBoard[MAX_2048_NZ][MAX_2048_NX];
    copy(mBoard, initBoard);
    bool flg_action = performAction( key );
    if ( flg_action ) {
        copy(initBoard, mPreviousBoard);
    }

}

double MY_2048::computeScore( ) const
{
    double score = 0.0;
    pair<int,int> largest;
    double largest_score = 0.0;
    for ( int j = 0; j < mNZ; ++j ) {
        for ( int i = 0; i < mNX; ++i ) {
            double s = mBoard[ j ][ i ];
            if (s > largest_score) {
                largest_score = s;
                largest.first = j;
                largest.second = j;

            }
            score += s*pow(s, 0.35);
        }
    }
    /*

    if (largest.first == 0 
        ||
        largest.first == mNX -1
        ||
        largest.second == 0 
        ||
        largest.second == mNX -1) {
            score *= 1.2;
    }
    */

    /*
    if (largest.first == 0 
        ||
        largest.first == mNX -1
        ) {
            score += largest_score;

            int x = largest.first;
            int z = largest.second;



    }

    if ( largest.second == 0 
        ||
        largest.second == mNX -1) {
            score += largest_score;
    }
    */

    return score;
}


void MY_2048::play_Random( )
{
    int action_index = rand()%numSpecialKeys;
    handleSpecialKeyPressedEvent( specialKeys[action_index] );
}

double MY_2048::play_Smart_Recursive( 
    int numSteps, 
    unsigned char action_key,
    int board[][MAX_2048_NX]
    )
{
    double tScore = 0.0;
    if (numSteps <=0) {
        tScore = computeScore( );
        return tScore;
    }

    copy(board, mBoard);
    handleSpecialKeyPressedEvent( action_key );
    //double tScore = computeScore( );

    int initBoard[MAX_2048_NZ][MAX_2048_NX];
    copy(mBoard, initBoard);
    double rate = 0.85;
    double score = 0.0;

    
    for (int i = 0; i < numSpecialKeys; ++i ) {
        score = 
            
            score*pow(rate, numSteps) 
            + 
            
            play_Smart_Recursive( 
            numSteps - 1, 
            specialKeys[ i ], 
            initBoard );
    }
    

    /*
    int num = rand()%(numSpecialKeys-1)+2;
    int actions[4];
    bool mark[4] = {0, 0, 0, 0};
    for (int i = 0; i < num; ++i) {
        int index = 0;
        while (true) {
            index = rand()%numSpecialKeys;
            if (!mark[index]) break;
        }
        mark[index] = true;
        actions[i] = specialKeys[ index ];
    }

    for (int i = 0; i < num; ++i ) {
        score = 
            
            score*pow(rate, numSteps) 
            + 
            
            play_Smart_Recursive( 
            numSteps - 1, 
            actions[i], 
            initBoard );
    }
    */
    return score;
}

double MY_2048::play_Smart( 
    int numForwardMoves, 
    unsigned char action_index 
    )
{
    double score = 0.0;

    int initBoard[MAX_2048_NZ][MAX_2048_NX];
    copy(mBoard, initBoard);
    

    //handleSpecialKeyPressedEvent( action_index );

    //double tScore = computeScore( );
    //
    double fScore = play_Smart_Recursive( 
        numForwardMoves, 
        action_index, 
        initBoard );
    //
    copy(initBoard, mBoard);
    //
    score = fScore;
    return score;
}

void MY_2048::play_Smart( int numForwardMoves )
{
    vector<double> score(numSpecialKeys, 0);
    for (int i = 0; i < numSpecialKeys; ++i ) {
        score[i] = play_Smart( numForwardMoves, specialKeys[i] );
    }

    double best_score = score[0];
    int action_index = 0;
    for (int i = 1; i < numSpecialKeys; ++i ) {
        if (score[i] > best_score) {
            best_score = score[i];
            action_index = i;
        }
    }
    handleSpecialKeyPressedEvent( specialKeys[action_index] );
}


void MY_2048::update( )
{
    if (!flg_AutoPlay) return;
    cout << "MY_2048::update( )" << endl;

    //int freeCount = countFreeCells( );

    //if (!freeCount) return;

    //play_Random( );

    copy(mBoard, mPreviousBoard);
    //

    //int mPreviousBoard[MAX_2048_NZ][MAX_2048_NX];

    int numForwardMoves = 7;
    play_Smart( numForwardMoves );
}

bool MY_2048::isAutoPlay() const
{
    return flg_AutoPlay;
}

