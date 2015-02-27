#include <sstream>
#include <fstream>
#include "BinaryWriter.h"

void BinaryWriter::Write() {
  outfile->write(reinterpret_cast<char*>(smooth.StartOfWritingBlock()),
      smooth.LocalXSize()*smooth.Sizey()*sizeof(double));
}

BinaryWriter::~BinaryWriter(){
  delete outfile; // closes it
}

BinaryWriter::BinaryWriter(Smooth & smooth, int rank, int size)
    :SmoothWriter(smooth,rank,size)
{
     std::ostringstream fname;
     fname << "frames" << rank << ".dat" << std::flush;
     outfile=new std::ofstream(fname.str().c_str(),std::ios::binary);
}

void BinaryWriter::Header(int frames){
  int sizey=smooth.Sizey();
  int sizex=smooth.LocalXSize();
  outfile->write(reinterpret_cast<char*>(&sizex),sizeof(int));
  outfile->write(reinterpret_cast<char*>(&sizey),sizeof(int));
  outfile->write(reinterpret_cast<char*>(&rank),sizeof(int));
  outfile->write(reinterpret_cast<char*>(&size),sizeof(int));
  outfile->write(reinterpret_cast<char*>(&frames),sizeof(int));
}
