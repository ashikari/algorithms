#include <utility>
#include <vector>
#include <queue>
#include <stack>
#include <assert.h>
#include <algorithm>
#include <unordered_set>

#include "graph.h"
#include "geometry.h"

//Graph Node Implementation
GraphNode::GraphNode(const std::string& name, float charge_rate, double lat, double lon): 
name_(name),
location_(name),
charge_rate_(charge_rate),
lat_(lat),
lon_(lon)
{}


GraphNode::GraphNode(const std::string& name, const std::string& location, float charge_rate, double lat, double lon, float charge_level): 
name_(name),
location_(location),
charge_rate_(charge_rate),
lat_(lat),
lon_(lon),
charge_level_(charge_level)
{}


std::string GraphNode::getName() const {return name_; }
std::string GraphNode::getLocation() const {return location_;}

float GraphNode::getChargeRate() const {return charge_rate_;};
float GraphNode::getLat() const {return lat_;}
float GraphNode::getLon() const {return lon_;}
float GraphNode::getChargeLevel() const {return charge_level_;}


std::vector<std::string> GraphNode::getNeighbors() const {return neighbor_names_;}

void GraphNode::addNeighbor(std::string new_neighbor_name){
    neighbor_names_.push_back(new_neighbor_name);
}


//Graph Implementation
Graph::Graph(): start_(""), end_(""){};

void Graph::setStart(GraphNode* start){start_ = start->getName();}
void Graph::setEnd(GraphNode* end){end_ = end->getName();}
void Graph::setStart(std::string start){start_ = start;}
void Graph::setEnd(std::string end){end_ = end;}
int Graph::getNumVertices() const {return num_vertices_;}
std::unordered_map<std::string, GraphNode*> Graph::getVertices() const {return vertices_;}
std::vector<GraphNode*> Graph::getVertexList() const {return vertex_list;}


void Graph::addVertex(const row& city){
    GraphNode* node = new GraphNode(city.name, city.rate, city.lat, city.lon);
    vertices_[city.name] = node;
    vertex_list.push_back(node);
    num_vertices_ += 1;
}


void Graph::addVertex(const row& city, const float charge_level){
    std::string name = city.name + std::to_string(charge_level);
    GraphNode* node = new GraphNode(name, city.name, city.rate, city.lat, city.lon, charge_level);
    vertices_[name] = node;
    vertex_list.push_back(node);
    num_vertices_ +=1;
}


void Graph::addEdge(const row& city1, const row& city2){
    assert(inUMap(vertices_, city1.name) and inUMap(vertices_, city2.name));

    if (city1.name>city2.name){ // gives single key from two unique keys
        edge_dists_[city1.name + city2.name] = getGCDist(city1, city2);
    }
    else{
        edge_dists_[city2.name + city1.name] = getGCDist(city2, city1);
    }

    vertices_[city1.name]->addNeighbor(city2.name);
    vertices_[city2.name]->addNeighbor(city1.name);
}


void Graph::addEdge(const GraphNode*  const node1, const GraphNode* const node2){
    std::string name1, name2;
    name1 = node1->getName();
    name2 = node2->getName();
    assert(inUMap(vertices_, name1) and inUMap(vertices_, name2));

    if (name1>name2){ // gives single key from two unique keys
        edge_dists_[name1 + name2] = getGCDist(node1, node2);
    }
    else{
        edge_dists_[name2 + name1] = getGCDist(node2, node1);
    }

    // vertices_[name1]->addNeighbor(name2);
    // vertices_[name2]->addNeighbor(name1);

    if(node1->getLocation()!=node2->getLocation()){
        if (node1->getChargeLevel()>node2->getChargeLevel()){
            vertices_[name1]->addNeighbor(name2);
        }
        else{
            vertices_[name2]->addNeighbor(name1);
        }
    }
    else{
        if (node1->getChargeLevel()>node2->getChargeLevel()){
            vertices_[name2]->addNeighbor(name1);
        }
        else{
            vertices_[name1]->addNeighbor(name2);
        }
    }
    
}


float Graph::getEdgeDist( std::string name1, std::string name2) {
    assert(inUMap(vertices_, name1) and inUMap(vertices_, name2));

    float dist;

    if (name1>name2){
        assert(inUMap(edge_dists_, name1+name2));
        dist = edge_dists_[name1 + name2];
    }
    else{
        assert(inUMap(edge_dists_, name2 + name1));
        dist = edge_dists_[ name2 + name1];
    }

    return dist;
}


void Graph::printVertices(){
    assert(start_ != "");
    std::unordered_set<GraphNode*> visited;
    std::stack<GraphNode*> st;
    
    //Start at the start
    st.push(vertices_[start_]);
    visited.insert(vertices_[start_]);
    
    while(!st.empty()){
        //get the next node in the stack
        GraphNode* node = st.top();
        st.pop();
        
        //for each neighbor
        for(auto name: node->getNeighbors()){
            GraphNode* next_node = vertices_[name];
            //If we haven't been to this node
            if(!inUSet(visited, next_node)){
                st.push(next_node);
                visited.insert(next_node);
            }
        }
        std::cout<<node->getName()<<std::endl;
    }
}


std::stack<std::string> Graph::dijkstra(bool a_star){ //same as A* if we add cost heuristic as done below
    assert(start_ != "");
    assert(end_ != "");

    //make containers for priority q and completed q
    std::priority_queue<Node*, std::vector<Node*>, CompareCost> active_q;
    std::unordered_set<std::string> completed, inQ;
    float cost_to_go;

    std::unordered_map<std::string, Node*> q_lookup;

    //load active_q with start_node
    GraphNode* start_node = vertices_[start_];
    active_q.push( new Node(start_node, nullptr, 0));
    q_lookup[start_] = active_q.top(); 
    

    Node eval_node, temp_node;
    while(!active_q.empty()){
        eval_node = *active_q.top();
        if(eval_node.curr_node->getName()==end_){
            break;
        }

        completed.insert(eval_node.curr_node->getName());
        active_q.pop();

        std::vector<std::string> neighbors = eval_node.curr_node->getNeighbors();
        //run algorithm
        for(auto neighbor_name: neighbors){
            if(!inUSet(completed, neighbor_name)){
                GraphNode* neighbor = vertices_[neighbor_name];
                
                if (a_star){
                    cost_to_go = getAStarCost(neighbor, eval_node.curr_node, eval_node.cost);
                }
                else{
                    cost_to_go = getDijkstraCost(neighbor, eval_node.curr_node, eval_node.cost);
                }
                
                
                if(!inUSet(inQ, neighbor_name)){
                    Node* node_ptr = new Node(neighbor, eval_node.curr_node, cost_to_go);
                    active_q.push(node_ptr);
                    inQ.insert(neighbor->getName());
                    q_lookup[neighbor->getName()] = node_ptr;
                }
                else if(cost_to_go<q_lookup[neighbor->getName()]->cost){
                    q_lookup[neighbor->getName()]->cost = cost_to_go;
                    q_lookup[neighbor->getName()]->prev_node = eval_node.curr_node;
                }

            }
        }
    }
    
    //construct output
    std::stack<std::string> output;
    Node* dummy_node = q_lookup[end_];
    output.push(dummy_node->curr_node->getName());
    while(dummy_node->prev_node!=nullptr){
        output.push(dummy_node->prev_node->getName());
        dummy_node = q_lookup[dummy_node->prev_node->getName()];
    }

    return output;
}


std::stack<std::string> Graph::dijkstraTime(bool a_star){ //same as A* if we add cost heuristic as done below
    assert(start_ != "");
    assert(end_ != "");

    //make containers for priority q and completed q
    std::priority_queue<Node*, std::vector<Node*>, CompareCost> active_q;
    std::unordered_set<std::string> completed, inQ;
    float cost_to_go;

    std::unordered_map<std::string, Node*> q_lookup;

    //load active_q with start_node
    GraphNode* start_node = vertices_[start_];
    active_q.push( new Node(start_node, nullptr, 0));
    q_lookup[start_] = active_q.top(); 
    

    Node eval_node, temp_node;
    while(!active_q.empty()){
        eval_node = *active_q.top();
        if(eval_node.curr_node->getName()==end_){
            break;
        }

        completed.insert(eval_node.curr_node->getName());
        active_q.pop();

        std::vector<std::string> neighbors = eval_node.curr_node->getNeighbors();
        //run algorithm
        for(auto neighbor_name: neighbors){
            if(!inUSet(completed, neighbor_name)){
                GraphNode* neighbor = vertices_[neighbor_name];
                
                //run A* vs Dijkstra based on flag
                if (a_star){
                    cost_to_go = getAStarTimeCost(neighbor, eval_node.curr_node, eval_node.cost);
                }
                else{
                    cost_to_go = getDijkstraTimeCost(neighbor, eval_node.curr_node, eval_node.cost);
                }
                
                if(!inUSet(inQ, neighbor_name)){
                    Node* node_ptr = new Node(neighbor, eval_node.curr_node, cost_to_go);
                    active_q.push(node_ptr);
                    inQ.insert(neighbor->getName());
                    q_lookup[neighbor->getName()] = node_ptr;
                }
                else if(cost_to_go<q_lookup[neighbor->getName()]->cost){
                    q_lookup[neighbor->getName()]->cost = cost_to_go;
                    q_lookup[neighbor->getName()]->prev_node = eval_node.curr_node;
                }
            }
        }
    }
    
    //construct output
    std::stack<std::string> output;
    Node* dummy_node = q_lookup[end_];
    output.push(dummy_node->curr_node->getName());
    while(dummy_node->prev_node!=nullptr){
        output.push(dummy_node->prev_node->getName());
        dummy_node = q_lookup[dummy_node->prev_node->getName()];
    }

    return output;
}


float Graph::getDijkstraCost(GraphNode* current, GraphNode* previous, const float prev_cost) {
    return getEdgeDist(current->getName(), previous->getName()) + prev_cost;
}


float Graph::getDijkstraTimeCost(GraphNode* current, GraphNode* previous, const float prev_cost) {
    float distance = getEdgeDist(current->getName(), previous->getName());
    float driving_time = (1/SPEED)*distance;
    float charge_time = (1/current->getChargeRate()) * ( ( current->getChargeLevel() )*MAX_CAPACITY - (previous->getChargeLevel())*MAX_CAPACITY + distance );
    return driving_time + prev_cost + charge_time;
}


float Graph::getAStarTimeCost(GraphNode* current, GraphNode* previous, const float prev_cost) {
    float distance = getEdgeDist(current->getName(), previous->getName());
    float driving_time = (1/SPEED) * distance;
    float charge_time = (1/current->getChargeRate()) * ( ( current->getChargeLevel() )*MAX_CAPACITY - (previous->getChargeLevel())*MAX_CAPACITY + distance );
    float heuristic_time = (1/SPEED) * getGCDist(previous, vertices_[end_]);
    return driving_time + prev_cost + charge_time + heuristic_time;
}


float Graph::getAStarCost(GraphNode* current, GraphNode* previous, const float prev_cost) {
    return getEdgeDist(current->getName(), previous->getName()) + getGCDist(current, previous) + prev_cost;
}

//uses Greedy heuristic
std::string Graph::parseGreedySolution(std::stack<std::string> solution) {
    std::string sol_str, current, next;
    sol_str = "";
    current = solution.top();
    sol_str += current + ", ";
    solution.pop();
    next = solution.top();
    solution.pop();

    sol_str += next;
    float charge_time, distance;
    float battery_charge = MAX_CAPACITY - getEdgeDist(current, next);

    while(!solution.empty()){
        current = next;
        next = solution.top();
        solution.pop();
        
        distance = getEdgeDist(current, next);
        if( battery_charge - distance>0 ){// if there is enough charge
            battery_charge -= distance; //don't refuel
            sol_str+= ", 0.0, ";
        }
        else{
            charge_time = (distance - battery_charge)/(vertices_[current]->getChargeRate());
            battery_charge = 0;
            sol_str += ", " + std::to_string(charge_time) + ", ";
        }
        sol_str += next;

    }
    std::cout<<sol_str<<std::endl;
    return sol_str;
}


std::string Graph::parseDiscreteSolution(std::stack<std::string> solution){
    std::string sol_str, current, next, current_loc, next_loc;
    float current_charge_level, next_charge_level;

    sol_str = "";
    current = solution.top();
    current_loc = vertices_[current]->getLocation();
    current_charge_level = vertices_[current]->getChargeLevel();
    sol_str += current_loc + ", ";
    solution.pop();

    next = solution.top();
    next_loc = vertices_[next]->getLocation();
    next_charge_level = vertices_[next]->getChargeLevel();
    sol_str += next_loc;
    solution.pop();

    float charge_time, distance, battery_charge, driving_time;
    distance = getEdgeDist(current, next);
    battery_charge = MAX_CAPACITY - distance; //battery charge upon arriving at the next station

    while(!solution.empty()){
        current = next;
        current_charge_level = vertices_[current]->getChargeLevel();
        next = solution.top();
        next_charge_level = vertices_[next]->getChargeLevel();
        solution.pop();

        charge_time = ( (current_charge_level*MAX_CAPACITY) - battery_charge )/( vertices_[current]->getChargeRate() );
        sol_str += ", " + std::to_string(charge_time) + ", " + vertices_[next]->getLocation();

        distance = getEdgeDist(current, next);
        battery_charge = current_charge_level*MAX_CAPACITY - distance;
    }

    std::cout<<sol_str<<std::endl;
    return sol_str;
}

//====================HELPERS========================

void printStrStack(std::stack<std::string> str_stack){
    while(!str_stack.empty()){
        std::cout<<str_stack.top()<<std::endl;
        str_stack.pop();
    }
}


Node::Node(){}
Node::Node(GraphNode* curr, GraphNode* prev, float cost_to_go ): 
curr_node(curr), 
prev_node(prev), 
cost(cost_to_go){}