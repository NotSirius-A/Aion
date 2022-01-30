typedef struct State {
  uint32_t period;
  uint32_t pause_period;
  char verboseName[30];
};

State states[] = {
  {
   3698000,
   1000,
   "Work"
  },
  {
   5000,
   200,
   "Pause 1"
  },
  {
   2000,
   1000,
   "Pause 2"
  },
};

size_t numOfStates = sizeof(states)/sizeof(*states);
