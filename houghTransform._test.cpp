#include <iostream>
#include <stdio.h>
#include "houghTransform.h"

Image<T_data, img_rows, img_cols> out_result = { 0 };
T_acc possible_rho[img_cols * img_rows] = { 0 };
T_acc possible_theta[img_cols * img_rows] = { 0 };
int main(){

	houghTransform(img.mat,out_result.mat,accumulator.mat,possible_rho,possible_theta);

	/*std::cout<<"Accumulator matrix:"<<"\n";
	for(int i=0;i<acc_cols;i++){	//theta
		for(int j=0;j<acc_rows;j++){	//rho
			//std::cout<< img.mat[i][j] << " ";
			std::cout<< j<<","<<i<<": "<<accumulator.mat[i][j] << "\n";
		}
		std::cout<<"\n";
	}*/
	for(int i=0;i<100;i++){
		std::cout<<"Possible rho: "<<possible_rho[i]<<"\n";
		std::cout<<"Possible theta: "<<possible_theta[i]<<"\n";
	}
	std::cout<<"Output with lines:"<<"\n";
	for(int i=0;i<img_cols;i++){
		for(int j=0;j<img_rows;j++){
			//std::cout<< j<<","<<i<<": "<<out_result.mat[i][j] << "\n";
			std::cout<<out_result.mat[i][j]<<" ";
		}
		std::cout<<"\n";
	}


	return 0;
}
