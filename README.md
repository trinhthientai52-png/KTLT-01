# HỆ THỐNG QUẢN LÝ KHO HÀNG (WAREHOUSE MANAGEMENT SYSTEM)
> **Đề tài Bài tập lớn Học phần: Kỹ thuật Lập trình C (C99)**  
> Trường Đại học Giao thông Vận tải - Phân hiệu tại TP. Hồ Chí Minh (UTC2)  
> Khoa Công nghệ Thông tin - Bộ môn Công nghệ Thông tin

---

## 👥 Danh sách thành viên nhóm thực hiện (5 Thành viên)

| STT | Họ và tên sinh viên | Mã số sinh viên (MSSV) | Vai trò trong nhóm | Tệp tin phụ trách |
|:---:|:---|:---:|:---|:---|
| 1 | **LÊ PHẠM HOÀNG NAM** | **6751071050** | **Nhóm trưởng** / Tech Lead | `types.h`, `main.c`, `build.bat`, `.gitignore`, `README.md` |
| 2 | **LÃ THUẬN THẢO** | **6751071070** | Thành viên (Developer) | `utils.h`, `utils.c` |
| 3 | **TRỊNH THIÊN TÀI** | **6751071068** | Thành viên (Developer) | `sanpham.h`, `sanpham.c` |
| 4 | **NGUYỄN MINH TÂM** | **6751071069** | Thành viên (Developer) | `nhanvien.h`, `nhanvien.c`, `baocao.h`, `baocao.c` |
| 5 | **NGUYỄN TRẦN GIA HUY** | **6751071026** | Thành viên (Developer) | `giaodich.h`, `giaodich.c`, `fileio.h`, `fileio.c` |

---

## 📌 Giới thiệu dự án
Ứng dụng quản lý kho hàng chạy trên giao diện dòng lệnh (Console CLI), được xây dựng hoàn toàn bằng ngôn ngữ C (tiêu chuẩn C99) theo kiến trúc module hóa:
* **Quản lý danh mục sản phẩm**: Thêm, sửa, xóa, tìm kiếm và thuật toán sắp xếp `qsort` đa tiêu chí.
* **Quản lý lô hàng & Xuất nhập kho**: Kiểm soát tồn kho an toàn, theo dõi hạn sử dụng theo nguyên tắc FEFO/FIFO.
* **Bảo mật & Phân quyền**: Đăng nhập phân quyền (`ROLE_ADMIN` vs `ROLE_NHAN_VIEN`), cơ chế khóa tài khoản lũy tiến chống vét cạn (brute-force).
* **Báo cáo & Thống kê**: Tự động cảnh báo hàng cận date/hết hạn, cảnh báo chạm ngưỡng tồn kho tối thiểu, tính tổng giá trị kho hàng.
* **Lưu trữ dữ liệu an toàn**: Đọc/ghi tệp nhị phân (`.dat`) tự động và xuất báo cáo bảng tính định dạng CSV chuẩn RFC 4180.

---

## 🛠 Hướng dẫn biên dịch và chạy

### Cách 1: Sử dụng kịch bản tự động
Chạy tệp `build.bat` trên Windows (CMD hoặc PowerShell):
```cmd
build.bat
```

### Cách 2: Biên dịch thủ công bằng GCC
```cmd
gcc -Wall -Wextra -std=c99 -o quanlykho.exe main.c utils.c sanpham.c nhanvien.c giaodich.c baocao.c fileio.c
quanlykho.exe
```

---

## 🔑 Tài khoản mặc định thử nghiệm
* **Quản trị viên (Admin)**: `admin` / `admin123` (Toàn quyền 16 chức năng)
* **Nhân viên kho**: `nhanvien1` / `123456`
