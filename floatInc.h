float incFloat(float currFloat, float incFloat) {
  int decimalPoint = 1;
  float tempIncFloat = incFloat;
  while (tempIncFloat <= 1) {
    decimalPoint *= 10;
    tempIncFloat *= 10;
  }
  return round((currFloat + incFloat) * decimalPoint) / decimalPoint;
}