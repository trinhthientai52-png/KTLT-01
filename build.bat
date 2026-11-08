@echo off
echo Compiling Quan Ly Kho Hang...
gcc -Wall -std=c99 -o quanlykho.exe main.c utils.c sanpham.c nhanvien.c giaodich.c baocao.c fileio.c
if %errorlevel% equ 0 (
    echo Build thanh cong! Chay: quanlykho.exe
) else (
    echo Build that bai!
)
pause
