#ifndef DAEMON_H
#define DAEMON_H

int daemon_init(); // return 0 on success
int daemon_main_loop();
void daemon_release();

#endif
