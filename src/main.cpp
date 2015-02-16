#include <iostream>
#include <fstream>
#include "Smooth.h"

int main(int argc, char **argv){
  Smooth smooth(300);
  smooth.SeedRing();
  std::ofstream outfile("test.dat");
  while (smooth.Frame()<10) {
    smooth.Write(outfile);
    smooth.QuickUpdate();
    std::cout << smooth.Frame() << " " << std::flush;
  }
}
