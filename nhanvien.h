#ifndef NHANVIEN_H
#define NHANVIEN_H

#include "types.h"

void tao_admin_mac_dinh(KhoHang *kho);
int dang_nhap(KhoHang *kho);
void them_canh_bao(KhoHang *kho, const char *username, const char *noi_dung);
void menu_quan_ly_nv(KhoHang *kho);

#endif
