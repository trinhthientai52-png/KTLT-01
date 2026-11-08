#include "nhanvien.h"
#include "utils.h"

void tao_admin_mac_dinh(KhoHang *kho) {
    if (kho->so_nv > 0) return;
    NhanVien admin;
    memset(&admin, 0, sizeof(NhanVien));
    strcpy(admin.ma_nv, "NV001");
    strcpy(admin.ten, "Quan Tri Vien");
    strcpy(admin.username, "admin");
    strcpy(admin.password, "admin123");
    admin.vai_tro = ROLE_ADMIN;
    if (kho->so_nv >= kho->cap_nv) {
        kho->cap_nv *= 2;
        kho->ds_nv = (NhanVien *)realloc(kho->ds_nv, kho->cap_nv * sizeof(NhanVien));
    }
    kho->ds_nv[kho->so_nv++] = admin;
}

void them_canh_bao(KhoHang *kho, const char *username, const char *noi_dung) {
    if (kho->so_cb >= kho->cap_cb) {
        kho->cap_cb *= 2;
        kho->ds_cb = (CanhBao *)realloc(kho->ds_cb, kho->cap_cb * sizeof(CanhBao));
    }
    CanhBao cb;
    memset(&cb, 0, sizeof(CanhBao));
    strncpy(cb.username, username, MAX_USERNAME - 1);
    time_t t = time(NULL);
    struct tm *tm_info = localtime(&t);
    snprintf(cb.thoi_gian, sizeof(cb.thoi_gian), "%02d/%02d/%04d %02d:%02d",
             tm_info->tm_mday, tm_info->tm_mon + 1, tm_info->tm_year + 1900,
             tm_info->tm_hour, tm_info->tm_min);
    strncpy(cb.noi_dung, noi_dung, sizeof(cb.noi_dung) - 1);
    kho->ds_cb[kho->so_cb++] = cb;
}

int dang_nhap(KhoHang *kho) {
    clear_screen();
    set_mau(MAU_VANG);
    printf("============================================================\n");
    printf("               HE THONG QUAN LY KHO HANG\n");
    printf("============================================================\n");
    printf("  DANG NHAP HE THONG (Nhap '0' de thoat)\n");
    set_mau(MAU_XAM);
    printf("------------------------------------------------------------\n");
    reset_mau();
    printf("\n");

    char username[MAX_USERNAME], password[MAX_PASSWORD];
    printf("  Ten dang nhap : ");
    doc_chuoi(username, MAX_USERNAME);
    if (strcmp(username, "0") == 0) return -1;

    printf("  Mat khau      : ");
    nhap_mat_khau(password, MAX_PASSWORD);

    for (int i = 0; i < kho->so_nv; i++) {
        NhanVien *nv = &kho->ds_nv[i];
        if (strcmp(nv->username, username) != 0) continue;

        if (nv->bi_khoa) {
            if (nv->vai_tro == ROLE_NHAN_VIEN) {
                printf("\n");
                in_loi("Tai khoan tam thoi bi khoa! Vui long lien he Admin.\n");
                pause_screen();
                return 0;
            }
            int thoi_gian_cho = THOI_GIAN_KHOA_BASE * (1 << (nv->lan_khoa - 1));
            double elapsed = difftime(time(NULL), nv->thoi_gian_khoa);
            if (elapsed < thoi_gian_cho) {
                printf("\n");
                in_loi("Tai khoan dang bi khoa, vui long thu lai sau!\n");
                pause_screen();
                return 0;
            }
            nv->bi_khoa = 0;
            nv->so_lan_sai = 0;
        }

        if (strcmp(nv->password, password) == 0) {
            nv->so_lan_sai = 0;
            kho->nguoi_dung = nv;
            return 1;
        }

        nv->so_lan_sai++;
        printf("\n");
        in_loi("Sai mat khau!\n");

        if (nv->so_lan_sai >= MAX_SAI) {
            nv->so_lan_sai = 0;
            if (nv->vai_tro == ROLE_NHAN_VIEN) {
                nv->bi_khoa = 1;
                char msg[200];
                sprintf(msg, "Nhan vien '%s' bi khoa sau %d lan sai", username, MAX_SAI);
                them_canh_bao(kho, username, msg);
            } else {
                nv->bi_khoa = 1;
                nv->lan_khoa++;
                nv->thoi_gian_khoa = time(NULL);
                int cho = THOI_GIAN_KHOA_BASE * (1 << (nv->lan_khoa - 1));
                char msg[200];
                sprintf(msg, "Admin '%s' tam khoa %ds", username, cho);
                them_canh_bao(kho, username, msg);
            }
            in_loi("Tai khoan da bi khoa do nhap sai nhieu lan!\n");
        }
        pause_screen();
        return 0;
    }

    printf("\n");
    in_loi("Tai khoan khong ton tai!\n");
    pause_screen();
    return 0;
}

static void xem_ds_nv(KhoHang *kho) {
    in_header("DANH SACH NHAN VIEN", NULL, kho);

    set_mau(MAU_XANH_DUONG);
    printf("+-----+--------+----------------------+---------------+-------------+--------------+\n");
    set_mau(MAU_VANG);
    printf("| STT | Ma NV  | Ho va ten            | Ten dang nhap |   Vai tro   |  Trang thai  |\n");
    set_mau(MAU_XANH_DUONG);
    printf("+-----+--------+----------------------+---------------+-------------+--------------+\n");
    reset_mau();

    for (int i = 0; i < kho->so_nv; i++) {
        NhanVien *nv = &kho->ds_nv[i];
        printf("| %3d | %-6s | %-20.20s | %-13.13s | %-11s | %-12s |\n",
               i + 1, nv->ma_nv, nv->ten, nv->username,
               (nv->vai_tro == ROLE_ADMIN) ? "Admin" : "Nhan vien",
               nv->bi_khoa ? "Bi khoa" : "Hoat dong");
    }

    set_mau(MAU_XANH_DUONG);
    printf("+-----+--------+----------------------+---------------+-------------+--------------+\n");
    reset_mau();

    pause_screen();
}

static void them_nhan_vien(KhoHang *kho) {
    in_header("THEM NHAN VIEN MOI", NULL, kho);

    NhanVien nv;
    memset(&nv, 0, sizeof(NhanVien));

    printf("  Ma nhan vien: ");
    doc_chuoi(nv.ma_nv, MAX_MA);
    for (int i = 0; i < kho->so_nv; i++) {
        if (strcmp(kho->ds_nv[i].ma_nv, nv.ma_nv) == 0) {
            in_loi("Ma da ton tai!\n");
            pause_screen();
            return;
        }
    }

    printf("  Ho va ten: ");
    doc_chuoi(nv.ten, MAX_TEN);

    printf("  Ten dang nhap: ");
    doc_chuoi(nv.username, MAX_USERNAME);
    for (int i = 0; i < kho->so_nv; i++) {
        if (strcmp(kho->ds_nv[i].username, nv.username) == 0) {
            in_loi("Username da ton tai!\n");
            pause_screen();
            return;
        }
    }

    printf("  Mat khau khoi tao: ");
    doc_chuoi(nv.password, MAX_PASSWORD);

    printf("  Vai tro [1] Admin  [2] Nhan vien: ");
    nv.vai_tro = (doc_so_nguyen() == 1) ? ROLE_ADMIN : ROLE_NHAN_VIEN;

    if (kho->so_nv >= kho->cap_nv) {
        kho->cap_nv *= 2;
        kho->ds_nv = (NhanVien *)realloc(kho->ds_nv, kho->cap_nv * sizeof(NhanVien));
    }
    kho->ds_nv[kho->so_nv++] = nv;

    in_thanh_cong("Them nhan vien thanh cong!\n");
    pause_screen();
}

static void xoa_nhan_vien(KhoHang *kho) {
    in_header("XOA NHAN VIEN", NULL, kho);

    printf("  Ma nhan vien can xoa (go '0' de huy): ");
    char ma[MAX_MA];
    doc_chuoi(ma, MAX_MA);
    if (strcmp(ma, "0") == 0 || strlen(ma) == 0) return;

    for (int i = 0; i < kho->so_nv; i++) {
        if (strcmp(kho->ds_nv[i].ma_nv, ma) != 0) continue;
        if (&kho->ds_nv[i] == kho->nguoi_dung) {
            in_loi("Khong the xoa tai khoan cua chinh minh!\n");
            pause_screen();
            return;
        }

        printf("  Xac nhan xoa '%s'? (Y/N): ", kho->ds_nv[i].ten);
        char xn[5];
        doc_chuoi(xn, sizeof(xn));
        if (xn[0] == 'Y' || xn[0] == 'y') {
            for (int j = i; j < kho->so_nv - 1; j++)
                kho->ds_nv[j] = kho->ds_nv[j + 1];
            kho->so_nv--;
            in_thanh_cong("Xoa nhan vien thanh cong!\n");
        }
        pause_screen();
        return;
    }

    in_loi("Khong tim thay nhan vien!\n");
    pause_screen();
}

static void mo_khoa(KhoHang *kho) {
    in_header("MO KHOA TAI KHOAN", NULL, kho);

    printf("  Ma nhan vien can mo khoa: ");
    char ma[MAX_MA];
    doc_chuoi(ma, MAX_MA);

    for (int i = 0; i < kho->so_nv; i++) {
        if (strcmp(kho->ds_nv[i].ma_nv, ma) == 0) {
            kho->ds_nv[i].bi_khoa = 0;
            kho->ds_nv[i].so_lan_sai = 0;
            kho->ds_nv[i].lan_khoa = 0;
            in_thanh_cong("Mo khoa tai khoan thanh cong!\n");
            pause_screen();
            return;
        }
    }

    in_loi("Khong tim thay nhan vien!\n");
    pause_screen();
}

static void xem_canh_bao(KhoHang *kho) {
    in_header("NHAT KY CANH BAO BAO MAT", NULL, kho);

    if (kho->so_cb == 0) {
        printf("  (Khong co canh bao)\n");
        pause_screen();
        return;
    }

    for (int i = 0; i < kho->so_cb; i++) {
        printf("  [%d] %s | %s | %s\n",
               i + 1, kho->ds_cb[i].thoi_gian, kho->ds_cb[i].username, kho->ds_cb[i].noi_dung);
    }

    printf("\n  Xoa tat ca canh bao? (Y/N): ");
    char xn[5];
    doc_chuoi(xn, sizeof(xn));
    if (xn[0] == 'Y' || xn[0] == 'y') {
        kho->so_cb = 0;
        in_thanh_cong("Da xoa nhat ky canh bao.\n");
    }
    pause_screen();
}

void menu_quan_ly_nv(KhoHang *kho) {
    int chon;
    do {
        in_header("QUAN LY NHAN VIEN", NULL, kho);
        printf("  1. Xem danh sach nhan vien\n");
        printf("  2. Them nhan vien moi\n");
        printf("  3. Mo khoa tai khoan\n");
        printf("  4. Xoa nhan vien\n");
        printf("  5. Xem nhat ky canh bao\n");
        printf("  0. Quay lai\n");
        set_mau(MAU_XAM);
        printf("------------------------------------------------------------\n");
        reset_mau();
        printf("  Nhap lua chon: ");
        chon = doc_so_nguyen();
        switch (chon) {
            case 1: xem_ds_nv(kho); break;
            case 2: them_nhan_vien(kho); break;
            case 3: mo_khoa(kho); break;
            case 4: xoa_nhan_vien(kho); break;
            case 5: xem_canh_bao(kho); break;
        }
    } while (chon != 0);
}
