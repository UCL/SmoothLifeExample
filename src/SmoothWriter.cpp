#include <sstream>
#include <fstream>
#include <mpi.h>
#include "SmoothWriter.h"

void SmoothWriter::Write() {

  unsigned int total_element_count=smooth.Sizex()*smooth.Sizey();
  unsigned int local_element_count=smooth.LocalXSize()*smooth.Sizey();
  double * receive_buffer;

  if (rank==0){
     receive_buffer=new double[total_element_count];
  }
  
  MPI_Gather(smooth.StartOfWritingBlock(),local_element_count,MPI_DOUBLE,
      receive_buffer,local_element_count,MPI_DOUBLE,0,MPI_COMM_WORLD); 

  if (rank==0){
     xdr_vector(&xdrfile,reinterpret_cast<char*>(receive_buffer),
         total_element_count,sizeof(double),reinterpret_cast<xdrproc_t>(xdr_double));
  }
}

SmoothWriter::~SmoothWriter(){
}

SmoothWriter::SmoothWriter(Smooth & smooth, int rank, int size)
    :smooth(smooth),rank(rank),size(size)
{
     if (rank!=0) {
       return;
     }
     std::ostringstream fname;
     fname << "frames.dat" << std::flush;
     std::string mode("w");
     std::FILE * myFile = std::fopen(fname.str().c_str(),mode.c_str());
     xdrstdio_create(&xdrfile, myFile, XDR_ENCODE);
}

void SmoothWriter::Header(int frames){
  if (rank!=0) {
    return;
  }
  int sizey=smooth.Sizey();
  int sizex=smooth.Sizex();
  xdr_int(&xdrfile,&sizex);
  xdr_int(&xdrfile,&sizey);
  xdr_int(&xdrfile,&size);
  xdr_int(&xdrfile,&frames);

}

