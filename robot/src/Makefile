CC=gcc								
CFLAGS=-Wall 
CFLAGS_OPENCV = -I/usr/include/opencv -I$(HOME)/git/robidouille/raspicam_cv 	
LDFLAGS2_OPENCV = -lopencv_highgui -lopencv_core -lopencv_legacy -lopencv_video -lopencv_features2d -lopencv_calib3d -lopencv_imgproc -lpthread -lm
LDFLAGS= -lwiringPi -lpthread  -D_REENTRANT
LDFLAGS_PI =  -L$(HOME)/git/raspberrypi/userland/build/lib -lmmal_core -lmmal -l mmal_util -lvcos -lbcm_host	
Q_LEARNING_PATH=../../ai/q_learning/src/

DEBUG=yes
ifeq ($(DEBUG),yes)
CFLAGS+=-g -funwind-tables
endif

all: testCamera colourTrack

colourTrack: colourTrack.o stylus.o imageBroadcast.o imageProcessing.o tracking.o pipeTracking.o ia.o q_learning.o qmatrix.o file_manager.o game_state.o utils.o
	$(CC) $^ $(LDFLAGS) $(LDFLAGS2_OPENCV) $(LDFLAGS_PI) -L./lib/ lib/libraspicamcv.a -o $@

testCamera: testCamera.o stylus.o
	$(CC) $^ $(LDFLAGS) $(LDFLAGS2_OPENCV) $(LDFLAGS_PI) -L./lib/ lib/libraspicamcv.a -o $@

testCamera.o: stylus.h configuration.h
stylus.o: stylus.h configuration.h
colourTrack.o: stylus.h configuration.h imageBroadcast.h tracking.h imageProcessing.h pipeTracking.h $(Q_LEARNING_PATH)q_learning.h
imageBroadcast.o: imageBroadcast.h configuration.h tracking.h pipeTracking.h
imageProcessing.o: imageProcessing.h tracking.h imageBroadcast.h
tracking.o: tracking.h configuration.h imageBroadcast.h imageProcessing.h
pipeTracking.o: pipeTracking.h configuration.h
ia.o: ia.h stylus.h $(Q_LEARNING_PATH)q_learning.h $(Q_LEARNING_PATH)conf.h
q_learning.o: $(Q_LEARNING_PATH)qmatrix.h $(Q_LEARNING_PATH)file_manager.h $(Q_LEARNING_PATH)utils.h $(Q_LEARNING_PATH)conf.h
qmatrix.o: $(Q_LEARNING_PATH)game_state.h $(Q_LEARNING_PATH)utils.h $(Q_LEARNING_PATH)conf.h
file_manager.o: $(Q_LEARNING_PATH)qmatrix.h
game_state.o: $(Q_LEARNING_PATH)conf.h

%.o: $(Q_LEARNING_PATH)%.c
	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.c
	$(CC) $(CFLAGS) $(CFLAGS_OPENCV) -c $< -o $@

clean:
	rm -f *.o
	rm -f testCamera colourTrack

