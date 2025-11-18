#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_FILM 100
#define MAX_JUDUL 100
#define MAX_DESKRIPSI 200

typedef struct {
    char judul[MAX_JUDUL];
    char deskripsi[MAX_DESKRIPSI];
} Film;

// Struktur data untuk tiket
typedef struct {
    char judul_film[MAX_JUDUL];
    int jumlah_tiket;
} Tiket;

// Array untuk menyimpan data film dan tiket
Film daftar_film[MAX_FILM];
Tiket data_tiket[MAX_FILM];  // Asumsi jumlah tiket tidak melebihi jumlah film
int jumlah_film = 0;
int jumlah_tiket = 0;
int jumlah_kursi = 50; // Jumlah kursi awal

// Fungsi untuk menambah film
void tambah_film() {
    if (jumlah_film < MAX_FILM) {
        printf("Masukkan judul film: ");
        scanf(" %[^\n]s", daftar_film[jumlah_film].judul);
        printf("Masukkan deskripsi film: ");
        scanf(" %[^\n]s", daftar_film[jumlah_film].deskripsi);
        jumlah_film++;
        printf("Film berhasil ditambahkan.\n");
    } else {
        printf("Maaf, daftar film sudah penuh.\n");
    }
}

// Fungsi untuk melihat daftar film
void lihat_daftar_film() {
    if (jumlah_film == 0) {
        printf("Tidak ada film yang tersedia saat ini.\n");
        return;
    }
    printf("Daftar Film:\n");
    for (int i = 0; i < jumlah_film; i++) {
        printf("%d. %s: %s\n", i + 1, daftar_film[i].judul, daftar_film[i].deskripsi);
    }
}

// Fungsi untuk mengedit film
void edit_film() {
    int pilihan;
    lihat_daftar_film();
    if (jumlah_film == 0) return;

    printf("Masukkan nomor film yang ingin diedit: ");
    scanf("%d", &pilihan);

    if (pilihan > 0 && pilihan <= jumlah_film) {
        printf("Masukkan deskripsi baru: ");
        scanf(" %[^\n]s", daftar_film[pilihan - 1].deskripsi);
        printf("Deskripsi film berhasil diperbarui.\n");
    } else {
        printf("Nomor film tidak valid.\n");
    }
}

// Fungsi untuk menghapus film
void hapus_film() {
    int pilihan;
    lihat_daftar_film();
    if (jumlah_film == 0) return;

    printf("Masukkan nomor film yang ingin dihapus: ");
    scanf("%d", &pilihan);

    if (pilihan > 0 && pilihan <= jumlah_film) {
        // Menggeser elemen array untuk menimpa film yang dihapus
        for (int i = pilihan - 1; i < jumlah_film - 1; i++) {
            daftar_film[i] = daftar_film[i + 1];
        }
        jumlah_film--;
        printf("Film berhasil dihapus.\n");
    } else {
        printf("Nomor film tidak valid.\n");
    }
}

// Fungsi untuk booking tiket
void booking_tiket() {
    char judul_film[MAX_JUDUL];
    int jumlah;
    char metode_pembayaran[20];

    printf("Masukkan judul film yang ingin dibooking: ");
    scanf(" %[^\n]s", judul_film);

    // Cek apakah film tersedia
    int film_tersedia = 0;
    for (int i = 0; i < jumlah_film; i++) {
        if (strcmp(daftar_film[i].judul, judul_film) == 0) {
            film_tersedia = 1;
            break;
        }
    }

    if (!film_tersedia) {
        printf("Film tidak tersedia.\n");
        return;
    }

    printf("Masukkan jumlah tiket: ");
    scanf("%d", &jumlah);

    if (jumlah > jumlah_kursi) {
        printf("Jumlah kursi tidak mencukupi.\n");
        return;
    }

    printf("Masukkan metode pembayaran (QRIS/CASH): ");
    scanf("%s", metode_pembayaran);

    // Simpan data booking
    strcpy(data_tiket[jumlah_tiket].judul_film, judul_film);
    data_tiket[jumlah_tiket].jumlah_tiket = jumlah;
    jumlah_tiket++;

    jumlah_kursi -= jumlah; // Kurangi jumlah kursi yang tersedia

    printf("Booking berhasil!\n");
    printf("Metode pembayaran: %s\n", metode_pembayaran);
}

// Fungsi untuk refund tiket
void refund_tiket() {
    char judul_film[MAX_JUDUL];
    int jumlah;

    printf("Masukkan judul film yang tiketnya akan di-refund: ");
    scanf(" %[^\n]s", judul_film);

    printf("Masukkan jumlah tiket yang akan di-refund: ");
    scanf("%d", &jumlah);

    // Cari tiket yang sesuai
    int tiket_ditemukan = 0;
    for (int i = 0; i < jumlah_tiket; i++) {
        if (strcmp(data_tiket[i].judul_film, judul_film) == 0) {
            if (data_tiket[i].jumlah_tiket >= jumlah) {
                data_tiket[i].jumlah_tiket -= jumlah;
                jumlah_kursi += jumlah; // Tambahkan kembali jumlah kursi

                printf("Refund berhasil!\n");
                tiket_ditemukan = 1;
                break;
            } else {
                printf("Jumlah tiket yang di-refund melebihi jumlah yang dibooking.\n");
                return;
            }
        }
    }

    if (!tiket_ditemukan) {
        printf("Tidak ada tiket yang bisa di-refund untuk film ini.\n");
    }
}

// Fungsi untuk melihat tiket
void lihat_tiket() {
    char judul_film[MAX_JUDUL];

    printf("Masukkan judul film yang tiketnya akan dilihat: ");
    scanf(" %[^\n]s", judul_film);

    int tiket_ditemukan = 0;
    for (int i = 0; i < jumlah_tiket; i++) {
        if (strcmp(data_tiket[i].judul_film, judul_film) == 0) {
            printf("Detail Tiket Film '%s':\n", judul_film);
            printf("- Jumlah tiket: %d\n", data_tiket[i].jumlah_tiket);
            tiket_ditemukan = 1;
            break;
        }
    }

    if (!tiket_ditemukan) {
        printf("Tidak ada tiket yang dibooking untuk film ini.\n");
    }
}

// Fungsi untuk menampilkan film yang sedang tayang (sementara)
void lihat_film_tayang() {
    printf("Film yang sedang tayang:\n");
    if (jumlah_film == 0) {
        printf("Tidak ada film yang tersedia saat ini.\n");
    } else {
        printf("Saat ini semua film dalam daftar sedang tayang.\n");
    }
}

int main() {
    int pilihan;

    while (1) {
    	printf("\n=========================================\n");
        printf("||             Menu Bioskop            ||\n");
        printf("=========================================\n");
        printf("||1. Tambah Film                       ||\n");
        printf("||2. Lihat Daftar Film                 ||\n");
        printf("||3. Edit Film                         ||\n");
        printf("||4. Hapus Film                        ||\n");
        printf("||5. Booking Tiket                     ||\n");
        printf("||6. Refund Tiket                      ||\n");
        printf("||7. Lihat Tiket                       ||\n");
        printf("||8. Lihat Film Tayang                 ||\n");
        printf("||0. Exit                              ||\n");
        printf("=========================================\n");

        printf("Masukkan pilihan: ");
        scanf("%d", &pilihan);

        switch (pilihan) {
            case 1:
                tambah_film();
                break;
            case 2:
                lihat_daftar_film();
                break;
            case 3:
                edit_film();
                break;
            case 4:
                hapus_film();
                break;
            case 5:
                booking_tiket();
                break;
            case 6:
                refund_tiket();
                break;
            case 7:
                lihat_tiket();
                break;
            case 8:
                lihat_film_tayang();
                break;
            case 0:
                printf("Terima kasih!\n");
                exit(0);
            default:
                printf("Pilihan tidak valid.\n");
        }
    }

    return 0;
}