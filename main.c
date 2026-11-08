#include "types.h"
#include "utils.h"
#include "sanpham.h"
#include "nhanvien.h"
#include "giaodich.h"
#include "baocao.h"
#include "fileio.h"

static KhoHang* khoi_tao_kho(void) {
    KhoHang *kho = (KhoHang *)malloc(sizeof(KhoHang));
    kho->cap_sp = INIT_CAP;
    kho->ds_sp = (SanPham *)malloc(kho->cap_sp * sizeof(SanPham));
    kho->so_sp = 0;
    kho->cap_lo = INIT_CAP;
    kho->ds_lo = (LoHang *)malloc(kho->cap_lo * sizeof(LoHang));
    kho->so_lo = 0;
    kho->cap_nv = INIT_CAP;
    kho->ds_nv = (NhanVien *)malloc(kho->cap_nv * sizeof(NhanVien));
    kho->so_nv = 0;
    kho->cap_gd = INIT_CAP;
    kho->ds_gd = (GiaoDich *)malloc(kho->cap_gd * sizeof(GiaoDich));
    kho->so_gd = 0;
    kho->cap_cb = INIT_CAP;
    kho->ds_cb = (CanhBao *)malloc(kho->cap_cb * sizeof(CanhBao));
    kho->so_cb = 0;
    kho->nguoi_dung = NULL;
    return kho;
}

static void giai_phong_kho(KhoHang *kho) {
    free(kho->ds_sp);
    free(kho->ds_lo);
    free(kho->ds_nv);
    free(kho->ds_gd);
    free(kho->ds_cb);
    free(kho);
}

static void tao_du_lieu_mau(KhoHang *kho) {
    if (kho->so_sp > 0) return;

    SanPham mau[] = {
        {"SP001", "Gao ST25",       "Luong thuc", 500, 50, 18000, 25000, "01/11/2026", "01/04/2027", CON_HANG},
        {"SP002", "Nuoc mam Phu Q", "Gia vi",     200, 30, 35000, 52000, "15/10/2026", "15/04/2027", CON_HANG},
        {"SP003", "Dau an Tuong An","Gia vi",      80,  20, 42000, 58000, "20/10/2026", "20/03/2027", CON_HANG},
        {"SP004", "Mi Hao Hao",     "Thuc pham",  1000, 100,3500,  5000,  "05/11/2026", "05/05/2027", CON_HANG},
        {"SP005", "Sua TH True Milk","Do uong",    300, 40, 28000, 38000, "01/11/2026", "15/12/2026", CON_HANG},
        {"SP006", "Bia Tiger",      "Do uong",     150, 30, 15000, 22000, "10/10/2026", "10/10/2027", CON_HANG},
        {"SP007", "Bot giat OMO",   "Hoa chat",    100, 20, 85000,115000, "01/09/2026", "01/09/2028", CON_HANG},
        {"SP008", "Banh Chocopie",  "Banh keo",     15, 20, 55000, 75000, "20/10/2026", "20/01/2027", CON_HANG},
        {"SP009", "Ca phe G7",      "Do uong",     400, 50, 45000, 62000, "15/10/2026", "15/10/2027", CON_HANG},
        {"SP010", "Dau goi Clear",  "Hoa chat",     60, 15, 72000, 95000, "01/10/2026", "01/10/2028", CON_HANG},
        {"SP011", "Sua dac Ong Tho","Thuc pham",   250, 30, 22000, 32000, "05/11/2026", "05/11/2027", CON_HANG},
        {"SP012", "Nuoc ngot Pepsi","Do uong",      10, 25, 8000,  12000, "01/11/2026", "01/02/2027", CON_HANG},
    };
    int n = sizeof(mau) / sizeof(mau[0]);

    for (int i = 0; i < n; i++) {
        cap_nhat_trang_thai(&mau[i]);
        them_san_pham(kho, mau[i]);
    }

    LoHang lo_mau[] = {
        {"LO0001", "SP001", 200, 18000, "20/10/2026", "01/04/2027"},
        {"LO0002", "SP001", 300, 18000, "01/11/2026", "01/06/2027"},
        {"LO0003", "SP002", 200, 35000, "15/10/2026", "15/04/2027"},
        {"LO0004", "SP003",  80, 42000, "20/10/2026", "20/03/2027"},
        {"LO0005", "SP004",1000,  3500, "05/11/2026", "05/05/2027"},
        {"LO0006", "SP005", 100, 28000, "20/10/2026", "15/12/2026"},
        {"LO0007", "SP005", 200, 28000, "01/11/2026", "15/01/2027"},
        {"LO0008", "SP006", 150, 15000, "10/10/2026", "10/10/2027"},
        {"LO0009", "SP007", 100, 85000, "01/09/2026", "01/09/2028"},
        {"LO0010", "SP008",  15, 55000, "20/10/2026", "20/01/2027"},
        {"LO0011", "SP009", 400, 45000, "15/10/2026", "15/10/2027"},
        {"LO0012", "SP010",  60, 72000, "01/10/2026", "01/10/2028"},
        {"LO0013", "SP011", 250, 22000, "05/11/2026", "05/11/2027"},
        {"LO0014", "SP012",  10,  8000, "01/11/2026", "01/02/2027"},
    };
    int nlo = sizeof(lo_mau) / sizeof(lo_mau[0]);
    for (int i = 0; i < nlo; i++) {
        them_lo_hang(kho, lo_mau[i]);
    }

    for (int i = 0; i < kho->so_sp; i++) {
        cap_nhat_ton_va_date_tu_lo(kho, kho->ds_sp[i].ma_sp);
    }

    NhanVien nv_mau;
    memset(&nv_mau, 0, sizeof(NhanVien));
    strcpy(nv_mau.ma_nv, "NV002");
    strcpy(nv_mau.ten, "Nguyen Van A");
    strcpy(nv_mau.username, "nhanvien1");
    strcpy(nv_mau.password, "123456");
    nv_mau.vai_tro = ROLE_NHAN_VIEN;
    if (kho->so_nv >= kho->cap_nv) {
        kho->cap_nv *= 2;
        kho->ds_nv = (NhanVien *)realloc(kho->ds_nv, kho->cap_nv * sizeof(NhanVien));
    }
    kho->ds_nv[kho->so_nv++] = nv_mau;

    GiaoDich gd_mau[] = {
        {"GD0001","SP001","NV001",GD_NHAP, 500,18000,"01/11/2026"},
        {"GD0002","SP002","NV001",GD_NHAP, 200,35000,"15/10/2026"},
        {"GD0003","SP004","NV001",GD_NHAP,1000, 3500,"05/11/2026"},
        {"GD0004","SP005","NV001",GD_NHAP, 300,28000,"01/11/2026"},
        {"GD0005","SP001","NV002",GD_XUAT, 100,25000,"03/11/2026"},
        {"GD0006","SP004","NV002",GD_XUAT, 200, 5000,"04/11/2026"},
    };
    int ngd = sizeof(gd_mau) / sizeof(gd_mau[0]);
    for (int i = 0; i < ngd; i++) {
        if (kho->so_gd >= kho->cap_gd) {
            kho->cap_gd *= 2;
            kho->ds_gd = (GiaoDich *)realloc(kho->ds_gd, kho->cap_gd * sizeof(GiaoDich));
        }
        kho->ds_gd[kho->so_gd++] = gd_mau[i];
    }
}

static void menu_hang_hoa(KhoHang *kho) {
    int is_admin = (kho->nguoi_dung->vai_tro == ROLE_ADMIN);
    int chon;
    do {
        in_header("QUAN LY HANG HOA", NULL, kho);
        printf("  1. Xem danh sach hang hoa (Tong hop ton kho)\n");
        printf("  2. Xem chi tiet cac lo hang (Theo date tung lo)\n");
        printf("  3. Nhap hang theo lo\n");
        printf("  4. Xuat kho hang hoa (Tu dong theo FEFO)\n");
        if (is_admin) {
            printf("  5. Sua thong tin san pham\n");
            printf("  6. Xoa san pham\n");
        }
        printf("  0. Quay lai\n");
        set_mau(MAU_XAM);
        printf("------------------------------------------------------------\n");
        reset_mau();
        printf("  Nhap lua chon: ");
        chon = doc_so_nguyen();
        switch (chon) {
            case 1: xem_danh_sach_sp(kho); break;
            case 2: xem_chi_tiet_lo_hang(kho); break;
            case 3: nhap_hang_theo_lo(kho); break;
            case 4: xuat_hang(kho); break;
            case 5: if (is_admin) sua_san_pham(kho); else { in_loi("Chuc nang chi danh cho Admin!\n"); pause_screen(); } break;
            case 6: if (is_admin) xoa_san_pham(kho); else { in_loi("Chuc nang chi danh cho Admin!\n"); pause_screen(); } break;
        }
    } while (chon != 0);
}

static void menu_tra_cuu(KhoHang *kho) {
    int is_admin = (kho->nguoi_dung->vai_tro == ROLE_ADMIN);
    int chon;
    do {
        in_header("TRA CUU & SAP XEP", NULL, kho);
        printf("  1. Tim kiem san pham\n");
        printf("  2. Sap xep danh sach san pham\n");
        if (is_admin) {
            printf("  3. Dat nguong canh bao ton kho\n");
        }
        printf("  0. Quay lai\n");
        set_mau(MAU_XAM);
        printf("------------------------------------------------------------\n");
        reset_mau();
        printf("  Nhap lua chon: ");
        chon = doc_so_nguyen();
        switch (chon) {
            case 1: tim_kiem_sp(kho); break;
            case 2: sap_xep_sp(kho); break;
            case 3: if (is_admin) dat_nguong_canh_bao(kho); else { in_loi("Chuc nang chi danh cho Admin!\n"); pause_screen(); } break;
        }
    } while (chon != 0);
}

static void menu_bao_cao(KhoHang *kho) {
    int is_admin = (kho->nguoi_dung->vai_tro == ROLE_ADMIN);
    int chon;
    do {
        in_header("BAO CAO & TAI CHINH", NULL, kho);
        printf("  1. Canh bao ton kho (Het han / Ton thap)\n");
        if (is_admin) {
            printf("  2. Bao cao thong ke tong quan\n");
            printf("  3. Tong gia tri kho theo danh muc\n");
            printf("  4. Lich su giao dich nhap / xuat\n");
        }
        printf("  0. Quay lai\n");
        set_mau(MAU_XAM);
        printf("------------------------------------------------------------\n");
        reset_mau();
        printf("  Nhap lua chon: ");
        chon = doc_so_nguyen();
        switch (chon) {
            case 1: canh_bao_ton_kho(kho); break;
            case 2: if (is_admin) bao_cao_thong_ke(kho); else { in_loi("Chuc nang chi danh cho Admin!\n"); pause_screen(); } break;
            case 3: if (is_admin) tong_gia_tri_kho(kho); else { in_loi("Chuc nang chi danh cho Admin!\n"); pause_screen(); } break;
            case 4: if (is_admin) xem_lich_su_gd(kho); else { in_loi("Chuc nang chi danh cho Admin!\n"); pause_screen(); } break;
        }
    } while (chon != 0);
}

static void menu_he_thong(KhoHang *kho) {
    int chon;
    do {
        in_header("QUAN LY HE THONG", NULL, kho);
        printf("  1. Quan ly nhan vien\n");
        printf("  2. Luu du lieu ra file (.dat)\n");
        printf("  3. Doc du lieu tu file (.dat)\n");
        printf("  4. Xuat bao cao CSV (Excel)\n");
        printf("  0. Quay lai\n");
        set_mau(MAU_XAM);
        printf("------------------------------------------------------------\n");
        reset_mau();
        printf("  Nhap lua chon: ");
        chon = doc_so_nguyen();
        switch (chon) {
            case 1: menu_quan_ly_nv(kho); break;
            case 2: luu_du_lieu(kho); break;
            case 3: tai_du_lieu(kho); break;
            case 4: xuat_csv(kho); break;
        }
    } while (chon != 0);
}

int main(void) {
    init_console();
    KhoHang *kho = khoi_tao_kho();
    tai_du_lieu_auto(kho);
    tao_admin_mac_dinh(kho);
    tao_du_lieu_mau(kho);

    int thoat = 0;
    while (!thoat) {
        int res = dang_nhap(kho);
        if (res == -1) { thoat = 1; break; }
        if (res == 0) continue;

        cap_nhat_tat_ca_trang_thai(kho);
        int is_admin = (kho->nguoi_dung->vai_tro == ROLE_ADMIN);
        int chon;

        do {
            in_header(NULL, NULL, kho);
            printf("  1. Quan ly hang hoa\n");
            printf("  2. Tra cuu & sap xep\n");
            printf("  3. Bao cao & tai chinh\n");
            if (is_admin) {
                printf("  4. Nhan vien & he thong\n");
            }
            printf("  0. Dang xuat\n");
            printf("  9. Thoat chuong trinh\n");
            set_mau(MAU_XAM);
            printf("------------------------------------------------------------\n");
            reset_mau();

            printf("  Nhap lua chon: ");
            chon = doc_so_nguyen();

            switch (chon) {
                case 1: menu_hang_hoa(kho); break;
                case 2: menu_tra_cuu(kho); break;
                case 3: menu_bao_cao(kho); break;
                case 4:
                    if (is_admin) {
                        menu_he_thong(kho);
                    } else {
                        in_loi("Chuc nang chi danh cho Admin!\n");
                        pause_screen();
                    }
                    break;
                case 9:
                    printf("\n  Luu du lieu truoc khi thoat? (Y/N): ");
                    {
                        char xn[5]; doc_chuoi(xn, sizeof(xn));
                        if (xn[0] == 'Y' || xn[0] == 'y' || xn[0] == '\0') {
                            luu_du_lieu_auto(kho);
                            in_thanh_cong("Da luu du lieu an toan!\n");
                        }
                    }
                    thoat = 1;
                    chon = 0;
                    break;
                case 0:
                    kho->nguoi_dung = NULL;
                    break;
                default:
                    in_loi("Lua chon khong hop le!\n");
                    pause_screen();
                    break;
            }
        } while (chon != 0);
    }

    printf("\n");
    in_thanh_cong("Cam on ban da su dung chuong trinh!\n\n");
    giai_phong_kho(kho);
    return 0;
}
