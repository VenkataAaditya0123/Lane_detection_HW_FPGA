#include "accumulator.h"
#include <cmath>


void houghTransform(T_data in[img_rows][img_cols],
		T_data out[img_rows][img_cols],
		T_acc acc[acc_rows][acc_cols],
		T_acc possible_rho[acc_rows * acc_cols],
		T_acc possible_theta[acc_rows * acc_cols]);
