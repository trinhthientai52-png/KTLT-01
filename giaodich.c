#include "giaodich.h"
#include "sanpham.h"
#include "utils.h"

static void tao_ma_gd(KhoHang *kho, char *ma) {
    sprintf(ma, "GD%04d", kho->so_gd + 1);
}

static void them_giao_dich(KhoHang *kho, GiaoDich gd) {
    if (kho->so_gd >= kho->cap_gd) {
        kho->cap_gd *= 2;
        kho->ds_gd = (GiaoDich *)realloc(kho->ds_gd, kho->cap_gd * sizeof(GiaoDich));
    }
    kho->ds_gd[kho->so_gd++] = gd;
}

void nhap_hang_theo_lo(KhoHang *kho) {
    in_header("NHAP HANG VAO KHO THEO LO", NULL, kho);

    char ngay_nhap[MAX_NGAY];
    char hom_nay[MAX_NGAY];
    ngay_hom_nay(hom_nay);

    printf("  Nhap Ngay nhap (go Enter dung ngay hom nay: %s): ", hom_nay);
    doc_chuoi(ngay_nhap, MAX_NGAY);
    if (strlen(ngay_nhap) == 0) strcpy(ngay_nhap, hom_nay);

    if (!ngay_hop_le(ngay_nhap)) {
        in_loi("Dinh dang ngay khong hop le (can DD/MM/YYYY)!\n");
        pause_screen();
        return;
    }

    printf("  So luong mat hang trong lo: ");
    int n = doc_so_nguyen();
    if (n <= 0) return;

    int thanh_cong = 0;
    for (int i = 0; i < n; i++) {
        printf("\n  --- Mat hang %d/%d ---\n", i + 1, n);
        char ma_sp[MAX_MA];
        printf("  Ma san pham: ");
        doc_chuoi(ma_sp, MAX_MA);
        if (strlen(ma_sp) == 0) continue;

        int vi_tri = tim_vi_tri_sp(kho, ma_sp);
        int so_luong = 0;
        double gia_nhap = 0;
        char ma_lo[MAX_MA];
        sprintf(ma_lo, "LO%04d", kho->so_lo + 1);

        if (vi_tri >= 0) {
            SanPham *sp = &kho->ds_sp[vi_tri];
            printf("  [Da co: %s - Ton: %d sp - HSD gan nhat: %s]\n", sp->ten, sp->so_luong, sp->ngay_het_han);
            printf("  So luong nhap them: ");
            so_luong = doc_so_nguyen();
            if (so_luong <= 0) continue;

            printf("  Don gia nhap (hien tai %.0f): ", sp->gia_nhap);
            gia_nhap = doc_so_thuc();
            if (gia_nhap <= 0) gia_nhap = sp->gia_nhap;

            char ngay_het_han_lo[MAX_NGAY];
            printf("  Ngay het han cua lo moi nay (DD/MM/YYYY): ");
            doc_chuoi(ngay_het_han_lo, MAX_NGAY);
            if (!ngay_hop_le(ngay_het_han_lo)) {
                in_loi("Ngay khong hop le, bo qua mat hang nay.\n");
                continue;
            }

            sp->gia_nhap = gia_nhap;
            strcpy(sp->ngay_nhap, ngay_nhap);

            LoHang lo;
            strcpy(lo.ma_lo, ma_lo);
            strcpy(lo.ma_sp, ma_sp);
            lo.so_luong = so_luong;
            lo.gia_nhap = gia_nhap;
            strcpy(lo.ngay_nhap, ngay_nhap);
            strcpy(lo.ngay_het_han, ngay_het_han_lo);
            them_lo_hang(kho, lo);

            cap_nhat_ton_va_date_tu_lo(kho, ma_sp);
            printf("  -> Da tao lo moi: %s (SL: %d, HSD: %s)\n", ma_lo, so_luong, ngay_het_han_lo);
        } else {
            printf("  [San pham moi]\n");
            SanPham sp;
            memset(&sp, 0, sizeof(SanPham));
            strcpy(sp.ma_sp, ma_sp);

            printf("  Ten san pham: "); doc_chuoi(sp.ten, MAX_TEN);
            printf("  Danh muc: "); doc_chuoi(sp.danh_muc, MAX_DANHMUC);
            printf("  So luong nhap: "); so_luong = doc_so_nguyen();
            if (so_luong <= 0) continue;
            sp.so_luong = so_luong;

            printf("  Gia nhap: "); gia_nhap = doc_so_thuc();
            sp.gia_nhap = gia_nhap;
            printf("  Gia ban du kien: "); sp.gia_ban = doc_so_thuc();
            printf("  Ngay het han (DD/MM/YYYY): "); doc_chuoi(sp.ngay_het_han, MAX_NGAY);
            if (!ngay_hop_le(sp.ngay_het_han)) {
                in_loi("Ngay khong hop le, bo qua san pham nay.\n");
                continue;
            }
            printf("  Nguong canh bao ton: "); sp.nguong = doc_so_nguyen();

            strcpy(sp.ngay_nhap, ngay_nhap);
            them_san_pham(kho, sp);

            LoHang lo;
            strcpy(lo.ma_lo, ma_lo);
            strcpy(lo.ma_sp, ma_sp);
            lo.so_luong = so_luong;
            lo.gia_nhap = gia_nhap;
            strcpy(lo.ngay_nhap, ngay_nhap);
            strcpy(lo.ngay_het_han, sp.ngay_het_han);
            them_lo_hang(kho, lo);

            cap_nhat_ton_va_date_tu_lo(kho, ma_sp);
            printf("  -> Da tao lo ban dau: %s (SL: %d, HSD: %s)\n", ma_lo, so_luong, sp.ngay_het_han);
        }

        GiaoDich gd;
        tao_ma_gd(kho, gd.ma_gd);
        strcpy(gd.ma_sp, ma_sp);
        strcpy(gd.ma_nv, kho->nguoi_dung->ma_nv);
        gd.loai = GD_NHAP;
        gd.so_luong = so_luong;
        gd.don_gia = gia_nhap;
        strcpy(gd.ngay, ngay_nhap);
        them_giao_dich(kho, gd);

        thanh_cong++;
    }

    printf("\n");
    printf("  [+] Nhap lo hang thanh cong (%d san pham)!\n", thanh_cong);
    pause_screen();
}

void xuat_hang(KhoHang *kho) {
    in_header("XUAT KHO HANG HOA", NULL, kho);

    printf("  Nhap Ma san pham can xuat (go '0' de huy): ");
    char ma_sp[MAX_MA];
    doc_chuoi(ma_sp, MAX_MA);
    if (strcmp(ma_sp, "0") == 0 || strlen(ma_sp) == 0) return;

    int vi_tri = tim_vi_tri_sp(kho, ma_sp);
    if (vi_tri < 0) {
        in_loi("Khong tim thay san pham!\n");
        pause_screen();
        return;
    }

    SanPham *sp = &kho->ds_sp[vi_tri];
    printf("\n  San pham: %s - %s | Danh muc: %s\n", sp->ma_sp, sp->ten, sp->danh_muc);
    printf("  Ton kho hien tai: %d sp | Don gia ban: %.0f | Ngay het han: %s\n",
           sp->so_luong, sp->gia_ban, sp->ngay_het_han);

    if (sp->trang_thai == DA_HET_HAN) {
        printf("\n  ");
        in_canh_bao("San pham da het han su dung! Van muon xuat huy? (Y/N): ");
        char xn[5];
        doc_chuoi(xn, sizeof(xn));
        if (xn[0] != 'Y' && xn[0] != 'y') return;
    }

    printf("  Nhap so luong can xuat: ");
    int sl = doc_so_nguyen();
    if (sl <= 0) {
        in_loi("So luong xuat phai > 0!\n");
        pause_screen();
        return;
    }

    if (sl > sp->so_luong) {
        in_loi("So luong xuat vuot qua ton kho kha dung!\n");
        pause_screen();
        return;
    }

    // Kiem tra FEFO: Tim cac lo hang cua san pham con ton
    int idx_lo[100];
    int so_lo_sp = 0;
    for (int i = 0; i < kho->so_lo; i++) {
        if (strcmp(kho->ds_lo[i].ma_sp, ma_sp) == 0 && kho->ds_lo[i].so_luong > 0) {
            if (so_lo_sp < 100) {
                idx_lo[so_lo_sp++] = i;
            }
        }
    }

    if (so_lo_sp > 0) {
        // Sap xep cac lo theo ngay het han tang dan (FEFO: Het han truoc xuat truoc)
        for (int i = 0; i < so_lo_sp - 1; i++) {
            for (int j = i + 1; j < so_lo_sp; j++) {
                if (so_sanh_ngay(kho->ds_lo[idx_lo[i]].ngay_het_han, kho->ds_lo[idx_lo[j]].ngay_het_han) > 0) {
                    int tmp = idx_lo[i];
                    idx_lo[i] = idx_lo[j];
                    idx_lo[j] = tmp;
                }
            }
        }

        // Tru ton tung lo theo thu tu FEFO
        int sl_con_lai = sl;
        printf("\n  [XUAT THEO NGUYEN TAC FEFO - HET HAN TRUOC XUAT TRUOC]:\n");
        for (int i = 0; i < so_lo_sp && sl_con_lai > 0; i++) {
            LoHang *lo = &kho->ds_lo[idx_lo[i]];
            int tru = (lo->so_luong >= sl_con_lai) ? sl_con_lai : lo->so_luong;
            lo->so_luong -= tru;
            sl_con_lai -= tru;
            printf("   * Lo %-6s | HSD: %-10s | Xuat: %4d sp | Con lai: %4d sp\n",
                   lo->ma_lo, lo->ngay_het_han, tru, lo->so_luong);
        }

        cap_nhat_ton_va_date_tu_lo(kho, ma_sp);
    } else {
        sp->so_luong -= sl;
        cap_nhat_trang_thai(sp);
    }

    GiaoDich gd;
    tao_ma_gd(kho, gd.ma_gd);
    strcpy(gd.ma_sp, ma_sp);
    strcpy(gd.ma_nv, kho->nguoi_dung->ma_nv);
    gd.loai = GD_XUAT;
    gd.so_luong = sl;
    gd.don_gia = sp->gia_ban;
    ngay_hom_nay(gd.ngay);
    them_giao_dich(kho, gd);

    double thanh_tien = (double)sl * sp->gia_ban;
    char thanh_tien_str[32];
    format_tien(thanh_tien, thanh_tien_str);

    printf("\n");
    in_thanh_cong("Xuat kho thanh cong!\n");
    printf("  Ma phieu: %s | So luong: %d sp | Thanh tien: %s | Ton con: %d sp\n",
           gd.ma_gd, sl, thanh_tien_str, sp->so_luong);
    if (sp->so_luong > 0) {
        printf("  (HSD lo tiep theo: %s)\n", sp->ngay_het_han);
    }

    if (sp->so_luong <= sp->nguong) {
        in_canh_bao("Ton kho da cham/duoi nguong an toan!\n");
    }

    pause_screen();
}

void xem_lich_su_gd(KhoHang *kho) {
    in_header("LICH SU GIAO DICH XUAT / NHAP", NULL, kho);

    if (kho->so_gd == 0) {
        in_canh_bao("Chua co giao dich nao!\n");
        pause_screen();
        return;
    }

    set_mau(MAU_XANH_DUONG);
    printf("+----------+----------+---------+---------+----------+--------------+----------------+------------+\n");
    set_mau(MAU_VANG);
    printf("|  Ma GD   |   Loai   |  Ma SP  |  Ma NV  | So luong |    Don gia   |   Thanh tien   |    Ngay    |\n");
    set_mau(MAU_XANH_DUONG);
    printf("+----------+----------+---------+---------+----------+--------------+----------------+------------+\n");
    reset_mau();

    double tong_nhap = 0, tong_xuat = 0;
    for (int i = 0; i < kho->so_gd; i++) {
        GiaoDich *gd = &kho->ds_gd[i];
        double tt = (double)gd->so_luong * gd->don_gia;
        char don_gia_str[24], thanh_tien_str[24];
        format_tien(gd->don_gia, don_gia_str);
        format_tien(tt, thanh_tien_str);

        printf("| %-8s | %-8s | %-7s | %-7s | %8d | %12s | %14s | %-10s |\n",
               gd->ma_gd,
               (gd->loai == GD_NHAP) ? "Nhap" : "Xuat",
               gd->ma_sp, gd->ma_nv, gd->so_luong, don_gia_str, thanh_tien_str, gd->ngay);

        if (gd->loai == GD_NHAP) tong_nhap += tt;
        else tong_xuat += tt;
    }

    set_mau(MAU_XANH_DUONG);
    printf("+----------+----------+---------+---------+----------+--------------+----------------+------------+\n");
    reset_mau();

    char tong_nhap_str[32], tong_xuat_str[32];
    format_tien(tong_nhap, tong_nhap_str);
    format_tien(tong_xuat, tong_xuat_str);
    printf("  Tong nhap: %s | Tong xuat: %s\n", tong_nhap_str, tong_xuat_str);

    pause_screen();
}
