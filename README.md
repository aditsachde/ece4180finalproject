# ece4180finalproject

## About 
### Centralized AHRS System
We were tasked with designing a project utilizing the skills and knowledge taught in ECE 4180 Embedded Systems.

Our project is based loosely on the Georgia Tech Experimental Rocketry (GTXR) team's flight computers, which utilize a ethernet network onboard the rocket. This network is also connected to the ground network while on the launch pad. 

Our implementation uses IMU and an ethernet connection to display the real time attitude of the mbed device on a website, and provides an API to allow other devices to query and use the current attitude. Many systems require access to the attitude of the launch vehicle, however the IMUs used are very expensive. This setup allows a single IMU to be used without sacrificing access to data.

### Features

- Obtains the orientation of the device, using a Madgwick to fuse the various sensors on the LSM9DS1 chip and get better data.

- Displays the website over an ethernet link between a PC and the mbed. This website displays the orientation of the mbed as calculated by the Madgwick filter. It also has a refresh mechanism to ensure that the orientation is automatically updated without requiring user interaction.

- Provides an API, used by the example terminal based project in the `api_demo` folder.

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
  - This filter is implemented with C++ classes, making it far easier to integrate into other programs.
- HTTP server
  - The server implements both `GET` and `POST` requests.
  - `GET` is used to serve the website
  - `POST` is used to provide the realtime API
- Remote Access
  - Utilizes Cloudflare tunnel, running on a PC with network access to the mbed as a bridge.
  - This solution also allows the website to be accessed with a url (https://aprs.koven.dev) instead of an IP address.

### Website Demo
<img width="529" alt="image" src="https://user-images.githubusercontent.com/61746589/165414752-313a4dc4-3bd5-4256-9f5b-e49d2f98db5e.png">


# Results
- The filter provides accurate data for pitch and yaw
  - The filter does not integrate the magnetometer, resulting in an inaccurate roll axis. This is because the axis is perpendicular to the gravity vector.
  - The filter lags slightly. It stabilizes to the correct attitude, but could stablize faster if it was better tuned to the particular hardware.
- API is simple and easy to use
  - It does not use the most appropriate HTTP method, but is an acceptable tradeoff for simplicity of implementation.
