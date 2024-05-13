uint32_t PCG_Hahs(uint32_t input) {
  uint32_t state = input * 747796405u + 2891336453u;
  uint32_t word = ((state >> ((state >> 28u) + 4u)) ^ state) * 27803737u;
  return (word >> 22u) ^ word;
}

float getRandomFloat(uint32_t& seed) {
  seed = PCG_Hahs(seed);
  return (float)seed / (float)UINT32_MAX;
}