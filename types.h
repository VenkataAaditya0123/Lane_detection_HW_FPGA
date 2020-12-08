//typedef long long T_size;
typedef int T_size;
// Union type allows same data to be accessed as either a vector or a matrix
template<typename T, T_size rows, T_size cols>
union Image
{
	T vect[rows*cols];
	T mat[rows][cols];
};
