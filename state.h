#ifndef _STATE_H
#define _STATE_H

struct state {
	// EWMA related to the RTT
	uint64_t ewmartt;
	// EWMA related to the loss
	uint64_t ewmaloss;
	// last ts useful for the rtt
	uint64_t last_ts;
	// last ts for the evaluation
	uint64_t last_evaluation;
	// last loss sent
	uint64_t last_lost_sent;
	// last loss rcv
	uint64_t last_lost_rcv;
	// current loss sent
	uint64_t lost_sent;
	// current loss rcv
	uint64_t lost_rcv;
};

// Global state
struct state node_state;

#endif