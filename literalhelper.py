#
#  literalhelper.py
#  Created by Ben Westcott on 11/28/24.
#

import re
import numpy as np

bracket_exp = r"\{([^\)]+)\}"

example = r'''const literal2D<std::complex<double>> ex_4x4fca = 
              {{1.0 + 0.5i, 1.5 + 1.0i, 2.0 + 1.5i, 2.5 + 2.0i},
               {3.0 + 2.5i, 3.5 + 3.0i, 4.0 + 3.5i, 4.5 + 4.0i},
               {5.0 + 4.5i, 5.5 + 5.0i, 6.0 + 5.5i, 6.5 + 6.0i},
               {7.0 + 6.5i, 7.5 + 7.0i, 8.0 + 7.5i, 8.5 + 8.0i}};'''

if __name__ == '__main__':
    
    br = re.split(bracket_exp, example)
    
    literal_tag = br[0]
    
