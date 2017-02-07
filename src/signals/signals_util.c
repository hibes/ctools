#include "signals_util.h"

int counter_sigabrt = 0;
int counter_sigint = 0;
int counter_sigsegv = 0;
int counter_other = 0;

void on_sig(int signum) {
	switch (signum) {
		case SIGABRT:
			++counter_sigabrt;
			break;
		case SIGINT:
			++counter_sigint;
			break;
		case SIGSEGV:
			++counter_sigsegv;
			break;
		default:
			++counter_other;
			break;
	}

	longjmp(env, 1);
}

void reset_signal_counters() {
	counter_sigabrt = 0;
	counter_sigint = 0;
	counter_sigsegv = 0;
	counter_other = 0;
}

int getCounterSigAbrt() {
	return counter_sigabrt;
}

int getCounterSigInt() {
	return counter_sigint;
}

int getCounterSigSegV() {
	return counter_sigsegv;
}

int getCounterOther() {
	return counter_other;
}
