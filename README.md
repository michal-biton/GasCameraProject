# GasCameraProject
Bootcamp project
The system receives a record [description] of the temperature in the air and checks where and if there are gas leaks.
The application uses a dynamic library that manages the reception of the images and processes them into a video that represents the state of temperatures in colors that symbolize the heat levels.
The management and activation of the functions is carried out by a command line adapted to the camera commands
The beginning of the recording is carried out with the help of the PIPELINE method which contains 5 steps. Each stage is powered by a separate wire. The communication between the different wires is carried out by a column synchronized by means of a binary semaphore.
Each cycle in PIPELINE creates the image of the gas in the room
So every second 25 images are created that are part of a video
The stages of the PIPELINE:
Capture - creating a gas state image in space that is transferred to the RGB stage
RGB_convertor: converts the image to RGB format according to the temperature of the gas and transfers the converted image to the YUV phase.
YUV_convertor: converts an image from RGB format to YUV format and passes it to the DECODER stage
DECODER : Creates a video file in mpeg-TS format using the ffmpeg library.
End of recording:
Terminates the recording operation in a controlled manner. After the current cycle ends, the THREADS will be freed and the dynamically allocated data structures
A performance test was conducted for each THREAD (output vs. waiting and sleeping)
Creating an image:
Take an image while recording by saving the current image from the recording, if there is no recording in the background we will perform the first two steps of the pipeline once: and RGB just capture and write the encoded image to a file.
Using a json file for the system configurations for example: the image will be in PPM or JPAG format according to the settings of the config read from the file.
Implementation of RPC for the camera in the UDP protocol (client, server):
Client side which includes the creation of a test tool - cmd. The client writes the action it wants to run with the required number of parameters. (For example: in order to record, the customer must enter the name of the function, start and the time of the video) The system checks the integrity of the input including the number of parameters and sends an instruction to the server to execute the command.
The server side is constantly waiting for the client's requests, when a request arrives it decodes the request, activates what is required and returns an answer to the client about the success or failure of the execution.
Experience status: GPIO - LED located on the raspberry pi
Another thread running in the background that translates the system's operation into LED flashing,
When the recording operation the flashing is fast, when recording stops the flashing slowly until it stops completely and when a picture is created the LED will light up continuously.