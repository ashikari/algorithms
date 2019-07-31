#include <iostream>
#include <unordered_map>
#include<vector>


int fib_recursive(int digit){
    if (digit<0){
        std::cout<<"The digitit is negative, please provide a positive desired fib number"<<std::endl;
        return -1;
    }

    if (digit<=1){
        return digit;
    }
    else{
        return fib_recursive(digit -1) + fib_recursive(digit -2);
    }
}


int fib_memo(int digit, std::unordered_map<int, int>& memo){
    if (digit<0){
        std::cout<<"The digitit is negative, please provide a positive desired fib number"<<std::endl;
        return -1;
    }

    if (digit<=1){
        memo[digit] = digit;
        return digit;
    }
    else{
        if(memo.find(digit -1) == memo.end()){
            memo[digit -1 ] = fib_memo(digit-1, memo);
        }
        if(memo.find(digit -2) == memo.end()){
            memo[digit -2 ] = fib_memo(digit-2, memo);
        }

        memo[digit] = memo[digit-1] + memo[digit - 2];
        return memo[digit];
    }
}

int fib_tableau(int digit){
    std::vector<int> fib_nums;


    fib_nums.push_back(0);
    fib_nums.push_back(1);
    for (int i = 2; i<=digit; i++){
        int val = fib_nums[i-1]+fib_nums[i-2];
        fib_nums.push_back(fib_nums[i-1]+fib_nums[i-2]);
    }

    return fib_nums[digit];
}

int main(){

    //change implementation types OR prompt error if number is above allowable size for int.
    int num_digits;
    bool RECURSIVE;
    std::cin>>num_digits>>RECURSIVE;



    std::unordered_map<int, int> memo;
    for (int i =0; i<num_digits; i++){
        if(RECURSIVE){
            std::cout<<fib_memo(i, memo)<<" "<<fib_recursive(i)<<" "<<fib_tableau(i)<<std::endl;
        }
        else{
            std::cout<<fib_memo(i, memo)<<" "<<fib_tableau(i)<<std::endl;
        }
    }

    return 0;
}