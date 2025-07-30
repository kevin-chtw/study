@echo off
echo 正在生成文件.......

for /r . %%i in (*.proto) do (
protoc -I=.\  --cpp_out=.\  .\%%~nxi
)

echo 拷贝文件......
copy *.cc ..\TKProto\
copy *.h ..\TKProto\

echo 清除文件......
del /f /s /q *.cc
del /f /s /q *.h

echo 文件生成完毕
pause