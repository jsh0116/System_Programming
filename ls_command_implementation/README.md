# System_Programming

## 'ls' command implementation

### 1. Intro
Assignment2-2는 ls command implemetation으로 input 디렉토리 경로의 정보를 출력하는 것인데 그 중 only file name만 출력하도록 했다 (‘.’ : current directory, ‘..’ : parent directory 포함) 디렉토리 정보를 확인하기 위해 opendir(), readdir(), closedir()를 사용하였고, 파일 정보인지 확인하기 위해 lstat function, current, parent directory를 포함하여 출력하기 위해 strcmp function를 사용하였다.

### 2. Flow Chart
![image](https://user-images.githubusercontent.com/62865808/88551171-84657900-d05d-11ea-85f9-77889fc8e412.png)

### 3. Result

* File existed in Directory

![image](https://user-images.githubusercontent.com/62865808/88551289-a9f28280-d05d-11ea-8803-65cf55163bfb.png)

* No file in Directory

![image](https://user-images.githubusercontent.com/62865808/88551392-c7bfe780-d05d-11ea-89cb-3850e07ce431.png)
