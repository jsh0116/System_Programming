# System_Programming

## User Authentication_Access Control

### 1. Intro
이번 Assignment 4-1에서는 User Authentication과 Access Control 을 구현하는 것이다.
client에서 server로 해당 port와 IP 주소로 접근을 시도하면 Server에서 접근 가능한 IP인지
access.txt에 저장되어 있는 IP주소(wildcard 처리 포함)로 판단하고 접근 불가능할 때는 올바른
IP주소로 접속을 요구하고 접근 가능할 때 passwd에 저장되어 있는 ID와 PASSWORD를
바탕으로 로그인이 성공했을 때와 3번 실패했을 때 다시 접속하도록 구현하였다.

### 2. Flow Chart
![image](https://user-images.githubusercontent.com/62865808/88549355-2899f080-d05b-11ea-86e9-250cae074892.png)

### 3. Result

* 접속이 불가한 IP를 가진 Client가 접속할 경우

![image](https://user-images.githubusercontent.com/62865808/88549485-4d8e6380-d05b-11ea-90fb-02592fbe37d8.png)

* 성공적으로 로그인을 마친 경우

![image](https://user-images.githubusercontent.com/62865808/88549649-86c6d380-d05b-11ea-9ea9-0b24bdc9634f.png)

* 로그인을 세 번 시도했지만 모두 실패한 경우

![image](https://user-images.githubusercontent.com/62865808/88549728-a2ca7500-d05b-11ea-87bf-1a49e3f49fad.png)
