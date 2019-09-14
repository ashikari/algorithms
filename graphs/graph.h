#pragma once

#include <unordered_map>
#include <utility>
#include <set>
#include <string>
#include <vector>
#include <stack>
#include <algorithm>
#include <unordered_set>

#include "network.h"

const int MAX_CAPACITY = 320; //km
const float SPEED = 105; //km/hr


class GraphNode{
private:
    std::string name_, location_;
    std::vector<std::string> neighbor_names_;
    float charge_rate_, charge_level_;
    double lat_, lon_;
public:
    GraphNode(const std::string& name, float charge_rate, double lat, double lon);
    GraphNode(const std::string& name, const std::string& location, float charge_rate, double lat, double lon, float charge_level);

    std::vector<std::string> getNeighbors() const;
    std::string getName() const;
    std::string getLocation() const;
    float getChargeRate() const;
    float getLat() const;
    float getLon() const;
    float getChargeLevel() const;

    void addNeighbor(std::string new_neighbor_name);
};


class Graph{
private:
    static const int MAX_NUM_NODES = 303*100; // hack to be fixed should take discretization parameter
    int num_vertices_ = 0;
    std::unordered_map<std::string, GraphNode*> vertices_;
    std::vector<GraphNode*> vertex_list;
    std::unordered_map< std::string, float> edge_dists_;
    std::string start_, end_;
public:
    Graph();

    void setStart(GraphNode* start);
    void setEnd(GraphNode* end);
    void setStart(std::string start);
    void setEnd(std::string end);
    int getNumVertices() const;
    std::unordered_map<std::string, GraphNode*> getVertices() const;
    std::vector<GraphNode*> getVertexList() const;

    void addVertex(const row& city);
    void addVertex(const row& city, const float charge_level);
    void addEdge(const row& city1, const row& city2);
    void addEdge(const GraphNode*  const node1, const GraphNode* const node2);
    float getEdgeDist( std::string  name1, std::string name2);

    void printVertices();
    float getDijkstraCost( GraphNode* current, GraphNode* previous, const float prev_cost);
    float getDijkstraTimeCost(GraphNode* current, GraphNode* previous, const float prev_cost);
    float getAStarCost(GraphNode* current, GraphNode* previous, const float prev_cost);
    float getAStarTimeCost(GraphNode* current, GraphNode* previous, const float prev_cost);

    std::stack<std::string> dijkstra(bool a_star);
    std::stack<std::string> dijkstraTime(bool a_star);

    std::string parseGreedySolution(std::stack<std::string> solution);
    std::string parseDiscreteSolution(std::stack<std::string> solution);
};



//===============HELPERS======================

void printStrStack(std::stack<std::string> str_stack);

struct Node{
    Node();
    Node(GraphNode* curr, GraphNode* prev, float cost_to_go);
    GraphNode *curr_node, *prev_node;
    double cost;
    bool operator==(const Node& rhs) const { return this->curr_node == rhs.curr_node;}
};


struct CompareCost{ 
    bool operator()(Node* const& n1, Node* const& n2) 
    { 
        return n1->cost > n2->cost; 
    } 
}; 


template <class T>
bool inVector(std::vector<T> vec, T val){
    return !(std::find(vec.begin(), vec.end(), val) == vec.end());
}

template <class T>
bool inUSet(const std::unordered_set<T>& set, const T& val){
    return !(set.find(val) == set.end());
}

template <class T1, class T2>
bool inUMap(const std::unordered_map<T1, T2>& map, const T1& val){
    return !(map.find(val) == map.end());
}