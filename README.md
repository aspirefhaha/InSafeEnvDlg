# Build Method

## in windows

```
set PATH=D:\VBox\qt-4.7.3\bin;%PATH%;
lupdate -pro InSaveEnvDlg.pro
lrelease zh_CN.ts
```

now could build in vs2010

如果需要编译64位程序
需要打开vs2010的64位命令行程序
把qt的64位的bin目录增加到PATH中来，因为需要使用qmake

1. qmake InSafeEnvDlg.pro
2. nmake (注意，需要将之前存在的release 和debug目录删除，否则会出现x86的 module conflict错误，因为之前调试的环境编译出了32位的库文件)
3. 或者 nmake release