#include "houghTransform.h"
#include <hls_math.h>
#include <stdio.h>
#include<iostream>
#include<ap_fixed.h>
//#include <ap_mem_if.h>

using namespace hls;
#define PAI 3.1415926
#define PAI_BY_180 0.01745329222
typedef ap_fixed<32,2> accuracy;	//was 17,2

void houghTransform(T_data in[img_rows][img_cols],
		T_data out[img_rows][img_cols],
		T_acc acc[acc_rows][acc_cols],
		T_acc possible_rho[acc_rows * acc_cols],
		T_acc possible_theta[acc_rows * acc_cols])
{

#pragma DATAFLOW
#pragma HLS INTERFACE axis port=in  depth= 307200 	//Depth for co-simulation
#pragma HLS INTERFACE axis port=out offset=slave depth=307200 	//axis - axi4 stream interface
#pragma HLS INTERFACE s_axilite port=return bundle=control

	//accuracy cosval[180];	//later reduce
	//accuracy sinval[180];	//later reduce
	//accuracy tanval[180];
	int nonZeroCount = 0;
	int outRow = 0;
	int outCol = 0;
	ap_fixed<32,1> angle;
//	float angle;
//#pragma HLS array_partition variable=in complete dim =1
#pragma HLS array_partition variable=acc complete dim=2		//dimension theta
//#pragma HLS array_partition variable=sinval complete
//#pragma HLS array_partition variable=cosval complete
	//Calculating sin & cos values
/*loop_sincos_table:	for(short int t=0;t<180;t++){
#pragma HLS PIPELINE
						angle = (PAI_BY_180*t);
						sinval[t] = hls::sin(angle);
						cosval[t] = hls::cos(angle);
						//tanval[t] = hls::tan(angle);
					}*/
//	ap_mem_chn<int,int, 180, ROM_2P> cotval
static ap_fixed<32,8> cotval[180] = {100,57.2899616307594,28.6362532829156,19.0811366877282,14.3006662567119,11.4300523027613,9.51436445422259,8.14434642797459,7.11536972238421,6.31375151467504,5.67128181961771,5.14455401597031,4.70463010947846,4.33147587428416,4.01078093353585,3.73205080756888,3.48741444384091,3.27085261848414,3.07768353717525,2.90421087767582,2.74747741945462,2.60508906469380,2.47508685341630,2.35585236582375,2.24603677390422,2.14450692050956,2.05030384157930,1.96261050550515,1.88072646534633,1.80404775527142,1.73205080756888,1.66427948235052,1.60033452904105,1.53986496381458,1.48256096851274,1.42814800674211,1.37638192047117,1.32704482162041,1.27994163219308,1.23489715653505,1.19175359259421,1.15036840722101,1.11061251482919,1.07236871002468,1.03553031379057,1,0.965688774807074,0.932515086137662,0.900404044297840,0.869286737816227,0.839099631177280,0.809784033195007,0.781285626506717,0.753554050102794,0.726542528005361,0.700207538209710,0.674508516842427,0.649407593197511,0.624869351909328,0.600860619027560,0.577350269189626,0.554309051452769,0.531709431661479,0.509525449494429,0.487732588565862,0.466307658154999,0.445228685308536,0.424474816209605,0.404026225835157,0.383864035035416,0.363970234266202,0.344327613289665,0.324919696232906,0.305730681458660,0.286745385758808,0.267949192431123,0.249328002843181,0.230868191125563,0.212556561670022,0.194380309137718,0.176326980708465,0.158384440324536,0.140540834702391,0.122784560902905,0.105104235265676,0.0874886635259240,0.0699268119435104,0.0524077792830412,0.0349207694917477,0.0174550649282176,0,-0.0174550649282176,-0.0349207694917477,-0.0524077792830412,-0.0699268119435104,-0.0874886635259240,-0.105104235265676,-0.122784560902905,-0.140540834702391,-0.158384440324536,-0.176326980708465,-0.194380309137718,-0.212556561670022,-0.230868191125563,-0.249328002843181,-0.267949192431123,-0.286745385758808,-0.305730681458660,-0.324919696232906,-0.344327613289665,-0.363970234266202,-0.383864035035416,-0.404026225835157,-0.424474816209605,-0.445228685308536,-0.466307658154999,-0.487732588565862,-0.509525449494429,-0.531709431661479,-0.554309051452769,-0.577350269189626,-0.600860619027560,-0.624869351909328,-0.649407593197511,-0.674508516842427,-0.700207538209710,-0.726542528005361,-0.753554050102794,-0.781285626506717,-0.809784033195007,-0.839099631177280,-0.869286737816227,-0.900404044297840,-0.932515086137662,-0.965688774807074,-1,-1.03553031379057,-1.07236871002468,-1.11061251482919,-1.15036840722101,-1.19175359259421,-1.23489715653505,-1.27994163219308,-1.32704482162041,-1.37638192047117,-1.42814800674211,-1.48256096851274,-1.53986496381458,-1.60033452904105,-1.66427948235052,-1.73205080756888,-1.80404775527142,-1.88072646534633,-1.96261050550515,-2.05030384157930,-2.14450692050956,-2.24603677390422,-2.35585236582375,-2.47508685341630,-2.60508906469380,-2.74747741945462,-2.90421087767582,-3.07768353717525,-3.27085261848414,-3.48741444384091,-3.73205080756888,-4.01078093353585,-4.33147587428416,-4.70463010947846,-5.14455401597031,-5.67128181961771,-6.31375151467504,-7.11536972238421,-8.14434642797459,-9.51436445422259,-11.4300523027613,-14.3006662567119,-19.0811366877282,-28.6362532829156,-57.2899616307594};
#pragma HLS array_partition variable=cotval complete

 //Calculating hough space
 T_acc accMax = 0;

loop_accum_c:	for(short int c=0; c<img_cols; c++){
	loop_accum_r:		for(short int r=0; r<img_rows; r++){
			if(in[r][c]){	//after canny.Threshold
			loop_accum_theta:	for(short int theta=30; theta<150; theta++){
	//was 45,135
#pragma HLS PIPELINE
						//angle = (PAI_BY_180*theta);
						//int rh = ((int)((r*(hls::cos(angle))+c*(hls::sin(angle)))));
						//int rh = ((int)((r*(cosval[theta])+c*(sinval[theta]))));
						//acc[rh][theta] = acc[rh][theta]+1;
						int b = (int((r*cotval[theta])+c));	//removed +500
						//std::cout<<"b: "<<b<<"\n";
						acc[b][theta] = acc[b][theta]+1;
						if(acc[b][theta]>accMax)
							accMax = acc[b][theta];
					}
			}
		}
	}

	//Finding max. value in accumulator for longest line
/*T_acc accMax = 0;
loop_accumMax_c:	for(short int ac=0; ac<acc_cols;ac++){
loop_accumMax_r:		for(short int ar=0; ar<acc_rows; ar++){
					#pragma HLS PIPELINE
								if(acc[ar][ac]>accMax){
									accMax = acc[ar][ac];
								}
							}
						}*/
	std::cout<<"ACCMAXXX new : "<<accMax<<"\n";
	int difference = 100;
	int threshold = accMax - difference;

	//Normalize hough space can also be done
	//Finding possible lines/points in hough space
	for(short int ac=0; ac<acc_cols;ac++){

#pragma HLS UNROLL factor=4
			for(short int ar=0; ar<acc_rows; ar++){
#pragma HLS PIPELINE
				if(acc[ar][ac]>threshold){
					possible_rho[nonZeroCount] = ar;	//removed -500
					possible_theta[nonZeroCount] = ac;
					nonZeroCount++;
				}
			}
	}
	//see pragma dataflow
	//ap_fixed<32,16> angle2 = 0;		//maybe possible to reduce
	//float angle2 = 0;
	for(short int k=0; k < nonZeroCount; k++){
#pragma HLS loop_tripcount min=1 max=5

#pragma HLS UNROLL factor=4
		//for(short int c=0; c<img_cols;c++){
		for(short int r=0; r<img_rows;r++){
#pragma HLS PIPELINE
//#pragma HLS UNROLL factor=4//here not needed
			/*angle2 = (PAI_BY_180*possible_theta[k]);
			outRow = (int)(-(hls::tan(angle2))*c + (possible_rho[k]) / (hls::cos(angle2)));
			if (outRow > 1 && outRow < img_rows)	//check if it's a valid row
						{
							out[outRow][c] = 1;	//Maybe return these as o/p from this block
						}*/
			outCol = (int)(possible_rho[k] - r*cotval[possible_theta[k]]);
			if (outCol > 1 && outCol < img_cols)	//check if it's a valid row
						{
							out[r][outCol] = 1;	//Maybe return these as o/p from this block
						}

		}
	}
}

