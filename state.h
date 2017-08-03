#ifndef _STATE_H
#define _STATE_H

struct state {
	// EWMA related to the RTT
	uint64_t ewmartt;
	// EWMA related to the loss
	uint64_t ewmaloss;
	// last ts for the evaluation
	uint64_t last_evaluation;
	// last sent
	uint64_t last_sent;
	// last rcv
	uint64_t last_rcv;
	// current sent
	uint64_t sent;
	// current rcv
	uint64_t rcv;
	// sent temp
	uint64_t sent_temp;
};

// Global state
struct state node_state;

#endif