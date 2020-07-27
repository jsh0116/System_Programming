# System_Programming

## Advanced Echo Server

### 1. Intro
이번 Assignment3-2에서는 Advanced echo server를 구현하는 것이다. client에서 text 내용을 입력하면 그 정보를 server에 보낸 후 text를 그대로 다시 client로 가져와 출력한다. server에서는 client에서 text를 보낼 때 child process를 생성한 후 parent와 child에서 각각의 역할수행이 이루어진다. 보낸 text가 QUIT일 때 Parent에서 child status 판단, Child process terminated 처리를 구현하고 다중 접속할 때도 추가하였다. 쉽게 확인하기 위해서 loopback “127.0.0.1”을 사용하였다.

### 2. Flow Chart
![image](https://user-images.githubusercontent.com/62865808/88546174-f4bccc00-d056-11ea-81cc-3c8c492391b0.png)

### 3. Result

* Server 실행, Client 실행 및 연결 확인

![image](https://user-images.githubusercontent.com/62865808/88546237-0aca8c80-d057-11ea-8fcc-6f14dd10065f.png)

* Text 전송 및 수신 확인

![image](https://user-images.githubusercontent.com/62865808/88546325-2e8dd280-d057-11ea-9098-e34856c34a4b.png)

* 서버에 QUIT 명령어 전달 후 SIGALRM, SIGCHLD 호출

![image](https://user-images.githubusercontent.com/62865808/88546356-3baac180-d057-11ea-87ad-62bc47771483.png)

그 이후 1~2초 후에 Client 자동 종료

* Client 다중 접속한 경우

![image](https://user-images.githubusercontent.com/62865808/88546433-55e49f80-d057-11ea-9b74-5d88aee66fea.png)

