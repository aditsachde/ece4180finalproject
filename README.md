# ece4180finalproject

## About 
### Centralized AHRS System
We were tasked with designing a project utilizing the skills and knowledge taught in ECE 4180 Embedded Systems.

Our Project is based loosely on the Georgia Tech Experimental Rocketry (GTXR) team's off the shelf amatuer rocketry flight computers which implements an ethernet network onboard the rocket that is also connected to the ground network while on the launch pad. 


Our implementation uses IMU and an ethernet connection to display the real time orientation of the mbed device on a website. 

### Features

- Obtains the orientation of the device, we will implement a Mahony or Madgwick to fuse the various sensors on the LSM9DS1 chip and get better data.

- Displays the website, we will use a point to point ethernet link between a PC and the mbed. This website will display the orientation of the mbed as calculated by the IMU filter. It will also have a refresh mechanism to ensure that the orientation is automatically updated without requiring user interaction.

  - These two functionalities will be implemented as separated threads, using a mutex or similar coordination mechanism to access the device orientation, which both functions need to read or update.

- Demonstrates this being integrated into a larger system, we plan to add an additional mbed or raspberry pi to the network. This device will query the mbed and have a simple demo, such as turning on an LED when the mbed is horizontal.

## Hardware
- MBED Microcontroller LPC1768
<img width="211" alt="image" src="https://user-images.githubusercontent.com/61746589/165410689-6b4865a8-90df-4283-958e-799f0c6c65d4.png">
<img width="250" alt="image" src="https://user-images.githubusercontent.com/61746589/165411090-4b59a8ab-6962-40d7-84da-d0d102b33483.png">

- IMU LSM9DS1 9 dof stick. | https://os.mbed.com/components/LSM9DS1-IMU/
<img width="230" alt="image" src="https://user-images.githubusercontent.com/61746589/165410469-9179f012-0fe8-4537-8067-d112359e96ba.png">

- Ethernet | https://os.mbed.com/platforms/IBMethernetKit/ 

<img width="178" alt="image" src="https://user-images.githubusercontent.com/61746589/165409733-cbef4fcb-86dd-4c78-aae2-972beff941e8.png"> 
<img width="187" alt="image" src="https://user-images.githubusercontent.com/61746589/165398851-032c4776-c351-44d8-8462-a3c286d3f17f.png">

### Schematic and Final Design
<img width="400" alt="image" src="https://user-images.githubusercontent.com/61746589/165412891-d5a6c617-780c-40e2-9a77-5ca26e5b5cd7.png">
<img width="200" alt="image" src="https://user-images.githubusercontent.com/61746589/165412936-377efc39-786a-4140-afe1-2e7739ef688b.png">


## Software
- Madgwick Filter
- Implemented with C++ classes
- Website
- HTTP server that supports GET and POST
- Remote Access
- Utilizes cloud infrastructure from Cloudflare
### Website Demo
<img width="529" alt="image" src="https://user-images.githubusercontent.com/61746589/165414752-313a4dc4-3bd5-4256-9f5b-e49d2f98db5e.png">


# Results
- Works pretty well for pitch and yaw
- API is simple and easy to use

- Extract filter into separate library
- Calibrate to improve response times
- Integrate magnetometer




