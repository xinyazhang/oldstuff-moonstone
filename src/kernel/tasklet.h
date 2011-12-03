#ifndef TASKLET_H
#define TASKLET_H

class tasklet_t 
{
public:
	tasklet_t(int evid):evid_(evid) {}
	virtual ~tasklet_t() {};

	int evid() { return evid_; }
	virtual int doit() = 0;
private:
	int evid_;
};

enum TASKLET_ID {
	TASKLET_QUIT = 0,
	VOLFDPREP_ADD,
	VOLFDPREP_RMV
};

#endif
