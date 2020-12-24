/* See LICENSE file for copyright and license details. */

#include <X11/XF86keysym.h>

/* appearance */
static const unsigned int borderpx  = 4;        /* border pixel of windows */
static const unsigned int gappx     = 5;	/* gaps between windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const unsigned int systraypinning = 0;   /* 0: sloppy systray follows selected monitor, >0: pin systray to monitor X */
static const unsigned int systrayspacing = 2;   /* systray spacing */
static const int systraypinningfailfirst = 1;   /* 1: if pinning fails, display systray on the first monitor, False: display systray on the last monitor*/
static const int showsystray        = 1;     /* 0 means no systray */
static const int swallowfloating    = 0;        /* 1 means swallow floating windows by default */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const char *fonts[]          = { "monospace:style=Bold:size=10" };
static const char col_gray1[]       = "#2e3440";
static const char col_gray2[]       = "#434c5e";
static const char col_gray3[]       = "#d8dee9";
static const char col_gray4[]       = "#eceff4";
static const char col_cyan[]        = "#81a1c1";
static const char col1[]            = "#3b4252";
static const char col2[]            = "#bf616a";
static const char col3[]            = "#a3be8c";
static const char col4[]            = "#ebcb8b";
static const char col5[]            = "#81a1c1";
static const char col6[]            = "#b48ead";
static const char col7[]            = "#88c0d0";
static const char col8[]            = "#e5e9f0";
static const char col9[]            = "#4c566a";
static const char col10[]           = "#8fbcbb";
static const char col11[]           = "#eceff4";
static const char col12[]           = "#d8dee9";
static const char *colors[][3]      = {
	/*               fg         bg         border   */
	[SchemeNorm]  = { col_gray3, col_gray1, col_gray1 }, // x0b
	[SchemeCol1]  = { col1,      col_gray1, col_gray2 }, // x0c
	[SchemeCol2]  = { col2,      col_gray1, col_gray2 }, // x0d
	[SchemeCol3]  = { col3,      col_gray1, col_gray2 }, // x0e
	[SchemeCol4]  = { col4,      col_gray1, col_gray2 }, // x0f
	[SchemeCol5]  = { col5,      col_gray1, col_gray2 }, // x10
	[SchemeCol6]  = { col6,      col_gray1, col_gray2 }, // x11
	[SchemeCol7]  = { col7,      col_gray1, col_gray2 }, // x12
	[SchemeCol8]  = { col8,      col_gray1, col_gray2 }, // x13
	[SchemeCol9]  = { col9,      col_gray1, col_gray2 }, // x14
	[SchemeCol10] = { col10,     col_gray1, col_gray2 }, // x15
	[SchemeCol11] = { col11,     col_gray1, col_gray2 }, // x16
	[SchemeCol12] = { col12,     col_gray1, col_gray2 }, // x17
	[SchemeSel]   = { col_gray4, col_cyan,  col_cyan  }, // x18
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class		instance	title			tags mask  isfloating  isterminal  noswallow  monitor */
	{ "Firefox",		NULL,		NULL,			1 << 8,    0,          0,          -1,        -1 },
	{ "st",			NULL,		NULL,			0,         0,          1,           0,        -1 },
	{ "st-floating",	NULL,		NULL,			0,         1,          1,           1,        -1 },
	{ "Thunderbird",	"Msgcompose",	NULL,			0,         1,          0,           0,        -1 },
	{ "Steam",		"Steam",	"Steam - News",		0,         1,          0,           0,        -1 },
	{ "Steam",		"Steam",	"Friends List",		0,         1,          0,           0,        -1 },
	{ "Dragon-drag-and-drop",NULL,		NULL,			0,         1,          0,           1,        -1 },
	{ NULL,			NULL,		"Clipboard Editor",	0,         1,          0,           1,        -1 },
	{ NULL,			NULL,		"Event Tester",		0,         0,          0,           1,        -1 }, /* xev */
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
#define MODKEY Mod1Mask
#define SUPER Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-nb", col_gray1, "-nf", col_gray3, "-sb", col_cyan, "-sf", col_gray4, NULL };
static const char *termcmd[]  = { "st", NULL };

// miscellaneous commands
static const char *powermenucmd[] = { "powermenu", NULL };
static const char *lockcmd[]  = { "light-locker-command", "-l", NULL };
static const char *clipeditcmd[] = { "clipedit", NULL };

// program commands
static const char *firefoxcmd[]  = { "firefox-nightly", NULL };
static const char *discordcanarycmd[]  = { "discord-canary", NULL };
static const char *muttcmd[]  = { "st", "-e", "neomutt", NULL };
static const char *rangercmd[]  = { "st", "-e", "ranger", NULL };
static const char *passmenucmd[]  = { "passmenu", NULL };

// dunst commands
static const char *dunstclose[] = { "dunstctl", "close", NULL };
static const char *dunstcloseall[] = { "dunstctl", "close-all", NULL };
static const char *dunsthistory[] = { "dunstctl", "history-pop", NULL };
static const char *dunstcontext[] = { "dunstctl", "context", NULL };

// upload keys
static const char *imupdragcmd[]  = { "imupdrag", NULL };
static const char *imupclipcmd[] = { "imupclip", NULL };

// screenshot commands
static const char *printscrcmd[]  = { "flameshot", "gui", NULL };
static const char *printscrshiftcmd[]  = { "flameshot", "screen", "-c", NULL };

// volume commands
static const char *mutecmd[]  = { "audioctl", "toggle", NULL };
static const char *raisevolcmd[]  = { "audioctl", "up", NULL };
static const char *lowervolcmd[]  = { "audioctl", "down", NULL };

static Key keys[] = {
	/* modifier                     key        function        argument */
	{ MODKEY,                       XK_p,      spawn,          {.v = dmenucmd } },
	{ MODKEY|ShiftMask,             XK_Return, spawn,          {.v = termcmd } },
	{ MODKEY,                       XK_b,      togglebar,      {0} },
	{ MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,      focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_i,      incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_d,      incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },
	{ MODKEY,                       XK_Return, zoom,           {0} },
	{ MODKEY,                       XK_Tab,    view,           {0} },
	{ MODKEY|ShiftMask,             XK_c,      killclient,     {0} },
	{ MODKEY,                       XK_t,      setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                       XK_f,      setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                       XK_m,      setlayout,      {.v = &layouts[2]} },
	{ MODKEY,                       XK_space,  setlayout,      {0} },
	{ MODKEY|ShiftMask,             XK_space,  togglefloating, {0} },
	{ MODKEY|ShiftMask,             XK_f,      togglefullscr,  {0} },
	{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period, focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },
	{ MODKEY,                       XK_minus,  setgaps,        {.i = -1 } },
	{ MODKEY,                       XK_equal,  setgaps,        {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_equal,  setgaps,        {.i = 0  } },
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

	/*                               CUSTOM KEYBINDS                               */
	{ MODKEY|ShiftMask,             XK_p,      spawn,          {.v = powermenucmd } },
	{ MODKEY|ShiftMask,             XK_l,      spawn,          {.v = lockcmd } },

	// miscellaneous
	{ MODKEY|ShiftMask,             XK_e,      spawn,          {.v = clipeditcmd } },

	// program keys
	{ SUPER,                        XK_f,      spawn,          {.v = firefoxcmd } },
	{ SUPER,                        XK_d,      spawn,          {.v = discordcanarycmd } },
	{ SUPER,                        XK_m,      spawn,          {.v = muttcmd } },
	{ SUPER,                        XK_r,      spawn,          {.v = rangercmd } },
	{ SUPER,                        XK_p,      spawn,          {.v = passmenucmd } },

	// dunst keys
	{ SUPER,                        XK_Escape, spawn,          {.v = dunstclose } },
	{ SUPER|ShiftMask,              XK_Escape, spawn,          {.v = dunstcloseall } },
	{ SUPER,                        XK_grave,  spawn,          {.v = dunsthistory } },
	{ SUPER|ShiftMask,              XK_period, spawn,          {.v = dunstcontext } },

	// upload keys
	{ SUPER,                        XK_u,      spawn,          {.v = imupdragcmd } },
	{ SUPER|ShiftMask,              XK_u,      spawn,          {.v = imupclipcmd } },

	// keyboard layout keys
	{ SUPER|MODKEY,                 XK_z,      spawn,          SHCMD("setxkbmap -layout us && sigdwmblocks 2") },
	{ SUPER|MODKEY,                 XK_x,      spawn,          SHCMD("setxkbmap -layout us -variant altgr-intl && sigdwmblocks 2") },

	// screenshot keys
	{ 0,                            XK_Print,  spawn,          {.v = printscrcmd } },
	{ ShiftMask,                    XK_Print,  spawn,          {.v = printscrshiftcmd } },

	// volume keys
	{ 0,              XF86XK_AudioMute,        spawn,          {.v = mutecmd } },
	{ 0,              XF86XK_AudioRaiseVolume, spawn,          {.v = raisevolcmd } },
	{ 0,              XF86XK_AudioLowerVolume, spawn,          {.v = lowervolcmd } },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button1,        sigdwmblocks,   {.i = 1} },
	{ ClkStatusText,        0,              Button2,        sigdwmblocks,   {.i = 2} },
	{ ClkStatusText,        0,              Button3,        sigdwmblocks,   {.i = 3} },
	{ ClkStatusText,        ShiftMask,      Button3,        sigdwmblocks,   {.i = 6} },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};

