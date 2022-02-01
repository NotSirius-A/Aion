typedef struct State {
  uint32_t period;
  uint32_t pausePeriod;
  char verboseName[30];
};

// 60s * 60s = 3600s = 1h, 3600s * 1000ms = 3600000ms = 1h
#define MIN_STATE_PERIOD 10000
#define MAX_STATE_PERIOD 18000000
#define STATE_CHANGE_MULTIPLIER 10000
State states[] = {
  {
   12000,
   1000,
   "Work"
  },
  {
   10000,
   200,
   "Pause 1"
  },
  {
   3698000,
   1000,
   "Pause 2"
  },
};

const size_t NUM_OF_STATES = sizeof(states)/sizeof(*states);
