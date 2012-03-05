#ifndef MACHINE_MANAGER_H
#define MACHINE_MANAGER_H

#include <pal/stdtype.h>
class machine_t;
class access_point;

class EXPORT_TO_DLL macmgr_t
{
public:
	macmgr_t(class Database*);

	void add_machine(machine_t&); /* would change machine_t::id */
	void update_machine(const machine_t&);
	void rm_machine(const machine_t&);

	void add_ap(access_point&);
	void update_ap(const access_point&);
	void rm_ap(const access_point&);

	int macnum();
	machine_t macfirst();
	machine_t macthis(idx_t);
	machine_t macnext(const machine_t&);

	int apnum(const machine_t&);
	access_point apfirst(const machine_t&);
	access_point apthis(idx_t);
	access_point apnext(const access_point&);
	machine_t aphost(const access_point&);
private:
	class Database* dbmgr_;
};

#endif
