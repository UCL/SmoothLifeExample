#include <mpi.h>
#include "Smooth.h"
#include <cstdio>
#include <rpc/types.h>
#include <rpc/xdr.h>

class SmoothWriter{
  public:
    SmoothWriter(Smooth & smooth, int rank, int size);
    ~SmoothWriter();
    void Write();
    void Header(int frames);
  private:
    Smooth &smooth;
    XDR xdrfile;
    int rank;
    int size;
};
