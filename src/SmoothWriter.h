#ifndef ONCE_SMOOTH_WRITER
#define ONCE_SMOOTH_WRITER

#include <mpi.h>
#include "Smooth.h"
#include <sstream>

class SmoothWriter{
  public:
    SmoothWriter(Smooth & smooth, int rank, int size);
    virtual ~SmoothWriter(){};
    virtual void Write()=0;
    virtual void Header(int frames)=0;
    virtual void Close(){};
  protected:
    Smooth &smooth;
    int rank;
    int size;
    std::ostringstream fname;
};

#endif // ONCE_SMOOTH_WRITER
