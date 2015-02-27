#include <mpi.h>
#include "Smooth.h"

class SmoothWriter{
  public:
    SmoothWriter(Smooth & smooth, int rank, int size);
    virtual ~SmoothWriter(){};
    virtual void Write()=0;
    virtual void Header()=0;
    virtual void Close(){};
  protected:
    Smooth &smooth;
    int rank;
    int size;
};
