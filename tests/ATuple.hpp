#ifndef ATUPLE_HPP
# define ATUPLE_HPP

typedef float	t_f4 __attribute__((ext_vector_type(4)));
// # include <iostream>
# include <string>
# include <iostream>

class ATuple {
	protected:
		t_f4 value;
	public:
		ATuple() {}
		~ATuple() {}
};

#endif