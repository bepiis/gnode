//
//  stats.h
//  Created by Ben Westcott on 9/25/24.
//

#pragma once

#include <cstdint>
#include <iostream>
#include "matrix.h"
#include "products.h"

/*
constexpr uint32_t ABSMAX = 1 << 0;
constexpr uint32_t ABSMAXCNT = 1 << 1;
constexpr uint32_t ARITHMEAN = 1 << 2;
constexpr uint32_t GEOMMEAN = 1 << 3;
constexpr uint32_t STDDEV = 1 << 4;
*/

struct stats
{
	double absmax = 0.0;
	size_t absmaxcnt = 0;
	double geommean = 1.0;
	double arithmean = 0.0;
	double rms = 0.0;
	double stddev = 0.0;
	
	double norm2 = 0.0;
	double inorm2 = 0.0;
	double cond = 1.0;
};

std::ostream& operator<<(std::ostream& os, const stats& st)
{
	std::cout << "stats: \n";
	std::cout << "\t|max(A)|\t=\t" << st.absmax << "\n";
	std::cout << "\t|max(A)| > tol\t=\t" << st.absmaxcnt << "\n";
	std::cout << "\tGM\t\t=\t" << st.geommean << "\n";
	std::cout << "\tAM\t\t=\t" << st.arithmean << "\n";
	std::cout << "\tRMS\t\t=\t" << st.rms << "\n";
	std::cout << "\tstddev\t\t=\t" << st.stddev << "\n";
	std::cout << "\tnorm2(A)\t~\t" << st.norm2 << "\n";
	std::cout << "\tinorm2(A)\t~\t" << st.inorm2 << "\n";
	std::cout << "\tcond(A) \t~\t" << st.cond << "\n";
	
	return os;
}

// [1] (Texts in Applied Mathematics 55) Grégoire Allaire, Sidi Mahmoud Kaber (auth.)
//     - Numerical Linear Algebra-Springer-Verlag New York (2008).pdf

/*
 * cond, norm and inorm in stats struct are approximations for triangularized matrices.
 * The idea here (from [1]) which I will summarize is that since if A = BC is a factorization
 * such that B and C are simple (triangular or orthongonal) then we can define an upper bound
 * on the condition number of A, namely that:
 *
 * cond(A) <= cond(B)cond(C)
 *
 * If we have, for example, a A = QR factorization, then Q is orthogonal and thus cond(Q) = 1
 * and we have:
 *
 * cond(A) <= cond(R)
 *
 * For all intents and purposes, I dont really care about the exact condition number of A, and
 * an upper bound is good enough to determine the sensitivity of A in response to perturbed inputs. 
 *
 */

// TODO: Condition number and norm estimates only work for lower triangular matrices so far, 
// as can be seen by the forward sub alg embedded in the loop. upper triangular matrices can easily
// be dealt with via an approximation by back substitution, and ig for now, we just have the fn
// caller specify whether the input matrix is upper or lower triangular if they want an estimate of
// cond(A). Note that I have already included variables backr and backc for this. The logic for
// choosing forward/back needs to be done, and the back sub computation needs to be put in.

// TODO: At some point, I would like to be able to specify which stats I would like to compute in this loop:
/*
 * Each stat has at most a first_step, col_step, row_step and a final_step.
 * So this fn takes in the stats we want to compute, and executes fns for the first
 * steps (to initialize variables associated with the stat), and then loops over
 * the matrix, and runs the col and row steps for each specified stat.
 * Then at the end, we finalize the stat calcs, (i.e. arithmean gets divided by N)
 * and we only compute & return the stats which we have specified.
 */
template<typename T>
stats eval_stats(const matrix<T> &in, size_t start_row, size_t break_row, size_t start_col, size_t break_col, double tolerance)
{
	double absmax = 0.0;
	size_t absmaxcnt = 0;
	double geommean = 0.0;
	double arithmean = 0.0;
	double rms = 0.0;
	double stddev = 0.0;
	
	stats box;
	
	size_t N = break_row - start_row;
	size_t M = break_col - start_col;
	 
	size_t sz = N * M;
	double dbl_sz = (double)sz;
	double dbl_isz = 1/dbl_sz;
		
	matrix<double> xc(M, 1);
	matrix<double> yc(N, 1), iyc(N, 1);
	
	double elem = static_cast<double>(in(0, 0));
	
	xc(0, 0) = 1.0;
	yc(0, 0) = elem;
	iyc(0, 0) = 1/elem;
	
	int64_t backr = (int64_t)break_row - 1;

	for(size_t r=start_row; r < break_row; r++, backr--)
	{
		double s = 0.0;
		double is = 0.0;
		double curr_diag = static_cast<double>(in(r, r));
		int64_t backc = (int64_t)break_col - 1;
		
		for(size_t c=start_col; c < break_col; c++, backc--)
		{
			elem = static_cast<double>(in(r, c));
			double abselem = std::abs(elem);	
						
			geommean *= std::pow(elem, dbl_isz);
			arithmean += elem;
			rms += (elem * elem);
			
			if(abselem >= absmax)
			{
				absmax = abselem;
			}
			
			if(abselem > tolerance)
			{
				absmaxcnt++;
			}
			
			if(c < r)
			{
				s += elem * xc(c, 0);
				is += elem * iyc(c, 0);				
			}
		}
		
		if(r > start_row)
		{
            // for cond, norm2, inorm2. Essentially, we estimate ||A||_2 by 
            // forward subbing on the off diagonals, and choosing ±1 for x which
            // maximizes the modulus of y. In turn, since the ||x||_2 = sqrt(M),
            // then ||A||_2 ~ ||y||_2/||x||_2 = ||y||_2/sqrt(M)
			if(std::abs(curr_diag + s) > std::abs(curr_diag - s))
			{
				xc(r, 0) = 1.0;
			}
			else xc(r, 0) = -1.0;
		
			iyc(r, 0) = -(((s > 0) ? 1 : -1) + s)/curr_diag;
			yc(r, 0) = curr_diag * xc(r, 0) + s;
		}
	}
	
	arithmean /= dbl_sz;
	
    // stddev requires another pass since we need arithmean to calc it.
	for(size_t r=start_row; r < break_row; r++)
	{
		for(size_t c=start_col; c < break_col; c++)
		{
			stddev += std::pow((static_cast<double>(in(r, c)) - arithmean), 2);
		}
	}
	
	box.arithmean = arithmean;
	box.stddev = std::sqrt(stddev/dbl_sz);
	box.rms = std::sqrt(0.5 * dbl_isz * rms);
	
	double dM = (double)M;
	box.norm2 = std::sqrt(col_norm2sq(yc, 0)/dM);
	box.inorm2 = std::sqrt(col_norm2sq(iyc, 0)/dM);
	box.cond = box.norm2 * box.inorm2;
	
	box.geommean = geommean;
	box.absmax = absmax;
	box.absmaxcnt = absmaxcnt;
	
	return box;
} 


