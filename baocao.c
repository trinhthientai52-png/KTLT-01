#include "baocao.h"
#include "sanpham.h"
#include "utils.h"

void canh_bao_ton_kho(KhoHang *kho) {
    cap_nhat_tat_ca_trang_thai(kho);
    in_header("CANH BAO TON KHO", NULL, kho);

    int d1 = 0, d2 = 0, d3 = 0;

    /* 1. HANG DA HET HAN */
    set_mau(MAU_DO);
    printf("--- [!] HANG DA HET HAN ---\n");
    reset_mau();
    for (int i = 0; i < kho->so_sp; i++) {
        if (kho->ds_sp[i].trang_thai == DA_HET_HAN) {
            SanPham *sp = &kho->ds_sp[i];
            printf("  - %s: %s (Ton: %d sp, Het han: %s)\n",
                   sp->ma_sp, sp->ten, sp->so_luong, sp->ngay_het_han);
            d1++;
        }
    }
    if (d1 == 0) printf("  (Khong co)\n");

    /* 2. HANG SAP HET HAN */
    printf("\n");
    set_mau(MAU_TIM);
    printf("--- [*] HANG SAP HET HAN (< %d ngay) ---\n", NGAY_CANH_BAO);
    reset_mau();
    for (int i = 0; i < kho->so_sp; i++) {
        if (kho->ds_sp[i].trang_thai == SAP_HET_HAN) {
            SanPham *sp = &kho->ds_sp[i];
            int con = ngay_con_lai(sp->ngay_het_han);
            printf("  - %s: %s (Ton: %d sp, Het han: %s - Con %d ngay)\n",
                   sp->ma_sp, sp->ten, sp->so_luong, sp->ngay_het_han, con);
            d2++;
        }
    }
    if (d2 == 0) printf("  (Khong co)\n");

    /* 3. HANG SAP HET HANG */
    printf("\n");
    set_mau(MAU_VANG);
    printf("--- [-] HANG SAP HET HANG (Duoi nguong ton an toan) ---\n");
    reset_mau();
    for (int i = 0; i < kho->so_sp; i++) {
        if (kho->ds_sp[i].trang_thai == SAP_HET_HANG) {
            SanPham *sp = &kho->ds_sp[i];
            printf("  - %s: %s (Ton: %d sp / Nguong: %d sp)\n",
                   sp->ma_sp, sp->ten, sp->so_luong, sp->nguong);
            d3++;
        }
    }
    if (d3 == 0) printf("  (Khong co)\n");

    printf("\n------------------------------------------------------------\n");
    printf("  Tong ket: %d het han | %d sap het han | %d sap het hang\n", d1, d2, d3);

    pause_screen();
}

void bao_cao_thong_ke(KhoHang *kho) {
    cap_nhat_tat_ca_trang_thai(kho);
    in_header("BAO CAO THONG KE TONG QUAN", NULL, kho);

    int tong_sl = 0;
    double tong_gt = 0;
    int dem[4] = {0};
    for (int i = 0; i < kho->so_sp; i++) {
        tong_sl += kho->ds_sp[i].so_luong;
        tong_gt += (double)kho->ds_sp[i].so_luong * kho->ds_sp[i].gia_nhap;
        dem[kho->ds_sp[i].trang_thai]++;
    }

    char tong_gt_str[32], tong_sl_str[32];
    format_tien(tong_gt, tong_gt_str);
    format_so(tong_sl, tong_sl_str);

    printf("  - Tong so mat hang  : %d ma\n", kho->so_sp);
    printf("  - Tong so luong ton : %s sp\n", tong_sl_str);
    printf("  - Tong gia tri kho  : %s\n", tong_gt_str);
    printf("  - Tong so giao dich : %d phieu\n", kho->so_gd);
    printf("  - So nhan vien      : %d\n\n", kho->so_nv);

    printf("  Trang thai hang hoa:\n");
    printf("    + Con hang        : %d\n", dem[CON_HANG]);
    printf("    + Sap het hang    : %d\n", dem[SAP_HET_HANG]);
    printf("    + Sap het han     : %d\n", dem[SAP_HET_HAN]);
    printf("    + Da het han      : %d\n", dem[DA_HET_HAN]);

    if (kho->so_sp > 0) {
        printf("\n  Top 5 mat hang gia tri ton lon nhat:\n");
        int idx[5];
        for (int k = 0; k < 5 && k < kho->so_sp; k++) {
            double mx = -1; idx[k] = -1;
            for (int i = 0; i < kho->so_sp; i++) {
                int skip = 0;
                for (int j = 0; j < k; j++) if (idx[j] == i) { skip = 1; break; }
                if (skip) continue;
                double v = (double)kho->ds_sp[i].so_luong * kho->ds_sp[i].gia_nhap;
                if (v > mx) { mx = v; idx[k] = i; }
            }
            if (idx[k] >= 0) {
                SanPham *sp = &kho->ds_sp[idx[k]];
                double v = (double)sp->so_luong * sp->gia_nhap;
                char val_str[24];
                format_tien(v, val_str);
                printf("    %d. %s - %s: %s (Ton: %d sp)\n",
                       k + 1, sp->ma_sp, sp->ten, val_str, sp->so_luong);
            }
        }
    }

    pause_screen();
}

void tong_gia_tri_kho(KhoHang *kho) {
    in_header("TONG GIA TRI KHO THEO DANH MUC", NULL, kho);

    char dm[100][MAX_DANHMUC];
    double gt[100];
    int sl[100];
    int sku[100];
    int so_dm = 0;
    double tong = 0;
    long long tong_so_luong = 0;

    for (int i = 0; i < kho->so_sp; i++) {
        double val = (double)kho->ds_sp[i].so_luong * kho->ds_sp[i].gia_nhap;
        tong += val;
        tong_so_luong += kho->ds_sp[i].so_luong;

        int found = -1;
        for (int j = 0; j < so_dm; j++) {
            if (strcmp(dm[j], kho->ds_sp[i].danh_muc) == 0) { found = j; break; }
        }
        if (found >= 0) {
            gt[found] += val;
            sl[found] += kho->ds_sp[i].so_luong;
            sku[found]++;
        } else if (so_dm < 100) {
            strcpy(dm[so_dm], kho->ds_sp[i].danh_muc);
            gt[so_dm] = val;
            sl[so_dm] = kho->ds_sp[i].so_luong;
            sku[so_dm] = 1;
            so_dm++;
        }
    }

    set_mau(MAU_XANH_DUONG);
    printf("+-----+----------------------+-------+------------+---------------+--------+\n");
    set_mau(MAU_VANG);
    printf("| STT | Danh muc             | So MH |  Tong ton  | Gia tri (VND) | Ty le  |\n");
    set_mau(MAU_XANH_DUONG);
    printf("+-----+----------------------+-------+------------+---------------+--------+\n");
    reset_mau();

    for (int i = 0; i < so_dm; i++) {
        double pct = tong > 0 ? (gt[i] / tong * 100) : 0;
        char gt_str[24], sl_str[16];
        format_tien(gt[i], gt_str);
        format_so(sl[i], sl_str);

        printf("| %3d | %-20.20s | %5d | %10s | %13s | %5.1f%% |\n",
               i + 1, dm[i], sku[i], sl_str, gt_str, pct);
    }

    set_mau(MAU_XANH_DUONG);
    printf("+-----+----------------------+-------+------------+---------------+--------+\n");
    reset_mau();

    char tong_gt_str[32], tong_sl_str[32];
    format_tien(tong, tong_gt_str);
    format_so(tong_so_luong, tong_sl_str);

    printf("  Tong toan bo kho: %d mat hang | Tong ton: %s sp | Tong gia tri: %s\n",
           kho->so_sp, tong_sl_str, tong_gt_str);

    pause_screen();
}
