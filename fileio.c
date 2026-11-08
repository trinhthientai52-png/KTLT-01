#include "fileio.h"
#include "utils.h"
#include "sanpham.h"

static void load_file(const char *path, void **data, int *count, int *cap, size_t elem_size) {
    FILE *f = fopen(path, "rb");
    if (!f) return;
    int n;
    if (fread(&n, sizeof(int), 1, f) != 1) { fclose(f); return; }
    while (n > *cap) *cap *= 2;
    *data = realloc(*data, (size_t)(*cap) * elem_size);
    if ((int)fread(*data, elem_size, n, f) == n)
        *count = n;
    fclose(f);
}

static void save_file(const char *path, const void *data, int count, size_t elem_size) {
    FILE *f = fopen(path, "wb");
    if (!f) return;
    fwrite(&count, sizeof(int), 1, f);
    fwrite(data, elem_size, count, f);
    fclose(f);
}

void tai_du_lieu_auto(KhoHang *kho) {
    load_file(FILE_SP, (void **)&kho->ds_sp, &kho->so_sp, &kho->cap_sp, sizeof(SanPham));
    load_file(FILE_LO, (void **)&kho->ds_lo, &kho->so_lo, &kho->cap_lo, sizeof(LoHang));
    load_file(FILE_NV, (void **)&kho->ds_nv, &kho->so_nv, &kho->cap_nv, sizeof(NhanVien));
    load_file(FILE_GD, (void **)&kho->ds_gd, &kho->so_gd, &kho->cap_gd, sizeof(GiaoDich));
    load_file(FILE_CB, (void **)&kho->ds_cb, &kho->so_cb, &kho->cap_cb, sizeof(CanhBao));
}

void tai_du_lieu(KhoHang *kho) {
    in_header("DOC DU LIEU TU FILE", NULL, kho);
    tai_du_lieu_auto(kho);
    in_thanh_cong("Doc du lieu thanh cong!\n");
    printf("  San pham: %d | Lo hang: %d | Nhan vien: %d | Giao dich: %d\n",
           kho->so_sp, kho->so_lo, kho->so_nv, kho->so_gd);
    pause_screen();
}

void luu_du_lieu_auto(KhoHang *kho) {
    save_file(FILE_SP, kho->ds_sp, kho->so_sp, sizeof(SanPham));
    save_file(FILE_LO, kho->ds_lo, kho->so_lo, sizeof(LoHang));
    save_file(FILE_NV, kho->ds_nv, kho->so_nv, sizeof(NhanVien));
    save_file(FILE_GD, kho->ds_gd, kho->so_gd, sizeof(GiaoDich));
    save_file(FILE_CB, kho->ds_cb, kho->so_cb, sizeof(CanhBao));
}

void luu_du_lieu(KhoHang *kho) {
    in_header("LUU DU LIEU RA FILE", NULL, kho);
    luu_du_lieu_auto(kho);
    in_thanh_cong("Luu du lieu ra file (.dat) thanh cong!\n");
    pause_screen();
}

void xuat_csv(KhoHang *kho) {
    cap_nhat_tat_ca_trang_thai(kho);
    in_header("XUAT BAO CAO CSV (EXCEL)", NULL, kho);

    char fname[MAX_TEN];
    printf("  Nhap ten file (Enter dat mac dinh 'baocao.csv'): ");
    doc_chuoi(fname, MAX_TEN);
    if (strlen(fname) == 0) strcpy(fname, "baocao.csv");

    FILE *f = fopen(fname, "w");
    if (!f) {
        in_loi("Khong the tao file!\n");
        pause_screen();
        return;
    }

    fprintf(f, "STT,Ma SP,Ten san pham,Danh muc,So luong,Nguong ton,Gia nhap,Gia ban,Tong gia tri,Ngay nhap,Ngay het han,Trang thai\n");
    for (int i = 0; i < kho->so_sp; i++) {
        SanPham *sp = &kho->ds_sp[i];
        fprintf(f, "%d,%s,\"%s\",\"%s\",%d,%d,%.0f,%.0f,%.0f,%s,%s,%s\n",
                i + 1, sp->ma_sp, sp->ten, sp->danh_muc, sp->so_luong, sp->nguong,
                sp->gia_nhap, sp->gia_ban, (double)sp->so_luong * sp->gia_nhap,
                sp->ngay_nhap, sp->ngay_het_han, trang_thai_str(sp->trang_thai));
    }
    fclose(f);

    in_thanh_cong("Xuat file CSV thanh cong: ");
    printf("%s\n", fname);
    pause_screen();
}
