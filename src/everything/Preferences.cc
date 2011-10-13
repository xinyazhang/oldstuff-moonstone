#include "Preferences.h"

Preferences* pref()
{
    static Preference* pref = NULL;
    if (!pref)
    {
        /* init */
    }
    return pref;
}
