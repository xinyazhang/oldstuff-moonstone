#include "tasklet_queue.h"
#include "tasklet.h"

tasklet_queue_t::tasklet_queue_t(int n)
{
	tg_ = globaltp()->allocate_threads(1, this);
}

tasklet_queue_t::~tasklet_queue_t()
{
	enqueue((tasklet_t*)NULL);
	delete tg_;
}

bool tasklet_queue_t::enqueue(tasklet_t* let)
{
	queue_lock_.lock();
	evid_queue_.push(let);
	queue_lock_.unlock();
	queue_cond_.notify_one();
}

int tasklet_queue_t::tp_working()
{
	while(true) {
		tasklet* let;

		/* Pick up a tasklet */
		{
			boost::thread::scoped_lock lock(queue_lock_);
			while (evid_queue_.empty()) {
				queue_cond_.wait(lock);
			}

			tasklet_t* let = evid_queue_.front();
			/* Put this before pop makes one TASKLET_QUIT event exits all the
			 * threads */
			if (!let || let->evid() == TASKLET_QUIT)
				return 0;
			evid_queue_.pop();
		}

		let->doit();
		delete let;
	}
}

