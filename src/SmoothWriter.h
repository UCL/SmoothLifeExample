#include <mpi.h>
#include "Smooth.h"
#include <cstdio>
#include <H5Cpp.h>

class SmoothWriter{
  public:
    SmoothWriter(Smooth & smooth, int rank, int size);
    ~SmoothWriter();
    void Write();
    void Close();
    void Header(int frames);
  private:
    Smooth &smooth;
    H5::H5File outfile;
    int rank;
    int size;
};
