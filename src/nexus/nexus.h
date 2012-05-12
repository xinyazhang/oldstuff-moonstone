#ifndef NEXUS_H
#define NEXUS_H

int nexus_init(const char* pipename);
int nexus_wait();

extern class logd *log;
extern class nexus_fd *nexus;
extern class fdpoll_t *fdp;

#endif
