#include <unordered_map>
#include <vector>
#include <utility>
#include <assert.h>

#include "network.h"
#include "geometry.h"
#include "graph.h"


int main(int argc, char** argv)
{
    if (argc != 3)
    {
        std::cout << "Error: requires initial and final supercharger names" << std::endl;        
        return -1;
    }
    
    std::string initial_charger_name = argv[1];
    std::string goal_charger_name = argv[2];

    // =================Abbas' Code Starts Here==================


    //##################SOLUTION PARAMETERS#######################
    const bool DISCRETE = true; // flag to use discretized solution (true) or distance based feasible solution (false)
    int num_charge_levels = 20; // changes the granularity of discrete solution. No effect on Fast Feasible Solution.


    const bool ASTAR = true; // flag to use A* (true) vs Dikjstra (false)
    //############################################################


    //-----------------CHARGE LEVEL DISCRETIZATION---------------
    //set discretization level and create charge levels
    std::vector<float> charge_levels;

    if(DISCRETE){
        assert(num_charge_levels>1); // Single Charge level doesn't make sense and divide by 0 error
        charge_levels.reserve(num_charge_levels);

        for(int i = 0; i<num_charge_levels; i++){
            charge_levels.push_back( i*(1.0/(num_charge_levels-1)) );
        }
    }

    //-----------------CONSTRUCT GRAPH---------------

    //Make Vertices
    Graph graph;

    if (DISCRETE){
        for(float charge_level: charge_levels){
            for(auto node : network){
                graph.addVertex(node, charge_level);
            }
        }
    }
    else{
        for(auto node : network){
            graph.addVertex(node);
        }
    }

    int edge_cnt = 0;
    std::vector<GraphNode*> vertex_list = graph.getVertexList();
    for(auto node1: vertex_list){
        for(auto node2: vertex_list){

            bool add_edge;
            if(DISCRETE){
                float charge_post_traverse = (node1->getChargeLevel())*MAX_CAPACITY - getGCDist(node1, node2);

                bool fuel_remaining = (charge_post_traverse >= 0);
                bool feasible_charge_level = (charge_post_traverse <= (node2->getChargeLevel())*MAX_CAPACITY);
                bool different_locations = node1->getLocation()!=node2->getLocation();

                add_edge = fuel_remaining and feasible_charge_level and different_locations;
            }
            else{
                add_edge = ( getGCDist(node1,node2)<MAX_CAPACITY );
            }

            if(add_edge){
                graph.addEdge(node1, node2);
                edge_cnt +=1;
            }
        }
    }

    //-----------------Define Goal---------------

    if(DISCRETE){
        //for optimal case
        initial_charger_name += std::to_string(charge_levels[charge_levels.size()-1]);
        goal_charger_name += std::to_string(charge_levels[1]);
    }

    //input validation
    assert(inUMap(graph.getVertices(), initial_charger_name));
    assert(inUMap(graph.getVertices(), goal_charger_name));

    //set start and end vertices
    graph.setStart(initial_charger_name);
    graph.setEnd(goal_charger_name);

    // //verify graph
    // graph.printVertices();
    // std::cout<<"Edge Count: "<<edge_cnt<<std::endl;


    //-----------------Search Graph for Optimal Path---------------
    std::stack<std::string> sol_stack;

    if(DISCRETE){
        GraphNode *initial_node, *goal_node;
        initial_node = graph.getVertices()[initial_charger_name];
        goal_node = graph.getVertices()[goal_charger_name];

        if(getGCDist(initial_node, goal_node)<MAX_CAPACITY){ //simplified solution if the goal charger is within 320km of initial charger
            std::cout<<initial_node->getLocation()<<", "<<goal_node->getLocation()<<std::endl;
        }
        else{
            sol_stack = graph.dijkstraTime(ASTAR); // run A* or Dijkstra (based on flag above)
            graph.parseDiscreteSolution(sol_stack); // Parse solution and print output string
        }

        
    }
    else{
        sol_stack = graph.dijkstra(ASTAR);
        graph.parseGreedySolution(sol_stack);
    }

    // //verify sol_stack
    // printStrStack(sol_stack);
     
    return 0;
}



