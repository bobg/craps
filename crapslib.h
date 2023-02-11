#define CRAPSLIB_VERSION (2)
#define CRAPSLIB_PATCHLEVEL (1)

typedef enum {
    craps_PassLine = 0, craps_PassLineOdds,
    craps_ComeBox, craps_Come4, craps_Come5, craps_Come6,
    craps_Come8, craps_Come9, craps_Come10,
    craps_ComeOdds4, craps_ComeOdds5, craps_ComeOdds6,
    craps_ComeOdds8, craps_ComeOdds9, craps_ComeOdds10,
    craps_DontPass, craps_DontPassOdds,
    craps_DontComeBox, craps_DontCome4, craps_DontCome5,
    craps_DontCome6, craps_DontCome8, craps_DontCome9,
    craps_DontCome10,
    craps_DontComeOdds4, craps_DontComeOdds5, craps_DontComeOdds6,
    craps_DontComeOdds8, craps_DontComeOdds9, craps_DontComeOdds10,
    craps_Place4, craps_Place5, craps_Place6, craps_Place8,
    craps_Place9, craps_Place10,
    craps_DontPlace4, craps_DontPlace5, craps_DontPlace6,
    craps_DontPlace8, craps_DontPlace9, craps_DontPlace10,
    craps_Field, craps_Big6, craps_Big8,
    craps_Hard4, craps_Hard6, craps_Hard8, craps_Hard10,
    craps_Any7, craps_AnyCraps,
    craps_OneRoll2, craps_OneRoll3, craps_OneRoll11, craps_OneRoll12,
    craps_Bets
}               CrapsBet_t;

typedef enum {
    crapserr_OK,
    crapserr_NoReturn,
    crapserr_AlreadyBought,
    crapserr_Nonpositive,
    crapserr_NoBet,
    crapserr_InsufficientFunds,
    crapserr_NotComeOut,
    crapserr_ComeOut,
    crapserr_TooMuch,
    crapserr_NotDivisible
}               CrapsError_t;

typedef struct {
    int             numerator, denominator;
}               CrapsPayoff_t;

typedef struct {
    int             bankroll, point, die1, die2;
    int             bought4, bought10; /* booleans */
    /* Configuration: */
    int             odds;
    CrapsPayoff_t   field2payoff, field12payoff, big6payoff;
    CrapsPayoff_t   big8payoff, oneroll2payoff, oneroll3payoff;
    CrapsPayoff_t   oneroll11payoff, oneroll12payoff;
    /* Bets: */
    int             bets[(int) craps_Bets];
}               CrapsTable_t;

#define Craps_Bankroll(t) ((t)->bankroll)
#define Craps_Point(t) ((t)->point)
#define Craps_Die1(t) ((t)->die1)
#define Craps_Die2(t) ((t)->die2)
#define Craps_Bought4(t) ((t)->bought4)
#define Craps_Bought10(t) ((t)->bought10)
#define Craps_Odds(t) ((t)->odds)
#define Craps_Field2Payoff(t) (&((t)->field2payoff))
#define Craps_Field12Payoff(t) (&((t)->field12payoff))
#define Craps_Big6Payoff(t) (&((t)->big6payoff))
#define Craps_Big8Payoff(t) (&((t)->big8payoff))
#define Craps_OneRoll2Payoff(t) (&((t)->oneroll2payoff))
#define Craps_OneRoll3Payoff(t) (&((t)->oneroll3payoff))
#define Craps_OneRoll11Payoff(t) (&((t)->oneroll11payoff))
#define Craps_OneRoll12Payoff(t) (&((t)->oneroll12payoff))
#define Craps_GetBet(t,type) ((t)->bets[(int)(type)])

#define Craps_SetBankroll(t,b) (((t)->bankroll)=(b))
#define Craps_SetOdds(t,o) (((t)->odds)=(o))

#define Craps_IncrBankroll(t,amt) (((t)->bankroll)+=(amt))

extern CrapsError_t Craps_Bet();
extern CrapsError_t Craps_Buy();
extern void     Craps_GetPayoff();
extern void     Craps_Init();
extern CrapsError_t Craps_TakeDown();
extern void     Craps_Update();
