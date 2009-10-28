#ifndef _KERNEL_COMMON_DELETER_
#define _KERNEL_COMMON_DELETER_

template<class T>
class CommonDeleter
{
public:
	void operator()(T* ptr)
	{
		ptr->rc_->destroy(ptr->idx(), ptr);
	}
};

#endif
