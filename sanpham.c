#include "sanpham.h"
#include "utils.h"

void them_san_pham(KhoHang *kho, SanPham sp) {
    if (kho->so_sp >= kho->cap_sp) {
        kho->cap_sp *= 2;
        kho->ds_sp = (SanPham *)realloc(kho->ds_sp, kho->cap_sp * sizeof(SanPham));
    }
    cap_nhat_trang_thai(&sp);
    kho->ds_sp[kho->so_sp++] = sp;
}

void them_lo_hang(KhoHang *kho, LoHang lo) {
    if (kho->so_lo >= kho->cap_lo) {
        kho->cap_lo = (kho->cap_lo == 0) ? INIT_CAP : kho->cap_lo * 2;
        kho->ds_lo = (LoHang *)realloc(kho->ds_lo, kho->cap_lo * sizeof(LoHang));
    }
    kho->ds_lo[kho->so_lo++] = lo;
}

void cap_nhat_ton_va_date_tu_lo(KhoHang *kho, const char *ma_sp) {
    int v_sp = tim_vi_tri_sp(kho, ma_sp);
    if (v_sp < 0) return;
    SanPham *sp = &kho->ds_sp[v_sp];

    int tong_ton = 0;
    int co_lo = 0;
    char date_som_nhat[MAX_NGAY] = "";

    for (int i = 0; i < kho->so_lo; i++) {
        if (strcmp(kho->ds_lo[i].ma_sp, ma_sp) == 0) {
            co_lo = 1;
            if (kho->ds_lo[i].so_luong > 0) {
                tong_ton += kho->ds_lo[i].so_luong;
                if (strlen(date_som_nhat) == 0 || so_sanh_ngay(kho->ds_lo[i].ngay_het_han, date_som_nhat) < 0) {
                    strcpy(date_som_nhat, kho->ds_lo[i].ngay_het_han);
                }
            }
        }
    }

    if (co_lo) {
        sp->so_luong = tong_ton;
        if (strlen(date_som_nhat) > 0) {
            strcpy(sp->ngay_het_han, date_som_nhat);
        }
    }
    cap_nhat_trang_thai(sp);
}

int tim_vi_tri_sp(KhoHang *kho, const char *ma_sp) {
    for (int i = 0; i < kho->so_sp; i++) {
        if (strcmp(kho->ds_sp[i].ma_sp, ma_sp) == 0) return i;
    }
    return -1;
}

void cap_nhat_trang_thai(SanPham *sp) {
    int ngay = ngay_con_lai(sp->ngay_het_han);
    if (ngay <= 0)
        sp->trang_thai = DA_HET_HAN;
    else if (ngay <= NGAY_CANH_BAO)
        sp->trang_thai = SAP_HET_HAN;
    else if (sp->so_luong <= sp->nguong)
        sp->trang_thai = SAP_HET_HANG;
    else
        sp->trang_thai = CON_HANG;
}

void cap_nhat_tat_ca_trang_thai(KhoHang *kho) {
    for (int i = 0; i < kho->so_sp; i++)
        cap_nhat_trang_thai(&kho->ds_sp[i]);
}

static void in_duong_vien_bang(void) {
    set_mau(MAU_XANH_DUONG);
    printf("+-----+-------+--------------------+--------------+------+------------+------------+---------------+------------+\n");
    reset_mau();
}

static void in_tieu_de_cot_bang(void) {
    in_duong_vien_bang();
    set_mau(MAU_VANG);
    printf("| STT | Ma SP | Ten san pham       | Danh muc     |  SL  |  Gia nhap  |  Gia ban   |  Het han    | Trang thai    |\n");
    reset_mau();
    in_duong_vien_bang();
}

void hien_thi_mot_sp(SanPham *sp, int stt) {
    char gia_nhap_str[24], gia_ban_str[24];
    format_tien(sp->gia_nhap, gia_nhap_str);
    format_tien(sp->gia_ban, gia_ban_str);

    printf("| %3d | %-5s | %-18.18s | %-12.12s | %4d | %10s | %10s | %-11s | ",
           stt, sp->ma_sp, sp->ten, sp->danh_muc, sp->so_luong,
           gia_nhap_str, gia_ban_str, sp->ngay_het_han);

    switch (sp->trang_thai) {
        case CON_HANG:
            set_mau(MAU_XANH_LA);
            printf("Con hang      ");
            break;
        case SAP_HET_HANG:
            set_mau(MAU_VANG);
            printf("Sap het hang  ");
            break;
        case SAP_HET_HAN:
            set_mau(MAU_TIM);
            printf("Sap het han   ");
            break;
        case DA_HET_HAN:
            set_mau(MAU_DO);
            printf("DA HET HAN    ");
            break;
        default:
            printf("N/A           ");
            break;
    }
    reset_mau();
    printf("|\n");
}

void xem_danh_sach_sp(KhoHang *kho) {
    cap_nhat_tat_ca_trang_thai(kho);
    in_header("DANH SACH HANG HOA TRONG KHO", NULL, kho);

    if (kho->so_sp == 0) {
        in_canh_bao("Kho hang hien tai trong!\n");
        pause_screen();
        return;
    }

    in_tieu_de_cot_bang();

    long long tong_sl = 0;
    double tong_gt = 0;
    for (int i = 0; i < kho->so_sp; i++) {
        hien_thi_mot_sp(&kho->ds_sp[i], i + 1);
        tong_sl += kho->ds_sp[i].so_luong;
        tong_gt += (double)kho->ds_sp[i].so_luong * kho->ds_sp[i].gia_nhap;
    }
    in_duong_vien_bang();

    char tong_gt_str[32], tong_sl_str[32];
    format_tien(tong_gt, tong_gt_str);
    format_so(tong_sl, tong_sl_str);

    printf("  Tong so mat hang: %d | Tong ton kho: %s sp | Tong gia tri kho: %s\n",
           kho->so_sp, tong_sl_str, tong_gt_str);

    pause_screen();
}

void xem_chi_tiet_lo_hang(KhoHang *kho) {
    in_header("DANH SACH CHI TIET CAC LO HANG", NULL, kho);

    if (kho->so_lo == 0) {
        in_canh_bao("Chua co thong tin lo hang nao!\n");
        pause_screen();
        return;
    }

    set_mau(MAU_XANH_DUONG);
    printf("+--------+-------+--------------------+------+------------+------------+---------------+------------+\n");
    set_mau(MAU_VANG);
    printf("| Ma Lo  | Ma SP | Ten san pham       |  SL  |  Gia nhap  |  Ngay nhap |  Het han    | Trang thai    |\n");
    set_mau(MAU_XANH_DUONG);
    printf("+--------+-------+--------------------+------+------------+------------+---------------+------------+\n");
    reset_mau();

    for (int i = 0; i < kho->so_lo; i++) {
        LoHang *lo = &kho->ds_lo[i];
        int v_sp = tim_vi_tri_sp(kho, lo->ma_sp);
        const char *ten_sp = (v_sp >= 0) ? kho->ds_sp[v_sp].ten : "N/A";

        char gia_nhap_str[24];
        format_tien(lo->gia_nhap, gia_nhap_str);

        printf("| %-6s | %-5s | %-18.18s | %4d | %10s | %-10s | %-11s | ",
               lo->ma_lo, lo->ma_sp, ten_sp, lo->so_luong,
               gia_nhap_str, lo->ngay_nhap, lo->ngay_het_han);

        int con = ngay_con_lai(lo->ngay_het_han);
        if (lo->so_luong == 0) {
            set_mau(MAU_XAM);
            printf("Da xuat het   ");
        } else if (con <= 0) {
            set_mau(MAU_DO);
            printf("DA HET HAN    ");
        } else if (con <= NGAY_CANH_BAO) {
            set_mau(MAU_TIM);
            printf("Sap het han   ");
        } else {
            set_mau(MAU_XANH_LA);
            printf("Con hang      ");
        }
        reset_mau();
        printf("|\n");
    }

    set_mau(MAU_XANH_DUONG);
    printf("+--------+-------+--------------------+------+------------+------------+---------------+------------+\n");
    reset_mau();

    pause_screen();
}

void sua_san_pham(KhoHang *kho) {
    in_header("SUA THONG TIN SAN PHAM", NULL, kho);
    printf("  Nhap Ma san pham can sua (go '0' de huy): ");
    char ma[MAX_MA];
    doc_chuoi(ma, MAX_MA);
    if (strcmp(ma, "0") == 0 || strlen(ma) == 0) return;

    int vi_tri = tim_vi_tri_sp(kho, ma);
    if (vi_tri < 0) {
        in_loi("Khong tim thay ma san pham!\n");
        pause_screen();
        return;
    }

    SanPham *sp = &kho->ds_sp[vi_tri];
    printf("\n  Thong tin hien tai: %s - %s\n", sp->ma_sp, sp->ten);
    printf("  Danh muc: %s | Ton: %d | Gia nhap: %.0f | Gia ban: %.0f\n",
           sp->danh_muc, sp->so_luong, sp->gia_nhap, sp->gia_ban);
    printf("  Ngay het han: %s | Nguong canh bao: %d\n", sp->ngay_het_han, sp->nguong);
    set_mau(MAU_XAM);
    printf("------------------------------------------------------------\n");
    reset_mau();
    printf("  1. Ten san pham       2. Danh muc\n");
    printf("  3. Gia nhap           4. Gia ban\n");
    printf("  5. Nguong canh bao    6. Ngay het han\n");
    printf("  0. Huy bo\n");
    set_mau(MAU_XAM);
    printf("------------------------------------------------------------\n");
    reset_mau();
    printf("  Nhap lua chon: ");
    int chon = doc_so_nguyen();
    switch (chon) {
        case 1: printf("  Ten moi: "); doc_chuoi(sp->ten, MAX_TEN); break;
        case 2: printf("  Danh muc moi: "); doc_chuoi(sp->danh_muc, MAX_DANHMUC); break;
        case 3: printf("  Gia nhap moi: "); sp->gia_nhap = doc_so_thuc(); break;
        case 4: printf("  Gia ban moi: "); sp->gia_ban = doc_so_thuc(); break;
        case 5: printf("  Nguong moi: "); sp->nguong = doc_so_nguyen(); break;
        case 6:
            printf("  Ngay het han moi (DD/MM/YYYY): ");
            doc_chuoi(sp->ngay_het_han, MAX_NGAY);
            if (!ngay_hop_le(sp->ngay_het_han)) {
                in_loi("Ngay khong hop le!\n");
                pause_screen();
                return;
            }
            break;
        default: return;
    }
    cap_nhat_trang_thai(sp);
    in_thanh_cong("Cap nhat san pham thanh cong!\n");
    pause_screen();
}

void xoa_san_pham(KhoHang *kho) {
    in_header("XOA SAN PHAM", NULL, kho);
    printf("  Nhap Ma san pham can xoa (go '0' de huy): ");
    char ma[MAX_MA];
    doc_chuoi(ma, MAX_MA);
    if (strcmp(ma, "0") == 0 || strlen(ma) == 0) return;

    int vi_tri = tim_vi_tri_sp(kho, ma);
    if (vi_tri < 0) {
        in_loi("Khong tim thay san pham!\n");
        pause_screen();
        return;
    }

    SanPham *sp = &kho->ds_sp[vi_tri];
    if (sp->so_luong > 0) {
        in_canh_bao("Khong the xoa san pham vi ton kho con > 0 sp!\n");
        pause_screen();
        return;
    }

    printf("  Xac nhan xoa '%s - %s'? (Y/N): ", sp->ma_sp, sp->ten);
    char xn[5]; doc_chuoi(xn, sizeof(xn));
    if (xn[0] != 'Y' && xn[0] != 'y') return;

    for (int i = vi_tri; i < kho->so_sp - 1; i++) {
        kho->ds_sp[i] = kho->ds_sp[i + 1];
    }
    kho->so_sp--;
    in_thanh_cong("Xoa san pham thanh cong!\n");
    pause_screen();
}

void tim_kiem_sp(KhoHang *kho) {
    in_header("TIM KIEM SAN PHAM", NULL, kho);
    printf("  1. Theo Ma san pham\n");
    printf("  2. Theo Ten san pham\n");
    printf("  3. Theo Danh muc\n");
    printf("  0. Quay lai\n");
    set_mau(MAU_XAM);
    printf("------------------------------------------------------------\n");
    reset_mau();
    printf("  Nhap lua chon: ");
    int chon = doc_so_nguyen();
    if (chon <= 0 || chon > 3) return;

    char kw[MAX_TEN];
    printf("  Nhap tu khoa can tim: ");
    doc_chuoi(kw, MAX_TEN);

    in_header("KET QUA TIM KIEM", NULL, kho);
    in_tieu_de_cot_bang();
    int found = 0;
    for (int i = 0; i < kho->so_sp; i++) {
        SanPham *sp = &kho->ds_sp[i];
        int match = 0;
        if (chon == 1) match = (strstr(sp->ma_sp, kw) != NULL);
        else if (chon == 2) match = (strstr(sp->ten, kw) != NULL);
        else if (chon == 3) match = (strstr(sp->danh_muc, kw) != NULL);

        if (match) {
            found++;
            hien_thi_mot_sp(sp, found);
        }
    }
    in_duong_vien_bang();
    printf("  Tim thay %d san pham phu hop.\n", found);
    pause_screen();
}

static int cmp_ten(const void *a, const void *b) {
    return strcmp(((const SanPham *)a)->ten, ((const SanPham *)b)->ten);
}
static int cmp_so_luong(const void *a, const void *b) {
    return ((const SanPham *)b)->so_luong - ((const SanPham *)a)->so_luong;
}
static int cmp_gia(const void *a, const void *b) {
    double d = ((const SanPham *)b)->gia_nhap - ((const SanPham *)a)->gia_nhap;
    return (d > 0) - (d < 0);
}
static int cmp_het_han(const void *a, const void *b) {
    return so_sanh_ngay(((const SanPham *)a)->ngay_het_han, ((const SanPham *)b)->ngay_het_han);
}

void sap_xep_sp(KhoHang *kho) {
    in_header("SAP XEP DANH SACH SAN PHAM", NULL, kho);
    printf("  1. Theo Ten san pham (A -> Z)\n");
    printf("  2. Theo So luong ton kho (Giam dan)\n");
    printf("  3. Theo Gia nhap (Giam dan)\n");
    printf("  4. Theo Ngay het han (FEFO tang dan)\n");
    printf("  0. Quay lai\n");
    set_mau(MAU_XAM);
    printf("------------------------------------------------------------\n");
    reset_mau();
    printf("  Nhap lua chon: ");
    int chon = doc_so_nguyen();

    int (*fn)(const void *, const void *) = NULL;
    switch (chon) {
        case 1: fn = cmp_ten; break;
        case 2: fn = cmp_so_luong; break;
        case 3: fn = cmp_gia; break;
        case 4: fn = cmp_het_han; break;
        default: return;
    }

    qsort(kho->ds_sp, kho->so_sp, sizeof(SanPham), fn);
    in_thanh_cong("Sap xep thanh cong!\n\n");
    xem_danh_sach_sp(kho);
}

void dat_nguong_canh_bao(KhoHang *kho) {
    in_header("DAT NGUONG CANH BAO TON KHO", NULL, kho);
    printf("  Nhap Ma san pham (go '0' de huy): ");
    char ma[MAX_MA];
    doc_chuoi(ma, MAX_MA);
    if (strcmp(ma, "0") == 0 || strlen(ma) == 0) return;

    int vi_tri = tim_vi_tri_sp(kho, ma);
    if (vi_tri < 0) {
        in_loi("Khong tim thay san pham!\n");
        pause_screen();
        return;
    }

    SanPham *sp = &kho->ds_sp[vi_tri];
    printf("  San pham: %s - %s | Nguong hien tai: %d | Ton: %d\n",
           sp->ma_sp, sp->ten, sp->nguong, sp->so_luong);
    printf("  Nhap nguong ton toi thieu moi: ");
    sp->nguong = doc_so_nguyen();
    cap_nhat_trang_thai(sp);
    in_thanh_cong("Dat nguong thanh cong!\n");
    pause_screen();
}
