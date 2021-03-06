#ifndef EVID_H
#define EVID_H

enum EVENT_ID {
	/** Async proc. **/
	TASKLET_QUIT,
	VOLFDPREP_ADD,
	VOLFDPREP_RMV,
	EVENTD_REG,
	/** Log **/
	LOG_FATAL,
	LOG_ERROR,
	LOG_WARNING,
	LOG_NOTE,
	LOG_INFO,
	LOG_DEBUG,
	/** Interaction **/
	/* INDEXING */
	INDEX_PROGRESSED,
};


#endif
