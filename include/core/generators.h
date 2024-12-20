//
//  generators.h
//  Created by Ben Westcott on 12/19/24.
//

template<typename SeqT, typename GenT, typename...>
struct generator;

/*
 * GenT: generator type
 *      - power series
 *      - dirchlet
 *      - ...
 */

/*
 * GenT = power series:
 *
 * This type of generator takes the following form:
 *      a_n g_n(x)
 * Where a_n is a sequence, g_n(x) is of the form
 *      g_n(x) = g_n(x)' * x^n
 * 
 * Ordinary generator (OG):
 *      G(a_n ; x) = a_n x^n
 *          - g_n(x)' = 1
 *          - g_n(x) = x^n
 *
 * Exponential generator (EG):
 *      EG(a_n ; x) = a_n x^n/n!
 *          - g_n(x)' = 1/n!
 *          - g_n(x) = x^n/n!
 * 
 * Poisson generator (PG):
 *      PG(a_n ; x) = a_n exp(-x) x^n/n!
 *                  = exp(-x).EG(a_n ; x)
 * 
 * Lambert series generator (LG):
 *      LG(a_n ; x) = a_n . x^n/(1-x^n)
 * 
 */