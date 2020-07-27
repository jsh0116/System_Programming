# System_Programming

## ls command socket communication

### 1. Intro
이번 Assignment3-1에서는 FTP “ls” command를 socket programming을 통해 구현하는 것이다. client에서 command 내용을 입력하면 그 정보를 server에 보내서 processing 한 후, 결과값을 다시 client로 가져와 출력하는 것이다. 쉽게 확인하기 위해서 loopback “127.0.0.1”을 사용하였다. socket(), connect(), write() 함수를 사용해 client를 구현하고, socket(), bind(), listen() 등의 함수를 사용해 server를 구현한다. 

### 2. Flow Chart
* Basic TCP/IP Client & Server structure

![image](https://user-images.githubusercontent.com/62865808/88546174-f4bccc00-d056-11ea-81cc-3c8c492391b0.png)

* Designed flow chart

![image](https://user-images.githubusercontent.com/62865808/88551842-4d439780-d05e-11ea-9a4b-e326e2660c30.png)

### 3. Result

![image](https://user-images.githubusercontent.com/62865808/88551931-68aea280-d05e-11ea-9fe0-f607f983cd3e.png)
