#ifndef TYPES_H
#define TYPES_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_TEN 100
#define MAX_MA 20
#define MAX_NGAY 11
#define MAX_DANHMUC 50
#define MAX_USERNAME 50
#define MAX_PASSWORD 50
#define INIT_CAP 50
#define NGAY_CANH_BAO 30
#define MAX_SAI 3
#define THOI_GIAN_KHOA_BASE 300

typedef enum {
    CON_HANG,
    SAP_HET_HANG,
    SAP_HET_HAN,
    DA_HET_HAN
} TrangThai;

typedef enum {
    ROLE_ADMIN,
    ROLE_NHAN_VIEN
} VaiTro;

typedef enum {
    GD_NHAP,
    GD_XUAT
} LoaiGD;

typedef struct {
    char ma_sp[MAX_MA];
    char ten[MAX_TEN];
    char danh_muc[MAX_DANHMUC];
    int so_luong;
    int nguong;
    double gia_nhap;
    double gia_ban;
    char ngay_nhap[MAX_NGAY];
    char ngay_het_han[MAX_NGAY];
    TrangThai trang_thai;
} SanPham;

typedef struct {
    char ma_lo[MAX_MA];
    char ma_sp[MAX_MA];
    int so_luong;
    double gia_nhap;
    char ngay_nhap[MAX_NGAY];
    char ngay_het_han[MAX_NGAY];
} LoHang;

typedef struct {
    char ma_nv[MAX_MA];
    char ten[MAX_TEN];
    char username[MAX_USERNAME];
    char password[MAX_PASSWORD];
    VaiTro vai_tro;
    int bi_khoa;
    int so_lan_sai;
    time_t thoi_gian_khoa;
    int lan_khoa;
} NhanVien;

typedef struct {
    char ma_gd[MAX_MA];
    char ma_sp[MAX_MA];
    char ma_nv[MAX_MA];
    LoaiGD loai;
    int so_luong;
    double don_gia;
    char ngay[MAX_NGAY];
} GiaoDich;

typedef struct {
    char username[MAX_USERNAME];
    char thoi_gian[64];
    char noi_dung[200];
} CanhBao;

typedef struct {
    SanPham *ds_sp;
    int so_sp, cap_sp;
    LoHang *ds_lo;
    int so_lo, cap_lo;
    NhanVien *ds_nv;
    int so_nv, cap_nv;
    GiaoDich *ds_gd;
    int so_gd, cap_gd;
    CanhBao *ds_cb;
    int so_cb, cap_cb;
    NhanVien *nguoi_dung;
} KhoHang;

#endif
