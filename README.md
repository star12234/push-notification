gcc -municode -o 원하는 파일이름.exe 실행하고자하는파일.c -lgdi32

ex ) gcc -municode -o prototype.exe prototype.c -lgdi32

-lgdi32는 winapi를 사용하기 위해 포함해야하는 flag 입니다.
exe파일과 c파일의 순서를 틀리면 c파일이 삭제됩니다.


zip파일은 아이콘을 바꿔서 올려놓았습니다.실행이 안된다면 말씀해주세요
