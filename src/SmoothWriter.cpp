#include <sstream>
#include <fstream>
#include "SmoothWriter.h"

SmoothWriter::SmoothWriter(Smooth & smooth, int rank, int size)
    :smooth(smooth),rank(rank),size(size){
     fname << "frames.dat" << std::flush;
};

