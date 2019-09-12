/*
Interval: (a,b) represents a<c<b, a<b

Input: 2 sets of intervals
Output: Intersection of the two sets

Example:
Input: ([3,6], [8, 10], [1, 4], [8,9]) and  ( [11, 13], [4, 9],[14, 15])

Output: ([4,6], [8, 9])
*/

#include <iostream>
#include <vector>
#include <utility>
#include <algorithm>


using namespace std;

//Preprocessing


// value of end point + bool for start v end + bool for A v B
//float data
//if this->set = true then A
// if this-> start = true then start
struct EndPoint{
    int data;
    bool set, start;
    
    bool operator< (const EndPoint& other){
        return data<other.data;
    }
    
    bool operator> (const EndPoint& other){
        return data>other.data;
    }
    
}; 



void print_endpoints(const vector<EndPoint>& ends) {
    for (auto end:ends){
        std::cout<<end.data<<std::endl;
    }
}

void print_intervals(const vector<pair<int,int>>& intervals){
    for (auto interval:intervals){
        std::cout<<"("<<interval.first<<", "<<interval.second<<")"<<std::endl;
    }
}

vector<EndPoint> mergeIntervalSet(const vector<pair<int, int>>& A, const vector<pair<int, int>>& B){
    vector<EndPoint> out;
    out.reserve(A.size()+B.size());
    EndPoint E;
    
    E.set = true;
    for(int i = 0; i<A.size(); i++){
        E.data = A[i].first;
        E.start = true;
        out.push_back(E);
        
        E.data = A[i].second;
        E.start =false;
        out.push_back(E);
    }
    
    E.set = false;
    for(int i = 0; i<B.size(); i++){
        E.data = B[i].first;
        E.start = true;
        out.push_back(E);
        
        E.data = B[i].second;
        E.start =false;
        out.push_back(E);
    }
    
    sort(out.begin(), out.end());
    
    return out;
}


vector<pair<int,int>> findIntersection(const vector<pair<int,int>>& A, const vector<pair<int,int>>& B){
    vector<EndPoint> mergedIntervalSet = mergeIntervalSet(A, B);
    vector<pair<int,int>> output;
    output.reserve(A.size()+B.size());
    
    bool inA = false;
    bool inB = false;
    // bool inIntersection = false;

    int inIntervalA = 0;
    int inIntervalB = 0;
    int inIntersection = 0;

    int sum = 0;

    
    for (auto element: mergedIntervalSet){
        //If in A interval
        if(element.set){
            if( element.start && !inA){
                inIntervalA = 1;
                inA = true;
            }
            else if(!element.start && inA){
                inIntervalA = -1;
                inA = false;
            }
        }

        //If in B interval
        if(!element.set){
            if(element.start && !inB){
                inIntervalB = 1;
                inB = true;
            }
            else if(!element.start && inB){
                inIntervalB = -1;
                inB = false;
            }
        }

        inIntersection += inIntervalA + inIntervalB;
        // if(inIntersection>2){
        //     inIntersection = 2;
        // }
        
        std::cout<<element.data<<": "<<inIntersection<<std::endl;



        // sum  = inIntervalA + inIntervalB;
        
        // if(!inIntersection && inIntervalA+inIntervalB==2){
        //     //start intersection
        // }
        // else if(inIntervalA+inIntervalb <2 && )
        

        //If inside both A and B then inside intersection
        // if(inIntervalA && inIntervalB){
        //     inIntersection = true;
        //     //create interval
        //     pair<int, int> interval;
        //     interval.first = element.data;
        // }
        // else if(inIntersection){
        //     interval.second = element.data;
        //     output.push_back(interval);
        // }
    }

    //output interval generation
    return output;  
}


vector<pair<int,int>> generate_intervals(const vector<int>& endpoints) {
    vector<pair<int,int>> intervals;
    intervals.reserve(endpoints.size()/2);
    for(int i = 0; i<endpoints.size()/2; i++){
        intervals.push_back(make_pair(endpoints[2*i], endpoints[2*i + 1]));
    }
    return intervals;
}


int main(){
    vector<int> endpoints_A = {3, 6, 8, 10, 1, 5, 8, 9};
    vector<int> endpoints_B = {11, 13, 4, 9, 14, 15};

    vector<pair<int, int>> intervals_A = generate_intervals(endpoints_A);
    vector<pair<int, int>> intervals_B = generate_intervals(endpoints_B);
    // print_intervals(intervals_A);
    // print_intervals(intervals_B);

    findIntersection(intervals_A, intervals_B);


    return 0;
}
