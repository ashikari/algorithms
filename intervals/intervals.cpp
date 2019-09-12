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

//helpers


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


vector<pair<int,int>> generate_intervals(const vector<int>& endpoints) {
    vector<pair<int,int>> intervals;
    intervals.reserve(endpoints.size()/2);
    for(int i = 0; i<endpoints.size()/2; i++){
        intervals.push_back(make_pair(endpoints[2*i], endpoints[2*i + 1]));
    }
    return intervals;
}


void trimEndpoints(const vector<EndPoint>& tmp, vector<EndPoint>& out){
    out.push_back(tmp[0]);
    for (int idx=1; idx<tmp.size(); idx++){
        if (tmp[idx].start){
            if( not(out[out.size()-1].start)){
                out.push_back(tmp[idx]);
            }
        }
        else{
            if(not( !out[out.size()-1].start)){
                out.push_back(tmp[idx]);
            }
            else{
                out[out.size()-1] =tmp[idx];
            }
        }
    }
}


void intervalsToEndPoints(const vector<pair<int,int>>& intervals, bool aOrB, vector<EndPoint>& out){
    vector<EndPoint> tmp;
    tmp.reserve(intervals.size());
    EndPoint E;
    
    E.set = aOrB;
    for(int i = 0; i<intervals.size(); i++){
        E.data = intervals[i].first;
        E.start = true;
        tmp.push_back(E);
        
        E.data = intervals[i].second;
        E.start =false;
        tmp.push_back(E);
    }
    sort(tmp.begin(), tmp.end());
    trimEndpoints(tmp, out);
}


vector<EndPoint> mergeIntervalSet(const vector<pair<int, int>>& A, const vector<pair<int, int>>& B){
    vector<EndPoint> out;
    out.reserve(A.size()+B.size());

    intervalsToEndPoints(A, true, out);
    intervalsToEndPoints(B, false, out);
    sort(out.begin(), out.end());
   
    return out;
}


vector<pair<int,int>> findIntersection(const vector<pair<int,int>>& A, const vector<pair<int,int>>& B){
    vector<EndPoint> mergedIntervalSet = mergeIntervalSet(A, B);
    vector<pair<int,int>> output;
    output.reserve(A.size()+B.size());
    
    bool inA = false;
    bool inB = false;
    vector<int> inIntersection;
    inIntersection.reserve(A.size()+ B.size());

    for (auto element: mergedIntervalSet){
        //If in A interval
        if(element.set){
            if( element.start && !inA){
                inA = true;
            }
            else if(!element.start && inA){
                inA = false;
            }
        }

        //If in B interval
        if(!element.set){
            if(element.start && !inB){
                inB = true;
            }
            else if(!element.start && inB){
                inB = false;
            }
        }

        inIntersection.push_back(inA+inB);      
    }

    //build output
    pair<int, int> overlap;
    bool inside;
    if(inIntersection[0]==2){
        inside = true;
        overlap.first = mergedIntervalSet[0].data;
    }
    else{
        inside = false;
    }

    for(int idx=1; idx<inIntersection.size(); idx++){
        if (inside){
            if(inIntersection[idx]!=2){
                overlap.second = mergedIntervalSet[idx].data;
                output.push_back(overlap);
                inside = false;
            }
        }
        else{
            if(inIntersection[idx]==2){
                overlap.first = mergedIntervalSet[idx].data;
                inside = true;
            }
        }
    }

    //output interval generation
    return output;  
}


int main(){
    // vector<int> endpoints_A = {8, 10, 1, 9};
    // vector<int> endpoints_B = {11, 13, 4, 8, 14, 15};

    vector<int> endpoints_A = {3, 6, 8, 10, 1, 4, 8, 9};
    vector<int> endpoints_B = {11, 13, 4, 9, 14, 15};

    // vector<int> endpoints_A = {1, 5, 2, 3, 7, 10};
    // vector<int> endpoints_B = {1, 4, 8, 11};

    vector<pair<int, int>> intervals_A = generate_intervals(endpoints_A);
    vector<pair<int, int>> intervals_B = generate_intervals(endpoints_B);
    vector<pair<int,int>> result = findIntersection(intervals_A, intervals_B);
    print_intervals(result);
    return 0;
}
