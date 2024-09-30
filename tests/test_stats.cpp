//
//  test_stats.cpp
//  Created by Ben Westcott on 9/25/24.
//

#include "../stats.h"

TEST_CASE("eval_stats")
{
	matrix<double> m(3, 3, {1, 0, 0, 1, 2, 0, 1, 2, 3});
	
	stats st = eval_stats(m, 0, m.rows(), 0, m.cols(), 0.000001);
		
	std::cout << st << "\n";
	
	for(size_t r=0; r < m.rows(); r++)
	{
		for(size_t c=0; c < m.cols(); c++)
		{
			if(c > r)
			{
				std::cout << m(r, c) << "\t";
			}
		}
		std::cout << "\n";
	}
	
}
