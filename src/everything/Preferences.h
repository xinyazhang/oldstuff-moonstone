#ifndef PREFERENCES_H
#define PREFERENCES_H

#include <kernel/search_engine.h>

class Preferences
{
    search_engine_t* search_engine;
};

Preferences* pref(); /* Singletion */

#endif // PREFERENCES_H
