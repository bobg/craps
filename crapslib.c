#include <crapslib.h>

#define ComeOutP(t) (((t)->point)==0)

int             CrapsErrorDatum;

extern void     Craps_ReportPayoff();
extern void     Craps_ReportReturn();
extern void     Craps_ReportCollect();
extern void     Craps_ReportCommission();
extern void     Craps_ReportMove();

void            Craps_GetPayoff(t, which, buf)
CrapsTable_t   *t;
CrapsBet_t      which;
CrapsPayoff_t  *buf;
{
    int             a = 1, b = 1;      /* Default payoff */

    switch (which) {
	case craps_PassLineOdds:
	    switch (t->point) {
		case 4:
		case 10:
		    a = 2;
		    break;
		case 5:
		case 9:
		    a = 3;
		    b = 2;
		    break;
		case 6:
		case 8:
		    a = 6;
		    b = 5;
		    break;
	    }
	    break;
	case craps_ComeOdds4:
	case craps_ComeOdds10:
	    a = 2;
	    break;
	case craps_ComeOdds5:
	case craps_ComeOdds9:
	    a = 3;
	    b = 2;
	    break;
	case craps_ComeOdds6:
	case craps_ComeOdds8:
	    a = 6;
	    b = 5;
	    break;
	case craps_DontPassOdds:
	    switch (t->point) {
		case 4:
		case 10:
		    b = 2;
		    break;
		case 5:
		case 9:
		    a = 2;
		    b = 3;
		    break;
		case 6:
		case 8:
		    a = 5;
		    b = 6;
		    break;
	    }
	    break;
	case craps_DontComeOdds4:
	case craps_DontComeOdds10:
	    b = 2;
	    break;
	case craps_DontComeOdds5:
	case craps_DontComeOdds9:
	    a = 2;
	    b = 3;
	    break;
	case craps_DontComeOdds6:
	case craps_DontComeOdds8:
	    a = 5;
	    b = 6;
	    break;
	case craps_Place4:
	    if (t->bought4)
		a = 2;
	    else {
		a = 9;
		b = 5;
	    }
	    break;
	case craps_Place5:
	case craps_Place9:
	    a = 7;
	    b = 5;
	    break;
	case craps_Place6:
	case craps_Place8:
	    a = 7;
	    b = 6;
	    break;
	case craps_Place10:
	    if (t->bought10)
		a = 2;
	    else {
		a = 9;
		b = 5;
	    }
	    break;
	case craps_DontPlace4:
	case craps_DontPlace10:
	    b = 2;
	    break;
	case craps_DontPlace5:
	case craps_DontPlace9:
	    a = 2;
	    b = 3;
	    break;
	case craps_DontPlace6:
	case craps_DontPlace8:
	    a = 5;
	    b = 6;
	    break;
	case craps_Field:
	    switch (t->die1 + t->die2) {
		case 2:
		    a = t->field2payoff.numerator;
		    b = t->field2payoff.denominator;
		    break;
		case 12:
		    a = t->field12payoff.numerator;
		    b = t->field12payoff.denominator;
		    break;
	    }
	    break;
	case craps_Big6:
	    a = t->big6payoff.numerator;
	    b = t->big6payoff.denominator;
	    break;
	case craps_Big8:
	    a = t->big8payoff.numerator;
	    b = t->big8payoff.denominator;
	    break;
	case craps_Hard4:
	case craps_Hard10:
	    a = 7;
	    break;
	case craps_Hard6:
	case craps_Hard8:
	    a = 9;
	    break;
	case craps_Any7:
	    a = 4;
	    break;
	case craps_AnyCraps:
	    a = 7;
	    break;
	case craps_OneRoll2:
	    a = t->oneroll2payoff.numerator;
	    b = t->oneroll2payoff.denominator;
	    break;
	case craps_OneRoll3:
	    a = t->oneroll3payoff.numerator;
	    b = t->oneroll3payoff.denominator;
	    break;
	case craps_OneRoll11:
	    a = t->oneroll11payoff.numerator;
	    b = t->oneroll11payoff.denominator;
	    break;
	case craps_OneRoll12:
	    a = t->oneroll12payoff.numerator;
	    b = t->oneroll12payoff.denominator;
	    break;
    }
    buf->numerator = a;
    buf->denominator = b;
}

static void     Payoff(t, which)
CrapsTable_t   *t;
CrapsBet_t      which;
{
    if (t->bets[(int) which]) {
	CrapsPayoff_t   payoff;
	int             win;

	Craps_GetPayoff(t, which, &payoff);
	win = (t->bets[(int) which] / payoff.denominator) * payoff.numerator;
	Craps_ReportPayoff(t, which, &payoff, win);
	t->bankroll += (t->bets[(int) which] + win);
	t->bets[(int) which] = 0;
    }
}

static void     Return(t, which)
CrapsTable_t   *t;
CrapsBet_t      which;
{
    if (t->bets[(int) which]) {
	Craps_ReportReturn(t, which);
	t->bankroll += t->bets[(int) which];
	t->bets[(int) which] = 0;
    }
}

static void     Collect(t, which)
CrapsTable_t   *t;
CrapsBet_t      which;
{
    if (t->bets[(int) which]) {
	Craps_ReportCollect(t, which);
	t->bets[(int) which] = 0;
    }
}

void            Craps_Init(t)
CrapsTable_t   *t;
{
    int             i;

    t->bankroll = 0;
    t->point = 0;
    t->die1 = 5;
    t->die2 = 6;
    t->bought4 = t->bought10 = 0;
    t->odds = 2;
    t->field2payoff.numerator = 2;
    t->field2payoff.denominator = 1;
    t->field12payoff.numerator = 2;
    t->field12payoff.denominator = 1;
    t->big6payoff.numerator = t->big6payoff.denominator = 1;
    t->big8payoff.numerator = t->big8payoff.denominator = 1;
    t->oneroll2payoff.numerator = 29;
    t->oneroll2payoff.denominator = 1;
    t->oneroll3payoff.numerator = 14;
    t->oneroll3payoff.denominator = 1;
    t->oneroll11payoff.numerator = 14;
    t->oneroll11payoff.denominator = 1;
    t->oneroll12payoff.numerator = 29;
    t->oneroll12payoff.denominator = 1;
    for (i = 0; i < (int) craps_Bets; ++i)
	t->bets[i] = 0;
}

static void     Decr(t, which, amt)
CrapsTable_t   *t;
CrapsBet_t      which;
int             amt;
{
    t->bets[(int) which] += amt;
    t->bankroll -= amt;
}

void            Craps_Update(t, die1, die2)
CrapsTable_t   *t;
int             die1, die2;
{
    t->die1 = die1;
    t->die2 = die2;
    switch (die1 + die2) {
	case 2:
	    if (ComeOutP(t))
		Collect(t, craps_PassLine);
	    Collect(t, craps_ComeBox);
	    if (ComeOutP(t))
		Payoff(t, craps_DontPass);
	    Payoff(t, craps_DontComeBox);
	    Payoff(t, craps_Field);
	    Collect(t, craps_Any7);
	    Payoff(t, craps_AnyCraps);
	    Payoff(t, craps_OneRoll2);
	    Collect(t, craps_OneRoll3);
	    Collect(t, craps_OneRoll11);
	    Collect(t, craps_OneRoll12);
	    break;
	case 3:
	    if (ComeOutP(t))
		Collect(t, craps_PassLine);
	    Collect(t, craps_ComeBox);
	    if (ComeOutP(t))
		Payoff(t, craps_DontPass);
	    Payoff(t, craps_DontComeBox);
	    Payoff(t, craps_Field);
	    Collect(t, craps_Any7);
	    Payoff(t, craps_AnyCraps);
	    Collect(t, craps_OneRoll2);
	    Payoff(t, craps_OneRoll3);
	    Collect(t, craps_OneRoll11);
	    Collect(t, craps_OneRoll12);
	    break;
	case 4:
	    if (t->point == 4) {
		Payoff(t, craps_PassLine);
		Payoff(t, craps_PassLineOdds);
	    }
	    Payoff(t, craps_Come4);
	    if (ComeOutP(t))
		Return(t, craps_ComeOdds4);
	    else
		Payoff(t, craps_ComeOdds4);
	    if (t->point == 4) {
		Collect(t, craps_DontPass);
		Collect(t, craps_DontPassOdds);
	    }
	    Collect(t, craps_DontCome4);
	    Collect(t, craps_DontComeOdds4);
	    if (t->point) {
		Payoff(t, craps_Place4);
		t->bought4 = 0;
	    }
	    Collect(t, craps_DontPlace4);
	    Payoff(t, craps_Field);
	    if (die1 == die2)
		Payoff(t, craps_Hard4);
	    else
		Collect(t, craps_Hard4);
	    Collect(t, craps_Any7);
	    Collect(t, craps_AnyCraps);
	    Collect(t, craps_OneRoll2);
	    Collect(t, craps_OneRoll3);
	    Collect(t, craps_OneRoll11);
	    Collect(t, craps_OneRoll12);
	    if (t->bets[(int) craps_ComeBox]) {
		Craps_ReportMove(t, craps_ComeBox, craps_Come4);
		t->bets[(int) craps_Come4] = t->bets[(int) craps_ComeBox];
		t->bets[(int) craps_ComeBox] = 0;
	    }
	    if (t->bets[(int) craps_DontComeBox]) {
		Craps_ReportMove(t, craps_DontComeBox, craps_DontCome4);
		t->bets[(int) craps_DontCome4] = t->bets[(int) craps_DontComeBox];
		t->bets[(int) craps_DontComeBox] = 0;
	    }
	    if (ComeOutP(t))
		t->point = 4;
	    else
		if (t->point == 4)
		    t->point = 0;
	    break;
	case 5:
	    if (t->point == 5) {
		Payoff(t, craps_PassLine);
		Payoff(t, craps_PassLineOdds);
	    }
	    Payoff(t, craps_Come5);
	    if (ComeOutP(t))
		Return(t, craps_ComeOdds5);
	    else
		Payoff(t, craps_ComeOdds5);
	    if (t->point == 5) {
		Collect(t, craps_DontPass);
		Collect(t, craps_DontPassOdds);
	    }
	    Collect(t, craps_DontCome5);
	    Collect(t, craps_DontComeOdds5);
	    if (t->point)
		Payoff(t, craps_Place5);
	    Collect(t, craps_DontPlace5);
	    Collect(t, craps_Field);
	    Collect(t, craps_Any7);
	    Collect(t, craps_AnyCraps);
	    Collect(t, craps_OneRoll2);
	    Collect(t, craps_OneRoll3);
	    Collect(t, craps_OneRoll11);
	    Collect(t, craps_OneRoll12);
	    if (t->bets[(int) craps_ComeBox]) {
		Craps_ReportMove(t, craps_ComeBox, craps_Come5);
		t->bets[(int) craps_Come5] = t->bets[(int) craps_ComeBox];
		t->bets[(int) craps_ComeBox] = 0;
	    }
	    if (t->bets[(int) craps_DontComeBox]) {
		Craps_ReportMove(t, craps_DontComeBox, craps_DontCome5);
		t->bets[(int) craps_DontCome5] = t->bets[(int) craps_DontComeBox];
		t->bets[(int) craps_DontComeBox] = 0;
	    }
	    if (ComeOutP(t))
		t->point = 5;
	    else
		if (t->point == 5)
		    t->point = 0;
	    break;
	case 6:
	    if (t->point == 6) {
		Payoff(t, craps_PassLine);
		Payoff(t, craps_PassLineOdds);
	    }
	    Payoff(t, craps_Come6);
	    if (ComeOutP(t))
		Return(t, craps_ComeOdds6);
	    else
		Payoff(t, craps_ComeOdds6);
	    if (t->point == 6) {
		Collect(t, craps_DontPass);
		Collect(t, craps_DontPassOdds);
	    }
	    Collect(t, craps_DontCome6);
	    Collect(t, craps_DontComeOdds6);
	    if (t->point)
		Payoff(t, craps_Place6);
	    Collect(t, craps_DontPlace6);
	    Collect(t, craps_Field);
	    Payoff(t, craps_Big6);
	    if (die1 == die2)
		Payoff(t, craps_Hard6);
	    else
		Collect(t, craps_Hard6);
	    Collect(t, craps_Any7);
	    Collect(t, craps_AnyCraps);
	    Collect(t, craps_OneRoll2);
	    Collect(t, craps_OneRoll3);
	    Collect(t, craps_OneRoll11);
	    Collect(t, craps_OneRoll12);
	    if (t->bets[(int) craps_ComeBox]) {
		Craps_ReportMove(t, craps_ComeBox, craps_Come6);
		t->bets[(int) craps_Come6] = t->bets[(int) craps_ComeBox];
		t->bets[(int) craps_ComeBox] = 0;
	    }
	    if (t->bets[(int) craps_DontComeBox]) {
		Craps_ReportMove(t, craps_DontComeBox, craps_DontCome6);
		t->bets[(int) craps_DontCome6] = t->bets[(int) craps_DontComeBox];
		t->bets[(int) craps_DontComeBox] = 0;
	    }
	    if (ComeOutP(t))
		t->point = 6;
	    else
		if (t->point == 6)
		    t->point = 0;
	    break;
	case 7:
	    if (ComeOutP(t))
		Payoff(t, craps_PassLine);
	    else {
		Collect(t, craps_PassLine);
		Collect(t, craps_PassLineOdds);
	    }
	    Payoff(t, craps_ComeBox);
	    Collect(t, craps_Come4);
	    Collect(t, craps_Come5);
	    Collect(t, craps_Come6);
	    Collect(t, craps_Come8);
	    Collect(t, craps_Come9);
	    Collect(t, craps_Come10);
	    if (ComeOutP(t)) {
		Return(t, craps_ComeOdds4);
		Return(t, craps_ComeOdds5);
		Return(t, craps_ComeOdds6);
		Return(t, craps_ComeOdds8);
		Return(t, craps_ComeOdds9);
		Return(t, craps_ComeOdds10);
		Collect(t, craps_DontPass);
	    }
	    else {
		Collect(t, craps_ComeOdds4);
		Collect(t, craps_ComeOdds5);
		Collect(t, craps_ComeOdds6);
		Collect(t, craps_ComeOdds8);
		Collect(t, craps_ComeOdds9);
		Collect(t, craps_ComeOdds10);
		Payoff(t, craps_DontPass);
	    }
	    Payoff(t, craps_DontPassOdds);
	    Collect(t, craps_DontComeBox);
	    Payoff(t, craps_DontCome4);
	    Payoff(t, craps_DontComeOdds4);
	    Payoff(t, craps_DontCome5);
	    Payoff(t, craps_DontComeOdds5);
	    Payoff(t, craps_DontCome6);
	    Payoff(t, craps_DontComeOdds6);
	    Payoff(t, craps_DontCome8);
	    Payoff(t, craps_DontComeOdds8);
	    Payoff(t, craps_DontCome9);
	    Payoff(t, craps_DontComeOdds9);
	    Payoff(t, craps_DontCome10);
	    Payoff(t, craps_DontComeOdds10);
	    if (t->point) {
		Collect(t, craps_Place4);
		Collect(t, craps_Place5);
		Collect(t, craps_Place6);
		Collect(t, craps_Place8);
		Collect(t, craps_Place9);
		Collect(t, craps_Place10);
		t->bought4 = t->bought10 = 0;
	    }
	    Payoff(t, craps_DontPlace4);
	    Payoff(t, craps_DontPlace5);
	    Payoff(t, craps_DontPlace6);
	    Payoff(t, craps_DontPlace8);
	    Payoff(t, craps_DontPlace9);
	    Payoff(t, craps_DontPlace10);
	    Collect(t, craps_Field);
	    Collect(t, craps_Big6);
	    Collect(t, craps_Big8);
	    Collect(t, craps_Hard4);
	    Collect(t, craps_Hard6);
	    Collect(t, craps_Hard8);
	    Collect(t, craps_Hard10);
	    Payoff(t, craps_Any7);
	    Collect(t, craps_AnyCraps);
	    Collect(t, craps_OneRoll2);
	    Collect(t, craps_OneRoll3);
	    Collect(t, craps_OneRoll11);
	    Collect(t, craps_OneRoll12);
	    if (t->point)
		t->point = 0;
	    break;
	case 8:
	    if (t->point == 8) {
		Payoff(t, craps_PassLine);
		Payoff(t, craps_PassLineOdds);
	    }
	    Payoff(t, craps_Come8);
	    if (ComeOutP(t))
		Return(t, craps_ComeOdds8);
	    else
		Payoff(t, craps_ComeOdds8);
	    if (t->point == 8) {
		Collect(t, craps_DontPass);
		Collect(t, craps_DontPassOdds);
	    }
	    Collect(t, craps_DontCome8);
	    Collect(t, craps_DontComeOdds8);
	    if (t->point)
		Payoff(t, craps_Place8);
	    Collect(t, craps_DontPlace8);
	    Collect(t, craps_Field);
	    Payoff(t, craps_Big8);
	    if (die1 == die2)
		Payoff(t, craps_Hard8);
	    else
		Collect(t, craps_Hard8);
	    Collect(t, craps_Any7);
	    Collect(t, craps_AnyCraps);
	    Collect(t, craps_OneRoll2);
	    Collect(t, craps_OneRoll3);
	    Collect(t, craps_OneRoll11);
	    Collect(t, craps_OneRoll12);
	    if (t->bets[(int) craps_ComeBox]) {
		Craps_ReportMove(t, craps_ComeBox, craps_Come8);
		t->bets[(int) craps_Come8] = t->bets[(int) craps_ComeBox];
		t->bets[(int) craps_ComeBox] = 0;
	    }
	    if (t->bets[(int) craps_DontComeBox]) {
		Craps_ReportMove(t, craps_DontComeBox, craps_DontCome8);
		t->bets[(int) craps_DontCome8] = t->bets[(int) craps_DontComeBox];
		t->bets[(int) craps_DontComeBox] = 0;
	    }
	    if (ComeOutP(t))
		t->point = 8;
	    else
		if (t->point == 8)
		    t->point = 0;
	    break;
	case 9:
	    if (t->point == 9) {
		Payoff(t, craps_PassLine);
		Payoff(t, craps_PassLineOdds);
	    }
	    Payoff(t, craps_Come9);
	    if (ComeOutP(t))
		Return(t, craps_ComeOdds9);
	    else
		Payoff(t, craps_ComeOdds9);
	    if (t->point == 9) {
		Collect(t, craps_DontPass);
		Collect(t, craps_DontPassOdds);
	    }
	    Collect(t, craps_DontCome9);
	    Collect(t, craps_DontComeOdds9);
	    if (t->point)
		Payoff(t, craps_Place9);
	    Collect(t, craps_DontPlace9);
	    Payoff(t, craps_Field);
	    Collect(t, craps_Any7);
	    Collect(t, craps_AnyCraps);
	    Collect(t, craps_OneRoll2);
	    Collect(t, craps_OneRoll3);
	    Collect(t, craps_OneRoll11);
	    Collect(t, craps_OneRoll12);
	    if (t->bets[(int) craps_ComeBox]) {
		Craps_ReportMove(t, craps_ComeBox, craps_Come9);
		t->bets[(int) craps_Come9] = t->bets[(int) craps_ComeBox];
		t->bets[(int) craps_ComeBox] = 0;
	    }
	    if (t->bets[(int) craps_DontComeBox]) {
		Craps_ReportMove(t, craps_DontComeBox, craps_DontCome9);
		t->bets[(int) craps_DontCome9] = t->bets[(int) craps_DontComeBox];
		t->bets[(int) craps_DontComeBox] = 0;
	    }
	    if (ComeOutP(t))
		t->point = 9;
	    else
		if (t->point == 9)
		    t->point = 0;
	    break;
	case 10:
	    if (t->point == 10) {
		Payoff(t, craps_PassLine);
		Payoff(t, craps_PassLineOdds);
	    }
	    Payoff(t, craps_Come10);
	    if (ComeOutP(t))
		Return(t, craps_ComeOdds10);
	    else
		Payoff(t, craps_ComeOdds10);
	    if (t->point == 10) {
		Collect(t, craps_DontPass);
		Collect(t, craps_DontPassOdds);
	    }
	    Collect(t, craps_DontCome10);
	    Collect(t, craps_DontComeOdds10);
	    if (t->point) {
		Payoff(t, craps_Place10);
		t->bought10 = 0;
	    }
	    Collect(t, craps_DontPlace10);
	    Payoff(t, craps_Field);
	    if (die1 == die2)
		Payoff(t, craps_Hard10);
	    else
		Collect(t, craps_Hard10);
	    Collect(t, craps_Any7);
	    Collect(t, craps_AnyCraps);
	    Collect(t, craps_OneRoll2);
	    Collect(t, craps_OneRoll3);
	    Collect(t, craps_OneRoll11);
	    Collect(t, craps_OneRoll12);
	    if (t->bets[(int) craps_ComeBox]) {
		Craps_ReportMove(t, craps_ComeBox, craps_Come10);
		t->bets[(int) craps_Come10] = t->bets[(int) craps_ComeBox];
		t->bets[(int) craps_ComeBox] = 0;
	    }
	    if (t->bets[(int) craps_DontComeBox]) {
		Craps_ReportMove(t, craps_DontComeBox, craps_DontCome10);
		t->bets[(int) craps_DontCome10] = t->bets[(int) craps_DontComeBox];
		t->bets[(int) craps_DontComeBox] = 0;
	    }
	    if (ComeOutP(t))
		t->point = 10;
	    else
		if (t->point == 10)
		    t->point = 0;
	    break;
	case 11:
	    if (ComeOutP(t))
		Payoff(t, craps_PassLine);
	    Payoff(t, craps_ComeBox);
	    if (ComeOutP(t))
		Collect(t, craps_DontPass);
	    Collect(t, craps_DontComeBox);
	    Payoff(t, craps_Field);
	    Collect(t, craps_Any7);
	    Collect(t, craps_AnyCraps);
	    Collect(t, craps_OneRoll2);
	    Collect(t, craps_OneRoll3);
	    Payoff(t, craps_OneRoll11);
	    Collect(t, craps_OneRoll12);
	    break;
	case 12:
	    if (ComeOutP(t))
		Collect(t, craps_PassLine);
	    Collect(t, craps_ComeBox);
	    Payoff(t, craps_Field);
	    Collect(t, craps_Any7);
	    Payoff(t, craps_AnyCraps);
	    Collect(t, craps_OneRoll2);
	    Collect(t, craps_OneRoll3);
	    Collect(t, craps_OneRoll11);
	    Payoff(t, craps_OneRoll12);
	    break;
    }
}

CrapsError_t    Craps_Bet(t, which, bet)
CrapsTable_t   *t;
CrapsBet_t      which;
int             bet;
{
    int             amount = bet - t->bets[(int) which], max, orig, commission;
    CrapsPayoff_t   payoff;

    if (bet <= 0)
	return (crapserr_Nonpositive);
    if (amount > t->bankroll)
	return (crapserr_InsufficientFunds);
    switch (which) {
	case craps_PassLine:
	    if (ComeOutP(t))
		Decr(t, which, amount);
	    else
		return (crapserr_NotComeOut);
	    break;
	case craps_PassLineOdds:
	    if (ComeOutP(t))
		return (crapserr_ComeOut);
	    else {
		Craps_GetPayoff(t, which, &payoff);
		orig = t->bets[(int) craps_PassLine];
		orig *= t->odds;
		if (orig % payoff.denominator)
		    max = (orig - (orig % payoff.denominator)) + payoff.denominator;
		else
		    max = orig;
		if (!max)
		    return (crapserr_NoBet);
		if (bet > max) {
		    CrapsErrorDatum = max;
		    return (crapserr_TooMuch);
		}
		if (bet % payoff.denominator) {
		    CrapsErrorDatum = payoff.denominator;
		    return (crapserr_NotDivisible);
		}
		Decr(t, which, amount);
	    }
	    break;
	case craps_ComeBox:
	    if (ComeOutP(t))
		return (crapserr_ComeOut);
	    Decr(t, which, amount);
	    break;
	case craps_ComeOdds4:
	    if (!(max = t->bets[(int) craps_Come4]))
		return (crapserr_NoBet);
	    max *= t->odds;
	    if (bet > max) {
		CrapsErrorDatum = max;
		return (crapserr_TooMuch);
	    }
	    Decr(t, which, amount);
	    break;
	case craps_ComeOdds10:
	    if (!(max = t->bets[(int) craps_Come10]))
		return (crapserr_NoBet);
	    max *= t->odds;
	    if (bet > max) {
		CrapsErrorDatum = max;
		return (crapserr_TooMuch);
	    }
	    Decr(t, which, amount);
	    break;
	case craps_ComeOdds5:
	    if (!(max = t->bets[(int) craps_Come5]))
		return (crapserr_NoBet);
	    max *= t->odds;
	    if (max % 2)
		++max;
	    if (bet > max) {
		CrapsErrorDatum = max;
		return (crapserr_TooMuch);
	    }
	    if (bet % 2) {
		CrapsErrorDatum = 2;
		return (crapserr_NotDivisible);
	    }
	    Decr(t, which, amount);
	    break;
	case craps_ComeOdds9:
	    if (!(max = t->bets[(int) craps_Come9]))
		return (crapserr_NoBet);
	    max *= t->odds;
	    if (max % 2)
		++max;
	    if (bet > max) {
		CrapsErrorDatum = max;
		return (crapserr_TooMuch);
	    }
	    if (bet % 2) {
		CrapsErrorDatum = 2;
		return (crapserr_NotDivisible);
	    }
	    Decr(t, which, amount);
	    break;
	case craps_ComeOdds6:
	    if (!(orig = t->bets[(int) craps_Come6]))
		return (crapserr_NoBet);
	    orig *= t->odds;
	    if (orig % 5)
		max = (orig - (orig % 5)) + 5;
	    else
		max = orig;
	    if (bet > max) {
		CrapsErrorDatum = max;
		return (crapserr_TooMuch);
	    }
	    if (bet % 5) {
		CrapsErrorDatum = 5;
		return (crapserr_NotDivisible);
	    }
	    Decr(t, which, amount);
	    break;
	case craps_ComeOdds8:
	    if (!(orig = t->bets[(int) craps_Come8]))
		return (crapserr_NoBet);
	    orig *= t->odds;
	    if (orig % 5)
		max = (orig - (orig % 5)) + 5;
	    else
		max = orig;
	    if (bet > max) {
		CrapsErrorDatum = max;
		return (crapserr_TooMuch);
	    }
	    if (bet % 5) {
		CrapsErrorDatum = 5;
		return (crapserr_NotDivisible);
	    }
	    Decr(t, which, amount);
	    break;
	case craps_DontPass:
	    if (ComeOutP(t))
		Decr(t, which, amount);
	    else
		return (crapserr_NotComeOut);
	    break;
	case craps_DontPassOdds:
	    if (ComeOutP(t))
		return (crapserr_ComeOut);
	    if (!(orig = t->bets[(int) craps_DontPass]))
		return (crapserr_NoBet);
	    orig *= t->odds;
	    Craps_GetPayoff(t, which, &payoff);
	    if (orig % payoff.numerator)
		max = (orig - (orig % payoff.numerator)) + payoff.numerator;
	    else
		max = orig;
	    max /= payoff.numerator;
	    max *= payoff.denominator;
	    if (bet > max) {
		CrapsErrorDatum = max;
		return (crapserr_TooMuch);
	    }
	    if (bet % payoff.denominator) {
		CrapsErrorDatum = payoff.denominator;
		return (crapserr_NotDivisible);
	    }
	    Decr(t, which, amount);
	    break;
	case craps_DontComeBox:
	    if (ComeOutP(t))
		return (crapserr_ComeOut);
	    Decr(t, which, amount);
	    break;
	case craps_DontComeOdds4:
	    if (!(orig = t->bets[(int) craps_DontCome4]))
		return (crapserr_NoBet);
	    orig *= t->odds;
	    max = orig * 2;
	    if (bet > max) {
		CrapsErrorDatum = max;
		return (crapserr_TooMuch);
	    }
	    if (bet % 2) {
		CrapsErrorDatum = 2;
		return (crapserr_NotDivisible);
	    }
	    Decr(t, which, amount);
	    break;
	case craps_DontComeOdds10:
	    if (!(orig = t->bets[(int) craps_DontCome10]))
		return (crapserr_NoBet);
	    orig *= t->odds;
	    max = orig * 2;
	    if (bet > max) {
		CrapsErrorDatum = max;
		return (crapserr_TooMuch);
	    }
	    if (bet % 2) {
		CrapsErrorDatum = 2;
		return (crapserr_NotDivisible);
	    }
	    Decr(t, which, amount);
	    break;
	case craps_DontComeOdds5:
	    if (!(orig = t->bets[(int) craps_DontCome5]))
		return (crapserr_NoBet);
	    orig *= t->odds;
	    max = (orig % 2) ? (orig + 1) : orig;
	    max /= 2;
	    max *= 3;
	    if (bet > max) {
		CrapsErrorDatum = max;
		return (crapserr_TooMuch);
	    }
	    if (bet % 3) {
		CrapsErrorDatum = 3;
		return (crapserr_NotDivisible);
	    }
	    Decr(t, which, amount);
	    break;
	case craps_DontComeOdds9:
	    if (!(orig = t->bets[(int) craps_DontCome9]))
		return (crapserr_NoBet);
	    orig *= t->odds;
	    max = (orig % 2) ? (orig + 1) : orig;
	    max /= 2;
	    max *= 3;
	    if (bet > max) {
		CrapsErrorDatum = max;
		return (crapserr_TooMuch);
	    }
	    if (bet % 3) {
		CrapsErrorDatum = 3;
		return (crapserr_NotDivisible);
	    }
	    Decr(t, which, amount);
	    break;
	case craps_DontComeOdds6:
	    if (!(orig = t->bets[(int) craps_DontCome6]))
		return (crapserr_NoBet);
	    orig *= t->odds;
	    if (orig % 5)
		max = (orig - (orig % 5)) + 5;
	    else
		max = orig;
	    max /= 5;
	    max *= 6;
	    if (bet > max) {
		CrapsErrorDatum = max;
		return (crapserr_TooMuch);
	    }
	    if (bet % 6) {
		CrapsErrorDatum = 6;
		return (crapserr_NotDivisible);
	    }
	    Decr(t, which, amount);
	    break;
	case craps_DontComeOdds8:
	    if (!(orig = t->bets[(int) craps_DontCome8]))
		return (crapserr_NoBet);
	    orig *= t->odds;
	    if (orig % 5)
		max = (orig - (orig % 5)) + 5;
	    else
		max = orig;
	    max /= 5;
	    max *= 6;
	    if (bet > max) {
		CrapsErrorDatum = max;
		return (crapserr_TooMuch);
	    }
	    if (bet % 6) {
		CrapsErrorDatum = 6;
		return (crapserr_NotDivisible);
	    }
	    Decr(t, which, amount);
	    break;
	case craps_Place4:
	    if (ComeOutP(t))
		return (crapserr_ComeOut);
	    if (bet % 5) {
		CrapsErrorDatum = 5;
		return (crapserr_NotDivisible);
	    }
	    Decr(t, which, amount);
	    t->bought4 = 0;
	    break;
	case craps_Place5:
	    if (ComeOutP(t))
		return (crapserr_ComeOut);
	    if (bet % 5) {
		CrapsErrorDatum = 5;
		return (crapserr_NotDivisible);
	    }
	    Decr(t, which, amount);
	    break;
	case craps_Place6:
	    if (ComeOutP(t))
		return (crapserr_ComeOut);
	    if (bet % 6) {
		CrapsErrorDatum = 6;
		return (crapserr_NotDivisible);
	    }
	    Decr(t, which, amount);
	    break;
	case craps_Place8:
	    if (ComeOutP(t))
		return (crapserr_ComeOut);
	    if (bet % 6) {
		CrapsErrorDatum = 6;
		return (crapserr_NotDivisible);
	    }
	    Decr(t, which, amount);
	    break;
	case craps_Place9:
	    if (ComeOutP(t))
		return (crapserr_ComeOut);
	    if (bet % 5) {
		CrapsErrorDatum = 5;
		return (crapserr_NotDivisible);
	    }
	    Decr(t, which, amount);
	    break;
	case craps_Place10:
	    if (ComeOutP(t))
		return (crapserr_ComeOut);
	    if (bet % 5) {
		CrapsErrorDatum = 5;
		return (crapserr_NotDivisible);
	    }
	    Decr(t, which, amount);
	    t->bought10 = 0;
	    break;
	case craps_DontPlace4:
	    if (ComeOutP(t))
		return (crapserr_ComeOut);
	    if (bet % 40) {
		CrapsErrorDatum = 40;
		return (crapserr_NotDivisible);
	    }
	    commission = bet / 40;
	    if ((amount + commission) > t->bankroll)
		return (crapserr_InsufficientFunds);
	    Craps_ReportCommission(t, commission);
	    t->bankroll -= commission;
	    Decr(t, which, amount);
	    break;
	case craps_DontPlace10:
	    if (ComeOutP(t))
		return (crapserr_ComeOut);
	    if (bet % 40) {
		CrapsErrorDatum = 40;
		return (crapserr_NotDivisible);
	    }
	    commission = bet / 40;
	    if ((amount + commission) > t->bankroll)
		return (crapserr_InsufficientFunds);
	    Craps_ReportCommission(t, commission);
	    t->bankroll -= commission;
	    Decr(t, which, amount);
	    break;
	case craps_DontPlace5:
	    if (ComeOutP(t))
		return (crapserr_ComeOut);
	    if (bet % 30) {
		CrapsErrorDatum = 30;
		return (crapserr_NotDivisible);
	    }
	    commission = bet / 30;
	    if ((amount + commission) > t->bankroll)
		return (crapserr_InsufficientFunds);
	    Craps_ReportCommission(t, commission);
	    t->bankroll -= commission;
	    Decr(t, which, amount);
	    break;
	case craps_DontPlace9:
	    if (ComeOutP(t))
		return (crapserr_ComeOut);
	    if (bet % 30) {
		CrapsErrorDatum = 30;
		return (crapserr_NotDivisible);
	    }
	    commission = bet / 30;
	    if ((amount + commission) > t->bankroll)
		return (crapserr_InsufficientFunds);
	    Craps_ReportCommission(t, commission);
	    t->bankroll -= commission;
	    Decr(t, which, amount);
	    break;
	case craps_DontPlace6:
	    if (ComeOutP(t))
		return (crapserr_ComeOut);
	    if (bet % 24) {
		CrapsErrorDatum = 24;
		return (crapserr_NotDivisible);
	    }
	    commission = bet / 24;
	    if ((amount + commission) > t->bankroll)
		return (crapserr_InsufficientFunds);
	    Craps_ReportCommission(t, commission);
	    t->bankroll -= commission;
	    Decr(t, which, amount);
	    break;
	case craps_DontPlace8:
	    if (ComeOutP(t))
		return (crapserr_ComeOut);
	    if (bet % 24) {
		CrapsErrorDatum = 24;
		return (crapserr_NotDivisible);
	    }
	    commission = bet / 24;
	    if ((amount + commission) > t->bankroll)
		return (crapserr_InsufficientFunds);
	    Craps_ReportCommission(t, commission);
	    t->bankroll -= commission;
	    Decr(t, which, amount);
	    break;
	default:
	    Decr(t, which, amount);
	    break;
    }
    return (crapserr_OK);
}

CrapsError_t    Craps_Buy(t, which)
CrapsTable_t   *t;
int             which;
{
    int             orig, commission;

    switch (which) {
	case 4:
	    if (t->bought4)
		return (crapserr_AlreadyBought);
	    if (!(orig = t->bets[(int) craps_Place4]))
		return (crapserr_NoBet);
	    if (orig % 20) {
		CrapsErrorDatum = 20;
		return (crapserr_NotDivisible);
	    }
	    if ((commission = orig / 20) > t->bankroll)
		return (crapserr_InsufficientFunds);
	    Craps_ReportCommission(t, commission);
	    t->bankroll -= commission;
	    t->bought4 = 1;
	    break;
	case 10:
	    if (t->bought10)
		return (crapserr_AlreadyBought);
	    if (!(orig = t->bets[(int) craps_Place10]))
		return (crapserr_NoBet);
	    if (orig % 20) {
		CrapsErrorDatum = 20;
		return (crapserr_NotDivisible);
	    }
	    if ((commission = orig / 20) > t->bankroll)
		return (crapserr_InsufficientFunds);
	    Craps_ReportCommission(t, commission);
	    t->bankroll -= commission;
	    t->bought10 = 1;
	    break;
    }
    return (crapserr_OK);
}

CrapsError_t    Craps_TakeDown(t, which)
CrapsTable_t   *t;
CrapsBet_t      which;
{
    switch (which) {
	case craps_PassLine:
	    if (ComeOutP(t))
		Return(t, which);
	    else
		return (crapserr_NoReturn);
	    break;
	case craps_PassLineOdds:
	case craps_ComeBox:
	    Return(t, which);
	    break;
	case craps_Come4:
	case craps_Come5:
	case craps_Come6:
	case craps_Come8:
	case craps_Come9:
	case craps_Come10:
	    return (crapserr_NoReturn);
	case craps_ComeOdds4:
	case craps_ComeOdds5:
	case craps_ComeOdds6:
	case craps_ComeOdds8:
	case craps_ComeOdds9:
	case craps_ComeOdds10:
	    Return(t, which);
	    break;
	case craps_DontPass:
	    if (ComeOutP(t))
		Return(t, which);
	    else
		return (crapserr_NoReturn);
	    break;
	case craps_DontPassOdds:
	case craps_DontComeBox:
	    Return(t, which);
	    break;
	case craps_DontCome4:
	case craps_DontCome5:
	case craps_DontCome6:
	case craps_DontCome8:
	case craps_DontCome9:
	case craps_DontCome10:
	    return (crapserr_NoReturn);
	case craps_Place4:
	    Return(t, which);
	    t->bought4 = 0;
	    break;
	case craps_Place10:
	    Return(t, which);
	    t->bought10 = 0;
	    break;
	case craps_DontComeOdds4:
	case craps_DontComeOdds5:
	case craps_DontComeOdds6:
	case craps_DontComeOdds8:
	case craps_DontComeOdds9:
	case craps_DontComeOdds10:
	case craps_Place5:
	case craps_Place6:
	case craps_Place8:
	case craps_Place9:
	case craps_DontPlace4:
	case craps_DontPlace5:
	case craps_DontPlace6:
	case craps_DontPlace8:
	case craps_DontPlace9:
	case craps_DontPlace10:
	case craps_Field:
	case craps_Big6:
	case craps_Big8:
	case craps_Hard4:
	case craps_Hard6:
	case craps_Hard8:
	case craps_Hard10:
	case craps_Any7:
	case craps_AnyCraps:
	case craps_OneRoll2:
	case craps_OneRoll3:
	case craps_OneRoll11:
	case craps_OneRoll12:
	    Return(t, which);
	    break;
    }
    return (crapserr_OK);
}
