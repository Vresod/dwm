/* See LICENSE file for copyright and license details. */
#include <X11/XF86keysym.h>

/* appearance */
static const unsigned int borderpx  = 0;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const char *fonts[]          = { "monospace:size=10" };
static const char dmenufont[]       = "monospace:size=10";
static const char col_gray1[]       = "#1f1923"; // unactive background color; default is #222222
static const char col_gray2[]       = "#000000"; // nothing; default is #444444
static const char col_gray3[]       = "#bbbbbb"; // unactive font color; default is #bbbbbb
static const char col_gray4[]       = "#eeeeee"; // active font color; default is #eeeeee
static const char col_cyan[]        = "#b73109"; // background color of active; default is #005577
static const char *colors[][3]      = {
	/*               fg         bg         border   */
	[SchemeNorm] = { col_gray3, col_gray1, col_gray2 },
	[SchemeSel]  = { col_gray4, col_cyan,  col_cyan  },
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class      instance    title       tags mask     isfloating   monitor */
  //{ "Gimp",     NULL,       NULL,       0,            1,           -1 },
  //{ "Firefox",  NULL,       NULL,       1 << 8,       0,           -1 },
	{ "feh",      NULL,       NULL,       0,            1,           -1 },
	{ "xarchiver",NULL,       NULL,       0,            1,           -1 },
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] =   { "rofi", "-show", "drun", "-show-icons" };
static const char *termcmd[]  =   { "st", NULL };
static const char *xkillcmd[] =   { "xkill", NULL };
static const char *maimcmd[]  =   { "scrshot", "select", "clip" };
static const char *volup[]    =   { "volcontrol","up","5"};
static const char *voldown[]  =   { "volcontrol","down","5" };
static const char *volmute[]  =   { "volcontrol","mute"};
static const char *brightup[] =   { "bklcontrol", "up", "5" };
static const char *brightdown[] = { "bklcontrol", "down", "5" };
static const char *micon[]    =   { "mictest", "on" };
static const char *micoff[]   =   { "mictest", "off" };
static const char *killactivewindow[] = { "killactivewindow", NULL };
static const char *killcmd[]   =  { "killxorgconf" };
static const char *dmenuunicode[]   =   { "dmenuunicode", NULL };
static const char *eject[]    =   { "eject", NULL };
static const char *scrot[]    =   { "/home/lukeb/Repos/sh-scripts/scrot.sh", NULL };
static const char *mpdctrl[]  =   { "/home/lukeb/Repos/rofi-mpd/control.sh", NULL };
static const char *shutupjoey[]  =   { "st", "nvim", "/home/lukeb/tmp/shutupjoey" };
static const char *maimfeh[]  =   { "scrshot", "select", "feh" };

static Key keys[] = {
	/* modifier                     key        function        argument */
	{ MODKEY,                       XK_d,      spawn,          {.v = dmenucmd } },
	{ MODKEY,                       XK_Return, spawn,          {.v = termcmd } },
	{ MODKEY,                       XK_b,      togglebar,      {0} },
	{ MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,      focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_i,      incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_o,      incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },
	{ MODKEY|ShiftMask,             XK_Return, zoom,           {0} },
	{ MODKEY,                       XK_Tab,    view,           {0} },
	{ MODKEY,                       XK_w,      killclient,     {0} },
	{ MODKEY,                       XK_t,      setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                       XK_f,      togglefullscr,  {0} },
	{ MODKEY,                       XK_m,      setlayout,      {.v = &layouts[2]} },
	{ MODKEY,                       XK_space,  setlayout,      {0} },
	{ MODKEY|ShiftMask,             XK_space,  togglefloating, {0} },
	{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period, focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
	{ MODKEY,                       XK_x,      spawn,          {.v = xkillcmd } },
	{ MODKEY|ShiftMask,             XK_s,      spawn,          {.v = maimcmd } },
	{ MODKEY|ControlMask,           XK_s,      spawn,          {.v = maimfeh } },
	{ MODKEY|ControlMask,           XK_s,      spawn,          {.v = scrot } },
	{ MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_w,      spawn,          {.v = killactivewindow } },
	{ MODKEY|ShiftMask,             XK_m,      spawn,          {.v = mpdctrl } },
	{ MODKEY,                       XK_grave,  spawn,          {.v = dmenuunicode } },
	{ MODKEY|ShiftMask,             XK_grave,  spawn,          {.v = shutupjoey } },
	{ MODKEY,          XF86XK_AudioRaiseVolume,spawn,          {.v =  micon } },
	{ MODKEY,          XF86XK_AudioLowerVolume,spawn,          {.v =  micoff } },
	{ 0,               XF86XK_AudioRaiseVolume,spawn,          {.v =  volup } },
	{ 0,               XF86XK_AudioLowerVolume,spawn,          {.v =  voldown } },
	{ 0,                     XF86XK_AudioMute, spawn,          {.v =  volmute } },
	{ 0,              XF86XK_MonBrightnessDown,spawn,          {.v =  brightdown } },
	{ 0,                XF86XK_MonBrightnessUp,spawn,          {.v =  brightup } },
	{ 0,                        XF86XK_Display,spawn,          {.v =  eject } },
	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	TAGKEYS(                        XK_6,                      5)
	TAGKEYS(                        XK_7,                      6)
	TAGKEYS(                        XK_8,                      7)
	TAGKEYS(                        XK_9,                      8)
	{ MODKEY|ShiftMask,             XK_r,      quit,           {0} },
	{ MODKEY|ShiftMask,             XK_q,      spawn,          {.v = killcmd } },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkClientWin,        MODKEY|ShiftMask,Button2,        killclient,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};

static const char *ipcsockpath = "/tmp/dwm.sock";
static IPCCommand ipccommands[] = {
  IPCCOMMAND(  view,                1,      {ARG_TYPE_UINT}   ),
  IPCCOMMAND(  toggleview,          1,      {ARG_TYPE_UINT}   ),
  IPCCOMMAND(  tag,                 1,      {ARG_TYPE_UINT}   ),
  IPCCOMMAND(  toggletag,           1,      {ARG_TYPE_UINT}   ),
  IPCCOMMAND(  tagmon,              1,      {ARG_TYPE_UINT}   ),
  IPCCOMMAND(  focusmon,            1,      {ARG_TYPE_SINT}   ),
  IPCCOMMAND(  focusstack,          1,      {ARG_TYPE_SINT}   ),
  IPCCOMMAND(  zoom,                1,      {ARG_TYPE_NONE}   ),
  IPCCOMMAND(  incnmaster,          1,      {ARG_TYPE_SINT}   ),
  IPCCOMMAND(  killclient,          1,      {ARG_TYPE_SINT}   ),
  IPCCOMMAND(  togglefloating,      1,      {ARG_TYPE_NONE}   ),
  IPCCOMMAND(  setmfact,            1,      {ARG_TYPE_FLOAT}  ),
  IPCCOMMAND(  setlayoutsafe,       1,      {ARG_TYPE_PTR}    ),
  IPCCOMMAND(  quit,                1,      {ARG_TYPE_NONE}   )
};

