//********************************************
// Student Name			:
// Student ID			:
// Student Email Address:
//********************************************
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
//
// Add your own functions if necessary
//
#ifndef __MY_GRAPH_SYSTEM_H_
#define __MY_GRAPH_SYSTEM_H_
#include "../headers.h"
#include <string>
#include "../primitives/vector3d.h"
#include <vector>
#include "base_class.h"
#include "graph_basics.h"

using std::vector;
using std::string;

class Param {
public: 
    static int GRAPH_MAX_NUM_NODES;
    static int GRAPH_MAX_NUM_EDGES;
};

enum class GRAPH_OPERATION {
    GRAPH_OPERATION_NODE_DEPTH = 0
    , GRAPH_OPERATION_ADD_EDGE
    , GRAPH_OPERATION_SHORTEST_PATH
};

class GRAPH_SYSTEM : public BASE_SYSTEM {
    
protected:
    void drawNodes( ) const;
    void drawEdges( ) const;
    void drawSelectedNode( ) const;
    //
    void drawNodes_Indirect( ) const;
    void drawEdges_Indirect( ) const;
    void drawSelectedNode_Indirect( ) const;
    //
    void drawShortestPath() const;
    //
    void showCurrentOperation() const;

    
    //
    void resetPathInformationOfAllNodes();
    void computeShortestPath(GRAPH_NODE* node);
    void computeShortestPath();
    //

    void resetDepthOfAllNodes();
    void computeDepthOfAllNodesFromSelectedNode(GRAPH_NODE* node, int depth);
    void computeDepthOfAllNodesFromSelectedNode();
    float getNodeDepthFromSelectedNode(int nodeIndex) const;

    //
    void initMemoryPool( );
    GRAPH_NODE *getFreeNode( );
    GRAPH_EDGE *getFreeEdge( );
    GRAPH_NODE *findNearestNode( double x, double y, double &cur_distance2 ) const;
    void deleteNode( int nodeID );
    void deleteSelectedNode(  );
    void deleteEdge( int edgeID );
    void deleteEdgesOfNode( int nodeID );
    void removeEdgeFromNode( const GRAPH_EDGE *e, int nodeID );

    //
    void createDefaultGraph();
    void createNet_Circular( int n, int num_layers );
    void createNet_Square( int n, int num_layers );
    void createNet_RadicalCircular( int n );
    void createRandomGraph_DoubleCircles(int n);
public:
    GRAPH_SYSTEM( );
    void update( );
    void stopAutoNodeDeletion();
    void reset( );
    void askForInput( );
    void clickAt(double x, double z);
    void moveTo(double x, double y );
    void handleKeyPressedEvent( unsigned char key );
    void handlePassiveMouseEvent( double x, double y );
    void draw( ) const;
    
    int addNode( float x, float y, float z, float r = 1.0 );
    int addEdge( int nodeID_0, int nodeID_1 );
    bool isSelectedNode( ) const;
    void getInfoOfSelectedPoint( double &r, vector3 &p ) const;
    //
    int getNumOfEdges( ) const;
    vector3 getNodePositionOfEdge( int edgeIndex, int nodeIndex ) const;

    int getNumOfNodes( ) const;
    void getNodeInfo( int nodeIndex, double &r, vector3 &p ) const;
    void getNodeInfo(int nodeIndex, double& r, vector3& p, float& depth) const;
    //

    void changeOperation();

    void performOperation(GRAPH_NODE *node);

protected:
    int mMaxNumNodes;
    int mMaxNumEdges;
    GRAPH_NODE *mNodeArr_Pool; 
    GRAPH_EDGE *mEdgeArr_Pool;
    //
    int *mActiveNodeArr;
    int mCurNumOfActiveNodes;
    int *mActiveEdgeArr;
    int mCurNumOfActiveEdges;
    //
    int *mFreeNodeArr;
    int *mFreeEdgeArr;
    int mCurNumOfFreeNodes;
    int mCurNumOfFreeEdges;
    //
    GRAPH_NODE *mSelectedNode;
    GRAPH_NODE *mPassiveSelectedNode;
    //
    bool mFlgAutoNodeDeletion;

    int mNumPoints_DoubleCircles;
    //
    string mOperationName;
    GRAPH_OPERATION mOperation;

    float mMaxNodeDepth;
    bool mFlgShowNodeDepth;
    //
    vector<GRAPH_NODE *> mPaths;
    GRAPH_NODE *mStartNode, *mDestinationNode;
};

#endif