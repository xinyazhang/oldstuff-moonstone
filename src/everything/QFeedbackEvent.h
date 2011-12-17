#ifndef QFEEDBACK_EVENT_H
#define QFEEDBACK_EVENT_H

#include <QtCore/QEvent>
#include <kernel/feedback.h>

class QFeedbackEvent
	:public QEvent
{
public:
	QFeedbackEvent(feedback_event& ev)
		:ev_(ev), QEvent((QEvent::Type)(ev.evid + QEvent::User))
	{
	}

	~QFeedbackEvent() { }
	feedback_event& ev() { return ev_; }
private:
	feedback_event ev_;
};

#endif
