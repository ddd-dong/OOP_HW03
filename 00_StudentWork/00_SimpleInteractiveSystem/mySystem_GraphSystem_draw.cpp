//
//
// Instructor: Sai-Keung WONG
// Email:	cswingo@cs.nctu.edu.tw
//			wingo.wong@gmail.com
//
// National Yang Ming Chiao Tung University, Taiwan
// Computer Science
// Date: 2026/02
//
//
#include "mySystem_GraphSystem.h"
#include "../headers.h"
#include <string>
#include "../../openGLbasic/opengl_stuff.h"
#include <iostream>
using namespace std;

#include "../../openGLbasic/ogl_basic_tools.h"

#include <iostream>
using namespace std;

COLOR_TABLE _colorTable;

void initColorTable()
{
    static bool flg = false;
    if (flg) return;
    flg = true;
    _colorTable.addEntry(0.0, vector3(1.0, 0.0, 0.0));
    _colorTable.addEntry(0.0, vector3(1.0, 0.5, 0.0));
    _colorTable.addEntry(0.0, vector3(1.0, 1.0, 0.0));
    _colorTable.addEntry(0.0, vector3(0.0, 1.0, 0.0));
    _colorTable.addEntry(0.0, vector3(0.0, 1.0, 1.0));
    _colorTable.addEntry(0.0, vector3(0.0, 0.0, 1.0));
    _colorTable.addEntry(0.0, vector3(0.0, 0.0, 0.5));

}

void GRAPH_SYSTEM::drawEdges( ) const
{
    glColor3f(0.0, 0.0, 1.0);
    glLineWidth(3.0);
    glBegin(GL_LINES);
    for ( int i = 0; i < mCurNumOfActiveEdges; ++i ) {
        int id = mActiveEdgeArr[ i ];
        const GRAPH_EDGE *edge = &mEdgeArr_Pool[id];
        int n_0 = edge->nodeID[ 0 ];
        int n_1 = edge->nodeID[ 1 ];

        const vector3 &p0 = mNodeArr_Pool[n_0].p;
        const vector3 &p1 = mNodeArr_Pool[n_1].p;
        glVertex3f( p0.x, p0.y, p0.z );
        glVertex3f( p1.x, p1.y, p1.z );

    }
    glEnd();
    glLineWidth(1.0);
}


void GRAPH_SYSTEM::drawNodes( ) const
{
    glColor3f(1.0, 0.75, 0.0);
    glLineWidth(2.0);
    for ( int i = 0; i < mCurNumOfActiveNodes; ++i ) {
        int id = mActiveNodeArr[ i ];

        const GRAPH_NODE *node = &mNodeArr_Pool[id];
        const vector3 &p = node->p;
        double r = node->r;
        ogl_drawSimpleSolidCircleXZ(p.x, p.y, p.z, r, 12);
    }
    glLineWidth(1.0);
}

void GRAPH_SYSTEM::drawSelectedNode( ) const
{
    if ( mPassiveSelectedNode ) {
        glColor3f(0.0, 0.5, 1.0);

        vector3 p = mPassiveSelectedNode->p;
        double r = mPassiveSelectedNode->r;
        ogl_drawSimpleSolidCircleXZ(
            p.x, 
            p.y -1.0, 
            p.z, 
            r, 
            12);

    }

    if ( mSelectedNode == 0 ) return;
    glColor3f(1.0, 0.3, 0.3);

    ogl_drawSimpleSolidCircleXZ(
        mSelectedNode->p.x, 
        mSelectedNode->p.y -1.0, 
        mSelectedNode->p.z, 
        mSelectedNode->r, 
        12);
}

void GRAPH_SYSTEM::drawEdges_Indirect( ) const
{
    int numEdges = getNumOfEdges( );

    glColor3f(0.0, 0.8, 0.0);
    glLineWidth(3.0);
    glBegin(GL_LINES);
    for ( int i = 0; i < numEdges; ++i ){
        vector3 p0 = getNodePositionOfEdge( i, 0 );
        vector3 p1 = getNodePositionOfEdge( i, 1 );
        glVertex3f( p0.x, p0.y, p0.z );
        glVertex3f( p1.x, p1.y, p1.z );
    }
    glEnd();
    glLineWidth(1.0);
}

/*
void GRAPH_SYSTEM::drawNodes_Indirect( ) const
{
    int numEdges = getNumOfNodes( );
    glColor3f(0.5, 0.5, 0.5);
    glLineWidth(2.0);
    for ( int i = 0; i < numEdges; ++i ) {
        double r;
        vector3 p;
        getNodeInfo( i, r, p );

        ogl_drawSimpleSolidCircleXZ(p.x, p.y, p.z, r, 12);
    }
    glLineWidth(1.0);
}
*/

void GRAPH_SYSTEM::drawNodes_Indirect() const
{
    initColorTable();

    _colorTable.enableAutoScale(true);
    //_colorTable.updateMaxValue(10);
    _colorTable.resetMaxValue(mMaxNodeDepth);
    _colorTable.autoScale();

    int numNodes = getNumOfNodes();
    glColor3f(0.0, 0.0, 1.0);
    glLineWidth(2.0);
    for (int i = 0; i < numNodes; ++i) {

        double r;
        vector3 p;
        float depth;
        getNodeInfo(i, r, p, depth);

        if (mFlgShowNodeDepth) {
            vector3 c = _colorTable.getColor_Linear(depth);
            if (depth == mMaxNodeDepth) {
                //c.x = 0.05;
                //c.y = 0.0;
                //c.z = 0.05;

            }
            glColor3f(c.x, c.y, c.z);
        }

        ogl_drawSimpleSolidCircleXZ(p.x, p.y, p.z, r, 12);
    }
    glLineWidth(1.0);
}

void GRAPH_SYSTEM::drawSelectedNode_Indirect( ) const
{
    if ( mPassiveSelectedNode ) {
        glColor3f(0.0, 0.5, 1.0);

        vector3 p = mPassiveSelectedNode->p;
        double r = mPassiveSelectedNode->r;
        ogl_drawSimpleSolidCircleXZ(
            p.x, 
            p.y -1.0, 
            p.z, 
            r, 
            12);

    }
    if ( isSelectedNode( ) == false ) return;
    glColor3f(1.0, 0.0, 0.0);

    vector3 p;
    double r;
    getInfoOfSelectedPoint( r, p );
    ogl_drawSimpleSolidCircleXZ(
        p.x, 
        p.y -1.0, 
        p.z, 
        r, 
        12);
}

void GRAPH_SYSTEM::drawShortestPath() const
{
    
    if (!mDestinationNode) return;
    GRAPH_NODE* cur = mDestinationNode;

    _colorTable.enableAutoScale(true);
    //_colorTable.updateMaxValue(10);
    _colorTable.resetMaxValue(mDestinationNode->path_cost);
    _colorTable.autoScale();

    glLineWidth(8.0);

    glBegin(GL_LINES);
    while (cur) {
        GRAPH_NODE* pre = cur->path_parent;
        if (!pre) break;
        vector3 p0 = cur->p;
        vector3 p1 = pre->p;

        vector3 c0 = _colorTable.getColor_Linear(cur->path_cost);
        vector3 c1 = _colorTable.getColor_Linear(pre->path_cost);
 
        glColor3f(c0.x, c0.y, c0.z);

        glVertex3f(p0.x, p0.y, p0.z);

        glColor3f(c1.x, c1.y, c1.z);
        glVertex3f(p1.x, p1.y, p1.z);
        cur = pre;
    }
    glEnd();
}

void GRAPH_SYSTEM::showCurrentOperation() const {

    vector3 p0 = vector3(0,0,0);
    
    glColor3f(0, 0, 0);
    glRasterPos3i(p0.x + 1, p0.y - 1, p0.z + 1);

    //glutBitmapString( GLUT_BITMAP_9_BY_15, (const unsigned char *)str.data( ) );
    //
    glutBitmapString(GLUT_BITMAP_TIMES_ROMAN_24, (const unsigned char*) mOperationName.data());
}
void GRAPH_SYSTEM::draw( ) const
{
    drawEdges_Indirect( );
    drawNodes_Indirect( );
    drawSelectedNode_Indirect( );
    drawShortestPath();
    //
    showCurrentOperation();
}
