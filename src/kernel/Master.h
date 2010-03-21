#ifndef KERNEL_MASTER_H
#define KERNEL_MASTER_H

/*
 * direct creator or indirect creator of all kernel layer objects
 * Tasks:
 * 		+ read Master Preference Table
 * 		+ provice service for Establishing database connection 
 */
class Master
{
public:
	Master(const unistr& mpt); 
};

#endif
