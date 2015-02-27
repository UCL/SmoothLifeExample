import sys,os
import numpy
import matplotlib.animation
import matplotlib.pyplot
from StringIO import StringIO

folder=sys.argv[1]

with open(os.path.join(folder,'frames0.dat')) as data:
    header=numpy.fromfile(data,'i4',5)
    rows,columns,rank,size,frame_count=header
    print header

process_frames=[]
for process in range(size):
    with open(os.path.join(folder,'frames'+str(process)+'.dat')) as data:
        header=numpy.fromfile(data,'i4',5)
        width, height, rank, size, frame_count = header
        buffer=numpy.fromfile(data,'<f8',frame_count*width*height)
        print buffer.shape
        print frame_count,width,height
        print buffer[0]
        process_frames.append(buffer.reshape([frame_count,width,height]))

frames=numpy.concatenate(process_frames, 1)
figure=matplotlib.pyplot.figure()
def animate(frame_id):
    print "Processing frame", frame_id
    matplotlib.pyplot.imshow(frames[frame_id],vmin=0,vmax=1)

anim=matplotlib.animation.FuncAnimation(figure,animate,frame_count,interval=100)
anim.save(os.path.join(folder,'smooth.mp4'))

