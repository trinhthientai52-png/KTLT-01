#ifndef UTILS_H
#define UTILS_H

#include "types.h"

/* Bang mau chuan Windows Console */
#define MAU_MAC_DINH     7   /* Xam mac dinh */
#define MAU_XANH_DUONG   11  /* Cyan sang - Tieu de, Khung vien, Nhap kho */
#define MAU_XANH_LA      10  /* Xanh la sang - Thanh cong, Con hang, Xuat kho */
#define MAU_VANG         14  /* Vang sang - Canh bao, Menu phan muc */
#define MAU_DO           12  /* Do sang - Loi, Da het han, Bi khoa */
#define MAU_TRANG_DAM    15  /* Trang sang - So lieu, Phim tat, Nhap lieu */
#define MAU_XAM           8  /* Xam toi - Duong ke vien phu */
#define MAU_TIM          13  /* Tim/Hong sang - Danh muc, Don gia */

void set_mau(int mau);
void reset_mau(void);

void init_console(void);
void clear_screen(void);
void pause_screen(void);
void doc_chuoi(char *buf, int max_len);
int doc_so_nguyen(void);
double doc_so_thuc(void);
void nhap_mat_khau(char *buf, int max_len);
int ngay_hop_le(const char *ngay);
int so_sanh_ngay(const char *ngay1, const char *ngay2);
void ngay_hom_nay(char *buf);
int ngay_con_lai(const char *ngay_het_han);
void in_duong_ke(int len);
void in_duong_ngang(int len, char ch);
const char* trang_thai_str(TrangThai tt);

/* Dinh dang tien te & so lieu */
void format_tien(double tien, char *buf);
void format_so(long long so, char *buf);

/* Tien ich hien thi Header & Khung */
void in_header(const char *tieu_de, const char *bread_crumb, const KhoHang *kho);
void in_tieu_de(const char *text);
void in_thanh_cong(const char *text);
void in_loi(const char *text);
void in_canh_bao(const char *text);
void in_thong_tin(const char *text);
void in_badge_trang_thai(TrangThai tt);
int dem_canh_bao_ton(const KhoHang *kho);

#endif
