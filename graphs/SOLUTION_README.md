## Autopilot Candidate: Abbas Shikari

# Compile String

`g++ -std=c++11 -O1 main.cpp geometry.cpp graph.cpp network.cpp -o candidate_solution`


# Notes on Solution

The problem can be handled as a constrained shortest path problem. I have provided solutions:
* **Fast Feasible Solution:** Simple A* implementation on a distance graph of nodes followed by a greedy post processing that gives a feasible but not necessarily optimal result very quickly.
* **Psuedo-Optimal Solution:** Discretize the allowed charge levels of the vehicle by a parameter, `num_charge_levels`, run A* while costing on time to get a solution that is optimal to the granularity of the discretization. In the limit of infinite `num_charge_levels`, this solution converges to the continuous space optimal solution at the cost of higher runtime. Pre-compiled solution variants are appropriately titled and attached in submission.

Both solutions are accessible from the given code. Simply change the `DISCRETE` boolean in the solution parameters section to switch between Fast Feasible, and Psuedo-Optimal solutions. `num_charge_levels` can also be changed to vary the 'optimality' and runtime of the Psuedo-Optimal solution. Lastly, the `ASTAR` flag in solution paramters switches between A* and Dijkstra's algorithm as the graph solver.

***All custom data structures and algorithms were written from scratch for this problem***


# Algorithm Deep Dive:

**Fast Feasible Solution:**
This approach provides a fast feasible, yet non-optimal solution based on a greedy charging heuristic. The algorithm can be summarized as follows:
* A graph is constructed where the vertices are the superchargers. Edges are established between vertices iff the great circle distance between them is less than the max charge distance (320km).
* A* / Dijkstra's Algorithm is used to find the shortest distance path (same as shortest time path if we don't consider charging time) amongst superchargers
* The ordered list of vertices that result from A* / Dijkstra's search are then traversed by a virtual car that recharges iff the distance to the next node is greater than the current battery charge level. All recharging is done so the virtual car can arrive at the next vertex with 0 battery charge remaining.


**Psuedo-Optimal Solution:**
This approach gives an approximately optimal (with discretization error) solution by adding vertices in the graph to such that each supercharger has numerous nodes corresponding to different departure battery charge levels:
* Select some discretization parameter, `num_charge_levels`, by which to discretize the charge level of the car. For example if `num_charge_levels`=11 then there will be 11 allowable charge levels for the car (0%, 10% ... 90%, 100%)
* for each of the charge levels, define a set of vertices, each set corresponding to a particular charge level and each vertex within a set corresponding to a unique geographic supercharger.
* Define the edges for this graph such that the following criteria are met:
** the great circle distance between vertices is less than the max battery capacity (same as original criteria) AND
** the charge level drop between vertices corresponds to the actual battery capacity usage. For example, if two vertices are 10 km apart, then the energy levels of the vertices that connect the two chargers must be at least 10 km apart
* Cost the edges based on TIME (edges that connect vertices in the same location but different charge level have time cost equal to the [difference_in_charge_level/charge_rate])
* perform A* with a cost function in time. The heuristic cost of is [great_circle_distance_to_target_charger/speed].


This proposed solution gives an approximation of the optimal solution that gets better and better with increasing values of `num_charge_levels`. However, as `num_charge_levels` increases, the compute cost also rises. This proposed solution is presented by this paper: (https://pdfs.semanticscholar.org/dd62/0e8e55dc27ec660b763a4d3a21af5940b66f.pdf). 


# Room for Improvement:

* There is some sort of bug I believe. Specifically in the test case **Atlanta_GA Newburgh_NY**, the results from the Psuedo-optimal result give higher cost in the checker than the Fast Feasible solution. I was unable to track down the cause of this issue, I suspect large influence of discretization error but that is only a guess. In other cases the discrete solution is better then the fast feasible solution as in **Council_Bluffs_IA Cadillac_MI** 
* In the current implementation a lot of data is copied from the given dataset, wasting both time and memory. If time had allowed, I would have populated the GraphNode's data fields by simply storing a pointer to the associated row in the given dataset.
* The graph could probably be preprocessed with some sort of clever pruning to decrease search runtime i nthe Psuedo-Optimal Solution
* There is a way to relax the discretization assumption after nodes are selected using non-linear optimization. This approach could be employed to reduce high optimality runtime.
* Could probably have done more unit testing and edge case evaluation