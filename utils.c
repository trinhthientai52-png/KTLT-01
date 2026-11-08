#include "utils.h"

#ifdef _WIN32
#include <windows.h>
#include <conio.h>
#include <io.h>
#endif

void set_mau(int mau) {
#ifdef _WIN32
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (WORD)mau);
#else
    (void)mau;
#endif
}

void reset_mau(void) {
    set_mau(MAU_MAC_DINH);
}

void init_console(void) {
#ifdef _WIN32
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
#endif
}

void clear_screen(void) {
#ifdef _WIN32
    if (_isatty(_fileno(stdout))) {
        system("cls");
    }
#else
    if (isatty(fileno(stdout))) {
        system("clear");
    }
#endif
}

void pause_screen(void) {
#ifdef _WIN32
    if (!_isatty(_fileno(stdin))) return;
#endif
    set_mau(MAU_XAM);
    printf("\n  [+] Nhan Enter de tiep tuc...");
    reset_mau();
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void doc_chuoi(char *buf, int max_len) {
    if (buf && max_len > 0) buf[0] = '\0';
    if (fgets(buf, max_len, stdin)) {
        buf[strcspn(buf, "\r\n")] = '\0';
    } else {
        if (feof(stdin) && buf && max_len > 1) {
            strcpy(buf, "0");
        }
    }
}

int doc_so_nguyen(void) {
    char buf[32];
    doc_chuoi(buf, sizeof(buf));
    return atoi(buf);
}

double doc_so_thuc(void) {
    char buf[32];
    doc_chuoi(buf, sizeof(buf));
    return atof(buf);
}

void nhap_mat_khau(char *buf, int max_len) {
#ifdef _WIN32
    if (!_isatty(_fileno(stdin))) {
        doc_chuoi(buf, max_len);
        return;
    }
    int i = 0;
    int c;
    while (i < max_len - 1) {
        c = _getch();
        if (c == '\r' || c == '\n') break;
        if (c == '\b' || c == 127) {
            if (i > 0) { 
                i--; 
                printf("\b \b"); 
            }
        } else if (c >= 32 && c <= 126) {
            buf[i++] = (char)c;
            set_mau(MAU_VANG);
            printf("*");
            reset_mau();
        }
    }
    buf[i] = '\0';
    printf("\n");
#else
    doc_chuoi(buf, max_len);
#endif
}

int ngay_hop_le(const char *ngay) {
    int d, m, y;
    if (sscanf(ngay, "%d/%d/%d", &d, &m, &y) != 3) return 0;
    if (y < 2000 || y > 2100 || m < 1 || m > 12) return 0;
    int days[] = {0,31,28,31,30,31,30,31,31,30,31,30,31};
    if ((y % 4 == 0 && y % 100 != 0) || y % 400 == 0) days[2] = 29;
    return (d >= 1 && d <= days[m]);
}

static int ngay_sang_so(const char *ngay) {
    int d, m, y;
    sscanf(ngay, "%d/%d/%d", &d, &m, &y);
    return y * 10000 + m * 100 + d;
}

int so_sanh_ngay(const char *ngay1, const char *ngay2) {
    int n1 = ngay_sang_so(ngay1), n2 = ngay_sang_so(ngay2);
    return (n1 > n2) - (n1 < n2);
}

void ngay_hom_nay(char *buf) {
    time_t t = time(NULL);
    struct tm *tm_info = localtime(&t);
    sprintf(buf, "%02d/%02d/%04d",
            tm_info->tm_mday, tm_info->tm_mon + 1, tm_info->tm_year + 1900);
}

int ngay_con_lai(const char *ngay_het_han) {
    int d, m, y;
    if (sscanf(ngay_het_han, "%d/%d/%d", &d, &m, &y) != 3) return 9999;
    struct tm het_han;
    memset(&het_han, 0, sizeof(struct tm));
    het_han.tm_mday = d;
    het_han.tm_mon = m - 1;
    het_han.tm_year = y - 1900;
    time_t t_het = mktime(&het_han);
    time_t t_now = time(NULL);
    return (int)(difftime(t_het, t_now) / 86400);
}

void in_duong_ke(int len) {
    for (int i = 0; i < len; i++) printf("-");
    printf("\n");
}

void in_duong_ngang(int len, char ch) {
    for (int i = 0; i < len; i++) putchar(ch);
    putchar('\n');
}

const char* trang_thai_str(TrangThai tt) {
    switch (tt) {
        case CON_HANG:     return "Con hang";
        case SAP_HET_HANG: return "Sap het hang";
        case SAP_HET_HAN:  return "Sap het han";
        case DA_HET_HAN:   return "DA HET HAN";
        default:           return "N/A";
    }
}

void format_tien(double tien, char *buf) {
    long long n = (long long)tien;
    int am = (n < 0);
    if (am) n = -n;

    char raw[64];
    sprintf(raw, "%lld", n);
    int len = (int)strlen(raw);

    char num_part[80];
    int idx = 0;
    for (int i = 0; i < len; i++) {
        num_part[idx++] = raw[i];
        if ((len - 1 - i) % 3 == 0 && i != len - 1) {
            num_part[idx++] = ',';
        }
    }
    num_part[idx] = '\0';

    if (am) sprintf(buf, "-%s d", num_part);
    else sprintf(buf, "%s d", num_part);
}

void format_so(long long so, char *buf) {
    int am = (so < 0);
    if (am) so = -so;

    char raw[32];
    sprintf(raw, "%lld", so);
    int len = (int)strlen(raw);

    char num_part[40];
    int idx = 0;
    for (int i = 0; i < len; i++) {
        num_part[idx++] = raw[i];
        if ((len - 1 - i) % 3 == 0 && i != len - 1) {
            num_part[idx++] = ',';
        }
    }
    num_part[idx] = '\0';

    if (am) sprintf(buf, "-%s", num_part);
    else sprintf(buf, "%s", num_part);
}

int dem_canh_bao_ton(const KhoHang *kho) {
    if (!kho) return 0;
    int cnt = 0;
    for (int i = 0; i < kho->so_sp; i++) {
        if (kho->ds_sp[i].trang_thai != CON_HANG) cnt++;
    }
    return cnt;
}

void in_header(const char *tieu_de, const char *bread_crumb, const KhoHang *kho) {
    (void)bread_crumb;
    clear_screen();
    set_mau(MAU_VANG);
    printf("============================================================\n");
    if (tieu_de && strlen(tieu_de) > 0) {
        printf("               %s\n", tieu_de);
    } else {
        printf("               HE THONG QUAN LY KHO HANG\n");
    }
    printf("============================================================\n");
    reset_mau();

    if (kho && kho->nguoi_dung) {
        int is_admin = (kho->nguoi_dung->vai_tro == ROLE_ADMIN);
        printf("  Xin chao: ");
        set_mau(MAU_TRANG_DAM);
        printf("%s", kho->nguoi_dung->ten);
        reset_mau();
        printf(" (%s)\n", is_admin ? "Admin" : "Nhan vien");
        set_mau(MAU_XAM);
        printf("------------------------------------------------------------\n");
        reset_mau();
    }
}

void in_tieu_de(const char *text) {
    set_mau(MAU_VANG);
    printf("%s", text);
    reset_mau();
}

void in_thanh_cong(const char *text) {
    set_mau(MAU_XANH_LA);
    printf("  [+] %s", text);
    reset_mau();
}

void in_loi(const char *text) {
    set_mau(MAU_DO);
    printf("  [!] LOI: %s", text);
    reset_mau();
}

void in_canh_bao(const char *text) {
    set_mau(MAU_VANG);
    printf("  [*] CANH BAO: %s", text);
    reset_mau();
}

void in_thong_tin(const char *text) {
    set_mau(MAU_XANH_DUONG);
    printf("  [i] %s", text);
    reset_mau();
}

void in_badge_trang_thai(TrangThai tt) {
    switch (tt) {
        case CON_HANG:
            set_mau(MAU_XANH_LA);
            printf("[ Con hang  ]");
            break;
        case SAP_HET_HANG:
            set_mau(MAU_VANG);
            printf("[SAP HET HANG]");
            break;
        case SAP_HET_HAN:
            set_mau(MAU_TIM);
            printf("[Sap het han ]");
            break;
        case DA_HET_HAN:
            set_mau(MAU_DO);
            printf("[ DA HET HAN ]");
            break;
        default:
            printf("[    N/A    ]");
            break;
    }
    reset_mau();
}
