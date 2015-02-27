import sys,os
import numpy
import matplotlib.animation
import matplotlib.pyplot
from StringIO import StringIO

folder=sys.argv[1]

with open(os.path.join(folder,'frames.dat')) as data:
    header=numpy.fromfile(data,'>i4',4)
    width,height,size,frame_count=header
    print header
    buffer=numpy.fromfile(data,'>f8',frame_count*width*height)
    print frame_count,width,height
    frames=buffer.reshape(frame_count,width,height)

figure=matplotlib.pyplot.figure()
def animate(frame_id):
    print "Processing frame", frame_id
    matplotlib.pyplot.imshow(frames[frame_id],vmin=0,vmax=1)

anim=matplotlib.animation.FuncAnimation(figure,animate,frame_count,interval=100)
anim.save(os.path.join(folder,'smooth.mp4'))

