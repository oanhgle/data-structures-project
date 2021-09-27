#include <iostream>
#include <unordered_map>
#include <cstring>
using namespace std;

#include "hilbert.h"

const int HILBERT_LEVEL = 8;
const int IMAGE_SIZE = 600;
const string OUTPUT_FILE = "hilbert.bmp";

int main()
{
  HilbertCurve curve(HILBERT_LEVEL, IMAGE_SIZE);
  curve.writeToFile(OUTPUT_FILE);
  
  return 0;
}