/*This is Gray Code enumeration Package
-1 -> enumeration out of range.
*/

#ifndef __GRAY__
#define __GRAY__
#include <iostream>
class grayCodeGenerator
{
public:
	grayCodeGenerator(int n)
	{
		this->length = n;
		this->p = new int[n + 1];
		for (int i = 0;i < n + 1;i++)
		{
			this->p[i] = i + 1;
		}
	}

	~grayCodeGenerator()
	{
		delete this->p;
	}

	int next_grayCode()
	{
		int k = this->p[0];
		if (k > length)
		{
			return -1;
		}
		this->p[k - 1] = this->p[k];
		this->p[k] = k + 1;
		if (k != 1)
		{
			this->p[0] = 1;
		}
		return k;
	}
	int length;
	int *p;
};
#endif
