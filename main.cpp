import matrix;
import std;

int main()
{
	Matrix<int, 2> m1{{0, 1, 2}, {10, 11, 12}};
	std::cout << m1(1, 0) << std::endl;
	std::cout << m1(1, 1) << std::endl;
	std::cout << m1(1, 2) << std::endl;
}



