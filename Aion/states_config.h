typedef struct State {
  uint32_t period;
  uint32_t pausePeriod;
  char verboseName[15];
  uint8_t type;
};

// 60s * 60s = 3600s = 1h, 3600s * 1000ms = 3600000ms = 1h
#define MIN_STATE_PERIOD 10000
#define MAX_STATE_PERIOD 18000000
#define STATE_CHANGE_MULTIPLIER 10000
State states[] = {
  {
   3000,
   4000,
   "Work I",
   0,
  },
  {
   3000,
   4000,
   "Pause I",
   1,
  },
  {
   10000,
   4000,
   "Work II",
   0,
  },
  {
   10000,
   4000,
   "Pause II",
   1,
  },
  {
   10000,
   4000,
   "Work III",
   0,
  },
  {
   10000,
   4000,
   "Pause III",
   1,
  },
};

const size_t NUM_OF_STATES = sizeof(states)/sizeof(*states);
const size_t SIZE_OF_STATE = sizeof(State);
