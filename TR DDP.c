#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <windows.h>

#define MAX_FILM 100
#define MAX_JUDUL 50
#define MAX_BOOKING 100
#define MAX_JAM 4
#define ANSI_COLOR_RED      "\x1b[41m"
#define ANSI_COLOR_WHITE    "\x1b[47m" 
#define ANSI_COLOR_RESET    "\x1b[0m"  
#define PINK  "\x1b[95m"
#define GRAY  "\x1b[90m"
#define GREEN "\x1b[32m"
#define BROWN "\x1b[33m"
#define RESET "\x1b[0m"

int global_tinggi_pola = 5;    
int global_lebar_pola = 40;

char *jam_tayang[MAX_JAM] = {"10.00", "13.00", "16.00", "19.00"};

struct Film {
    int id_film;
    char judul[MAX_JUDUL];
    int kursi_tersedia;
    float harga_tiket;
};

struct Tiket {
    int id_booking;
    int id_film;
    char judul_film[MAX_JUDUL];
    char jam_tayang[6]; 
    int jumlah_kursi;
    float total_harga;
};

struct Film daftar_film[MAX_FILM];
int jumlah_film = 0;

struct Tiket daftar_tiket[MAX_BOOKING];
int jumlah_tiket = 0;
int next_booking_id = 1000;

void clearScreen() { system("cls"); }
void pauseScreen() { system("pause"); }

int searchIndex(int id) {
    for (int i = 0; i < jumlah_film; i++) {
        if (daftar_film[i].id_film == id) return i;
    }
    return -1;
}

int searchTiketIndexById(int id) {
    for (int i = 0; i < jumlah_tiket; i++) {
        if (daftar_tiket[i].id_booking == id) return i;
    }
    return -1;
}

void addFilm(int id, char judul[], int kursi, float harga) {
    if (jumlah_film >= MAX_FILM) return;
    daftar_film[jumlah_film].id_film = id;
    strcpy(daftar_film[jumlah_film].judul, judul);
    daftar_film[jumlah_film].kursi_tersedia = kursi;
    daftar_film[jumlah_film].harga_tiket = harga;
    jumlah_film++;
}

void showLoading() {
    system("cls");
    printf("Loading");
    for (int i = 0; i < 5; i++) {
        printf(".");
        Sleep(300);
    }
    Sleep(200);
    system("cls");
}

void showAllFilm(int is_admin_mode) {
	clearScreen();
    if (jumlah_film == 0) {
        printf("\nBelum ada film yang terdaftar.\n");
        pauseScreen();
        return;
    }

    printf("\n--- DAFTAR FILM YANG SEDANG TAYANG ---\n");
    printf("====================================================================================================\n");
    printf("|     |                                 |         JAM TAYANG            |        |                 |\n");
    printf("| ID  | JUDUL FILM                      |===============================| KURSI  |    HARGA RP     |\n");
    printf("|     |                                 | 10.00 | 13.00 | 16.00 | 19.00 |        |                 |\n");
    printf("====================================================================================================\n");

    for (int i = 0; i < jumlah_film; i++) {
        if (is_admin_mode || daftar_film[i].kursi_tersedia > 0) {
            printf("| %-3d |%-30s   | %-5s | %-5s | %-5s | %-5s | %-6d |  %-13.0f  |\n",
                    daftar_film[i].id_film,
                    daftar_film[i].judul,
                    jam_tayang[0], jam_tayang[1], jam_tayang[2], jam_tayang[3], 
                    daftar_film[i].kursi_tersedia,
                    daftar_film[i].harga_tiket);
        } else if (!is_admin_mode && daftar_film[i].kursi_tersedia == 0) {
            printf("| %-3d |%-30s   | %-5s | %-5s | %-5s | %-5s | %-6s |  %-13.0f  |\n",
                    daftar_film[i].id_film,
                    daftar_film[i].judul,
                    jam_tayang[0], jam_tayang[1], jam_tayang[2], jam_tayang[3], 
                    "HABIS",
                    daftar_film[i].harga_tiket);
        }
    }
    printf("====================================================================================================\n");
    pauseScreen();
}

void tambah() {
    int id, kursi;
    char judul[MAX_JUDUL];
    float harga;

    clearScreen();
    printf("\n--- TAMBAH FILM BARU ---\n");

    do {
        printf("Masukkan ID Film (unik): ");
        if (scanf("%d", &id) != 1 || id <= 0) {
            printf("Input tidak valid. Masukkan ID Film (angka positif): ");
            while(getchar() != '\n');
            continue;
        }
        if (searchIndex(id) != -1) {
            printf("ID Film sudah ada. Silakan masukkan ID lain.\n");
        }
    } while (searchIndex(id) != -1);

    printf("Masukkan Judul Film: ");
    getchar();
    fgets(judul, sizeof(judul), stdin);
    judul[strcspn(judul, "\n")] = '\0';

    printf("Masukkan Jumlah Kursi Awal: ");
    while(scanf("%d", &kursi) != 1 || kursi < 0) {
        printf("Input tidak valid. Masukkan Jumlah Kursi Awal: ");
        while(getchar() != '\n');
    }

    printf("Masukkan Harga Tiket (Rp): ");
    while(scanf("%f", &harga) != 1 || harga <= 0) {
        printf("Input tidak valid. Masukkan Harga Tiket: ");
        while(getchar() != '\n');
    }

    addFilm(id, judul, kursi, harga);
    printf("\nFilm \"%s\" berhasil ditambahkan!!\n", judul);
    pauseScreen();
}

void edit() {
    int id_edit;
    clearScreen();
    printf("\n--- EDIT FILM ---\n");
    showAllFilm(1);

    if (jumlah_film == 0) return;

    printf("\nMasukkan ID Film yang ingin diubah: ");
    if (scanf("%d", &id_edit) != 1) {
        printf("Input ID tidak valid.\n");
        while(getchar() != '\n');
        pauseScreen();
        return;
    }

    int index = searchIndex(id_edit);
    if (index < 0) {
        printf("\nFilm dengan ID %d tidak ditemukan.", id_edit);
        pauseScreen();
        return;
    }

    struct Film *film_update = &daftar_film[index];
    printf("\nMengedit Film: %s (ID: %d)\n", film_update->judul, film_update->id_film);

    printf("Masukkan Judul baru (Saat ini: %s, kosongkan untuk lewati): ", film_update->judul);
    getchar();
    char judul_baru[MAX_JUDUL];
    if (fgets(judul_baru, sizeof(judul_baru), stdin) != NULL) {
        judul_baru[strcspn(judul_baru, "\n")] = '\0';
        if (strlen(judul_baru) > 0) {
            strcpy(film_update->judul, judul_baru);
        }
    }

    int kursi_baru;
    printf("Masukkan Kursi Tersedia baru (Saat ini: %d, -1 untuk lewati): ", film_update->kursi_tersedia);
    if (scanf("%d", &kursi_baru) == 1) {
        if (kursi_baru >= 0) film_update->kursi_tersedia = kursi_baru;
    } else { while(getchar() != '\n'); }

    float harga_baru;
    printf("Masukkan Harga Tiket baru (Saat ini: %.0f, 0 untuk lewati): ", film_update->harga_tiket);
    if (scanf("%f", &harga_baru) == 1) {
        if (harga_baru > 0) film_update->harga_tiket = harga_baru;
    } else { while(getchar() != '\n'); }

    printf("\nData Film ID %d berhasil diubah.", film_update->id_film);
    pauseScreen();
}

void hapus() {
    int id_hapus;
    clearScreen();
    printf("\n--- HAPUS FILM ---\n");
    showAllFilm(1);

    if (jumlah_film == 0) return;

    printf("\nMasukkan ID Film yang ingin dihapus: ");
    if (scanf("%d", &id_hapus) != 1) {
        printf("Input ID tidak valid.\n");
        while(getchar() != '\n');
        pauseScreen();
        return;
    }
    while(getchar() != '\n');

    int index = searchIndex(id_hapus);
    if (index < 0) {
        printf("\nFilm dengan ID %d tidak ditemukan.", id_hapus);
        pauseScreen();
        return;
    }

    for (int i = index; i < jumlah_film - 1; i++) {
        daftar_film[i] = daftar_film[i+1];
    }
    jumlah_film--;
    printf("\nFilm ID %d berhasil dihapus.", id_hapus);
    pauseScreen();
}

void menuFilm() {
    int pilihan;
    do {
        clearScreen();
        printf("=====================================\n");
        printf("=         MENU CRUD FILM            =\n");
        printf("=====================================\n");
        printf("= 1. Tambah Film                    =\n");
        printf("= 2. Lihat Semua Film               =\n");
        printf("= 3. Edit Film                      =\n");
        printf("= 4. Hapus Film                     =\n");
        printf("= 5. Kembali ke Menu Utama          =\n");
        printf("=====================================\n");
        printf("Pilihan anda : ");
        if (scanf("%d", &pilihan) != 1) {
            pilihan = -1;
            while(getchar() != '\n');
        }

        switch (pilihan) {
            case 1: tambah(); break;
            case 2: showAllFilm(1); break;
            case 3: edit(); break;
            case 4: hapus(); break;
            case 5: break;
            default: printf("\nPilihan tidak valid.\n"); pauseScreen();
        }
    } while (pilihan != 5);
}

void bookingTiket() {
    showLoading();

    if (jumlah_film == 0) {
        printf("Belum ada film yang tersedia untuk dibooking.\n");
        pauseScreen();
        return;
    }

    int id_film, jumlah, pilihan_jam;
    showAllFilm(0);

    printf("\nMasukkan ID Film yang ingin dipesan: ");
    if (scanf("%d", &id_film) != 1) {
        printf("Input tidak valid.\n");
        while(getchar() != '\n');
        pauseScreen();
        return;
    }

    int index = searchIndex(id_film);
    if (index < 0) {
        printf("Film tidak ditemukan.\n");
        pauseScreen();
        return;
    }

    printf("Jam Tayang:\n");
    for (int i = 0; i < MAX_JAM; i++) {
        printf("%d. %s\n", i + 1, jam_tayang[i]);
    }
    printf("Pilih Jam (1-%d): ", MAX_JAM);
    
    if (scanf("%d", &pilihan_jam) != 1 || pilihan_jam < 1 || pilihan_jam > MAX_JAM) {
        printf("Pilihan Jam Tayang tidak valid.\n");
        while(getchar() != '\n');
        pauseScreen();
        return;
    }

    printf("Masukkan jumlah kursi yang ingin dibooking: ");
    if (scanf("%d", &jumlah) != 1 || jumlah <= 0 || jumlah > daftar_film[index].kursi_tersedia) {
        printf("Jumlah kursi tidak valid atau melebihi ketersediaan.\n");
        while(getchar() != '\n');
        pauseScreen();
        return;
    }

    daftar_tiket[jumlah_tiket].id_booking = next_booking_id++;
    daftar_tiket[jumlah_tiket].id_film = daftar_film[index].id_film;
    strcpy(daftar_tiket[jumlah_tiket].judul_film, daftar_film[index].judul);
    strcpy(daftar_tiket[jumlah_tiket].jam_tayang, jam_tayang[pilihan_jam - 1]); 
    daftar_tiket[jumlah_tiket].jumlah_kursi = jumlah;
    daftar_tiket[jumlah_tiket].total_harga = daftar_film[index].harga_tiket * jumlah;
    jumlah_tiket++;
    daftar_film[index].kursi_tersedia -= jumlah;

    printf("\nTiket berhasil dibooking!\n");
    printf("Detail Booking:\n");
    printf("  ID Booking: %d\n", daftar_tiket[jumlah_tiket-1].id_booking);
    printf("  Film: %s\n", daftar_tiket[jumlah_tiket-1].judul_film);
    printf("  Jam Tayang: %s\n", daftar_tiket[jumlah_tiket-1].jam_tayang);
    printf("  Jumlah Kursi: %d\n", daftar_tiket[jumlah_tiket-1].jumlah_kursi);
    printf("  Total Harga: Rp %.0f\n", daftar_tiket[jumlah_tiket-1].total_harga);
    pauseScreen();
}

void tampilkanTiket() {
    showLoading();

    if (jumlah_tiket == 0) {
        printf("Belum ada tiket yang dibooking.\n");
        pauseScreen();
        return;
    }

    printf("\n--- DAFTAR TIKET YANG DIBOOKING ---\n");
    printf("==========================================================================================\n");
    printf("| ID Booking | ID Film | Judul Film         | Jam Tayang | Jumlah Kursi | Total Harga(Rp)|\n");
    printf("==========================================================================================\n");
    for (int i = 0; i < jumlah_tiket; i++) {
        printf("| %-10d | %-7d | %-18s | %-10s | %-12d | %-13.0f  |\n",
                daftar_tiket[i].id_booking,
                daftar_tiket[i].id_film,
                daftar_tiket[i].judul_film,
                daftar_tiket[i].jam_tayang, 
                daftar_tiket[i].jumlah_kursi,
                daftar_tiket[i].total_harga);
    }
    printf("==========================================================================================\n");
    pauseScreen();
}

void refundTiket() {
    int id_booking_refund, index_tiket, index_film;

    clearScreen();
    printf("\n--- REFUND TIKET ---\n");

    if (jumlah_tiket == 0) {
        printf("\nBelum ada tiket yang dibooking.\n");
        pauseScreen(); 
        return;
    }

    tampilkanTiket(); 
    printf("\nMasukkan ID Booking yang ingin di-refund: ");
    if (scanf("%d", &id_booking_refund) != 1) {
        printf("Input ID tidak valid.\n");
        while(getchar() != '\n');
        pauseScreen(); 
        return;
    }

    index_tiket = searchTiketIndexById(id_booking_refund);
    if (index_tiket < 0) {
        printf("\nTiket dengan ID Booking %d tidak ditemukan.", id_booking_refund);
        pauseScreen(); 
        return;
    }

    struct Tiket *tiket_refund = &daftar_tiket[index_tiket];
    index_film = searchIndex(tiket_refund->id_film);

    if (index_film != -1) {
        daftar_film[index_film].kursi_tersedia += tiket_refund->jumlah_kursi;
        printf("%d kursi berhasil dikembalikan ke Film \"%s\".\n",
                tiket_refund->jumlah_kursi, daftar_film[index_film].judul);
    } else {
        printf("Film asli tidak ditemukan, kursi tidak dikembalikan.\n");
    }

    printf("ID Booking: %d\n", tiket_refund->id_booking);
    printf("Total Refund: Rp %.0f\n", tiket_refund->total_harga);
    printf("Jam Tayang Tiket: %s\n", tiket_refund->jam_tayang);

    for (int i = index_tiket; i < jumlah_tiket - 1; i++) {
        daftar_tiket[i] = daftar_tiket[i+1];
    }
    jumlah_tiket--;

    printf("\nREFUND BERHASIL! Tiket telah dihapus.\n");
}

void tampilkanStrukPembayaran(int jumlah, float harga, float total, char metode[]) {
    clearScreen();
    printf("====================================================================\n");
    printf("=                     STRUK PEMBAYARAN BIOSKOP                     =\n");
    printf("====================================================================\n");
    printf("  Jumlah Tiket      : %d unit, Harga Rp %.0f per unit = Rp %.0f\n", jumlah, harga, total);
    printf("  Total Pembayaran  : Rp %.0f\n", total);
    printf("  Metode Bayar      : %s\n", metode);
    printf("====================================================================\n\n");
}

void pembayaranTiket() {
    if (jumlah_tiket == 0) {
        printf("\nBelum ada tiket yang dibooking.\n");
        pauseScreen();
        return;
    }

    tampilkanTiket(); 

    int id_booking;
    printf("\nMasukkan ID Booking yang ingin dibayar: ");
    if (scanf("%d", &id_booking) != 1) {
        printf("Input ID tidak valid.\n");
        while(getchar() != '\n');
        pauseScreen();
        return;
    }

    int index = searchTiketIndexById(id_booking);
    if (index < 0) {
        printf("\nTiket dengan ID Booking %d tidak ditemukan.\n", id_booking);
        pauseScreen();
        return;
    }

    struct Tiket *tiket = &daftar_tiket[index];
    int metode;
    float totalHarga = tiket->total_harga;

    printf("\nTotal yang harus dibayar untuk tiket ini: Rp %.0f\n", totalHarga);
    printf("Pilih metode pembayaran:\n");
    printf("1. QRIS\n");
    printf("2. Cash/Tunai\n");
    printf("3. Batalkan Pembayaran\n");
    printf("Masukkan pilihan (1-3): ");
    if (scanf("%d", &metode) != 1) {
        while(getchar() != '\n');
        printf("Pilihan tidak valid.\n");
        pauseScreen();
        return;
    }

    switch (metode) {
        case 1: {
            tampilkanStrukPembayaran(tiket->jumlah_kursi, tiket->total_harga / tiket->jumlah_kursi, totalHarga, "QRIS");
            int uang;
            printf("Masukkan nominal yang dibayar: Rp ");
            scanf("%d", &uang);
            if (uang == totalHarga) {
                clearScreen();
                tampilkanStrukPembayaran(tiket->jumlah_kursi, tiket->total_harga / tiket->jumlah_kursi, totalHarga, "QRIS");
                printf("Jumlah Dibayar    : Rp %d\n", uang);
                printf("\nPEMBAYARAN BERHASIL! TERIMA KASIH!\n\n");
            } else if (uang > totalHarga) {
                printf("\nNominal tidak boleh lebih! Pembayaran dibatalkan.\n\n");
            } else {
                printf("\nNominal kurang! Pembayaran dibatalkan.\n\n");
            }
            break;
        }
        case 2: {
            tampilkanStrukPembayaran(tiket->jumlah_kursi, tiket->total_harga / tiket->jumlah_kursi, totalHarga, "Cash/Tunai");
            int uang, kembalian;
            printf("Masukkan jumlah uang yang dibayar: Rp ");
            scanf("%d", &uang);
            if (uang >= totalHarga) {
                kembalian = uang - totalHarga;
                clearScreen();
                tampilkanStrukPembayaran(tiket->jumlah_kursi, tiket->total_harga / tiket->jumlah_kursi, totalHarga, "Cash/Tunai");
                printf("Jumlah Bayar      : Rp %d\n", uang);
                printf("Kembalian         : Rp %d\n", kembalian);
                printf("\nPEMBAYARAN BERHASIL! TERIMA KASIH!\n\n");
            } else {
                printf("\nUang tidak cukup! Pembayaran gagal.\n\n");
            }
            break;
        }
        case 3:
            printf("\nPembayaran dibatalkan.\n\n");
            break;
        default:
            printf("\nPilihan tidak valid!\n\n");
            break;
    }

    pauseScreen();
}

void printTypewriterEffect(const char *text, int delay) {
    while (*text) {
        putchar(*text++);   
        fflush(stdout);     
        usleep(delay); 
   }
}
void rumahPola() {
	clearScreen();
	int tinggi, bahan;
    int lebarCoklat, lebarPutih;

    printf("=====================================================\n");
    printf("=            KITA USAHAKAN RUMAH ITU...             =\n");
    printf("=====================================================\n");
    printf("Masukkan tinggi rumah: \n");
    scanf("%d", &tinggi);

    printf("Masukkan bahan: \n");
    scanf("%d", &bahan);
    
	printf("\nKita usahakan rumah itu..\n", 50000);
	printf("Dari depan akan tampak sederhana..\n", 50000);
	printf("Tapi penerangannya.. diracik begitu romantis..\n", 50000);
	
    // ===== CEROBONG =====
    for (int i = 0; i < tinggi; i++) {
        // *** BAGIAN YANG DIUBAH ***
        for (int sp = 0; sp < tinggi + bahan * 2 - 1; sp++)
            printf(" ");
        // *************************
        printf(GREEN);
        for (int j = 0; j < bahan; j++)
            printf("*");
        printf(RESET "\n");
    }

    // Hitung lebar badan rumah berdasarkan baris terakhir atap
    lebarCoklat = 2 * tinggi - 1; // panjang segitiga pink di baris terakhir atap
    lebarPutih  = bahan * 3;      // panjang persegi panjang abu-abu di baris terakhir atap

    // ===== ATAP =====
    for (int i = 1; i <= tinggi; i++) {
        for (int sp = 0; sp < tinggi - i; sp++)
            printf(" ");
        // Segitiga kiri
        printf(PINK);
        for (int j = 0; j < (2 * i - 1); j++)
            printf("*");
        printf(RESET);
        // Persegi panjang samping
        printf(GRAY);
        for (int j = 0; j < bahan * 3; j++)
            printf("*");
        printf(RESET "\n");
    }

    // ===== BADAN RUMAH =====
    for (int i = 0; i < tinggi; i++) {
        // Bagian coklat
        printf(BROWN);
        for (int j = 0; j < lebarCoklat; j++)
            printf("*");
        printf(RESET);

        // Bagian putih
        printf(GREEN);
        for (int j = 0; j < lebarPutih; j++)
            printf("*");
        printf(RESET "\n");
    }

    printf("<0>    0\n" );
    printf("-|- 0 -|-\n");
    printf(" | -|- | \n");
    printf(" A  A  A   ");
    
    pauseScreen();
};

void tampilkanBendera() {
    int input_tinggi = global_tinggi_pola;
    int input_lebar = global_lebar_pola;
    char pilihan;
    int i, j;

    clearScreen();
    printf("===========================================\n");
    printf("= PENGATURAN DIMENSI BENDERA (Input 1x)   =\n");
    printf("===========================================\n");

    do {
        printf("1. Masukkan Tinggi Satuan Warna dan Lebar Bendera (Tinggi min 3, Lebar min 15, cth: 5 40): ");
        if (scanf("%d %d", &input_tinggi, &input_lebar) != 2) {
            while (getchar() != '\n');
            printf("? Input tidak valid! Masukkan 2 angka dipisahkan spasi.\n");
            continue;
        }
        if (input_tinggi < 3 || input_lebar < 15) {
            printf("? Tinggi minimal 3 dan Lebar minimal 15.\n");
            continue;
        }
        break;
    } while (1);

    do {
        clearScreen();
        printf("===========================================\n");
        printf("= BENDERA ANDA SUDAH JADI                 =\n");
        printf("= Tinggi: %d, Lebar: %d                     =\n", input_tinggi, input_lebar);
        printf("===========================================\n");

        // Bagian Merah
        printf(ANSI_COLOR_RED);
        for (i = 0; i < input_tinggi; i++) {
            for (j = 0; j < input_lebar; j++) {
                printf(" ");
            }
            printf("\n");
        }
        printf(ANSI_COLOR_RESET);

        // Bagian Putih
        printf(ANSI_COLOR_WHITE);
        for (i = 0; i < input_tinggi; i++) {
            for (j = 0; j < input_lebar; j++) {
                printf(" ");
            }
            printf("\n");
        }
        printf(ANSI_COLOR_RESET);

        printf("\n===========================================\n");
        printf("Tekan U untuk gambar ulang (dimensi sama)...\n"); 
        printf("Tekan X untuk kembali\nPilihan: ");

        while(scanf(" %c", &pilihan) != 1) {
             while(getchar() != '\n');
             printf("Input tidak valid. Ulangi (U/X): ");
        }

        if (pilihan == 'x' || pilihan == 'X') break;

    } while (pilihan == 'u' || pilihan == 'U');
}

void tampilAuthor() {
	clearScreen();
    printf("============================================\n");
    printf("=                  AUTHOR                  =\n");
    printf("============================================\n");
    printf("= Author:                                  =\n");
    printf("= ONINDA HAGACHA HAUTEAS(672025210)        =\n");
    printf("= DIVA SYAHRANI PADADI(672025187)          =\n");
    printf("= PUTRI EUKARISTI HATALAIBESSY(672025184)  =\n");
    printf("= ALVIN NAJIB AL FATH(672025203)           =\n");
    printf("============================================\n");
    printf("= Kelas    : ASD DDP(D)                    =\n");
    printf("= Semester : 1 (AKU MABA KAMU KATING)      =\n");
    printf("= Angkatan : 2025                          =\n");
    printf("============================================\n");
    getchar(); getchar();
}

int login() {
    int pilihan;
    while(1) {
        clearScreen();
        printf("=====================================\n");
        printf("=               LOGIN               =\n");
        printf("=====================================\n");
        printf("= 1. Login Admin                    =\n");
        printf("= 2. Keluar Program                 =\n");
        printf("=====================================\n");
        printf("Pilih menu: ");

        if (scanf("%d", &pilihan) != 1) {
            pilihan = -1;
            while(getchar() != '\n');
        }

        if (pilihan == 1) {
            clearScreen();
            char username[50], password[50];
            printf("===================================\n");
            printf("=           MENU LOGIN            =\n");
            printf("===================================\n");
            printf("Username : "); scanf("%s", username);
            printf("Password : "); scanf("%s", password);

            if(strcmp(username, "admin") == 0 && strcmp(password, "admin123") == 0) {
                showLoading();
                return 1;
            } else {
                printf("\nLogin salah!\nTekan Enter untuk mencoba ulang...");
                getchar(); getchar();
            }
        }
        else if (pilihan == 2) return 0;
        else { printf("\nPilihan tidak valid!\nTekan Enter untuk mencoba lagi..."); getchar(); getchar(); }
    }
}

void menuUtama() {
    int pilihan;
    do {
        clearScreen();
        printf("=====================================================\n");
        printf("=    MENU UTAMA BIOSKOP - Grand Aether              =\n");
        printf("=====================================================\n");
        printf("= 1.  CRUD FILM (Admin)                             =\n");
        printf("= 2.  Film yang Sedang Tayang                       =\n");
        printf("= 3.  Booking Tiket                                 =\n");
        printf("= 4.  Refund Tiket                                  =\n");
        printf("= 5.  Tampilkan Tiket (Booking)                     =\n");
        printf("= 6.  Metode Pembayaran                             =\n");
        printf("=====================================================\n");
        printf("= 7.  Rumah Pola                                    =\n");
        printf("= 8.  Bendera                                       =\n");
        printf("= 9.  Author Program                                =\n");
        printf("= 10. Reset Pilihan                                 =\n");
        printf("= 0.  Keluar Program                                =\n");
        printf("=====================================================\n");
        printf("Pilih menu: ");
        
        if (scanf("%d", &pilihan) != 1) {
            pilihan = -1; 
            while(getchar() != '\n');
        }
        
        switch (pilihan) {
            case 1: 
                menuFilm(); 
                break;
            case 2:
				showLoading(); 
                showAllFilm(0); 
                break;
            case 3: 
                bookingTiket();
                break;
            case 4: 
                refundTiket(); 
                break;
            case 5: 
                tampilkanTiket();
                break;
            case 6: 
                pembayaranTiket(); 
                break;
            case 7: 
                rumahPola();
                break;
            case 8: 
                tampilkanBendera();
                break;
            case 9: 
            	tampilAuthor();    
                break;
            case 10: 
                break; 
            case 0: 
                printf("\nTerima kasih! Sampai jumpa.\n"); 
                break;
            default: 
                printf("\nPilihan tidak valid.\n"); 
                pauseScreen();
        }
    } while (pilihan != 0);
}

int main() {
    addFilm(101, "The Killer", 50, 45000); 
    addFilm(102, "Komang", 75, 40000);
    addFilm(103, "Venom", 0, 42000); 
    
    if (login()) {
        menuUtama();
    }
    
    return 0;
}