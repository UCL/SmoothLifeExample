#include <sstream>
#include <fstream>
#include <mpi.h>
#include "SmoothWriter.h"

void SmoothWriter::Write() {

  unsigned int total_element_count=smooth.Sizex()*smooth.Sizey();
  unsigned int local_element_count=smooth.LocalXSize()*smooth.Sizey();
 
  int offset=4*sizeof(int) + // The header
             rank*local_element_count*sizeof(double) + // Offset within the frame
             smooth.Frame()*total_element_count*sizeof(double); // Frame offset in the file

  MPI_File_seek(outfile,offset,MPI_SEEK_SET);
  MPI_File_write(outfile,smooth.StartOfWritingBlock(),local_element_count,MPI_DOUBLE,MPI_STATUS_IGNORE);
}

void SmoothWriter::Close(){
}

SmoothWriter::~SmoothWriter(){
}

SmoothWriter::SmoothWriter(Smooth & smooth, int rank, int size)
    :smooth(smooth),rank(rank),size(size)
{
     std::ostringstream fname;
     fname << "frames.dat" << std::flush;
     outfile.openFile( fname.str(), H5F_ACC_TRUNC );
}

void SmoothWriter::Header(int frames){
  if (rank!=0) {
    return;
  }
  int sizey=smooth.Sizey();
  int sizex=smooth.Sizex();
  MPI_File_write(outfile,&sizex,1,MPI_INT,MPI_STATUS_IGNORE);
  MPI_File_write(outfile,&sizey,1,MPI_INT,MPI_STATUS_IGNORE);
  MPI_File_write(outfile,&size,1,MPI_INT,MPI_STATUS_IGNORE);
  MPI_File_write(outfile,&frames,1,MPI_INT,MPI_STATUS_IGNORE);
}

