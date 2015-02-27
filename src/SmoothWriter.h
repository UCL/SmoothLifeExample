#include <mpi.h>
#include "Smooth.h"

class SmoothWriter{
  public:
    SmoothWriter(Smooth & smooth, int rank, int size);
    ~SmoothWriter();
    void Write();
    void Header();
  private:
    Smooth &smooth;
    std::ostream *outfile;
    int rank;
    int size;
};