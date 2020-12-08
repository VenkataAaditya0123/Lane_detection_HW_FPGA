#pragma once
#include "Image.h"
#include <ap_fixed.h>
//const T_size acc_rows = (img_rows + img_cols) * 2;
const T_size acc_rows = 1600;	//fixing as per resolution and MATLAB
const T_size acc_cols = 180;

//typedef float T_acc;
typedef unsigned int T_acc;
//ypedef ap_fixed<32,16> T_acc;
// arrays for hough transform
static Image<T_acc, acc_rows, acc_cols> accumulator = { 0 };
