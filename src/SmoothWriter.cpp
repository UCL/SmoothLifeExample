#include <sstream>
#include <fstream>
#include "SmoothWriter.h"

void SmoothWriter::Write() {
  outfile->write(reinterpret_cast<char*>(smooth.StartOfWritingBlock()),
      smooth.LocalXSize()*smooth.Sizey()*sizeof(double));
}

SmoothWriter::~SmoothWriter(){
  delete outfile; // closes it
}

SmoothWriter::SmoothWriter(Smooth & smooth, int rank, int size)
    :smooth(smooth),rank(rank),size(size)
{
     std::ostringstream fname;
     fname << "frames" << rank << ".dat" << std::flush;
     outfile=new std::ofstream(fname.str().c_str(),std::ios::binary);
}

void SmoothWriter::Header(int frames){
  int sizey=smooth.Sizey();
  int sizex=smooth.LocalXSize();
  outfile->write(reinterpret_cast<char*>(&sizex),sizeof(int));
  outfile->write(reinterpret_cast<char*>(&sizey),sizeof(int));
  outfile->write(reinterpret_cast<char*>(&rank),sizeof(int));
  outfile->write(reinterpret_cast<char*>(&size),sizeof(int));
  outfile->write(reinterpret_cast<char*>(&frames),sizeof(int));
}
