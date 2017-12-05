/* 
 * aech0r
 * 2009 Hedde Bosman
 *
 * This source code  is free software; you can  redistribute it and/or
 * modify it under the terms of the GNU Public License as published by
 * the Free Software  Foundation; either version 3 of  the License, or
 * (at your option) any later version.
 *
 * This source code is distributed in the hope that it will be useful,
 * but  WITHOUT ANY  WARRANTY; without  even the  implied  warranty of
 * MERCHANTABILITY or FITNESS FOR  A PARTICULAR PURPOSE.  Please refer
 * to the GNU Public License for more details.
 *
 * You should  have received  a copy of  the GNU Public  License along
 * with this source code; if  not, write to: Free Software Foundation,
 * Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 */
#include "frei0r.hpp"

#include <algorithm>
#include <vector>
#include <utility>
#include <cassert>

#include <iostream>

union px_t {
	uint32_t u;
	unsigned char c[4]; // 0=B, 1=G,2=R,3=A ? i think :P
};

class aech0r : public frei0r::filter {
private:
	f0r_param_double factor;
	unsigned int w, h;
	
public:
	aech0r(unsigned int width, unsigned int height) {
		factor = 1;
		w = width; h = height;
		register_param(factor, "Factor", "influence of mean px value. > 32 = 0");
	}
	~aech0r() {
	}

	virtual void update(double time,
	                    uint32_t* out,
                        const uint32_t* in) {
		unsigned char c = 0;
		unsigned long delay = 0;


		
		delay = double(size)*factor;
		
		for (int i = 0; i < size-delay; i++) {
			px_t pi, pi2, pi3;
			pi.u = in[i];
      pi2.u = in[i+delay];
      pi3.u = in[i+(delay>>1)];      

			pi.c[0] = pi.c[0] + pi2.c[1] + pi3.c[2] ;
			pi.c[1] = pi2.c[1];
			pi.c[2] = pi2.c[2];
			
			out[i] = pi.u;
		}
    int j =0;
		for (int i = size-delay; i < size; i++) {
			px_t pi, pi2, pi3;
			pi.u = in[i];
      pi2.u = in[j++];
      pi3.u = in[(j++)>>1];

			pi.c[0] = pi.c[0] + pi2.c[1] + pi3.c[2];
			pi.c[1] = pi2.c[1];
			pi.c[2] = pi2.c[2];
			
			out[i] = pi.u;
		}
	}
};


frei0r::construct<aech0r> plugin("aech0r",
									"Reduce image to primary colors",
									"Hedde Bosman",
									0,2);

