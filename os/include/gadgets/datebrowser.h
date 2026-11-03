#ifndef GADGETS_DATEBROWSER_H
#define GADGETS_DATEBROWSER_H
/*
    datebrowser.gadget: a month calendar.
    Part of the clang-dos Amiga SDK; written for it.
*/

#ifndef REACTION_REACTION_H
#include <reaction/reaction.h>
#endif
#ifndef INTUITION_GADGETCLASS_H
#include <intuition/gadgetclass.h>
#endif

#define DATEBROWSER_Dummy           (REACTION_Dummy + 0x61000)
#define DATEBROWSER_Day             (DATEBROWSER_Dummy + 0)    /* (UWORD) 1..31 */
#define DATEBROWSER_Month           (DATEBROWSER_Dummy + 1)    /* (UWORD) 1..12 */
#define DATEBROWSER_Year            (DATEBROWSER_Dummy + 2)    /* (LONG) */
#define DATEBROWSER_SelectedDays    (DATEBROWSER_Dummy + 3)    /* (ULONG) one bit per day */
#define DATEBROWSER_WeekDay         (DATEBROWSER_Dummy + 4)    /* (UWORD) day of the week (get) */
#define DATEBROWSER_FirstWDay       (DATEBROWSER_Dummy + 5)    /* (UWORD) first day of the week */
#define DATEBROWSER_NumDays         (DATEBROWSER_Dummy + 6)    /* (UWORD) days in the month (get) */
#define DATEBROWSER_ShowTitle       (DATEBROWSER_Dummy + 7)    /* (BOOL) show day names */
#define DATEBROWSER_MultiSelect     (DATEBROWSER_Dummy + 8)    /* (BOOL) select several days */
#define DATEBROWSER_DayTitles       (DATEBROWSER_Dummy + 9)    /* (STRPTR *) day names */
#define DATEBROWSER_WeekTitle       (DATEBROWSER_Dummy + 11)   /* (STRPTR) title of the week column */
#define DATEBROWSER_CalendarType    (DATEBROWSER_Dummy + 12)   /* (ULONG) */

#define DateBrowserObject   NewObject(DATEBROWSER_GetClass(), NULL
#define DateBrowserEnd      TAG_END)

#endif /* GADGETS_DATEBROWSER_H */
