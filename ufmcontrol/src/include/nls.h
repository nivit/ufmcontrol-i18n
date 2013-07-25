/*
 * usefull macros for I18N/L10N
 *
 * $Id: nls.h,v 1.1 2003/10/31 16:17:30 nivit Exp $
 */

#ifndef NLS_H_SEEN
#define NLS_H_SEEN

#include <locale.h>
#include <nl_types.h>

#define UFM_MESSAGE_CATALOG "ufmcontrol"

/*
 * catalog message set (see src/nls/en_US.US-ASCII/ufmcontrol.msg)
 *
 * list of files that use the macro NLS(message), sorted.
 */

#define DELTA_FREQ_C_SET	 1
#define HELP_C_SET		 2
#define MAIN_C_SET		 3
#define OPEN_DEVICE_C_SET	 4
#define PARSE_C_SET		 5
#define RUN_C_SET		 6
#define SET_DEVNAME_C_SET	 7
#define SET_LOCALE_C_SET	 8
#define SET_VOLUME_C_SET	 9
#define USAGE_C_SET		10

#define NLS(S) catgets(nl_cd, CAT_SETID, __LINE__, S)
#define NLS2(L, S) catgets(nl_cd, CAT_SETID, L, S)

#endif /* !NLS_H_SEEN */
