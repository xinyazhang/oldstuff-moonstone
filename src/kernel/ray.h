#ifndef RAY_H
#define RAY_H

/* Infrastructure: Ray
 * JOB: Scan the volume
 */

class Database;
class tracer_t;
struct volume;
struct dentry_t;

class ray_t
	:public tasklet_t
{
public:
	ray_t(const volume& vol, Database* db, tracer_t* tracer);
	~ray_t();

	virtual int doit();
protected:
	virtual int next(dentry_t& );
	Database *db_;
	volume vol;
private:
	tracer_t *tracer_;
};

#endif
