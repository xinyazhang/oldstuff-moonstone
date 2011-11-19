#ifndef TASKLET_H
#define TASKLET_H

class tasklet_t 
{
public:
	tasklet_t(int evid)
	virtual ~tasklet_t() {};

	int evid() { return evid_; }
	virtual int doit() = 0;
private:
	int evid_;
};

#endif
