#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

#define MAX_FILM 100
#define MAX_JUDUL 50
#define MAX_BOOKING 100
#define MAX_JAM 4

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
                clearScreen(); 
                printf("\n--- BOOKING TIKET (Belum diimplementasikan) ---\n"); 
                pauseScreen(); 
                break;
            case 4: 
                clearScreen(); 
                printf("\n--- Refund Tiket (Belum diimplementasikan) ---\n"); 
                pauseScreen(); 
                break;
            case 5: 
                clearScreen(); 
                printf("\n--- Tampikan Tiket (Belum diimplementasikan) ---\n"); 
                pauseScreen(); 
                break;
            case 6: 
                clearScreen(); 
                printf("\n--- PEMBAYARAN (Belum diimplementasikan) ---\n"); 
                pauseScreen(); 
                break;
            case 7: 
                clearScreen(); 
                printf("\n--- Rumah Pola (Belum diimplementasikan) ---\n"); 
                pauseScreen(); 
                break;
            case 8: 
                clearScreen(); 
                printf("\n--- Bendera Negara (Belum diimplementasikan) ---\n"); 
                pauseScreen(); 
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
    addFilm(103, "ONE PIECE", 0, 42000); 
    
    if (login()) {
        menuUtama();
    }
    
    return 0;
}
