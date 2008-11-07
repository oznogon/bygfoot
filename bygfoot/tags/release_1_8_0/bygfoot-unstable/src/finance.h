#ifndef FINANCES_H
#define FINANCES_H

#include "bygfoot.h"
#include "variables.h"

/* the amount of money the user has for transactions:
   the money on his account plus his drawing credit */
#define BUDGET finances[FIN_MONEY] + calculate_max_loan_credit(1)

/* return the costs for improving stadium safety or
   capacity */
gint
stadium_improve_costs(gint safety, gint build);

/* calculate the money for ticket sale and
   journey costs */
void
game_money(fixture fix);

/* deduct player, scout and physio wages */
void
update_finances_wages(void);

/* return interest rate depending on user's debt */
gfloat
interest_rate(void);

/* update the finances of the human player's team */
void
update_finances(void);

/* find out at the end of a season whether the human player's team
   was ranked among the first 3 or got promoted via promotion games
   (or was 4th in league 1);.
   if so, he gets some prize money */
void
prize_money(void);

/* return either the maximum amount of money the bank
   grants a team or the human player's drawing credit;
   both values depend on the players' values
   and the stadium */
gint
calculate_max_loan_credit(gint credit);

#endif
