@echo off
echo ���������ļ�.......

for /r . %%i in (*.proto) do (
protoc -I=.\  --cpp_out=.\  .\%%~nxi
)

echo �����ļ�......
copy *.cc ..\TKProto\
copy *.h ..\TKProto\

echo ����ļ�......
del /f /s /q *.cc
del /f /s /q *.h

echo �ļ��������
pause