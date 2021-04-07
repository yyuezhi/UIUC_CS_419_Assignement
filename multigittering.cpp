#include <iostream>
#include <vector>  
#include "pointlight.h"
#include <algorithm>
//this struct define the coordinate of the fine grid in jitted sampling
struct jitter_coordinate{
    int x;
    int y;
};

//overload operator for comparing the jitter_coordinate
inline bool operator==(const jitter_coordinate &a, const jitter_coordinate &b)
{
    if (a.x == b.x && a.y == b.y){
        return true;
    }
    else{
        return false;
    }
}


std::vector <jitter_coordinate> multigittering_grid_generator(int fine_size, int rough_size)
{
    // candidate contain the remaining grid which sample point could lie on
    // sample_result contain the grid being sampled
    std::vector <jitter_coordinate> candidate;
    std::vector <jitter_coordinate> sample_result;
    jitter_coordinate temp;

    if (fine_size % rough_size != 0){
        std::cout<<"Fatal error: invalid multigittering input"<<std::endl;
        exit(2);
    }
    int rough_dim = fine_size / rough_size; 
    //initialize the candidate
    for (int k=0;k<fine_size;k++){
        for (int l=0;l<fine_size;l++){
            temp.x = k;
            temp.y = l;
            candidate.push_back(temp);
        }
    }

    //generate a new sample first
    int sample_index;
    for (int q=0;q<rough_size*rough_size -1 ;q++){
        //sample a grid
        sample_index = random_int(0,candidate.size()-1);
        sample_result.push_back(candidate[sample_index]);
        
        //remove the sampled grid
        candidate.erase(candidate.begin() + sample_index);


        //remove the grid which have the same column/row with sample
        for (int k=0;k<fine_size;k++){
            auto it = std::find(candidate.begin(),candidate.end(),jitter_coordinate{sample_result[q].x,k});
            if (it != candidate.end()){
                candidate.erase(it);
            }
        }

        for (int k=0;k<fine_size;k++){
            auto it = std::find(candidate.begin(),candidate.end(),jitter_coordinate{k,sample_result[q].y});
            if (it != candidate.end()){
                candidate.erase(it);
            }
        }


        //remove the grid which is in the same coarse grid as sampled grid
        for (int g=0;g<rough_size;g++){
            for (int h=0;h<rough_size;h++){
                if (sample_result[q].x >= g*rough_dim && sample_result[q].y >= h*rough_dim &&
                    sample_result[q].x < (g+1)*rough_dim && sample_result[q].y < (h+1)*rough_dim){
                    for (int n=0;n<rough_dim;n++){
                        for (int m=0;m<rough_dim;m++){
                            auto it = std::find(candidate.begin(),candidate.end(),jitter_coordinate{g*rough_dim+n,h*rough_dim+m});
                            if (it != candidate.end()){
                               candidate.erase(it);
                            }
                        }
                    }
                }
            }
        }
    }
    // if for the last sample, there are more than one gird, then something go wrong
    if (candidate.size() !=1){
        std::cout<<"fatal problem on multijittering"<<std::endl;
        exit(1);
    }
    sample_index = random_int(0,candidate.size()-1);
    sample_result.push_back(candidate[sample_index]);
    return sample_result;
}


