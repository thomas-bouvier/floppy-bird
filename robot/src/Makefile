CC=gcc								
CFLAGS=-Wall 
CFLAGS_OPENCV = -I/usr/include/opencv -I$(HOME)/git/robidouille/raspicam_cv 	
LDFLAGS2_OPENCV = -lopencv_highgui -lopencv_core -lopencv_legacy -lopencv_video -lopencv_features2d -lopencv_calib3d -lopencv_imgproc -lpthread -lm
LDFLAGS= -lwiringPi   
LDFLAGS_PI =  -L$(HOME)/git/raspberrypi/userland/build/lib -lmmal_core -lmmal -l mmal_util -lvcos -lbcm_host	


DEBUG=no
ifeq ($(DEBUG),yes)
CFLAGS+=-g
endif

all: testCamera colourTrack

colourTrack: colourTrack.o stylus.o imageBroadcast.o imageProcessing.o tracking.o
	$(CC) $^ $(LDFLAGS) $(LDFLAGS2_OPENCV) $(LDFLAGS_PI) -L./lib/ lib/libraspicamcv.a -o $@

testCamera: testCamera.o stylus.o
	$(CC) $^ $(LDFLAGS) $(LDFLAGS2_OPENCV) $(LDFLAGS_PI) -L./lib/ lib/libraspicamcv.a -o $@

testCamera.o: stylus.h configuration.h
stylus.o: stylus.h configuration.h
colourTrack.o: stylus.h configuration.h RaspiCamCV.h imageBroadcast.h tracking.h
imageBroadcast.o: imageBroadcast.h configuration.h tracking.h RaspiCamCV.h
imageProcessing.o: imageProcessing.h tracking.h imageBroadcast.h
tracking.o: tracking.h configuration.h imageBroadcast.h imageProcessing.h

%.o: %.c
	$(CC) $(CFLAGS) $(CFLAGS_OPENCV) -c $< -o $@

clean:
	rm -f *.o
	rm -f testCamera colourTrack

