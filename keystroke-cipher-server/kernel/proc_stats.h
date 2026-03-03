#ifndef PROC_STATS_H
#define PROC_STATS_H

int  proc_stats_init(void);
void proc_stats_exit(void);

/* global counters - incremented by fifo_buffer.c and keycipher.c */
extern int stat_total_sent;
extern int stat_total_received;
extern int stat_total_blocked;

#endif /* PROC_STATS_H */
