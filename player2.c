#include <stdio.h>
#include <crapslib.h>

CrapsTable_t    Table;

extern int      optind;
extern char    *optarg;
extern FILE    *fopen();

int             quiet;

char           *BetName[] = {
  "Pass Line",
  "Pass Line Odds",
  "Come",
  "Come 4",
  "Come 5",
  "Come 6",
  "Come 8",
  "Come 9",
  "Come 10",
  "Come 4 Odds",
  "Come 5 Odds",
  "Come 6 Odds",
  "Come 8 Odds",
  "Come 9 Odds",
  "Come 10 Odds",
  "Don't Pass",
  "Don't Pass Odds",
  "Don't Come",
  "Don't Come 4",
  "Don't Come 5",
  "Don't Come 6",
  "Don't Come 8",
  "Don't Come 9",
  "Don't Come 10",
  "Don't Come 4 Odds",
  "Don't Come 5 Odds",
  "Don't Come 6 Odds",
  "Don't Come 8 Odds",
  "Don't Come 9 Odds",
  "Don't Come 10 Odds",
  "Place 4",
  "Place 5",
  "Place 6",
  "Place 8",
  "Place 9",
  "Place 10",
  "Don't Place 4",
  "Don't Place 5",
  "Don't Place 6",
  "Don't Place 8",
  "Don't Place 9",
  "Don't Place 10",
  "Field",
  "Big 6",
  "Big 8",
  "Hard 4",
  "Hard 6",
  "Hard 8",
  "Hard 10",
  "Any 7",
  "Any Craps",
  "One-Roll 2",
  "One-Roll 3",
  "One-Roll 11",
  "One-Roll 12"
};

int             RoundUp(num, mult)
  int             num, mult;
{
  if (num && (!(num % mult)))
    return (num);
  return ((num - (num % mult)) + mult);
}

void            Strategy1()
{				       /* 3 come bets, 6 and 8 always
				        * covered */
  if (Craps_Point(&Table)) {
    if (NumComePoints() < 3)
      Bet(&Table, craps_ComeBox, 5);
    if ((Craps_GetBet(&Table, craps_Come6) == 0)
        && (Craps_Point(&Table) != 6))
      Bet(&Table, craps_Place6, 6);
    else
      TakeDown(&Table, craps_Place6);
    if ((Craps_GetBet(&Table, craps_Come8) == 0)
        && (Craps_Point(&Table) != 8))
      Bet(&Table, craps_Place8, 6);
    else
      TakeDown(&Table, craps_Place8);
    if (Craps_GetBet(&Table, craps_PassLineOdds) == 0) {
      switch (Craps_Point(&Table)) {
       case 4:
       case 10:
        Bet(&Table, craps_PassLineOdds, 15);
        break;

       case 5:
       case 9:
        Bet(&Table, craps_PassLineOdds, 20);
        break;

       case 6:
       case 8:
        Bet(&Table, craps_PassLineOdds, 25);
        break;
      }
    }
  } else
    Bet(&Table, craps_PassLine, 5);

  if (Craps_GetBet(&Table, craps_Come4))
    Bet(&Table, craps_ComeOdds4, 15);

  if (Craps_GetBet(&Table, craps_Come5))
    Bet(&Table, craps_ComeOdds5, 20);

  if (Craps_GetBet(&Table, craps_Come6))
    Bet(&Table, craps_ComeOdds6, 25);

  if (Craps_GetBet(&Table, craps_Come8))
    Bet(&Table, craps_ComeOdds8, 25);

  if (Craps_GetBet(&Table, craps_Come9))
    Bet(&Table, craps_ComeOdds9, 20);

  if (Craps_GetBet(&Table, craps_Come10))
    Bet(&Table, craps_ComeOdds10, 15);
}

void            Strategy2()
{				       /* 2 come bets, 6 and 8 always
				        * covered */
  if (Craps_Point(&Table)) {
    if (NumComePoints() < 2)
      Bet(&Table, craps_ComeBox, 5);
    if ((Craps_GetBet(&Table, craps_Come6) == 0)
        && (Craps_Point(&Table) != 6))
      Bet(&Table, craps_Place6, 6);
    else
      TakeDown(&Table, craps_Place6);
    if ((Craps_GetBet(&Table, craps_Come8) == 0)
        && (Craps_Point(&Table) != 8))
      Bet(&Table, craps_Place8, 6);
    else
      TakeDown(&Table, craps_Place8);
    if (Craps_GetBet(&Table, craps_PassLineOdds) == 0) {
      switch (Craps_Point(&Table)) {
       case 4:
       case 10:
        Bet(&Table, craps_PassLineOdds, 15);
        break;
       case 5:
       case 9:
        Bet(&Table, craps_PassLineOdds, 20);
        break;
       case 6:
       case 8:
        Bet(&Table, craps_PassLineOdds, 25);
        break;
      }
    }
  } else
    Bet(&Table, craps_PassLine, 5);

  if (Craps_GetBet(&Table, craps_Come4))
    Bet(&Table, craps_ComeOdds4, 15);

  if (Craps_GetBet(&Table, craps_Come5))
    Bet(&Table, craps_ComeOdds5, 20);

  if (Craps_GetBet(&Table, craps_Come6))
    Bet(&Table, craps_ComeOdds6, 25);

  if (Craps_GetBet(&Table, craps_Come8))
    Bet(&Table, craps_ComeOdds8, 25);

  if (Craps_GetBet(&Table, craps_Come9))
    Bet(&Table, craps_ComeOdds9, 20);

  if (Craps_GetBet(&Table, craps_Come10))
    Bet(&Table, craps_ComeOdds10, 15);
}

void            Strategy3()
{				       /* 2 come bets */
  if (Craps_Point(&Table)) {
    if (NumComePoints() < 2)
      Bet(&Table, craps_ComeBox, 5);

    if (Craps_GetBet(&Table, craps_PassLineOdds) == 0) {
      switch (Craps_Point(&Table)) {
       case 4:
       case 10:
        Bet(&Table, craps_PassLineOdds, 15);
        break;

       case 5:
       case 9:
        Bet(&Table, craps_PassLineOdds, 20);
        break;

       case 6:
       case 8:
        Bet(&Table, craps_PassLineOdds, 25);
        break;
      }
    }
  } else
    Bet(&Table, craps_PassLine, 5);

  if (Craps_GetBet(&Table, craps_Come4))
    Bet(&Table, craps_ComeOdds4, 15);

  if (Craps_GetBet(&Table, craps_Come5))
    Bet(&Table, craps_ComeOdds5, 20);

  if (Craps_GetBet(&Table, craps_Come6))
    Bet(&Table, craps_ComeOdds6, 25);

  if (Craps_GetBet(&Table, craps_Come8))
    Bet(&Table, craps_ComeOdds8, 25);

  if (Craps_GetBet(&Table, craps_Come9))
    Bet(&Table, craps_ComeOdds9, 20);

  if (Craps_GetBet(&Table, craps_Come10))
    Bet(&Table, craps_ComeOdds10, 15);
}

void            Strategy4()
{				       /* 3 come bets */
  if (Craps_Point(&Table)) {
    if (NumComePoints() < 3)
      Bet(&Table, craps_ComeBox, 5);
    if (Craps_GetBet(&Table, craps_PassLineOdds) == 0) {
      switch (Craps_Point(&Table)) {
       case 4:
       case 10:
        Bet(&Table, craps_PassLineOdds, 15);
        break;
       case 5:
       case 9:
        Bet(&Table, craps_PassLineOdds, 20);
        break;
       case 6:
       case 8:
        Bet(&Table, craps_PassLineOdds, 25);
        break;
      }
    }
  } else
    Bet(&Table, craps_PassLine, 5);

  if (Craps_GetBet(&Table, craps_Come4))
    Bet(&Table, craps_ComeOdds4, 15);

  if (Craps_GetBet(&Table, craps_Come5))
    Bet(&Table, craps_ComeOdds5, 20);

  if (Craps_GetBet(&Table, craps_Come6))
    Bet(&Table, craps_ComeOdds6, 25);

  if (Craps_GetBet(&Table, craps_Come8))
    Bet(&Table, craps_ComeOdds8, 25);

  if (Craps_GetBet(&Table, craps_Come9))
    Bet(&Table, craps_ComeOdds9, 20);

  if (Craps_GetBet(&Table, craps_Come10))
    Bet(&Table, craps_ComeOdds10, 15);
}

void            Strategy5()
{				       /* Wrong bettor, 2 don't-come bets */
  if (Craps_Point(&Table)) {
    if (NumDontComePoints() < 2)
      Bet(&Table, craps_DontComeBox, 3);
    Bet(&Table, craps_DontPassOdds, 6);
  }
  else {
    Bet(&Table, craps_DontPass, 3);
  }
  if (Craps_GetBet(&Table, craps_DontCome4))
    Bet(&Table, craps_DontComeOdds4, 6);
  if (Craps_GetBet(&Table, craps_DontCome5))
    Bet(&Table, craps_DontComeOdds5, 6);
  if (Craps_GetBet(&Table, craps_DontCome6))
    Bet(&Table, craps_DontComeOdds6, 6);
  if (Craps_GetBet(&Table, craps_DontCome8))
    Bet(&Table, craps_DontComeOdds8, 6);
  if (Craps_GetBet(&Table, craps_DontCome9))
    Bet(&Table, craps_DontComeOdds9, 6);
  if (Craps_GetBet(&Table, craps_DontCome10))
    Bet(&Table, craps_DontComeOdds10, 6);
}

void            Strategy6()
{				       /* Wrong bettor, 3 don't-come bets */
  if (Craps_Point(&Table)) {
    if (NumDontComePoints() < 3)
      Bet(&Table, craps_DontComeBox, 3);
    Bet(&Table, craps_DontPassOdds, 6);
  }
  else {
    Bet(&Table, craps_DontPass, 3);
  }
  if (Craps_GetBet(&Table, craps_DontCome4))
    Bet(&Table, craps_DontComeOdds4, 6);
  if (Craps_GetBet(&Table, craps_DontCome5))
    Bet(&Table, craps_DontComeOdds5, 6);
  if (Craps_GetBet(&Table, craps_DontCome6))
    Bet(&Table, craps_DontComeOdds6, 6);
  if (Craps_GetBet(&Table, craps_DontCome8))
    Bet(&Table, craps_DontComeOdds8, 6);
  if (Craps_GetBet(&Table, craps_DontCome9))
    Bet(&Table, craps_DontComeOdds9, 6);
  if (Craps_GetBet(&Table, craps_DontCome10))
    Bet(&Table, craps_DontComeOdds10, 6);
}

void            Strategy7()
{
  int             bet;

  if (Craps_Point(&Table)) {
    if (NumComePoints() < 3)
      Bet(&Table, craps_ComeBox, RoundUp(Craps_Bankroll(&Table) >> 6, 3));
    if ((Craps_GetBet(&Table, craps_Come6) == 0)
        && (Craps_Point(&Table) != 6))
      Bet(&Table, craps_Place6, RoundUp(Craps_Bankroll(&Table) >> 6, 6));
    else
      TakeDown(&Table, craps_Place6);
    if ((Craps_GetBet(&Table, craps_Come8) == 0)
        && (Craps_Point(&Table) != 8))
      Bet(&Table, craps_Place8, RoundUp(Craps_Bankroll(&Table) >> 6, 6));
    else
      TakeDown(&Table, craps_Place8);
    if (Craps_GetBet(&Table, craps_PassLineOdds) == 0) {
      switch (Craps_Point(&Table)) {
       case 6:
       case 8:
        Bet(&Table, craps_PassLineOdds,
            RoundUp(Craps_GetBet(&Table, craps_PassLine) << 1, 5));
        break;
       default:
        Bet(&Table, craps_PassLineOdds,
            Craps_GetBet(&Table, craps_PassLine) << 1);
        break;
      }
    }
  }
  else
    Bet(&Table, craps_PassLine, RoundUp(Craps_Bankroll(&Table) >> 6, 3));
  if (bet = Craps_GetBet(&Table, craps_Come4))
    Bet(&Table, craps_ComeOdds4, bet << 1);
  if (bet = Craps_GetBet(&Table, craps_Come5))
    Bet(&Table, craps_ComeOdds5, bet << 1);
  if (bet = Craps_GetBet(&Table, craps_Come6))
    Bet(&Table, craps_ComeOdds6, RoundUp(bet << 1, 5));
  if (bet = Craps_GetBet(&Table, craps_Come8))
    Bet(&Table, craps_ComeOdds8, RoundUp(bet << 1, 5));
  if (bet = Craps_GetBet(&Table, craps_Come9))
    Bet(&Table, craps_ComeOdds9, bet << 1);
  if (bet = Craps_GetBet(&Table, craps_Come10))
    Bet(&Table, craps_ComeOdds10, bet << 1);
}

int             ispoint(num)
  int             num;
{
  switch (num) {
   case 4:
   case 5:
   case 6:
   case 8:
   case 9:
   case 10:
    return (1);
   default:
    return (0);
  }
}

int             NumComePoints()
{
  int             num = 0;

  if (Craps_GetBet(&Table, craps_Come4))
    ++num;
  if (Craps_GetBet(&Table, craps_Come5))
    ++num;
  if (Craps_GetBet(&Table, craps_Come6))
    ++num;
  if (Craps_GetBet(&Table, craps_Come8))
    ++num;
  if (Craps_GetBet(&Table, craps_Come9))
    ++num;
  if (Craps_GetBet(&Table, craps_Come10))
    ++num;
  return (num);
}

int             NumDontComePoints()
{
  int             num = 0;

  if (Craps_GetBet(&Table, craps_DontCome4))
    ++num;
  if (Craps_GetBet(&Table, craps_DontCome5))
    ++num;
  if (Craps_GetBet(&Table, craps_DontCome6))
    ++num;
  if (Craps_GetBet(&Table, craps_DontCome8))
    ++num;
  if (Craps_GetBet(&Table, craps_DontCome9))
    ++num;
  if (Craps_GetBet(&Table, craps_DontCome10))
    ++num;
  return (num);
}

main(argc, argv)
  int             argc;
  char          **argv;
{
  int             a, b, max = 300, limit = 200, count = 0, c;
  int             readrolls = 0, writerolls = 0;
  FILE           *rollfile;
  void            (*strategy) () = Strategy1;

  quiet = 0;
  while ((c = getopt(argc, argv, "1234567w:l:r:q")) != EOF) {
    switch (c) {
     case '1':
      strategy = Strategy1;
      break;
     case '2':
      strategy = Strategy2;
      break;
     case '3':
      strategy = Strategy3;
      break;
     case '4':
      strategy = Strategy4;
      break;
     case '5':
      strategy = Strategy5;
      break;
     case '6':
      strategy = Strategy6;
      break;
     case '7':
      strategy = Strategy7;
      break;
     case 'w':
      if (readrolls)
        exit(1);
      writerolls = 1;
      if (!(rollfile = fopen(optarg, "w")))
        exit(1);
      break;
     case 'r':
      if (writerolls)
        exit(1);
      readrolls = 1;
      if (!(rollfile = fopen(optarg, "r")))
        exit(1);
      break;
     case 'l':
      limit = atoi(optarg);
      break;
     case 'q':
      quiet = 1;
      break;
    }
  }
  srandom(time(0) + getpid());
  Craps_Init(&Table);
  Craps_SetOdds(&Table, 5);
  Craps_SetBankroll(&Table, 300);
  while (1) {
    if (++count > limit)
      Done(count, max);
    if (Craps_Bankroll(&Table) < 50)
      Done(count, max);
    if (readrolls) {
      if (2 != fscanf(rollfile, "%d %d", &a, &b)) {
        readrolls = 0;
        fclose(rollfile);
      }
    }
    if (!readrolls) {
      a = 1 + (random() % 6);
      b = 1 + (random() % 6);
    }
    if (writerolls)
      fprintf(rollfile, "%d %d\n", a, b);
    (*strategy) ();
    if (!quiet)
      printf("\t    Roll %d %d%s\n",
             a, b, (Craps_Point(&Table) ? "" : " (come out)"));
    Craps_Update(&Table, a, b);
    if (Craps_Bankroll(&Table) > max)
      max = Craps_Bankroll(&Table);
    if (!quiet)
      printf("\nBankroll: $%d (max %d)\n",
             Craps_Bankroll(&Table), max);
  }
}

void            Craps_ReportPayoff(t, bet, payoff, win)
  CrapsTable_t   *t;
  CrapsBet_t      bet;
  CrapsPayoff_t  *payoff;
  int             win;
{
  if (!quiet)
    printf("\t\t\tCollect $%d\n", win + Craps_GetBet(&Table, bet));
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
  if (!quiet)
    printf("\t\t\tCasino returns $%d\n", Craps_GetBet(&Table, bet));
}

void            Craps_ReportCommission(t, amount)
  CrapsTable_t   *t;
  int             amount;
{
}

void            Craps_ReportMove(t, from, to)
  CrapsTable_t   *t;
  CrapsBet_t      from, to;
{
}

Bet(t, which, amt)
  CrapsTable_t   *t;
  CrapsBet_t      which;
  int             amt;
{
  if (amt != Craps_GetBet(t, which)) {
    if (!quiet)
      printf("\t\t\tBet $%d on %s\n", amt, BetName[which]);
    Craps_Bet(t, which, amt);
  }
}

TakeDown(t, which)
  CrapsTable_t   *t;
  CrapsBet_t      which;
{
  if (Craps_GetBet(t, which)) {
    if (!quiet)
      printf("\t\t\tTake down %s bet\n", BetName[which]);
    Craps_TakeDown(t, which);
  }
}

Done(count, max)
  int count;
  int max;
{
  printf("%sFinal  %d  Max  %d  Throws  %d\n",
         quiet ? "" : "\n\n",
         Craps_Bankroll(&Table), max, count);
  exit(0);
}
