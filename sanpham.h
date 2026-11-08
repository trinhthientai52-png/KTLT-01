#ifndef SANPHAM_H
#define SANPHAM_H

#include "types.h"

void them_san_pham(KhoHang *kho, SanPham sp);
int tim_vi_tri_sp(KhoHang *kho, const char *ma_sp);
void cap_nhat_trang_thai(SanPham *sp);
void cap_nhat_tat_ca_trang_thai(KhoHang *kho);
void hien_thi_mot_sp(SanPham *sp, int stt);
void xem_danh_sach_sp(KhoHang *kho);
void xem_chi_tiet_lo_hang(KhoHang *kho);
void them_lo_hang(KhoHang *kho, LoHang lo);
void cap_nhat_ton_va_date_tu_lo(KhoHang *kho, const char *ma_sp);
void sua_san_pham(KhoHang *kho);
void xoa_san_pham(KhoHang *kho);
void tim_kiem_sp(KhoHang *kho);
void sap_xep_sp(KhoHang *kho);
void dat_nguong_canh_bao(KhoHang *kho);

#endif
