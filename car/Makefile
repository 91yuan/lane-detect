LIBS = `pkg-config --cflags --libs opencv`
INSTALL_PATH= /root/mjpeg_face_leaf_detection/mjpg-streamer/

all:
	gcc -o ./initial_car ./initial_car.c
	g++ -o ./facedetection ./facedetection.c ${LIBS}

install:
	cp -a ./facedetection $(INSTALL_PATH) 

clean:
	rm -rf ./facedetection
