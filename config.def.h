/* See LICENSE file for copyright and license details. */

/* Constants */
#define TERMINAL "st"
#define TERMCLASS "St"
#define BROWSER "brave"

/* appearance */
static unsigned int borderpx  		= 1;        /* border pixel of windows */
static unsigned int snap      		= 32;       /* snap pixel */
static unsigned int gappih 			= 20;  /* horiz inner gap between windows */
static unsigned int gappiv 			= 10;  /* vert inner gap between windows */
static unsigned int gappoh 			= 10;  /* horiz outer gap between windows and screen edge */
static unsigned int gappov 			= 30;  /* vert outer gap between windows and screen edge */
static int smartgaps 				= 1;				  /* 1 means no outer gap when there is only one window */
static int showbar            		= 1;        /* 0 means no bar */
static int topbar             		= 1;        /* 0 means bottom bar */
static const int focusonwheel 		= 1;
static const int horizpadbar 		= 3;		  /* horizontal padding for statusbar */
static const int vertpadbar 		= 3;		  /* vertical padding for statusbar */
static const char *fonts[] 			= {"monospace:size=12", "JoyPixels:pixelsize=10:antialias=true:autohint=true"};
static char normbgcolor[] 			= "#282C34";
static char normbordercolor[] 		= "#61afef";
static char normfgcolor[]           = "#979fad";
static char selfgcolor[]            = "#eeeeee";
static char selbordercolor[] 		= "#c678dd";
static char selbgcolor[]            = "#005577";

static char *colors[][3] = {
       /*               fg           bg           border   */
       [SchemeNorm] = { normfgcolor, normbgcolor, normbordercolor },
       [SchemeSel]  = { selfgcolor,  selbgcolor,  selbordercolor  },
};

 typedef struct
{
	const char *name;
	 const void *cmd;
} Sp;

const char *spcmd1[] = {"st", "-n", "spterm", "-g", "120x34", NULL};
static Sp scratchpads[] = {
	/* name          cmd  */
	{"spterm", spcmd1},
};
static const XPoint stickyicon[]    = { {0,0}, {4,0}, {4,8}, {2,6}, {0,8}, {0,0} }; /* represents the icon as an array of vertices */
static const XPoint stickyiconbb    = {4,8};	/* defines the bottom right corner of the polygon's bounding box (speeds up scaling) */

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };
static const char *tagsalt[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };
static const int momentaryalttags = 0; /* 1 means alttags will show only when key is held down*/

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class      instance    title       tags mask     isfloating   monitor */
	{ "Gimp",     NULL,       NULL,       0,            1,           -1 },
	{ "Firefox",  NULL,       NULL,       1 << 8,       0,           -1 },
};

/* layout(s) */
static float mfact     = 0.60; /* factor of master area size [0.05..0.95] */
static int nmaster     = 1;    /* number of clients in master area */
static int resizehints = 0;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */

#define FORCE_VSPLIT 1  /* nrowgrid layout: force two clients to always split vertically */
#include "vanitygaps.c"

static const Layout layouts[] = {
	/* symbol     arrange function */
	/* first entry is default */
	{ "H[]",      deck },
	{ "[]=",      tile },    
	{ "[M]",      monocle },
	{ "[@]",      spiral },
	{ "[\\]",     dwindle },
	{ "TTT",      bstack },
	{ "===",      bstackhoriz },
	{ "HHH",      grid },
	{ "###",      nrowgrid },
	{ "---",      horizgrid },
	{ ":::",      gaplessgrid },
	{ "|M|",      centeredmaster },
	{ ">M>",      centeredfloatingmaster },
	{ "><>",      NULL },    /* no layout function means floating behavior */
};

/* key definitions */
#define MODKEY Mod4Mask
#define ALTMOD Mod1Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ALTMOD|ShiftMask,           KEY,      swaptags,       {.ui = 1 << TAG} }, \
        { ALTMOD,                       KEY,      focusnthmon,    {.i  = TAG } }, \
        { ALTMOD|ShiftMask,             KEY,      tagnthmon,      {.i  = TAG } },


/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static const char *termcmd[]  = { TERMINAL, NULL };

/* 
 * Xresources preferences to load at startup
 */
ResourcePref resources[] = {
	{"color0", STRING, &normbordercolor},
	{"color8", STRING, &selbordercolor},
	{"color0", STRING, &normbgcolor},
	{"color4", STRING, &normfgcolor},
	{"color0", STRING, &selfgcolor},
	{"color4", STRING, &selbgcolor},
	{"borderpx", INTEGER, &borderpx},
	{"snap", INTEGER, &snap},
	{"showbar", INTEGER, &showbar},
	{"topbar", INTEGER, &topbar},
	{"nmaster", INTEGER, &nmaster},
	{"resizehints", INTEGER, &resizehints},
	{"mfact", FLOAT, &mfact},
	{"gappih", INTEGER, &gappih},
	{"gappiv", INTEGER, &gappiv},
	{"gappoh", INTEGER, &gappoh},
	{"gappov", INTEGER, &gappov},
	{"smartgaps", INTEGER, &smartgaps},
};

#include <X11/XF86keysym.h>

static Key keys[] = {
	/* modifier                     key        function        argument */
	// Run Apps
	{ MODKEY,                       XK_d,      spawn,          {.v = (const char*[]){"dmenu_run", NULL} } },
	{ MODKEY,             			XK_Return, spawn,          {.v = termcmd } },
	{ MODKEY|ShiftMask,				XK_Return, togglescratch,  {.ui = 0} },

	// Utility
	{ MODKEY,			XK_grave,	spawn,	{.v = (const char*[]){ "dmenuunicode", NULL } } },

	// Utility dwm	
	{ MODKEY,                       XK_b,      togglebar,      {0} },
	{ MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,      focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_o,      incnmaster,     {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_o,      incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },
	{ MODKEY,                       XK_space,  zoom,           {0} },
	
	// { MODKEY|Mod1Mask,              XK_u,      incrgaps,       {.i = +1 } },
	// { MODKEY|Mod1Mask|ShiftMask,    XK_u,      incrgaps,       {.i = -1 } },
	// { MODKEY|Mod1Mask,              XK_i,      incrigaps,      {.i = +1 } },
	// { MODKEY|Mod1Mask|ShiftMask,    XK_i,      incrigaps,      {.i = -1 } },
	// { MODKEY|Mod1Mask,              XK_o,      incrogaps,      {.i = +1 } },
	// { MODKEY|Mod1Mask|ShiftMask,    XK_o,      incrogaps,      {.i = -1 } },
	// { MODKEY|Mod1Mask,              XK_6,      incrihgaps,     {.i = +1 } },
	// { MODKEY|Mod1Mask|ShiftMask,    XK_6,      incrihgaps,     {.i = -1 } },
	// { MODKEY|Mod1Mask,              XK_7,      incrivgaps,     {.i = +1 } },
	// { MODKEY|Mod1Mask|ShiftMask,    XK_7,      incrivgaps,     {.i = -1 } },
	// { MODKEY|Mod1Mask,              XK_8,      incrohgaps,     {.i = +1 } },
	// { MODKEY|Mod1Mask|ShiftMask,    XK_8,      incrohgaps,     {.i = -1 } },
	// { MODKEY|Mod1Mask,              XK_9,      incrovgaps,     {.i = +1 } },
	// { MODKEY|Mod1Mask|ShiftMask,    XK_9,      incrovgaps,     {.i = -1 } },
	{ MODKEY|Mod1Mask,              XK_0,      togglegaps,     {0} },
	// { MODKEY|Mod1Mask|ShiftMask,    XK_0,      defaultgaps,    {0} },
	{ MODKEY,                       XK_Tab,    view,           {0} },
	{ MODKEY|ShiftMask,             XK_k,      killclient,     {0} },
	{ MODKEY|ShiftMask,             XK_space,  togglefloating, {0} },
	{ MODKEY,                       XK_s,      togglesticky,   {0} },
	{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },

	{ MODKEY,                       XK_Left,  focusmon,       {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_Left,  tagmon,         {.i = -1 } },
	{ MODKEY,                       XK_Right, focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_Right, tagmon,         {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_h,      layoutscroll,   {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_l,      layoutscroll,   {.i = +1 } },
	{ MODKEY,                       XK_n,      togglealttag,   {0} },

	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	TAGKEYS(                        XK_6,                      5)
	TAGKEYS(                        XK_7,                      6)
	TAGKEYS(                        XK_8,                      7)
	TAGKEYS(                        XK_9,                      8)
	{ MODKEY|ShiftMask,             XK_q,      quit,           {0} },
	{ MODKEY|ControlMask|ShiftMask, XK_q,      quit,           {1} },

	{ MODKEY,			XK_F11,		spawn,		SHCMD("mpv --untimed --no-cache --no-osc --no-input-default-bindings --profile=low-latency --input-conf=/dev/null --title=webcam $(ls /dev/video[0,2,4,6,8] | tail -n 1)") },


	// Function Keys
	{ 0,				XK_Print,		spawn,		SHCMD("maim ~/Media/Pictures/ScreenShots/pic-full-$(date '+%y%m%d-%H%M-%S').png") },
	{ ShiftMask,		XK_Print,		spawn,		{.v = (const char*[]){ "maimpick", NULL } } },
	{ MODKEY,			XK_Print,		spawn,		{.v = (const char*[]){ "dmenurecord", NULL } } },
	{ MODKEY|ShiftMask,	XK_Print,		spawn,		{.v = (const char*[]){ "dmenurecord", "kill", NULL } } },
	{ MODKEY,			XK_Delete,		spawn,		{.v = (const char*[]){ "dmenurecord", "kill", NULL } } },
	{ MODKEY,			XK_Scroll_Lock,	spawn,		SHCMD("killall screenkey || screenkey &") },
	{ 0, XF86XK_AudioMute,			spawn,		SHCMD("pamixer -t") },
	{ 0, XF86XK_AudioRaiseVolume,	spawn,		SHCMD("pamixer -i 3") },
	{ 0, XF86XK_AudioLowerVolume,	spawn,		SHCMD("pamixer -d 3") },
	{ 0, XF86XK_AudioPrev,			spawn,		{.v = (const char*[]){ "mpc", "prev",  NULL } } },
	{ 0, XF86XK_AudioNext,			spawn,		{.v = (const char*[]){ "mpc", "next",  NULL } } },
	{ 0, XF86XK_AudioPause,			spawn,		{.v = (const char*[]){ "mpc", "pause", NULL } } },
	{ 0, XF86XK_AudioPlay,			spawn,		{.v = (const char*[]){ "mpc", "play",  NULL } } },
	{ 0, XF86XK_AudioStop,			spawn,		{.v = (const char*[]){ "mpc", "stop",  NULL } } },

	{ 0, XF86XK_MonBrightnessUp,	spawn,		{.v = (const char*[]){ "light", "-A", "15", NULL } } },
	{ 0, XF86XK_MonBrightnessDown,	spawn,		{.v = (const char*[]){ "light", "-U", "15", NULL } } },

	{ 0, XF86XK_PowerOff,		spawn,		{.v = (const char*[]){ "sysact", NULL } } },
	{ 0, XF86XK_Calculator,		spawn,		{.v = (const char*[]){ TERMINAL, "-e", "bc", "-l", NULL } } },

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
	{ ClkClientWin,         MODKEY,         Button1,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};

