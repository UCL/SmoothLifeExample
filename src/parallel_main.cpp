#include <iostream>
#include <fstream>
#include <sstream>
#include <cassert>
#include "Smooth.h"
#include <mpi.h>

int main(int argc, char **argv){
  MPI_Init (&argc, &argv);
  int rank, size;
  MPI_Comm_rank (MPI_COMM_WORLD, &rank);
  MPI_Comm_size (MPI_COMM_WORLD, &size);
  std::string config_path = argv[1];
  std::ifstream config_file(config_path);
  int width;
  int height;
  int range;
  int spots;
  int frames;
  std::string label;
  config_file >> label >> width;
  assert(label=="width:");
  config_file >> label >> height;
  assert(label=="height:");
  config_file >> label >> range ;
  assert(label=="range:");
  Smooth smooth(width,height,range,rank,size);
  config_file >> label >> frames;
  assert(label=="frames:");
  std::cout << "Rank " << rank << " set up with range " << range << " and size " << width << "x" << height << std::endl;
  std::cout << "Rank " << rank << " will render " << frames << " frames." << std::endl;
  config_file >> label >> spots ;
  std::cout << "Seeding " << spots << " spots into rank " << rank << std::endl;
  assert(label=="spots:");
  for (int i=0;i<spots;i++){
    smooth.SeedRandomDisk();
  }
  std::ostringstream fname;
  fname << "frames" << rank << ".dat" << std::flush;
  std::ofstream outfile(fname.str().c_str());
  outfile << smooth.LocalXSize() << ", " << smooth.Sizey() << ", " << rank << ", " << size << std::endl;
  std::cout << "Rank " << rank << "ready" << std::endl;
  while (smooth.Frame()<frames) {
    smooth.Write(outfile);
    smooth.CommunicateMPI();
    smooth.QuickUpdate();
    std::cout << "Rank " << rank << " completed frame: " << smooth.Frame() << std::endl;
  }
  MPI_Finalize();
}
