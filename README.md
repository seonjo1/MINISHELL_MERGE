# MINISHELL
나만의 SHELL 만들기

## MINISHELL 실행 
```
./minishell
```

<img width="670" alt="스크린샷 2024-06-26 오후 3 21 33" src="https://github.com/seonjo1/MINISHELL_MERGE/assets/127729846/54355ab5-849c-4b66-861a-dd0d92f85796">

## Referencing the Bash Shell

- Bash Manual 을 참고하여 만들었기 때문에, Bash Shell과 유사하게 동작
- 따라서 사용법도 Bash Shell 과 같음
- Bash Shell 의 기능을 전부 구현한 것은 아님

## MINISHELL 의 기능
### Signal

- **ctrl-C (SIGINT)**
- **ctr-\ (SIGQUIT)**
- **ctr-D (EOF)**

### Command

- **quote**
  - **'**
    - 작은 따옴표 안에 있는 문자들은 하나의 인수로 처리
    - 환경 변수 확장을 허용하지 않음
    - 작은 따옴표의 짝이 맞지 않으면 systax error 처리
  - **"**
    - 큰 따옴표 안에 있는 문자들은 하나의 인수로 처리
    - 환경 변수 확장을 허용
    - 큰 따옴표의 짝이 맞지 않으면 systax error 처리

  - **환경 변수 확장 예시**

    <img width="497" alt="스크린샷 2024-06-26 오후 3 47 57" src="https://github.com/seonjo1/MINISHELL_MERGE/assets/127729846/6696d68e-2bc8-4049-ac16-43c3cb59882e">

- **redirection operator**
  - **< file**
    - input 을 file 로 redirection
  - **\> file**
    - 기존 file 에 output 을 덮어 쓰기
  - **<< LIMITER**
    - heredoc 기능
    - input 을 LIMITER 가 입력되기 전까지 standard input 으로 받음
  - **\>> file**
    - 기존 file 에 output 을 이어 쓰기

- **control operator**
  - **cmd1 \| cmd2**
    - 명령어를 이어주는 제어 연산자로 cmd1 의 output 을 cmd2 의 input 으로 제공
    - 각 cmd 들은 멀티 프로세싱으로 실행 됨

  - **pipeline1 && pipeline2**
    - pipeline 은 cmd 들의 집합
    - pipeline1 의 실행 결과, exit status 가 0 일 때만 (pipeline1 이 성공일 때만) pipeline2 를 실행
    - BONUS 기능

  - **pipeline1 || pipeline2** 
    - pipeline1 의 실행 결과, exit status 가 0 이 아닐 때만 (pipeline1 이 실패할 때만) pipeline2 를 실행
    - BONUS 기능

  - **( list )**
    - list 는 pipeline 들의 집합
    - || 나 && 연산자가 명령어 실행 결과를 확인 할때 ( ) 전체의 결과를 하나로 처리
    - BONUS 기능

  - **그 외**
    - ; ;; & \n 등의 control operator 기능은 구현하지 않음

- **Variables**
  - **environment variable**
    - minishell 실행시 envp 로 들어오는 환경 변수들
    - builtin 함수를 통해 추가 및 삭제 가능
  - **$?**
    - 가장 최근에 실행한 명령어의 exitcode 로 치환되는 특수 변수
  - **그 외**
    - $$, $0, $# 등 특수 변수들은 구현하지 않음

  
### Builtin Function
- **builtin function 은 $PATH 와 상관없이 언제든 실행 가능**
  - **echo**
    - -n 옵션 : 개행을 붙이지 않고 출력
  - **cd**
    - no arguments : $HOME 으로 이동
  - **pwd** 
  - **export**
  - **unset**
  - **env**
  - **exit**

## MINISHELL의 간단한 구현 로직
- **Read commandline**
  - 사용자가 입력한 commandline 을 Read
- **Tokenization**
  - commandline 을 metacharacter 기준으로 토큰화
- **Parsing**
  - 다음 EBNF 를 참고하여 토큰들로 재귀 하향 파서 구현
    ```
    EBNF
    
  	<list>	        ::= <pipeline> {("&&" | "||") <pipeline>}
  
  	<pipeline>	::= "(" <list> ")" | <command> {"|" <command>}  

  	<command>	::= <command_part> {<command_part>}  

  	<command_part>	::= (">" | ">>" | "<" | "<<") <word> | <word>
    ```
  - parse tree 를 만들면서 syntax error 도 확인
- **Tree traversal**
  - 트리를 순회하며 Expansion, Quote Removal, Redirection, wildcard 처리함 
    - Expansion 
    - Quote Removal
    - Redirection
- **Execution**

## MINISHELL 구현의 특징


### build

---
<img src="./.img/125.png" width=250px/>
