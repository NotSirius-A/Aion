
int32_t getTimeUntilTransition(unsigned long now, uint32_t lastTransitionTime, uint8_t currentState, uint32_t stateLengths[]) {
  int32_t rv = stateLengths[currentState] - (now - lastTransitionTime);
  return rv;
}
