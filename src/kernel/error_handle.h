#ifndef KERNEL_ERROR_HANDLE_H
#define KERNEL_ERROR_HANDLE_H

inline bool judge(int j) // judge if success or not
{
	return j < 0;
}

inline bool judge_and_replace(int j, int& r) // return false on error
{
	if ( judge(j) == false )
	{
		r = j;
		return false;
	} else
		return true;
}

#endif
