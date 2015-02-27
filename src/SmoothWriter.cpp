#include <sstream>
#include <fstream>
#include "SmoothWriter.h"

void SmoothWriter::Write() {
   for (int x=smooth.Range();x<smooth.LocalXSize()+smooth.Range();x++) {
     for (int y=0;y<smooth.Sizey();y++) {
        *outfile << smooth.Field(x,y) << " , ";
     }
     *outfile << std::endl;
   }
   *outfile<< std::endl;
}

SmoothWriter::~SmoothWriter(){
  delete outfile; // closes it
}

SmoothWriter::SmoothWriter(Smooth & smooth, int rank, int size)
    :smooth(smooth),rank(rank),size(size)
{
     std::ostringstream fname;
     fname << "frames" << rank << ".dat" << std::flush;
     outfile=new std::ofstream(fname.str().c_str());
}

void SmoothWriter::Header(){
  *outfile << smooth.LocalXSize() << ", " << smooth.Sizey() << ", " << rank << ", " << size << std::endl;
}
