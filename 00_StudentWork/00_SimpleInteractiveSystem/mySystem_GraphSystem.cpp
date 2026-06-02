//********************************************
// Student Name			:林育廷
// Student ID			:114550200
// Student Email Address:yutinglin.cs14@nycu.edu.tw
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
#include <iostream>
#include "mySystem_GraphSystem.h"
#include <time.h>

using namespace std;

namespace SYS_CONSTANTS {
     int max_int = INT_MAX;
     double max_double = DBL_MAX;
};

int Param::GRAPH_MAX_NUM_NODES = 10000;
int Param::GRAPH_MAX_NUM_EDGES = 10000;

GRAPH_SYSTEM::GRAPH_SYSTEM( )
{
    mFlgAutoNodeDeletion = false;

    mFlgShowNodeDepth = false;

    //mOperation = GRAPH_OPERATION_ADD_EDGE;
    //mFlgShowNodeDepth = false;

    //mOperation = GRAPH_OPERATION::GRAPH_OPERATION_NODE_DEPTH;
    //mFlgShowNodeDepth = true;

    mOperation = GRAPH_OPERATION::GRAPH_OPERATION_SHORTEST_PATH;
    mOperationName = "SHORTEST_PATH";

    mMaxNodeDepth = 0.0;

    mStartNode = nullptr;
    mDestinationNode = nullptr;

    initMemoryPool();
    createDefaultGraph();
    //
    // modify and add your code heres
    //
}

void GRAPH_SYSTEM::initMemoryPool( )
{
    mMaxNumNodes = Param::GRAPH_MAX_NUM_NODES;
    mMaxNumEdges = Param::GRAPH_MAX_NUM_EDGES;

    mNodeArr_Pool = new GRAPH_NODE[mMaxNumNodes];
    mEdgeArr_Pool = new GRAPH_EDGE[mMaxNumEdges];

    mCurNumOfActiveNodes = 0;
    mCurNumOfActiveEdges = 0;
    mActiveNodeArr = new int[mMaxNumNodes];
    mActiveEdgeArr = new int[mMaxNumEdges];

    mFreeNodeArr = new int[mMaxNumNodes];
    mFreeEdgeArr = new int[mMaxNumEdges];
    //
    for ( int i = 0; i < mMaxNumNodes; ++i ) {
        mNodeArr_Pool[ i ].id = i;
    }
    for ( int i = 0; i < mMaxNumEdges; ++i ) {
        mEdgeArr_Pool[ i ].id = i;
    }
    //
    reset( );
}

void GRAPH_SYSTEM::reset( )
{
    stopAutoNodeDeletion();

    //mNumPoints_DoubleCircles = 0;

    mCurNumOfActiveNodes = 0;
    mCurNumOfActiveEdges = 0;

    mCurNumOfFreeNodes = mMaxNumNodes;
    mCurNumOfFreeEdges = mMaxNumEdges;

    for ( int i = 0; i < mCurNumOfFreeNodes; ++i ) {
        mFreeNodeArr[ i ] = i;
    }
    for ( int i = 0; i < mCurNumOfFreeEdges; ++i ) {
        mFreeEdgeArr[ i ] = i;
    }

    mPassiveSelectedNode = 0;
    mSelectedNode = 0;
    //
    mFlgAutoNodeDeletion = false;

    mFlgShowNodeDepth = false;

    mMaxNodeDepth = 0.0;

    mStartNode = nullptr;
    mDestinationNode = nullptr;
}

GRAPH_NODE *GRAPH_SYSTEM::getFreeNode( ) 
{
    if ( mCurNumOfFreeNodes == 0 ) return 0;
    --mCurNumOfFreeNodes;
    int id = mFreeNodeArr[ mCurNumOfFreeNodes ];
    GRAPH_NODE *n = &mNodeArr_Pool[ id ];
    mActiveNodeArr[ mCurNumOfActiveNodes ] = id;
    n->dynamicID = mCurNumOfActiveNodes;
    ++mCurNumOfActiveNodes;
    return n;
}

GRAPH_EDGE *GRAPH_SYSTEM::getFreeEdge( )
{
    if ( mCurNumOfFreeEdges == 0 ) return 0;
    --mCurNumOfFreeEdges;
    int id = mFreeEdgeArr[ mCurNumOfFreeEdges ];
    GRAPH_EDGE *e = &mEdgeArr_Pool[ id ];
    mActiveEdgeArr[ mCurNumOfActiveEdges ] = id;
    e->dynamicID = mCurNumOfActiveEdges;

    ++mCurNumOfActiveEdges;
    return e;
}

void GRAPH_SYSTEM::createDefaultGraph( )
{
    cout << "here"<< endl;
    reset( );

    float offset_x = 90.;
    float offset_z = 65.;

    int n_0 = addNode(offset_x + 0.0, 0.0, offset_z + 0.0 );
    cout << "n_0:"<< n_0 << endl;
    int n_1 = addNode(offset_x + 10.0f, 0.0f, offset_z + 0.0f);
    int n_2 = addNode(offset_x, 0.0f, offset_z + 8.0f);

    addEdge(n_0, n_1);
    addEdge(n_1, n_2);
    //
    // ok
    //

    //addEdge( n_0, n_1 );
    //addEdge( n_1, n_2 );

}

void GRAPH_SYSTEM::createRandomGraph_DoubleCircles(int n)
{
    reset( );

    //n = 36;
    float dx = 5.0;
    float dz = 5.0;
    float r = 15; // radius
    float d = 10; // layer distance
    int R = r + d;
    float offset_x = 90.;
    float offset_z = 65.;
    const float PI = 3.14159265358979f;

    srand((unsigned)time(NULL));

    vector<int> innerRing(n, -1);
    vector<int> outerRing(n, -1);

    for (int i = 0; i < n; ++i) {
        float theta = 2.0f * PI * i / n;
        innerRing[i] = addNode(offset_x + r * cosf(theta), 0.0f, offset_z + r * sinf(theta));
        outerRing[i] = addNode(offset_x + R * cosf(theta), 0.0f, offset_z + R * sinf(theta));
    }

    for (int i = 0; i < n; ++i) {
        const vector3& A = mNodeArr_Pool[innerRing[i]].p;
        float ax = A.x - offset_x, az = A.z - offset_z;

        for (int tries = 0; tries < 100; ++tries) {
            int j = rand() % n;
            const vector3& B = mNodeArr_Pool[outerRing[j]].p;
            float bax = B.x - A.x;
            float baz = B.z - A.z;

            if (ax * bax + az * baz >= 0.0f) {
                addEdge(innerRing[i], outerRing[j]);
                break;
            }
        }
    }
    //
    // ok, by the reply in QA, the example of n=5 in ppt is not correct(intersect the inner circle).
    //
}

void GRAPH_SYSTEM::createNet_Circular( int n, int num_layers )
{
    reset( );

    float dx = 5.0;
    float dz = 5.0;
    float r = 5; // radius
    float d = 5; // layer distance 
    float offset_x = 90.;
    float offset_z = 60.;
    const float PI = 3.14159265358979f;

    vector<vector<int>> node_idx;

    for (int l = 0;l < num_layers;l++) {
        node_idx.push_back({});
        for (int i = 0;i < n;i++) {
            float theta = 2.0f * PI * i / n;
            float _x = offset_x + (r + l * d) * cosf(theta);
            float _z = offset_z + (r + l * d) * sinf(theta);
            node_idx[l].push_back(addNode(_x, 0.0, _z));
        }
    }

    for (int l = 0;l < num_layers-1;l++) {
        for (int i = 0;i < n;i++) {
            int a = node_idx[l][i];
            int b = node_idx[l][(i + 1) % n];
            addEdge(a, b);
        }
    }

    for (int l = 0;l < num_layers-1;l++) {
        for (int i = 0;i < n;i++) {
            int a = node_idx[l][i];
            int b = node_idx[l+1][i];
            addEdge(a, b);
        }
    }
    //
    // ok
    //
}
void GRAPH_SYSTEM::createNet_Square( int n, int num_layers )
{
    reset( );

    float dx = 5.0;
    float dz = 5.0;
    float r = 5; // radius
    float d = 5; // layer distance 
    float offset_x = 20.;
    float offset_z = 20.;
    vector<int> last_nodes(n + num_layers*2 - 2);
    for (int i = 0; i < n + num_layers*2 - 2;i++) {
        int n_l;
        for (int j = 0; j < n + num_layers*2 - 2;j++) {
            if ((i >= num_layers && i < n + num_layers - 2) && (j >= num_layers && j < n + num_layers - 2)) {
                n_l = -1;
                last_nodes[j] = -1;
                continue;
            }
            float _x = offset_x + i * dx;
            float _z = offset_z + j * dz;
            int _n = addNode(_x, 0.0f, _z);
            if (j != 0 && n_l != -1) addEdge(n_l,_n);
            if (i != 0 && last_nodes[j] != -1) addEdge(last_nodes[j], _n);
            n_l = _n;
            last_nodes[j] = n_l;
        }
    }
    //
    // ok
    //

}
void GRAPH_SYSTEM::createNet_RadicalCircular( int n ) {

    reset( );

    float offset_x = 90.0;
    float offset_z = 65.0;
    const float PI = 3.14159265358979f;
    float r = 15; // radius

    int n_c = addNode(offset_x, 0.0, offset_z);

    for (int i = 0;i < n;i++) {
        float _x = offset_x + r * cosf(2.0f * PI * i / n);
        float _z = offset_z + r * sinf(2.0f * PI * i / n);
        int _n = addNode(_x, 0.0f, _z);
        addEdge(n_c, _n);
    }
    //
    // ok
    //


}

//
// return the node id
// return -1: no free node
//
int GRAPH_SYSTEM::addNode( float x, float y, float z, float r )
{
    GRAPH_NODE *g;
    g = getFreeNode( );
    if (g == nullptr) return -1;
    g->p = vector3(x, y, z);
    g->r = r;
    g->edgeID.clear();
    g->visited = false;
    g->depth = 0;
    g->path_cost = 0.0;
    g->path_parent = nullptr;
    //
    // ok
    //
    return g->id;
}

//
// return the edge id
// return -1: no free edge
//
int GRAPH_SYSTEM::addEdge( int nodeID_0, int nodeID_1 )
{
    if (nodeID_0 == nodeID_1) return -1;

    GRAPH_EDGE *e;
    e = getFreeEdge( );
    if (e == nullptr) return -1;

    e->nodeID[0] = nodeID_0;
    e->nodeID[1] = nodeID_1;
    mNodeArr_Pool[nodeID_0].edgeID.push_back(e->id);
    mNodeArr_Pool[nodeID_1].edgeID.push_back(e->id);
    //
    // ok
    //

    return e->id;
}

void GRAPH_SYSTEM::askForInput( )
{
    cout << "GRAPH_SYSTEM" << endl;
    cout << "Key usage:" << endl;
    cout << "1: create the default graph" << endl;
    cout << "2: create a graph of connected circles (12 nodes per layer, 6 layers)" << endl;
    cout << "3: create a graph of squares (n = 15, 5 layers, hollow center)" << endl;
    cout << "4: create a radial graph (1 center node + 24 outer nodes)" << endl;
    cout << "5: create a random graph of double circles (24 nodes per circle)" << endl;
    cout << "<: decrease the number of nodes per circle for the double-circles graph (min 3)" << endl;
    cout << ">: increase the number of nodes per circle for the double-circles graph (max 36)" << endl;
    cout << "Delete: delete the selected node and all the edges incident to it" << endl;
    cout << "d / D : toggle automatic deletion of nodes (250 ms per node)" << endl;
    cout << "Spacebar: change operation (SHORTEST_PATH -> NODE_DEPTH -> ADD_EDGE)" << endl;
    cout << " " << endl;
    cout << "Use the mouse to select nodes." << endl;
    cout << "Click the left button to select/unselect a node, or to pick two nodes for the current operation." << endl;
    cout << " " << endl;
    cout << "A selected node is highlighted as red." << endl;
}


GRAPH_NODE *GRAPH_SYSTEM::findNearestNode( double x, double z, double &cur_distance2 ) const
{
    GRAPH_NODE * nearest = nullptr;
    cur_distance2 = SYS_CONSTANTS::max_double;
    for (int i = 0; i < mCurNumOfActiveNodes; ++i) {
        int id = mActiveNodeArr[i];
        GRAPH_NODE* n = &mNodeArr_Pool[id];

        double dx = n->p.x - x;
        double dz = n->p.z - z;
        double d2 = dx * dx + dz * dz;

        if (d2 < cur_distance2) {
            cur_distance2 = d2;
            nearest = n;
        }
    }
    //
    // ok
    //
    return nearest;
}

void GRAPH_SYSTEM::moveTo(double x, double y )
{
    if (!mSelectedNode) return;
    mSelectedNode->p.x = x;
    mSelectedNode->p.z = y;
}

//
// handle the mouse click event at position (x, z).
//
void GRAPH_SYSTEM::clickAt(double x, double z)
{
    double cur_d2;
    GRAPH_NODE *nearest_node = findNearestNode( x, z, cur_d2 );
    if (nearest_node == 0 ) {
        mSelectedNode = 0;
        return;
    }
    if ( cur_d2 > nearest_node->r* nearest_node->r ) {
        mSelectedNode = 0;
        return;

    }
    if ( mSelectedNode == nearest_node) {
        mSelectedNode = 0;
        return;
    }

    performOperation(nearest_node);

}

void GRAPH_SYSTEM::performOperation(GRAPH_NODE* node) {
    switch (mOperation) {
    case GRAPH_OPERATION::GRAPH_OPERATION_NODE_DEPTH:
        mSelectedNode = node;
        mFlgShowNodeDepth = true;
        computeDepthOfAllNodesFromSelectedNode();
        break;
    case GRAPH_OPERATION::GRAPH_OPERATION_ADD_EDGE:
        if (mSelectedNode != 0) {
            addEdge(mSelectedNode->id, node->id);
            mSelectedNode = 0;
            return;
        }
        mSelectedNode = node;
        break;
    case GRAPH_OPERATION::GRAPH_OPERATION_SHORTEST_PATH:
        if (mSelectedNode != 0) {
            mStartNode = mSelectedNode;
            mDestinationNode = node;
            computeShortestPath();
            mSelectedNode = 0;
            return;
        }
        mSelectedNode = node;
        break;
    }
}

void GRAPH_SYSTEM::deleteEdge( int edgeID )
{
    GRAPH_EDGE *e = &mEdgeArr_Pool[ edgeID ];
    int dynamicID = e->dynamicID;
    int lastEdgeID = mActiveEdgeArr[mCurNumOfActiveEdges-1];
    mActiveEdgeArr[dynamicID] = lastEdgeID;
    mEdgeArr_Pool[lastEdgeID].dynamicID = dynamicID;
    --mCurNumOfActiveEdges;
    mFreeEdgeArr[mCurNumOfFreeEdges] = edgeID;
    ++mCurNumOfFreeEdges;
    //
    // ok
    //

}

void GRAPH_SYSTEM::removeEdgeFromNode( const GRAPH_EDGE *e, int nodeID )
{
    GRAPH_NODE *n = &mNodeArr_Pool[ nodeID ];
    for (int i = 0; i < (int)n->edgeID.size(); ++i) {
        if (n->edgeID[i] == e->id) {
            n->edgeID[i] = n->edgeID.back();
            n->edgeID.pop_back();
            return;
        }
    }
    //
    // ok
    //

}
void GRAPH_SYSTEM::deleteEdgesOfNode( int nodeID )
{
    GRAPH_NODE *n  = &mNodeArr_Pool[ nodeID ];
    for (int i = 0; i < (int)n->edgeID.size(); ++i) {
        int eID = n->edgeID[i];
        GRAPH_EDGE* e = &mEdgeArr_Pool[eID];
        int otherNID = (nodeID == e->nodeID[0]) ? e->nodeID[1] : e->nodeID[0];
        removeEdgeFromNode(e, otherNID);
        deleteEdge(eID);

    }
    n->edgeID.clear();
    //
    // ok
    //

}

void GRAPH_SYSTEM::deleteNode( int nodeID ) {
    if ( mCurNumOfActiveNodes <= 0 ) return;

    deleteEdgesOfNode(nodeID);

    GRAPH_NODE *n = &mNodeArr_Pool[ nodeID ];
    if (n == nullptr) return;

    int dynID = n->dynamicID;
    int lastIdx = mCurNumOfActiveNodes - 1;
    int lastNodeID = mActiveNodeArr[lastIdx];

    mActiveNodeArr[dynID] = lastNodeID;
    mNodeArr_Pool[lastNodeID].dynamicID = dynID;
    --mCurNumOfActiveNodes;

    
    mFreeNodeArr[mCurNumOfFreeNodes] = nodeID;
    ++mCurNumOfFreeNodes;
    //
    // ok
    //

}

void GRAPH_SYSTEM::deleteSelectedNode(  ) {
    if ( mSelectedNode == 0 ) return;

    int selectID = mSelectedNode->id;

    mSelectedNode = 0;
    mPassiveSelectedNode = 0;
    if (mStartNode && mStartNode->id == selectID) mStartNode = nullptr;
    if (mDestinationNode && mDestinationNode->id == selectID) mDestinationNode = nullptr;

    deleteNode(selectID);
    //
    // ok
    //
}

bool GRAPH_SYSTEM::isSelectedNode( ) const
{
    return mSelectedNode != 0;
}

void GRAPH_SYSTEM::getInfoOfSelectedPoint( double &r, vector3 &p ) const
{
    if ( isSelectedNode( ) == false ) return;
    r = mSelectedNode->r;
    p = mSelectedNode->p;

}

void GRAPH_SYSTEM::changeOperation()
{
    switch (mOperation) {
    case GRAPH_OPERATION::GRAPH_OPERATION_NODE_DEPTH:
        mOperation = GRAPH_OPERATION::GRAPH_OPERATION_ADD_EDGE;
        mOperationName = "ADD_EDGE";
        break;
    case GRAPH_OPERATION::GRAPH_OPERATION_ADD_EDGE:
        mOperation = GRAPH_OPERATION::GRAPH_OPERATION_SHORTEST_PATH;
        mOperationName = "SHORTEST_PATH";
        break;
    case GRAPH_OPERATION::GRAPH_OPERATION_SHORTEST_PATH:
        mOperation = GRAPH_OPERATION::GRAPH_OPERATION_NODE_DEPTH;
        mOperationName = "NODE_DEPTH";

        break;
    }
}

void GRAPH_SYSTEM::resetDepthOfAllNodes()
{
    mMaxNodeDepth = 0;
    //
    // ok
    // 

    int numNodes = getNumOfNodes();
    for (int i = 0; i < numNodes; ++i) {
        //int nodeID = mActiveNodeArr[i];
        //GRAPH_NODE* n = &mNodeArr_Pool[nodeID];
        //
        // ok
        // 
        // set node's depth
        // and others if necessary
        //
        int nodeID = mActiveNodeArr[i];
        GRAPH_NODE* n = &mNodeArr_Pool[nodeID];
        n->depth = SYS_CONSTANTS::max_int;
        n->visited = false;
    }
}



/*
* k := computeDepthOfAllNodesFromSelectedNode

* void k( Node *n, int depth ) {
	if n is null, return
	set n�s depth to depth
	for each adjacent node m of n				; note m should not be n
		if (m�s depth < depth + 1) k(m, depth+1)
}

void computeDepthOfAllNodesFromSelectedNode( ) {
	for each node n, set its depth to FLT_MAX
	k(selected_node, 0)
	update mMaxNodeDepth
}
*/

void GRAPH_SYSTEM::computeDepthOfAllNodesFromSelectedNode(GRAPH_NODE* node, int depth)
{
    if (node == 0) return;
    node->depth = depth;
    node->visited = true;
    for (int i = 0; i < (int)node->edgeID.size(); ++i) {
        int edgeID = node->edgeID[i];
        GRAPH_EDGE* e = &mEdgeArr_Pool[edgeID];
        GRAPH_NODE* n0 = &mNodeArr_Pool[e->nodeID[0]];
        GRAPH_NODE* n1 = &mNodeArr_Pool[e->nodeID[1]];

        
        GRAPH_NODE* next = (n0 == node) ? n1 : n0;

        
        if (next->depth > depth + 1) {
            computeDepthOfAllNodesFromSelectedNode(next, depth + 1);
        }
    }
    //
    // ok
    //
    // for all edges incident to the node: node
        // get edge ID
        // get e = &mEdgeArr_Pool[edgeID];
        // n0 = &mNodeArr_Pool[e->nodeID[0]];
        // n1 = &mNodeArr_Pool[e->nodeID[1]];
        // determine the next node, which is set as the current node
        // update the current node's depth
        // recursively update all the nodes connected to the current node
        //     
    //for (int i = 0; i < node->edgeID.size(); ++i) {
        //
        // ok
        // 
        
    //}
}

void GRAPH_SYSTEM::computeDepthOfAllNodesFromSelectedNode()
{
    // reset the depth of all nodes
    // if mSelectedNode is nullpute, return
    // set  mMaxNodeDepth = 0;
    // set mSelectedNode->depth as zero
    // invoke computeDepthOfAllNodesFromSelectedNode
    // finally, determine the mMaxNodeDepth
    // 
    resetDepthOfAllNodes();

    if (mSelectedNode == nullptr) return;

    mMaxNodeDepth = 0;
    mSelectedNode->depth = 0;
    computeDepthOfAllNodesFromSelectedNode(mSelectedNode, 0);
    int numNodes = getNumOfNodes();
    for (int i = 0; i < numNodes; ++i) {
        int nodeID = mActiveNodeArr[i];
        GRAPH_NODE* n = &mNodeArr_Pool[nodeID];

        if (n->depth == SYS_CONSTANTS::max_int) continue;   // skip unreachable

        if ((float)n->depth > mMaxNodeDepth) {
            mMaxNodeDepth = (float)n->depth;
        }
    }


    //
    // ok
    //

}

// This member function is not used.
float GRAPH_SYSTEM::getNodeDepthFromSelectedNode(int nodeIndex) const
{
    float d = 0.0;
    //
    //skip
    //
    return d;
}

void GRAPH_SYSTEM::resetPathInformationOfAllNodes()
{
    //
    // ok
    // 

    int numNodes = getNumOfNodes();
    for (int i = 0; i < numNodes; ++i) {
        int nodeID = mActiveNodeArr[i];
        GRAPH_NODE* n = &mNodeArr_Pool[nodeID];
        n->path_cost = SYS_CONSTANTS::max_double;
        n->path_parent = nullptr;
        //
        // ok
        //
        // set path cost of node
        // set path_parent of node
    }
}

void GRAPH_SYSTEM::computeShortestPath(GRAPH_NODE *node)
{
    if (node == 0) return; 

    // for each edge incident to node, node 
    // DO
    for (int i = 0; i < node->edgeID.size(); ++i) {
        //
        // ok
        //
        int edgeID = node->edgeID[i];
        GRAPH_EDGE* e = &mEdgeArr_Pool[edgeID];
        GRAPH_NODE* n0 = &mNodeArr_Pool[e->nodeID[0]];
        GRAPH_NODE* n1 = &mNodeArr_Pool[e->nodeID[1]];
        GRAPH_NODE* next = (n0 == node) ? n1 : n0;

        double d = node->p.distance(next->p);
        double newCost = node->path_cost + d;

        if (newCost < next->path_cost) {
            next->path_cost = newCost;
            next->path_parent = node;
            computeShortestPath(next);
        }
        
        // get an edge
        // determine the next node. Set it as the current node.
        // compute distance d: node->p.distance(next->p);
        // if new path cost is not better, check for the other edges
        // if new path cost is better, update the node's path cost and path_parent
        // Also, invokte computeShortestPath for the current node.
    }
}

void GRAPH_SYSTEM::computeShortestPath()
{
    //
    // ok
    //
    // reset path information of all nodes
    // if mStartNode == nullptr || mDestinationNode == nullptr, return
    // invokte computeShortestPath with mStartNode
    //
    resetPathInformationOfAllNodes();
    if (mStartNode == nullptr || mDestinationNode == nullptr) return;

    mStartNode->path_cost = 0.0;
    mStartNode->path_parent = nullptr;
    computeShortestPath(mStartNode);
}

void GRAPH_SYSTEM::handleKeyPressedEvent( unsigned char key )
{
    
    switch( key ) {
    case 127: // delete
        mFlgAutoNodeDeletion = false;
        deleteSelectedNode( );
        break;
    case '1':
        mFlgAutoNodeDeletion = false;
        createDefaultGraph( );
        mSelectedNode = 0;
        break;
    case '2':
        mFlgAutoNodeDeletion = false;
        createNet_Circular(12, 6);
        mSelectedNode = 0;

        break;
    case '3':
        mFlgAutoNodeDeletion = false;
        createNet_Square(15, 5);
        mSelectedNode = 0;

        break;
    case '4':
        mFlgAutoNodeDeletion = false;
        createNet_RadicalCircular(24);
        mSelectedNode = 0;

        break;
    case '5':
        mNumPoints_DoubleCircles = 24;
        mFlgAutoNodeDeletion = false;
        createRandomGraph_DoubleCircles(mNumPoints_DoubleCircles);
        mSelectedNode = 0;
        break;
    case '<':
        mNumPoints_DoubleCircles--;
        if (mNumPoints_DoubleCircles<3) 
            mNumPoints_DoubleCircles = 3;
        mFlgAutoNodeDeletion = false;
        createRandomGraph_DoubleCircles(mNumPoints_DoubleCircles);
        mSelectedNode = 0;
        break;
    case '>':
        mNumPoints_DoubleCircles++;
        if (mNumPoints_DoubleCircles> 36) 
            mNumPoints_DoubleCircles = 36;
        mFlgAutoNodeDeletion = false;
        createRandomGraph_DoubleCircles(mNumPoints_DoubleCircles);
        mSelectedNode = 0;
        break;

    case 'r':
        mFlgAutoNodeDeletion = false;
        mSelectedNode = 0;
        break;
    case ' ':
        changeOperation();
        break;
    case 'd':
    case 'D':
        mFlgAutoNodeDeletion = !mFlgAutoNodeDeletion;
        break;
    }
}

void GRAPH_SYSTEM::handlePassiveMouseEvent( double x, double z )
{
    double cur_d2;
    GRAPH_NODE *n = findNearestNode( x, z, cur_d2 );
    if ( n == 0 ) return;
    if ( cur_d2 > n->r*n->r ) {
        mPassiveSelectedNode = 0;
        return;
    }
    mPassiveSelectedNode = n;
}

int GRAPH_SYSTEM::getNumOfNodes( ) const
{
    return mCurNumOfActiveNodes;
}

void GRAPH_SYSTEM::getNodeInfo( int nodeIndex, double &r, vector3 &p ) const
{
    int nodeID = mActiveNodeArr[ nodeIndex ];
    GRAPH_NODE *n = &mNodeArr_Pool[ nodeID ];
    r = n->r;
    p = n->p;
}

void GRAPH_SYSTEM::getNodeInfo(int nodeIndex, double& r, vector3& p, float& depth) const
{
    int nodeID = mActiveNodeArr[nodeIndex];
    GRAPH_NODE* n = &mNodeArr_Pool[nodeID];
    r = n->r;
    p = n->p;
    depth = n->depth;
}

int GRAPH_SYSTEM::getNumOfEdges( ) const
{
    return mCurNumOfActiveEdges;
}

vector3 GRAPH_SYSTEM::getNodePositionOfEdge( int edgeIndex, int nodeIndex ) const
{
    int edgeID = mActiveEdgeArr[ edgeIndex ];
    GRAPH_EDGE *e = &mEdgeArr_Pool[ edgeID ];
    return mNodeArr_Pool[ e->nodeID[ nodeIndex ] ].p;
}

void GRAPH_SYSTEM::stopAutoNodeDeletion()
{
    mFlgAutoNodeDeletion = false;
}

//
// For every frame, update( ) function is called.
//
// 
void GRAPH_SYSTEM::update( )
{
    if (!mFlgAutoNodeDeletion) {
     
        return;
    }
    if (mCurNumOfActiveNodes<=0) {
     mFlgAutoNodeDeletion = false;
        return;
    }
    Sleep(250);

    int nodeID = mActiveNodeArr[0];

    if (mStartNode && mStartNode->id == nodeID) mStartNode = nullptr;
    if (mDestinationNode && mDestinationNode->id == nodeID) mDestinationNode = nullptr;

    deleteNode(nodeID);

    //
    // ok
    // 
    // delete the selected node?
    // delete all the edges incident to the selected node?
    //

    mSelectedNode = 0;
    mPassiveSelectedNode = 0;
}