#include <iostream>
#include <fstream>
#include "Smooth.h"

int main(int argc, char **argv){
  Smooth smooth;
  smooth.SeedDisk();
  std::ofstream outfile("test.dat");
  while (smooth.Frame()<100) {
    smooth.Write(outfile);
    smooth.Update();
    std::cout << smooth.Frame() << " " << std::flush;
  }
}
