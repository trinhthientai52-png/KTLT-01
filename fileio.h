#ifndef FILEIO_H
#define FILEIO_H

#include "types.h"

#define FILE_SP "data_sanpham.dat"
#define FILE_LO "data_lohang.dat"
#define FILE_NV "data_nhanvien.dat"
#define FILE_GD "data_giaodich.dat"
#define FILE_CB "data_canhbao.dat"

void luu_du_lieu(KhoHang *kho);
void luu_du_lieu_auto(KhoHang *kho);
void tai_du_lieu(KhoHang *kho);
void tai_du_lieu_auto(KhoHang *kho);
void xuat_csv(KhoHang *kho);

#endif
