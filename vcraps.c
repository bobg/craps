#include <crapslib.h>
#include <curses.h>
#include <setjmp.h>

#define VCRAPS_VERSION (2)
#define VCRAPS_PATCHLEVEL (2)

#define MSGQUEUELEN (20)

extern int      CrapsErrorDatum;
extern int      optind;
extern char    *optarg, *malloc(), *index(), *rindex();

int             MsgQueueLen, MsgQueueLatest;
char           *MsgQueue[MSGQUEUELEN];

char           *ProgramName;

jmp_buf         LoopEnv;

WINDOW         *dontplace4win, *dontplace5win, *dontplace6win, *dontplace8win;
WINDOW         *dontplace9win, *dontplace10win;
WINDOW         *dontcome4win, *dontcome5win, *dontcome6win, *dontcome8win;
WINDOW         *dontcome9win, *dontcome10win;
WINDOW         *place4win, *place5win, *place6win, *place8win, *place9win;
WINDOW         *place10win;
WINDOW         *come4win, *come5win, *come6win, *come8win, *come9win;
WINDOW         *come10win;
WINDOW         *bankrollwin, *pointwin, *passlinewin, *comewin;
WINDOW         *dontpasswin, *dontcomewin, *fieldwin, *big6win, *big8win;
WINDOW         *hard4win, *hard6win, *hard8win, *hard10win;
WINDOW         *oneroll2win, *oneroll3win, *oneroll11win, *oneroll12win;
WINDOW         *any7win, *anycrapswin;
WINDOW         *dicewin;
WINDOW         *promptwin, *commandwin, *msgwin;

char           *BetName[] = {
    "Pass Line", "Pass Line Odds",
    "Come",
    "Come 4", "Come 5", "Come 6", "Come 8", "Come 9", "Come 10",
    "Come 4 Odds", "Come 5 Odds", "Come 6 Odds", "Come 8 Odds",
    "Come 9 Odds", "Come 10 Odds",
    "Don't Pass", "Don't Pass Odds",
    "Don't Come",
    "Don't Come 4", "Don't Come 5", "Don't Come 6",
    "Don't Come 8", "Don't Come 9", "Don't Come 10",
    "Don't Come 4 Odds", "Don't Come 5 Odds", "Don't Come 6 Odds",
    "Don't Come 8 Odds", "Don't Come 9 Odds", "Don't Come 10 Odds",
    "Place 4", "Place 5", "Place 6", "Place 8", "Place 9", "Place 10",
    "Don't Place 4", "Don't Place 5", "Don't Place 6",
    "Don't Place 8", "Don't Place 9", "Don't Place 10",
    "Field", "Big 6", "Big 8", "Hard 4", "Hard 6",
    "Hard 8", "Hard 10", "Any 7", "Any Craps",
    "One-Roll 2", "One-Roll 3", "One-Roll 11", "One-Roll 12"
};

CrapsTable_t    Table, OldTable;

init()
{
    initscr();
    cbreak();
    noecho();
    dontplace4win = newwin(1, 9, 1, 13);
    dontplace5win = newwin(1, 9, 1, 24);
    dontplace6win = newwin(1, 9, 1, 35);
    dontplace8win = newwin(1, 9, 1, 46);
    dontplace9win = newwin(1, 9, 1, 57);
    dontplace10win = newwin(1, 9, 1, 68);
    dontcome4win = newwin(1, 9, 2, 13);
    dontcome5win = newwin(1, 9, 2, 24);
    dontcome6win = newwin(1, 9, 2, 35);
    dontcome8win = newwin(1, 9, 2, 46);
    dontcome9win = newwin(1, 9, 2, 57);
    dontcome10win = newwin(1, 9, 2, 68);
    place4win = newwin(1, 9, 3, 13);
    place5win = newwin(1, 9, 3, 24);
    place6win = newwin(1, 9, 3, 35);
    place8win = newwin(1, 9, 3, 46);
    place9win = newwin(1, 9, 3, 57);
    place10win = newwin(1, 9, 3, 68);
    come4win = newwin(1, 9, 4, 13);
    come5win = newwin(1, 9, 4, 24);
    come6win = newwin(1, 9, 4, 35);
    come8win = newwin(1, 9, 4, 46);
    come9win = newwin(1, 9, 4, 57);
    come10win = newwin(1, 9, 4, 68);
    bankrollwin = newwin(1, 14, 7, 0);
    pointwin = newwin(1, 11, 7, 16);
    passlinewin = newwin(1, 19, 7, 31);
    comewin = newwin(1, 14, 7, 52);
    dontpasswin = newwin(1, 16, 10, 0);
    dontcomewin = newwin(1, 16, 10, 18);
    fieldwin = newwin(1, 11, 10, 38);
    big6win = newwin(1, 11, 10, 53);
    big8win = newwin(1, 11, 10, 66);
    hard4win = newwin(1, 9, 16, 0);
    hard6win = newwin(1, 9, 13, 0);
    hard8win = newwin(1, 9, 13, 11);
    hard10win = newwin(1, 9, 16, 11);
    oneroll2win = newwin(1, 9, 13, 24);
    oneroll3win = newwin(1, 9, 16, 24);
    oneroll11win = newwin(1, 9, 16, 35);
    oneroll12win = newwin(1, 9, 13, 35);
    any7win = newwin(1, 15, 13, 48);
    anycrapswin = newwin(1, 15, 16, 48);
    dicewin = newwin(1, 4, 20, 0);
    promptwin = newwin(1, 70, 19, 6);
    commandwin = newwin(1, 70, 20, 6);
    msgwin = newwin(1, 78, 22, 0);
    Craps_Init(&Table);
    srandom(time(0) + getpid());
}

void            wcenter(win, str, line, highlightp)
WINDOW         *win;
char           *str;
int             line, highlightp;
{
    wmove(win, line, 0);
    wclrtoeol(win);
    wmove(win, line, (int) ((win->_maxx - strlen(str)) / 2));
    if (highlightp)
	wstandout(win);
    waddstr(win, str);
    if (highlightp)
	wstandend(win);
}

main(argc, argv)
int             argc;
char          **argv;
{
    int             c;
    char           *tmp;

    ProgramName = (tmp = rindex(argv[0], '/')) ? tmp + 1 : argv[0];
    MsgQueueLen = 0;
    MsgQueueLatest = -1;
    init();
    Craps_SetBankroll(&Table, 1000);
    Craps_SetOdds(&Table, 2);
    while ((c = getopt(argc, argv, "b:o:sx")) != EOF) {
	switch (c) {
	    case 'b':
		Craps_SetBankroll(&Table, atoi(optarg));
		break;
	    case 'o':
		Craps_SetOdds(&Table, atoi(optarg));
		break;
	    case 's':
		Craps_SetOdds(&Table, 1);
		break;
	    default:
		deinit();
		fprintf(stderr, "Usage: %s [-b bankroll] [-s] [-o odds]\n",
			ProgramName);
		exit(1);
	}
    }
    Driver();
    deinit();
    exit(0);
}

Driver()
{
    int             c, amount, fullupdate = 1, jmpval, die1, die2, i;
    int             msgptr, msgcount;
    int             firsttime = 1;
    char            buf[20], welcomebuf[100];
    CrapsError_t    err;
    CrapsTable_t   *t = &Table;

    if ((jmpval = setjmp(LoopEnv)) == 1)
	Message("ABORTED");
    if (jmpval == 2)
	fullupdate = 1;
    while (1) {
	Update(fullupdate);
	bcopy(&Table, &OldTable, (sizeof(CrapsTable_t)));
	fullupdate = 0;
	if (firsttime) {
	    firsttime = 0;
	    sprintf(welcomebuf,
		    "Welcome to Vcraps version %d.%d (crapslib %d.%d)",
		    VCRAPS_VERSION, VCRAPS_PATCHLEVEL,
		    CRAPSLIB_VERSION, CRAPSLIB_PATCHLEVEL);
	    Message(welcomebuf);
	    Message("Copr. 1990 by Bob Glickstein");
	    Message("Please send comments to bobg@andrew.cmu.edu");
	}
	Prompt("Command [12345689abcdfhmprt$X?]:");
	werase(commandwin);
	wrefresh(commandwin);
	c = Getch("12345689abcdfhmprt$X?", 1);
	switch (c) {
	    case '1':
		Prompt("[012]:");
		c = Getch("012", 1);
		switch (c) {
		    case '0':
			Prompt("[cdp]:");
			c = Getch("cdp", 1);
			switch (c) {
			    case 'c':
				Prompt("Amount for come-odds on 10:");
				amount = ReadNumber();
				if (err = Craps_Bet(&Table, craps_ComeOdds10,
						    amount))
				    Error(err);
				break;
			    case 'd':
				Prompt("[cp]:");
				c = Getch("cp", 1);
				switch (c) {
				    case 'c':
					Prompt("Amount for don't-come odds on 10:");
					amount = ReadNumber();
					if (err = Craps_Bet(&Table,
						    craps_DontComeOdds10,
							    amount))
					    Error(err);
					break;
				    case 'p':
					Prompt("Amount for don't-pass bet on 10:");
					amount = ReadNumber();
					if (err = Craps_Bet(&Table,
						       craps_DontPlace10,
							    amount))
					    Error(err);
					break;
				}
				break;
			    case 'p':
				Prompt("Amount for place bet on 10:");
				amount = ReadNumber();
				if (err = Craps_Bet(&Table, craps_Place10, amount))
				    Error(err);
			}
			break;
		    case '1':
			Prompt("Amount for one-roll bet on 11:");
			amount = ReadNumber();
			if (err = Craps_Bet(&Table, craps_OneRoll11, amount))
			    Error(err);
			break;
		    case '2':
			Prompt("Amount for one-roll bet on 12:");
			amount = ReadNumber();
			if (err = Craps_Bet(&Table, craps_OneRoll12, amount))
			    Error(err);
			break;
		}
		break;
	    case '2':
		Prompt("Amount for one-roll bet on 2:");
		amount = ReadNumber();
		if (err = Craps_Bet(&Table, craps_OneRoll2, amount))
		    Error(err);
		break;
	    case '3':
		Prompt("Amount for one-roll bet on 3:");
		amount = ReadNumber();
		if (err = Craps_Bet(&Table, craps_OneRoll3, amount))
		    Error(err);
		break;
	    case '4':
		Prompt("[cdp]:");
		c = Getch("cdp", 1);
		switch (c) {
		    case 'c':
			Prompt("Amount for come-odds on 4:");
			amount = ReadNumber();
			if (err = Craps_Bet(&Table, craps_ComeOdds4, amount))
			    Error(err);
			break;
		    case 'd':
			Prompt("[cp]:");
			c = Getch("cp", 1);
			switch (c) {
			    case 'c':
				Prompt("Amount for don't-come odds on 4:");
				amount = ReadNumber();
				if (err = Craps_Bet(&Table, craps_DontComeOdds4,
						    amount))
				    Error(err);
				break;
			    case 'p':
				Prompt("Amount for don't-pass bet on 4:");
				amount = ReadNumber();
				if (err = Craps_Bet(&Table, craps_DontPlace4,
						    amount))
				    Error(err);
				break;
			}
			break;
		    case 'p':
			Prompt("Amount for place bet on 4:");
			amount = ReadNumber();
			if (err = Craps_Bet(&Table, craps_Place4, amount))
			    Error(err);
			break;
		}
		break;
	    case '5':
		Prompt("[cdp]:");
		c = Getch("cdp", 1);
		switch (c) {
		    case 'c':
			Prompt("Amount for come-odds on 5:");
			amount = ReadNumber();
			if (err = Craps_Bet(&Table, craps_ComeOdds5, amount))
			    Error(err);
			break;
		    case 'd':
			Prompt("[cp]:");
			c = Getch("cp", 1);
			switch (c) {
			    case 'c':
				Prompt("Amount for don't-come odds on 5:");
				amount = ReadNumber();
				if (err = Craps_Bet(&Table, craps_DontComeOdds5,
						    amount))
				    Error(err);
				break;
			    case 'p':
				Prompt("Amount for don't-pass bet on 5:");
				amount = ReadNumber();
				if (err = Craps_Bet(&Table, craps_DontPlace5,
						    amount))
				    Error(err);
				break;
			}
			break;
		    case 'p':
			Prompt("Amount for place bet on 5:");
			amount = ReadNumber();
			if (err = Craps_Bet(&Table, craps_Place5, amount))
			    Error(err);
			break;
		}
		break;
	    case '6':
		Prompt("[cdp]:");
		c = Getch("cdp", 1);
		switch (c) {
		    case 'c':
			Prompt("Amount for come-odds on 6:");
			amount = ReadNumber();
			if (err = Craps_Bet(&Table, craps_ComeOdds6, amount))
			    Error(err);
			break;
		    case 'd':
			Prompt("[cp]:");
			c = Getch("cp", 1);
			switch (c) {
			    case 'c':
				Prompt("Amount for don't-come odds on 6:");
				amount = ReadNumber();
				if (err = Craps_Bet(&Table, craps_DontComeOdds6,
						    amount))
				    Error(err);
				break;
			    case 'p':
				Prompt("Amount for don't-pass bet on 6:");
				amount = ReadNumber();
				if (err = Craps_Bet(&Table, craps_DontPlace6,
						    amount))
				    Error(err);
				break;
			}
			break;
		    case 'p':
			Prompt("Amount for place bet on 6:");
			amount = ReadNumber();
			if (err = Craps_Bet(&Table, craps_Place6, amount))
			    Error(err);
			break;
		}
		break;
	    case '8':
		Prompt("[cdp]:");
		c = Getch("cdp", 1);
		switch (c) {
		    case 'c':
			Prompt("Amount for come-odds on 8:");
			amount = ReadNumber();
			if (err = Craps_Bet(&Table, craps_ComeOdds8, amount))
			    Error(err);
			break;
		    case 'd':
			Prompt("[cp]:");
			c = Getch("cp", 1);
			switch (c) {
			    case 'c':
				Prompt("Amount for don't-come odds on 8:");
				amount = ReadNumber();
				if (err = Craps_Bet(&Table, craps_DontComeOdds8,
						    amount))
				    Error(err);
				break;
			    case 'p':
				Prompt("Amount for don't-pass bet on 8:");
				amount = ReadNumber();
				if (err = Craps_Bet(&Table, craps_DontPlace8,
						    amount))
				    Error(err);
				break;
			}
			break;
		    case 'p':
			Prompt("Amount for place bet on 8:");
			amount = ReadNumber();
			if (err = Craps_Bet(&Table, craps_Place8, amount))
			    Error(err);
			break;
		}
		break;
	    case '9':
		Prompt("[cdp]:");
		c = Getch("cdp", 1);
		switch (c) {
		    case 'c':
			Prompt("Amount for come-odds on 9:");
			amount = ReadNumber();
			if (err = Craps_Bet(&Table, craps_ComeOdds9, amount))
			    Error(err);
			break;
		    case 'd':
			Prompt("[cp]:");
			c = Getch("cp", 1);
			switch (c) {
			    case 'c':
				Prompt("Amount for don't-come odds on 9:");
				amount = ReadNumber();
				if (err = Craps_Bet(&Table, craps_DontComeOdds9,
						    amount))
				    Error(err);
				break;
			    case 'p':
				Prompt("Amount for don't-pass bet on 9:");
				amount = ReadNumber();
				if (err = Craps_Bet(&Table, craps_DontPlace9,
						    amount))
				    Error(err);
				break;
			}
			break;
		    case 'p':
			Prompt("Amount for place bet on 9:");
			amount = ReadNumber();
			if (err = Craps_Bet(&Table, craps_Place9, amount))
			    Error(err);
			break;
		}
		break;
	    case 'a':
		Prompt("[7c]:");
		c = Getch("7c", 1);
		switch (c) {
		    case '7':
			Prompt("Amount for bet on Any 7:");
			amount = ReadNumber();
			if (err = Craps_Bet(&Table, craps_Any7, amount))
			    Error(err);
			break;
		    case 'c':
			Prompt("Amount for bet on Any Craps:");
			amount = ReadNumber();
			if (err = Craps_Bet(&Table, craps_AnyCraps, amount))
			    Error(err);
			break;
		}
		break;
	    case 'b':
		Prompt("[1468]:");
		c = Getch("1468", 1);
		switch (c) {
		    case '1':
			Prompt("[0]:");
			Getch("0", 1);
			if (err = Craps_Buy(t, 10))
			    Error(err);
			break;
		    case '4':
			if (err = Craps_Buy(t, 4))
			    Error(err);
			break;
		    case '6':
			Prompt("Amount for bet on Big 6:");
			amount = ReadNumber();
			if (err = Craps_Bet(&Table, craps_Big6, amount))
			    Error(err);
			break;
		    case '8':
			Prompt("Amount for bet on Big 8:");
			amount = ReadNumber();
			if (err = Craps_Bet(&Table, craps_Big8, amount))
			    Error(err);
			break;
		}
		break;
	    case 'c':
		Prompt("Amount for bet on Come:");
		amount = ReadNumber();
		if (err = Craps_Bet(&Table, craps_ComeBox, amount))
		    Error(err);
		break;
	    case 'd':
		Prompt("[cp]:");
		c = Getch("cp", 1);
		switch (c) {
		    case 'c':
			Prompt("Amount for bet on Don't Come:");
			amount = ReadNumber();
			if (err = Craps_Bet(&Table, craps_DontComeBox, amount))
			    Error(err);
			break;
		    case 'p':
			if (Craps_Point(&Table)) {
			    Prompt("Amount for odds on Don't Pass:");
			    amount = ReadNumber();
			    if (err = Craps_Bet(&Table, craps_DontPassOdds, amount))
				Error(err);
			}
			else {
			    Prompt("Amount for bet on Don't Pass:");
			    amount = ReadNumber();
			    if (err = Craps_Bet(&Table, craps_DontPass, amount))
				Error(err);
			}
			break;
		}
		break;
	    case 'f':
		Prompt("Amount for bet on Field:");
		amount = ReadNumber();
		if (err = Craps_Bet(&Table, craps_Field, amount))
		    Error(err);
		break;
	    case 'h':
		Prompt("[2345]:");
		c = Getch("2345", 1);
		switch (c) {
		    case '2':
			Prompt("[2]:");
			Getch("2", 1);
			Prompt("Amount for bet on Hard 4:");
			amount = ReadNumber();
			if (err = Craps_Bet(&Table, craps_Hard4, amount))
			    Error(err);
			break;
		    case '3':
			Prompt("[3]:");
			Getch("3", 1);
			Prompt("Amount for bet on Hard 6:");
			amount = ReadNumber();
			if (err = Craps_Bet(&Table, craps_Hard6, amount))
			    Error(err);
			break;
		    case '4':
			Prompt("[4]:");
			Getch("4", 1);
			Prompt("Amount for bet on Hard 8:");
			amount = ReadNumber();
			if (err = Craps_Bet(&Table, craps_Hard8, amount))
			    Error(err);
			break;
		    case '5':
			Prompt("[5]:");
			Getch("5", 1);
			Prompt("Amount for bet on Hard 10:");
			amount = ReadNumber();
			if (err = Craps_Bet(&Table, craps_Hard10, amount))
			    Error(err);
			break;
		}
		break;
	    case 'm':
		msgptr = MsgQueueLatest;
		msgcount = 0;
		do {
		    if (++msgcount > MsgQueueLen)
			c = ' ';
		    else {
			wcenter(msgwin, MsgQueue[msgptr], 0, 0);
			wrefresh(msgwin);
			msgptr = (msgptr - 1 + MSGQUEUELEN) % MSGQUEUELEN;
			c = wgetch(msgwin);
			werase(msgwin);
		    }
		} while (c != ' ');
		wrefresh(msgwin);
		break;
	    case 'p':
		if (Craps_Point(&Table)) {
		    Prompt("Amount for odds on Pass Line:");
		    amount = ReadNumber();
		    if (err = Craps_Bet(&Table, craps_PassLineOdds, amount))
			Error(err);
		}
		else {
		    Prompt("Amount for bet on Pass Line:");
		    amount = ReadNumber();
		    if (err = Craps_Bet(&Table, craps_PassLine, amount))
			Error(err);
		}
		break;
	    case 'r':
		Prompt("ROLL");
		for (i = (random() % 40) + 40; i >= 0; --i) {
		    die1 = 1 + (random() % 6);
		    die2 = 1 + (random() % 6);
		    sprintf(buf, "%d  %d", die1, die2);
		    wcenter(dicewin, buf, 0, 1);
		    wrefresh(dicewin);
		}
		Craps_Update(&Table, die1, die2);
		break;
	    case 't':
		Prompt("[12345689abcdfhp]:");
		c = Getch("12345689abcdfhp", 1);
		switch (c) {
		    case '1':
			Prompt("[012]:");
			c = Getch("012", 1);
			switch (c) {
			    case '0':
				Prompt("[cdp]:");
				c = Getch("cdp", 1);
				switch (c) {
				    case 'c':
					if (err = Craps_TakeDown(t,
						       craps_ComeOdds10))
					    Error(err);
					break;
				    case 'd':
					Prompt("[cp]:");
					c = Getch("cp", 1);
					switch (c) {
					    case 'c':
						if (err = Craps_TakeDown(t,
						   craps_DontComeOdds10))
						    Error(err);
						break;
					    case 'p':
						if (err = Craps_TakeDown(t,
						      craps_DontPlace10))
						    Error(err);
						break;
					}
					break;
				    case 'p':
					if (err = Craps_TakeDown(t, craps_Place10))
					    Error(err);
					break;
				}
				break;
			    case '1':
				if (err = Craps_TakeDown(t, craps_OneRoll11))
				    Error(err);
				break;
			    case '2':
				if (err = Craps_TakeDown(t, craps_OneRoll12))
				    Error(err);
				break;
			}
			break;
		    case '2':
			if (err = Craps_TakeDown(t, craps_OneRoll2))
			    Error(err);
			break;
		    case '3':
			if (err = Craps_TakeDown(t, craps_OneRoll3))
			    Error(err);
			break;
		    case '4':
			Prompt("[cdp]:");
			c = Getch("cdp", 1);
			switch (c) {
			    case 'c':
				if (err = Craps_TakeDown(t, craps_ComeOdds4))
				    Error(err);
				break;
			    case 'd':
				Prompt("[cp]:");
				c = Getch("cp", 1);
				switch (c) {
				    case 'c':
					if (err = Craps_TakeDown(t,
						    craps_DontComeOdds4))
					    Error(err);
					break;
				    case 'p':
					if (err = Craps_TakeDown(t,
						       craps_DontPlace4))
					    Error(err);
					break;
				}
				break;
			    case 'p':
				if (err = Craps_TakeDown(t, craps_Place4))
				    Error(err);
				break;
			}
			break;
		    case '5':
			Prompt("[cdp]:");
			c = Getch("cdp", 1);
			switch (c) {
			    case 'c':
				if (err = Craps_TakeDown(t, craps_ComeOdds5))
				    Error(err);
				break;
			    case 'd':
				Prompt("[cp]:");
				c = Getch("cp", 1);
				switch (c) {
				    case 'c':
					if (err = Craps_TakeDown(t,
						    craps_DontComeOdds5))
					    Error(err);
					break;
				    case 'p':
					if (err = Craps_TakeDown(t,
						       craps_DontPlace5))
					    Error(err);
					break;
				}
				break;
			    case 'p':
				if (err = Craps_TakeDown(t, craps_Place5))
				    Error(err);
				break;
			}
			break;
		    case '6':
			Prompt("[cdp]:");
			c = Getch("cdp", 1);
			switch (c) {
			    case 'c':
				if (err = Craps_TakeDown(t, craps_ComeOdds6))
				    Error(err);
				break;
			    case 'd':
				Prompt("[cp]:");
				c = Getch("cp", 1);
				switch (c) {
				    case 'c':
					if (err = Craps_TakeDown(t,
						    craps_DontComeOdds6))
					    Error(err);
					break;
				    case 'p':
					if (err = Craps_TakeDown(t,
						       craps_DontPlace6))
					    Error(err);
					break;
				}
				break;
			    case 'p':
				if (err = Craps_TakeDown(t, craps_Place6))
				    Error(err);
				break;
			}
			break;
		    case '8':
			Prompt("[cdp]:");
			c = Getch("cdp", 1);
			switch (c) {
			    case 'c':
				if (err = Craps_TakeDown(t, craps_ComeOdds8))
				    Error(err);
				break;
			    case 'd':
				Prompt("[cp]:");
				c = Getch("cp", 1);
				switch (c) {
				    case 'c':
					if (err = Craps_TakeDown(t,
						    craps_DontComeOdds8))
					    Error(err);
					break;
				    case 'p':
					if (err = Craps_TakeDown(t,
						       craps_DontPlace8))
					    Error(err);
					break;
				}
				break;
			    case 'p':
				if (err = Craps_TakeDown(t, craps_Place8))
				    Error(err);
				break;
			}
			break;
		    case '9':
			Prompt("[cdp]:");
			c = Getch("cdp", 1);
			switch (c) {
			    case 'c':
				if (err = Craps_TakeDown(t, craps_ComeOdds9))
				    Error(err);
				break;
			    case 'd':
				Prompt("[cp]:");
				c = Getch("cp", 1);
				switch (c) {
				    case 'c':
					if (err = Craps_TakeDown(t,
						    craps_DontComeOdds9))
					    Error(err);
					break;
				    case 'p':
					if (err = Craps_TakeDown(t,
						       craps_DontPlace9))
					    Error(err);
					break;
				}
				break;
			    case 'p':
				if (err = Craps_TakeDown(t, craps_Place9))
				    Error(err);
				break;
			}
			break;
		    case 'a':
			Prompt("[7c]:");
			c = Getch("7c", 1);
			switch (c) {
			    case '7':
				if (err = Craps_TakeDown(t, craps_Any7))
				    Error(err);
				break;
			    case 'c':
				if (err = Craps_TakeDown(t, craps_AnyCraps))
				    Error(err);
				break;
			}
			break;
		    case 'b':
			Prompt("[68]:");
			c = Getch("68", 1);
			switch (c) {
			    case '6':
				if (err = Craps_TakeDown(t, craps_Big6))
				    Error(err);
				break;
			    case '8':
				if (err = Craps_TakeDown(t, craps_Big8))
				    Error(err);
				break;
			}
			break;
		    case 'c':
			if (err = Craps_TakeDown(t, craps_ComeBox))
			    Error(err);
			break;
		    case 'd':
			Prompt("[cp]:");
			c = Getch("cp", 1);
			switch (c) {
			    case 'c':
				if (err = Craps_TakeDown(t, craps_DontComeBox))
				    Error(err);
				break;
			    case 'p':
				if (err = Craps_TakeDown(t,
							 Craps_Point(t) ?
						     craps_DontPassOdds :
							 craps_DontPass))
				    Error(err);
				break;
			}
			break;
		    case 'f':
			if (err = Craps_TakeDown(t, craps_Field))
			    Error(err);
			break;
		    case 'h':
			Prompt("[2345]:");
			c = Getch("2345", 1);
			switch (c) {
			    case '2':
				Prompt("[2]:");
				Getch("2", 1);
				if (err = Craps_TakeDown(t, craps_Hard4))
				    Error(err);
				break;
			    case '3':
				Prompt("[3]:");
				Getch("3", 1);
				if (err = Craps_TakeDown(t, craps_Hard6))
				    Error(err);
				break;
			    case '4':
				Prompt("[4]:");
				Getch("4", 1);
				if (err = Craps_TakeDown(t, craps_Hard8))
				    Error(err);
				break;
			    case '5':
				Prompt("[5]:");
				Getch("5", 1);
				if (err = Craps_TakeDown(t, craps_Hard10))
				    Error(err);
				break;
			}
			break;
		    case 'p':
			if (err = Craps_TakeDown(t,
						 Craps_Point(t) ?
						 craps_PassLineOdds :
						 craps_PassLine))
			    Error(err);
			break;
		}
		break;
	    case '$':
		ShowTotal();
		break;
	    case 'X':
		return (0);
	    case '?':
		Help();
		fullupdate = 1;
		break;
	}
    }
}

Message(str)
char           *str;
{
    char           *copy = malloc(1 + strlen(str));
    int             c, msgptr, msgcount;

    if (copy) {
	strcpy(copy, str);
	MsgQueueLatest = (MsgQueueLatest + 1) % MSGQUEUELEN;
	if (MsgQueueLen == MSGQUEUELEN)
	    free(MsgQueue[MsgQueueLatest]);
	else
	    ++MsgQueueLen;
	MsgQueue[MsgQueueLatest] = copy;
    }
    msgptr = (MsgQueueLatest - 1 + MSGQUEUELEN) % MSGQUEUELEN;
    msgcount = 1;
    wcenter(msgwin, str, 0, 0);
    wrefresh(msgwin);
    do {
	c = wgetch(msgwin);
	if (c == 'm') {
	    if (++msgcount > MsgQueueLen)
		c = ' ';
	    else {
		wcenter(msgwin, MsgQueue[msgptr], 0, 0);
		wrefresh(msgwin);
		msgptr = (msgptr - 1 + MSGQUEUELEN) % MSGQUEUELEN;
	    }
	}
    } while (c != ' ');
    werase(msgwin);
    wrefresh(msgwin);
}

Prompt(str)
char           *str;
{
    werase(promptwin);
    wmove(promptwin, 0, 0);
    waddstr(promptwin, str);
    wrefresh(promptwin);
}

int             Getch(key, echop)
char           *key;
int             echop;
{
    int             c;

    do {
	c = wgetch(commandwin);
	if (c == 27)
	    longjmp(LoopEnv, 1);
	if (c == 12)
	    longjmp(LoopEnv, 2);
	if ((c == 127) && index(key, '\b'))
	    break;
    } while (!index(key, c));
    if (echop) {
	waddch(commandwin, c);
	wrefresh(commandwin);
    }
    return (c);
}

deinit()
{
    clear();
    refresh();
    endwin();
}

void            Craps_ReportPayoff(t, bet, payoff, win)
CrapsTable_t   *t;
CrapsBet_t      bet;
CrapsPayoff_t  *payoff;
int             win;
{
    char            buf[100];

    sprintf(buf, "Casino pays $%d %s bet at %d-%d; you collect $%d",
	    Craps_GetBet(t, bet), BetName[(int) bet], payoff->numerator,
	    payoff->denominator, win + Craps_GetBet(t, bet));
    Message(buf);
}

void            Craps_ReportCollect(t, bet)
CrapsTable_t   *t;
CrapsBet_t      bet;
{
}

void            Craps_ReportReturn(t, bet)
CrapsTable_t   *t;
CrapsBet_t      bet;
{
    char            buf[100];

    sprintf(buf, "Casino returns $%d %s bet",
	    Craps_GetBet(t, bet), BetName[(int) bet]);
    Message(buf);
}

void            Craps_ReportCommission(t, amount)
CrapsTable_t   *t;
int             amount;
{
    char            buf[100];

    sprintf(buf, "Casino takes $%d commission", amount);
    Message(buf);
}

void            Craps_ReportMove(t, from, to)
CrapsTable_t   *t;
CrapsBet_t      from, to;
{
}

Update(full)
int             full;
{
    char            buf[80];
    int             num;

    if (full) {
	clear();
	move(0, 0);
	addstr("	     ----4----  ----5----  ----6----  ----8----  ----9----  ---10----");
	move(1, 0);
	addstr("DON'T PLACE");
	move(2, 0);
	addstr(" DON'T COME");
	move(3, 0);
	addstr("      PLACE");
	move(4, 0);
	addstr("       COME");
	move(6, 0);
	addstr("---Bankroll---  ---Point---    -----Pass Line-----  -----Come-----");
	move(9, 0);
	addstr("---Don't Pass---  ---Don't Come---    ---Field---    ---Big 6---  ---Big 8---");
	move(12, 0);
	addstr("---3-3---  ---4-4---    ---1-1---  ---6-6---    ---Any Seven---");
	move(15, 0);
	addstr("---2-2---  ---5-5---    ---1-2---  ---5-6---	---Any Craps---");
	move(18, 0);
	addstr("=============================================================================");
	move(19, 0);
	addstr("DICE");
	move(21, 0);
	addstr("=============================================================================");
	refresh();
    }
    if (full || (Craps_GetBet(&Table, craps_DontPlace4) != Craps_GetBet(&OldTable, craps_DontPlace4))) {
	if (num = Craps_GetBet(&Table, craps_DontPlace4)) {
	    sprintf(buf, " %d ", num);
	    wcenter(dontplace4win, buf, 0, 1);
	}
	else {
	    werase(dontplace4win);
	}
	wrefresh(dontplace4win);
    }
    if (full || (Craps_GetBet(&Table, craps_DontPlace5) !=
		 Craps_GetBet(&OldTable, craps_DontPlace5))) {
	if (num = Craps_GetBet(&Table, craps_DontPlace5)) {
	    sprintf(buf, " %d ", num);
	    wcenter(dontplace5win, buf, 0, 1);
	}
	else {
	    werase(dontplace5win);
	}
	wrefresh(dontplace5win);
    }
    if (full || (Craps_GetBet(&Table, craps_DontPlace6) !=
		 Craps_GetBet(&OldTable, craps_DontPlace6))) {
	if (num = Craps_GetBet(&Table, craps_DontPlace6)) {
	    sprintf(buf, " %d ", num);
	    wcenter(dontplace6win, buf, 0, 1);
	}
	else {
	    werase(dontplace6win);
	}
	wrefresh(dontplace6win);
    }
    if (full || (Craps_GetBet(&Table, craps_DontPlace8) !=
		 Craps_GetBet(&OldTable, craps_DontPlace8))) {
	if (num = Craps_GetBet(&Table, craps_DontPlace8)) {
	    sprintf(buf, " %d ", num);
	    wcenter(dontplace8win, buf, 0, 1);
	}
	else {
	    werase(dontplace8win);
	}
	wrefresh(dontplace8win);
    }
    if (full || (Craps_GetBet(&Table, craps_DontPlace9) !=
		 Craps_GetBet(&OldTable, craps_DontPlace9))) {
	if (num = Craps_GetBet(&Table, craps_DontPlace9)) {
	    sprintf(buf, " %d ", num);
	    wcenter(dontplace9win, buf, 0, 1);
	}
	else {
	    werase(dontplace9win);
	}
	wrefresh(dontplace9win);
    }
    if (full || (Craps_GetBet(&Table, craps_DontPlace10) !=
		 Craps_GetBet(&OldTable, craps_DontPlace10))) {
	if (num = Craps_GetBet(&Table, craps_DontPlace10)) {
	    sprintf(buf, " %d ", num);
	    wcenter(dontplace10win, buf, 0, 1);
	}
	else {
	    werase(dontplace10win);
	}
	wrefresh(dontplace10win);
    }
    if (full || (Craps_GetBet(&Table, craps_DontCome4) !=
		 Craps_GetBet(&OldTable, craps_DontCome4)) ||
	(Craps_GetBet(&Table, craps_DontComeOdds4) !=
	 Craps_GetBet(&OldTable, craps_DontComeOdds4))) {
	if (num = Craps_GetBet(&Table, craps_DontCome4)) {
	    if (Craps_GetBet(&Table, craps_DontComeOdds4))
		sprintf(buf, " %d/%d ", num,
			Craps_GetBet(&Table, craps_DontComeOdds4));
	    else
		sprintf(buf, " %d ", num);
	    wcenter(dontcome4win, buf, 0, 1);
	}
	else {
	    werase(dontcome4win);
	}
	wrefresh(dontcome4win);
    }
    if (full || (Craps_GetBet(&Table, craps_DontCome5) !=
		 Craps_GetBet(&OldTable, craps_DontCome5)) ||
	(Craps_GetBet(&Table, craps_DontComeOdds5) !=
	 Craps_GetBet(&OldTable, craps_DontComeOdds5))) {
	if (num = Craps_GetBet(&Table, craps_DontCome5)) {
	    if (Craps_GetBet(&Table, craps_DontComeOdds5))
		sprintf(buf, " %d/%d ", num,
			Craps_GetBet(&Table, craps_DontComeOdds5));
	    else
		sprintf(buf, " %d ", num);
	    wcenter(dontcome5win, buf, 0, 1);
	}
	else {
	    werase(dontcome5win);
	}
	wrefresh(dontcome5win);
    }
    if (full || (Craps_GetBet(&Table, craps_DontCome6) !=
		 Craps_GetBet(&OldTable, craps_DontCome6)) ||
	(Craps_GetBet(&Table, craps_DontComeOdds6) !=
	 Craps_GetBet(&OldTable, craps_DontComeOdds6))) {
	if (num = Craps_GetBet(&Table, craps_DontCome6)) {
	    if (Craps_GetBet(&Table, craps_DontComeOdds6))
		sprintf(buf, " %d/%d ", num,
			Craps_GetBet(&Table, craps_DontComeOdds6));
	    else
		sprintf(buf, " %d ", num);
	    wcenter(dontcome6win, buf, 0, 1);
	}
	else {
	    werase(dontcome6win);
	}
	wrefresh(dontcome6win);
    }
    if (full || (Craps_GetBet(&Table, craps_DontCome8) !=
		 Craps_GetBet(&OldTable, craps_DontCome8)) ||
	(Craps_GetBet(&Table, craps_DontComeOdds8) !=
	 Craps_GetBet(&OldTable, craps_DontComeOdds8))) {
	if (num = Craps_GetBet(&Table, craps_DontCome8)) {
	    if (Craps_GetBet(&Table, craps_DontComeOdds8))
		sprintf(buf, " %d/%d ", num,
			Craps_GetBet(&Table, craps_DontComeOdds8));
	    else
		sprintf(buf, " %d ", num);
	    wcenter(dontcome8win, buf, 0, 1);
	}
	else {
	    werase(dontcome8win);
	}
	wrefresh(dontcome8win);
    }
    if (full || (Craps_GetBet(&Table, craps_DontCome9) !=
		 Craps_GetBet(&OldTable, craps_DontCome9)) ||
	(Craps_GetBet(&Table, craps_DontComeOdds9) !=
	 Craps_GetBet(&OldTable, craps_DontComeOdds9))) {
	if (num = Craps_GetBet(&Table, craps_DontCome9)) {
	    if (Craps_GetBet(&Table, craps_DontComeOdds9))
		sprintf(buf, " %d/%d ", num,
			Craps_GetBet(&Table, craps_DontComeOdds9));
	    else
		sprintf(buf, " %d ", num);
	    wcenter(dontcome9win, buf, 0, 1);
	}
	else {
	    werase(dontcome9win);
	}
	wrefresh(dontcome9win);
    }
    if (full || (Craps_GetBet(&Table, craps_DontCome10) !=
		 Craps_GetBet(&OldTable, craps_DontCome10)) ||
	(Craps_GetBet(&Table, craps_DontComeOdds10) !=
	 Craps_GetBet(&OldTable, craps_DontComeOdds10))) {
	if (num = Craps_GetBet(&Table, craps_DontCome10)) {
	    if (Craps_GetBet(&Table, craps_DontComeOdds10))
		sprintf(buf, " %d/%d ", num,
			Craps_GetBet(&Table, craps_DontComeOdds10));
	    else
		sprintf(buf, " %d ", num);
	    wcenter(dontcome10win, buf, 0, 1);
	}
	else {
	    werase(dontcome10win);
	}
	wrefresh(dontcome10win);
    }
    if (full || (Craps_GetBet(&Table, craps_Place4) !=
		 Craps_GetBet(&OldTable, craps_Place4)) ||
	(Craps_Bought4(&Table) != Craps_Bought4(&OldTable))) {
	if (num = Craps_GetBet(&Table, craps_Place4)) {
	    sprintf(buf, " %s%d ", Craps_Bought4(&Table) ? "buy " : "", num);
	    wcenter(place4win, buf, 0, 1);
	}
	else {
	    werase(place4win);
	}
	wrefresh(place4win);
    }
    if (full || (Craps_GetBet(&Table, craps_Place5) !=
		 Craps_GetBet(&OldTable, craps_Place5))) {
	if (num = Craps_GetBet(&Table, craps_Place5)) {
	    sprintf(buf, " %d ", num);
	    wcenter(place5win, buf, 0, 1);
	}
	else {
	    werase(place5win);
	}
	wrefresh(place5win);
    }
    if (full || (Craps_GetBet(&Table, craps_Place6) !=
		 Craps_GetBet(&OldTable, craps_Place6))) {
	if (num = Craps_GetBet(&Table, craps_Place6)) {
	    sprintf(buf, " %d ", num);
	    wcenter(place6win, buf, 0, 1);
	}
	else {
	    werase(place6win);
	}
	wrefresh(place6win);
    }
    if (full || (Craps_GetBet(&Table, craps_Place8) !=
		 Craps_GetBet(&OldTable, craps_Place8))) {
	if (num = Craps_GetBet(&Table, craps_Place8)) {
	    sprintf(buf, " %d ", num);
	    wcenter(place8win, buf, 0, 1);
	}
	else {
	    werase(place8win);
	}
	wrefresh(place8win);
    }
    if (full || (Craps_GetBet(&Table, craps_Place9) !=
		 Craps_GetBet(&OldTable, craps_Place9))) {
	if (num = Craps_GetBet(&Table, craps_Place9)) {
	    sprintf(buf, " %d ", num);
	    wcenter(place9win, buf, 0, 1);
	}
	else {
	    werase(place9win);
	}
	wrefresh(place9win);
    }
    if (full || (Craps_GetBet(&Table, craps_Place10) !=
		 Craps_GetBet(&OldTable, craps_Place10)) ||
	(Craps_Bought10(&Table) != Craps_Bought10(&Table))) {
	if (num = Craps_GetBet(&Table, craps_Place10)) {
	    sprintf(buf, " %s%d ", Craps_Bought10(&Table) ? "buy " : "", num);
	    wcenter(place10win, buf, 0, 1);
	}
	else {
	    werase(place10win);
	}
	wrefresh(place10win);
    }
    if (full || (Craps_GetBet(&Table, craps_Come4) !=
		 Craps_GetBet(&OldTable, craps_Come4)) ||
	(Craps_GetBet(&Table, craps_ComeOdds4) !=
	 Craps_GetBet(&OldTable, craps_ComeOdds4)) ||
	((Craps_Point(&Table) == 4) && (Craps_Point(&OldTable) !=
					4)) ||
	((Craps_Point(&Table) != 4) && (Craps_Point(&OldTable) == 4))) {
	if (num = Craps_GetBet(&Table, craps_Come4)) {
	    if (Craps_GetBet(&Table, craps_ComeOdds4))
		sprintf(buf, " %d/%d ", num,
			Craps_GetBet(&Table, craps_ComeOdds4));
	    else
		sprintf(buf, " %d ", num);
	    wcenter(come4win, buf, 0, 1);
	}
	else {
	    if (Craps_Point(&Table) == 4)
		wcenter(come4win, "*****", 0, 1);
	    else
		werase(come4win);
	}
	wrefresh(come4win);
    }
    if (full || (Craps_GetBet(&Table, craps_Come5) !=
		 Craps_GetBet(&OldTable, craps_Come5)) ||
	(Craps_GetBet(&Table, craps_ComeOdds5) !=
	 Craps_GetBet(&OldTable, craps_ComeOdds5)) ||
	((Craps_Point(&Table) == 5) && (Craps_Point(&OldTable) !=
					5)) ||
	((Craps_Point(&Table) != 5) && (Craps_Point(&OldTable) == 5))) {
	if (num = Craps_GetBet(&Table, craps_Come5)) {
	    if (Craps_GetBet(&Table, craps_ComeOdds5))
		sprintf(buf, " %d/%d ", num,
			Craps_GetBet(&Table, craps_ComeOdds5));
	    else
		sprintf(buf, " %d ", num);
	    wcenter(come5win, buf, 0, 1);
	}
	else {
	    if (Craps_Point(&Table) == 5)
		wcenter(come5win, "*****", 0, 1);
	    else
		werase(come5win);
	}
	wrefresh(come5win);
    }
    if (full || (Craps_GetBet(&Table, craps_Come6) !=
		 Craps_GetBet(&OldTable, craps_Come6)) ||
	(Craps_GetBet(&Table, craps_ComeOdds6) !=
	 Craps_GetBet(&OldTable, craps_ComeOdds6)) ||
	((Craps_Point(&Table) == 6) && (Craps_Point(&OldTable) != 6)) ||
	((Craps_Point(&Table) != 6) && (Craps_Point(&OldTable) == 6))) {
	if (num = Craps_GetBet(&Table, craps_Come6)) {
	    if (Craps_GetBet(&Table, craps_ComeOdds6))
		sprintf(buf, " %d/%d ", num,
			Craps_GetBet(&Table, craps_ComeOdds6));
	    else
		sprintf(buf, " %d ", num);
	    wcenter(come6win, buf, 0, 1);
	}
	else {
	    if (Craps_Point(&Table) == 6)
		wcenter(come6win, "*****", 0, 1);
	    else
		werase(come6win);
	}
	wrefresh(come6win);
    }
    if (full || (Craps_GetBet(&Table, craps_Come8) !=
		 Craps_GetBet(&OldTable, craps_Come8)) ||
	(Craps_GetBet(&Table, craps_ComeOdds8) !=
	 Craps_GetBet(&OldTable, craps_ComeOdds8)) ||
	((Craps_Point(&Table) == 8) && (Craps_Point(&OldTable) !=
					8)) ||
	((Craps_Point(&Table) != 8) && (Craps_Point(&OldTable) == 8))) {
	if (num = Craps_GetBet(&Table, craps_Come8)) {
	    if (Craps_GetBet(&Table, craps_ComeOdds8))
		sprintf(buf, " %d/%d ", num,
			Craps_GetBet(&Table, craps_ComeOdds8));
	    else
		sprintf(buf, " %d ", num);
	    wcenter(come8win, buf, 0, 1);
	}
	else {
	    if (Craps_Point(&Table) == 8)
		wcenter(come8win, "*****", 0, 1);
	    else
		werase(come8win);
	}
	wrefresh(come8win);
    }
    if (full || (Craps_GetBet(&Table, craps_Come9) !=
		 Craps_GetBet(&OldTable, craps_Come9)) ||
	(Craps_GetBet(&Table, craps_ComeOdds9) !=
	 Craps_GetBet(&OldTable, craps_ComeOdds9)) ||
	((Craps_Point(&Table) == 9) && (Craps_Point(&OldTable) != 9)) ||
	((Craps_Point(&Table) != 9) && (Craps_Point(&OldTable) == 9))) {
	if (num = Craps_GetBet(&Table, craps_Come9)) {
	    if (Craps_GetBet(&Table, craps_ComeOdds9))
		sprintf(buf, " %d/%d ", num,
			Craps_GetBet(&Table, craps_ComeOdds9));
	    else
		sprintf(buf, " %d ", num);
	    wcenter(come9win, buf, 0, 1);
	}
	else {
	    if (Craps_Point(&Table) == 9)
		wcenter(come9win, "*****", 0, 1);
	    else
		werase(come9win);
	}
	wrefresh(come9win);
    }
    if (full || (Craps_GetBet(&Table, craps_Come10) !=
		 Craps_GetBet(&OldTable, craps_Come10)) ||
	(Craps_GetBet(&Table, craps_ComeOdds10) !=
	 Craps_GetBet(&OldTable, craps_ComeOdds10)) ||
	((Craps_Point(&Table) == 10) && (Craps_Point(&OldTable) != 10)) ||
	((Craps_Point(&Table) != 10) && (Craps_Point(&OldTable) == 10))) {
	if (num = Craps_GetBet(&Table, craps_Come10)) {
	    if (Craps_GetBet(&Table, craps_ComeOdds10))
		sprintf(buf, " %d/%d ", num,
			Craps_GetBet(&Table, craps_ComeOdds10));
	    else
		sprintf(buf, " %d ", num);
	    wcenter(come10win, buf, 0, 1);
	}
	else {
	    if (Craps_Point(&Table) == 10)
		wcenter(come10win, "*****", 0, 1);
	    else
		werase(come10win);
	}
	wrefresh(come10win);
    }
    if (full || (Craps_Bankroll(&Table) != Craps_Bankroll(&OldTable))) {
	sprintf(buf, "%d", Craps_Bankroll(&Table));
	wcenter(bankrollwin, buf, 0, 0);
	wrefresh(bankrollwin);
    }
    if (full || (Craps_Point(&Table) != Craps_Point(&OldTable))) {
	if (num = Craps_Point(&Table)) {
	    sprintf(buf, " %d ", num);
	}
	else {
	    strcpy(buf, " COME  OUT ");
	}
	wcenter(pointwin, buf, 0, 1);
	wrefresh(pointwin);
    }
    if (full || (Craps_GetBet(&Table, craps_PassLine) !=
		 Craps_GetBet(&OldTable, craps_PassLine)) ||
	(Craps_GetBet(&Table, craps_PassLineOdds) !=
	 Craps_GetBet(&OldTable, craps_PassLineOdds))) {
	if (num = Craps_GetBet(&Table, craps_PassLine)) {
	    if (Craps_GetBet(&Table, craps_PassLineOdds))
		sprintf(buf, " %d/%d ", num,
			Craps_GetBet(&Table, craps_PassLineOdds));
	    else
		sprintf(buf, " %d ", num);
	    wcenter(passlinewin, buf, 0, 1);
	}
	else {
	    werase(passlinewin);
	}
	wrefresh(passlinewin);
    }
    if (full || (Craps_GetBet(&Table, craps_ComeBox) !=
		 Craps_GetBet(&OldTable, craps_ComeBox))) {
	if (num = Craps_GetBet(&Table, craps_ComeBox)) {
	    sprintf(buf, " %d ", num);
	    wcenter(comewin, buf, 0, 1);
	}
	else {
	    werase(comewin);
	}
	wrefresh(comewin);
    }
    if (full || (Craps_GetBet(&Table, craps_DontPass) !=
		 Craps_GetBet(&OldTable, craps_DontPass)) ||
	(Craps_GetBet(&Table, craps_DontPassOdds) !=
	 Craps_GetBet(&OldTable, craps_DontPassOdds))) {
	if (num = Craps_GetBet(&Table, craps_DontPass)) {
	    if (Craps_GetBet(&Table, craps_DontPassOdds))
		sprintf(buf, " %d/%d ", num,
			Craps_GetBet(&Table, craps_DontPassOdds));
	    else
		sprintf(buf, " %d ", num);
	    wcenter(dontpasswin, buf, 0, 1);
	}
	else {
	    werase(dontpasswin);
	}
	wrefresh(dontpasswin);
    }
    if (full || (Craps_GetBet(&Table, craps_DontComeBox) !=
		 Craps_GetBet(&OldTable, craps_DontComeBox))) {
	if (num = Craps_GetBet(&Table, craps_DontComeBox)) {
	    sprintf(buf, " %d ", num);
	    wcenter(dontcomewin, buf, 0, 1);
	}
	else {
	    werase(dontcomewin);
	}
	wrefresh(dontcomewin);
    }
    if (full || (Craps_GetBet(&Table, craps_Field) !=
		 Craps_GetBet(&OldTable, craps_Field))) {
	if (num = Craps_GetBet(&Table, craps_Field)) {
	    sprintf(buf, " %d ", num);
	    wcenter(fieldwin, buf, 0, 1);
	}
	else {
	    werase(fieldwin);
	}
	wrefresh(fieldwin);
    }
    if (full || (Craps_GetBet(&Table, craps_Big6) !=
		 Craps_GetBet(&OldTable, craps_Big6))) {
	if (num = Craps_GetBet(&Table, craps_Big6)) {
	    sprintf(buf, " %d ", num);
	    wcenter(big6win, buf, 0, 1);
	}
	else {
	    werase(big6win);
	}
	wrefresh(big6win);
    }
    if (full || (Craps_GetBet(&Table, craps_Big8) !=
		 Craps_GetBet(&OldTable, craps_Big8))) {
	if (num = Craps_GetBet(&Table, craps_Big8)) {
	    sprintf(buf, " %d ", num);
	    wcenter(big8win, buf, 0, 1);
	}
	else {
	    werase(big8win);
	}
	wrefresh(big8win);
    }
    if (full || (Craps_GetBet(&Table, craps_Hard4) !=
		 Craps_GetBet(&OldTable, craps_Hard4))) {
	if (num = Craps_GetBet(&Table, craps_Hard4)) {
	    sprintf(buf, " %d ", num);
	    wcenter(hard4win, buf, 0, 1);
	}
	else {
	    werase(hard4win);
	}
	wrefresh(hard4win);
    }
    if (full || (Craps_GetBet(&Table, craps_Hard6) !=
		 Craps_GetBet(&OldTable, craps_Hard6))) {
	if (num = Craps_GetBet(&Table, craps_Hard6)) {
	    sprintf(buf, " %d ", num);
	    wcenter(hard6win, buf, 0, 1);
	}
	else {
	    werase(hard6win);
	}
	wrefresh(hard6win);
    }
    if (full || (Craps_GetBet(&Table, craps_Hard8) !=
		 Craps_GetBet(&OldTable, craps_Hard8))) {
	if (num = Craps_GetBet(&Table, craps_Hard8)) {
	    sprintf(buf, " %d ", num);
	    wcenter(hard8win, buf, 0, 1);
	}
	else {
	    werase(hard8win);
	}
	wrefresh(hard8win);
    }
    if (full || (Craps_GetBet(&Table, craps_Hard10) !=
		 Craps_GetBet(&OldTable, craps_Hard10))) {
	if (num = Craps_GetBet(&Table, craps_Hard10)) {
	    sprintf(buf, " %d ", num);
	    wcenter(hard10win, buf, 0, 1);
	}
	else {
	    werase(hard10win);
	}
	wrefresh(hard10win);
    }
    if (full || (Craps_GetBet(&Table, craps_OneRoll2) !=
		 Craps_GetBet(&OldTable, craps_OneRoll2))) {
	if (num = Craps_GetBet(&Table, craps_OneRoll2)) {
	    sprintf(buf, " %d ", num);
	    wcenter(oneroll2win, buf, 0, 1);
	}
	else {
	    werase(oneroll2win);
	}
	wrefresh(oneroll2win);
    }
    if (full || (Craps_GetBet(&Table, craps_OneRoll3) !=
		 Craps_GetBet(&OldTable, craps_OneRoll3))) {
	if (num = Craps_GetBet(&Table, craps_OneRoll3)) {
	    sprintf(buf, " %d ", num);
	    wcenter(oneroll3win, buf, 0, 1);
	}
	else {
	    werase(oneroll3win);
	}
	wrefresh(oneroll3win);
    }
    if (full || (Craps_GetBet(&Table, craps_OneRoll11) !=
		 Craps_GetBet(&OldTable, craps_OneRoll11))) {
	if (num = Craps_GetBet(&Table, craps_OneRoll11)) {
	    sprintf(buf, " %d ", num);
	    wcenter(oneroll11win, buf, 0, 1);
	}
	else {
	    werase(oneroll11win);
	}
	wrefresh(oneroll11win);
    }
    if (full || (Craps_GetBet(&Table, craps_OneRoll12) !=
		 Craps_GetBet(&OldTable, craps_OneRoll12))) {
	if (num = Craps_GetBet(&Table, craps_OneRoll12)) {
	    sprintf(buf, " %d ", num);
	    wcenter(oneroll12win, buf, 0, 1);
	}
	else {
	    werase(oneroll12win);
	}
	wrefresh(oneroll12win);
    }
    if (full || (Craps_GetBet(&Table, craps_Any7) !=
		 Craps_GetBet(&OldTable, craps_Any7))) {
	if (num = Craps_GetBet(&Table, craps_Any7)) {
	    sprintf(buf, " %d ", num);
	    wcenter(any7win, buf, 0, 1);
	}
	else {
	    werase(any7win);
	}
	wrefresh(any7win);
    }
    if (full || (Craps_GetBet(&Table, craps_AnyCraps) !=
		 Craps_GetBet(&OldTable, craps_AnyCraps))) {
	if (num = Craps_GetBet(&Table, craps_AnyCraps)) {
	    sprintf(buf, " %d ", num);
	    wcenter(anycrapswin, buf, 0, 1);
	}
	else {
	    werase(anycrapswin);
	}
	wrefresh(anycrapswin);
    }
    if (full || (Craps_Die1(&Table) !=
		 Craps_Die1(&OldTable)) ||
	(Craps_Die2(&Table) != Craps_Die2(&OldTable))) {
	sprintf(buf, "%d  %d", Craps_Die1(&Table), Craps_Die2(&Table));
	wcenter(dicewin, buf, 0, 1);
	wrefresh(dicewin);
    }
}

Error(err)
CrapsError_t    err;
{
    char            buf[100];

    switch (err) {
	case crapserr_NoReturn:
	    Message("Cannot take down bet");
	    break;
	case crapserr_AlreadyBought:
	    Message("Already bought");
	    break;
	case crapserr_Nonpositive:
	    Message("Bet must be positive");
	    break;
	case crapserr_NoBet:
	    Message("No bet to take odds on");
	    break;
	case crapserr_InsufficientFunds:
	    Message("Insufficient funds");
	    break;
	case crapserr_NotComeOut:
	    Message("Not come-out roll");
	    break;
	case crapserr_ComeOut:
	    Message("Come-out roll");
	    break;
	case crapserr_TooMuch:
	    sprintf(buf, "Maximum is $%d", CrapsErrorDatum);
	    Message(buf);
	    break;
	case crapserr_NotDivisible:
	    sprintf(buf, "Bet must be divisible by %d", CrapsErrorDatum);
	    Message(buf);
	    break;
    }
}

int             ReadNumber()
{
    int             c, len = 0;
    char            buf[100], *bufptr = buf;

    buf[0] = '\0';
    werase(commandwin);
    wrefresh(commandwin);
    while (1) {
	c = Getch("0123456789\n\r\b", 0);
	switch (c) {
	    case '\n':
	    case '\r':
		if (len)
		    return (atoi(buf));
		break;
	    case '\b':
	    case 127:
		if (len > 0) {
		    *(--bufptr) = '\0';
		    --len;
		}
		break;
	    default:
		*(bufptr++) = c;
		*bufptr = '\0';
		++len;
		break;
	}
	werase(commandwin);
	waddstr(commandwin, buf);
	wrefresh(commandwin);
    }
}

Help()
{
    clear();
    addstr("p   - pass-line (or odds)     dp  - don't pass (or odds)\n");
    addstr("c   - come bet                dc  - don't come\n");
    addstr("b6  - big 6                   b8  - big 8\n");
    addstr("h22 - hard 4                  h33 - hard 6\n");
    addstr("h44 - hard 8                  h55 - hard 10\n");
    addstr("2   - one-roll bet on 2       3   - one-roll bet on 3\n");
    addstr("11  - one-roll bet on 11      12  - one-roll bet on 12\n");
    addstr("a7  - any 7                   ac  - any craps\n");
    addstr("b4  - buy the 4               b10 - buy the 10\n");
    addstr("f   - field                   r   - ROLL DICE\n\n");
    addstr("Come-odds, place bets, don't-come odds, and don't place bets\n");
    addstr("are made by typing the desired number, then c, p, dc, or dp.\n");
    addstr("For example:\n\n");
    addstr("4c  - come-odds on 4          4p  - place 4\n");
    addstr("4dc - don't-come odds on 4    4dp - don't place 4\n\n");
    addstr("Bets are taken down by typing t followed by one of the\n");
    addstr("bet commands above.\n\n");
    addstr("$ - show total bets   m - review messages   X - exit\n");
    addstr("                                                        continue...");
    refresh();
    getch();
}

ShowTotal()
{
    int             i, total = 0, returnable = 0;
    char            buf[80];

    for (i = 0; i < (int) craps_Bets; ++i) {
	total += Craps_GetBet(&Table, i);
	switch (i) {
	    case craps_PassLine:
	    case craps_DontPass:
		if (Craps_Point(&Table) == 0)
		    returnable += Craps_GetBet(&Table, i);
		break;
	    case craps_Come4:
	    case craps_Come5:
	    case craps_Come6:
	    case craps_Come8:
	    case craps_Come9:
	    case craps_Come10:
	    case craps_DontCome4:
	    case craps_DontCome5:
	    case craps_DontCome6:
	    case craps_DontCome8:
	    case craps_DontCome9:
	    case craps_DontCome10:
		break;
	    default:
		returnable += Craps_GetBet(&Table, i);
		break;
	}
    }
    sprintf(buf, "Wagers on table total $%d ($%d returnable)",
	    total, returnable);
    Message(buf);
}
