#ifndef JACKPOTWIN_RE_HYPERLINK_INC
#define JACKPOTWIN_RE_HYPERLINK_INC

#define HYPERLINK_DOMAIN	WM_USER+0x0400
#define HYLM_SETLINK 		HYPERLINK_DOMAIN+0
#define HYLM_GETLINK		HYPERLINK_DOMAIN+1

int init_hyperlink(void);

#endif