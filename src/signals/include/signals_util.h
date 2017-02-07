#include "setjmp.h"
#include "signal.h"

#define HIBES_CATCH(SIG) signal(SIG, &on_sig)
#define HIBES_NO_CATCH(SIG) signal(SIG, SIG_DFL)

#define TRY if (setjmp(env) == 0)

jmp_buf env;

void reset_signal_counters();
void on_sig(int signum);
int getCounterSigAbrt();
int getCounterSigInt();
int getCounterSigSegV();
int getCounterOther();
