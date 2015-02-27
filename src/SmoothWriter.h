#include <mpi.h>
#include "Smooth.h"
#include <cstdio>

class SmoothWriter{
  public:
    SmoothWriter(Smooth & smooth, int rank, int size);
    ~SmoothWriter();
    void Write();
    void Close();
    void Header(int frames);
  private:
    Smooth &smooth;
    MPI_File outfile;
    int rank;
    int size;
};
