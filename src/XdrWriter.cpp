#include "XdrWriter.h"
void XDRWriter::Write() {

  char * start_to_write=reinterpret_cast<char*>(smooth.StartOfWritingBlock());
  unsigned int element_count=smooth.LocalXSize()*smooth.Sizey();
  xdr_vector(&xdrfile,start_to_write,element_count,sizeof(double),reinterpret_cast<xdrproc_t>(xdr_double));
}

XDRWriter::~XDRWriter(){
}

XDRWriter::XDRWriter(Smooth & smooth, int rank, int size)
    :SmoothWriter(smooth,rank,size)
{
     fname << "." << rank << std::flush;    
     std::string mode("w");
     std::FILE * myFile = std::fopen(fname.str().c_str(),mode.c_str());
     xdrstdio_create(&xdrfile, myFile, XDR_ENCODE);
}

void XDRWriter::Header(int frames){
  int sizey=smooth.Sizey();
  int sizex=smooth.LocalXSize();
  xdr_int(&xdrfile,&sizex);
  xdr_int(&xdrfile,&sizey);
  xdr_int(&xdrfile,&rank);
  xdr_int(&xdrfile,&size);
  xdr_int(&xdrfile,&frames);

}

