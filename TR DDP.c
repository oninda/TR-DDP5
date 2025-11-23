#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h> 

#define MAX_FILM 100
#define MAX_JUDUL 50

struct Film {
    int id_film;
    char judul[MAX_JUDUL];
    int durasi_menit;
    int kursi_tersedia; 
    float harga_tiket;
};

struct Tiket {
    int id_booking;
    int id_film;
    char judul_film[MAX_JUDUL];
    int jumlah_kursi;
    float total_harga;
};

struct Film daftar_film[MAX_FILM];
int jumlah_film = 0;

#define MAX_BOOKING 100
struct Tiket daftar_tiket[MAX_BOOKING];
int jumlah_tiket = 0;
int next_booking_id = 1000; 

void clearScreen() {
    system("cls"); 
}

void pauseScreen() {
    system("pause");
}

void addFilm(int id, char judul[], int durasi, int kursi, float harga) {
    if (jumlah_film >= MAX_FILM) return;
    
    daftar_film[jumlah_film].id_film = id;
    strcpy(daftar_film[jumlah_film].judul, judul);
    daftar_film[jumlah_film].durasi_menit = durasi;
    daftar_film[jumlah_film].kursi_tersedia = kursi;
    daftar_film[jumlah_film].harga_tiket = harga;
    jumlah_film++;
}

int searchFilmIndexById(int id) {
    for (int i = 0; i < jumlah_film; i++) {
        if (daftar_film[i].id_film == id) {
            return i;
        }
    }
    return -1;
}

int searchTiketIndexById(int id) {
    for (int i = 0; i < jumlah_tiket; i++) {
        if (daftar_tiket[i].id_booking == id) {
            return i;
        }
    }
    return -1;
}

void showAllFilm(int is_admin_mode) {
    if (jumlah_film == 0) {
        printf("\nℹ️ Belum ada film yang terdaftar.\n");
        pauseScreen();
        return;
    }
    
    printf("\n--- DAFTAR FILM YANG SEDANG TAYANG ---\n");
    printf("+----+--------------------------------------------------+----------+-------+----------+\n");
    printf("| ID | JUDUL FILM                                       | DURASI   | KURSI | HARGA RP |\n");
    printf("+----+--------------------------------------------------+----------+-------+----------+\n");

    for (int i = 0; i < jumlah_film; i++) {
        if (is_admin_mode || daftar_film[i].kursi_tersedia > 0) {
             printf("| %-2d | %-48s | %-4d Min | %-5d | %-8.0f |\n", 
                    daftar_film[i].id_film, 
                    daftar_film[i].judul,
                    daftar_film[i].durasi_menit,
                    daftar_film[i].kursi_tersedia,
                    daftar_film[i].harga_tiket);
        } else if (!is_admin_mode && daftar_film[i].kursi_tersedia == 0) {
             printf("| %-2d | %-48s | %-4d Min | HABIS | %-8.0f |\n", 
                    daftar_film[i].id_film, 
                    daftar_film[i].judul,
                    daftar_film[i].durasi_menit,
                    daftar_film[i].kursi_tersedia, // Tampilkan 0 kursi
                    daftar_film[i].harga_tiket);
        }
    }
    printf("+----+--------------------------------------------------+----------+-------+----------+\n");
    if (is_admin_mode) {
        pauseScreen();
    }
}

void tambahFilm() {
    int id, durasi, kursi;
    char judul[MAX_JUDUL];
    float harga;

    printf("\n--- TAMBAH FILM BARU ---\n");

    do {
        printf("Masukkan ID Film (unik): ");
        if (scanf("%d", &id) != 1 || id <= 0) {
            printf("⚠️ Input tidak valid. Masukkan ID Film (angka positif): ");
            while(getchar() != '\n');
            continue;
        }
        if (searchFilmIndexById(id) != -1) {
            printf("⚠️ ID Film sudah ada. Silakan masukkan ID lain.\n");
        }
    } while (searchFilmIndexById(id) != -1);

    printf("Masukkan Judul Film: ");
    getchar(); 
    fgets(judul, sizeof(judul), stdin);
    judul[strcspn(judul, "\n")] = '\0'; 

    printf("Masukkan Durasi (menit): ");
    while(scanf("%d", &durasi) != 1 || durasi <= 0) {
        printf("⚠️ Input tidak valid. Masukkan Durasi (menit): ");
        while(getchar() != '\n');
    }
    
    printf("Masukkan Jumlah Kursi Awal: ");
    while(scanf("%d", &kursi) != 1 || kursi < 0) {
        printf("⚠️ Input tidak valid. Masukkan Jumlah Kursi Awal: ");
        while(getchar() != '\n');
    }
    
    printf("Masukkan Harga Tiket (Rp): ");
    while(scanf("%f", &harga) != 1 || harga <= 0) {
        printf("⚠️ Input tidak valid. Masukkan Harga Tiket: ");
        while(getchar() != '\n');
    }

    addFilm(id, judul, durasi, kursi, harga);
    printf("\n✅ Film \"%s\" berhasil ditambahkan!!\n", judul);
    pauseScreen();
}

void editFilm() {
    int id_edit;
    printf("\n--- EDIT FILM ---\n");
    showAllFilm(1); 
    
    if (jumlah_film == 0) return; 

    printf("\nMasukkan ID Film yang ingin diubah: ");
    if (scanf("%d", &id_edit) != 1) {
        printf("⚠️ Input ID tidak valid.\n");
        while(getchar() != '\n');
        pauseScreen();
        return;
    }
    
    int index = searchFilmIndexById(id_edit);
    if (index < 0) {
        printf("\n❌ Film dengan ID %d tidak ditemukan.", id_edit);
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
        if (kursi_baru >= 0) {
            film_update->kursi_tersedia = kursi_baru;
        } else if (kursi_baru != -1) {
            printf("⚠️ Input jumlah kursi tidak valid, menggunakan nilai lama.\n");
        }
    } else {
        while(getchar() != '\n'); 
    }
    
    float harga_baru;
    printf("Masukkan Harga Tiket baru (Saat ini: %.0f, 0 untuk lewati): ", film_update->harga_tiket);
    if (scanf("%f", &harga_baru) == 1) {
        if (harga_baru > 0) {
            film_update->harga_tiket = harga_baru;
        } else if (harga_baru != 0) {
            printf("⚠️ Input harga tiket tidak valid, menggunakan nilai lama.\n");
        }
    } else {
        while(getchar() != '\n'); 
    }

    printf("\n✅ Data Film ID %d berhasil diubah.", film_update->id_film);
    pauseScreen();
}

void hapusFilm() {
    int id_hapus;
    printf("\n--- HAPUS FILM ---\n");
    showAllFilm(1);
    
    if (jumlah_film == 0) return;

    printf("\nMasukkan ID Film yang ingin dihapus: ");
    if (scanf("%d", &id_hapus) != 1) {
        printf("⚠️ Input ID tidak valid.\n");
        while(getchar() != '\n');
        pauseScreen();
        return;
    }
    while(getchar() != '\n'); 

    int index = searchFilmIndexById(id_hapus);
    if (index < 0) {
        printf("\n❌ Film dengan ID %d tidak ditemukan.", id_hapus);
        pauseScreen();
        return;
    }

    for (int i = index; i < jumlah_film - 1; i++) {
        daftar_film[i] = daftar_film[i+1];
    }
    jumlah_film--;
    printf("\n✅ Film ID %d berhasil dihapus.", id_hapus);
    pauseScreen();
}

void menuFilm() {
    int pilihan;
    do {
        clearScreen();
        printf("\n--- MENU CRUD FILM (ADMIN) ---\n");
        printf("1. Tambah Film\n");
        printf("2. Lihat Semua Film\n");
        printf("3. Edit Film\n");
        printf("4. Hapus Film\n");
        printf("5. Kembali ke Menu Utama\n");
        printf("Pilihan anda : ");
        if (scanf("%d", &pilihan) != 1) {
            pilihan = -1; 
            while(getchar() != '\n');
        }
        
        switch (pilihan) {
            case 1: tambahFilm(); break;
            case 2: clearScreen(); showAllFilm(1); break; 
            case 3: editFilm(); break;
            case 4: hapusFilm(); break;
            case 5: printf("\nKembali ke Menu Utama...\n"); break;
            default: printf("\n⚠️ Pilihan tidak valid.\n"); pauseScreen();
        }
    } while (pilihan != 5);
}

void bookingTiket() {
    int id_pilih, jumlah_kursi_book, index_film;
    
    clearScreen();
    printf("\n--- BOOKING TIKET ---\n");
    
    showAllFilm(0); 
    
    if (jumlah_film == 0) return; 

    printf("\nMasukkan ID Film yang ingin dibooking: ");
    if (scanf("%d", &id_pilih) != 1) {
        printf("⚠️ Input ID tidak valid.\n");
        while(getchar() != '\n');
        pauseScreen();
        return;
    }
    
    index_film = searchFilmIndexById(id_pilih);
    if (index_film < 0) {
        printf("\n❌ Film dengan ID %d tidak ditemukan.", id_pilih);
        pauseScreen();
        return;
    }

    struct Film *film_book = &daftar_film[index_film];
    if (film_book->kursi_tersedia == 0) {
        printf("\n❌ Maaf, kursi untuk film \"%s\" sudah habis.", film_book->judul);
        pauseScreen();
        return;
    }
    
    printf("Film terpilih: %s\n", film_book->judul);
    printf("Kursi tersedia: %d\n", film_book->kursi_tersedia);
    
    do {
        printf("Masukkan jumlah kursi yang ingin dibooking: ");
        if (scanf("%d", &jumlah_kursi_book) != 1 || jumlah_kursi_book <= 0) {
            printf("⚠️ Input tidak valid. Jumlah kursi harus angka positif.\n");
            while(getchar() != '\n');
        } else if (jumlah_kursi_book > film_book->kursi_tersedia) {
            printf("⚠️ Jumlah kursi melebihi ketersediaan (%d).\n", film_book->kursi_tersedia);
        } else {
            break; 
        }
    } while(1);

    film_book->kursi_tersedia -= jumlah_kursi_book; 
    if (jumlah_tiket >= MAX_BOOKING) {
        printf("\n❌ Kapasitas Booking penuh (MAX_BOOKING).\n");
        // Kembalikan kursi karena booking gagal disimpan
        film_book->kursi_tersedia += jumlah_kursi_book; 
        pauseScreen();
        return;
    }

    struct Tiket *tiket_baru = &daftar_tiket[jumlah_tiket];
    tiket_baru->id_booking = next_booking_id++;
    tiket_baru->id_film = id_pilih;
    strcpy(tiket_baru->judul_film, film_book->judul);
    tiket_baru->jumlah_kursi = jumlah_kursi_book;
    tiket_baru->total_harga = jumlah_kursi_book * film_book->harga_tiket;
    jumlah_tiket++;
    
    printf("\n✅ BOOKING BERHASIL!\n");
    printf("   ID Booking: %d\n", tiket_baru->id_booking);
    printf("   Film: %s\n", tiket_baru->judul_film);
    printf("   Jumlah Kursi: %d\n", tiket_baru->jumlah_kursi);
    printf("   Total Bayar: Rp %.0f\n", tiket_baru->total_harga);
    pauseScreen();
}

void refundTiket() {
    int id_booking_refund, index_tiket, index_film;
    
    clearScreen();
    printf("\n--- REFUND TIKET ---\n");

    if (jumlah_tiket == 0) {
        printf("\nℹ️ Belum ada tiket yang dibooking.\n");
        pauseScreen();
        return;
    }
    
    tampilkanTiket(); 
    
    printf("\nMasukkan ID Booking yang ingin di-refund: ");
    if (scanf("%d", &id_booking_refund) != 1) {
        printf("⚠️ Input ID tidak valid.\n");
        while(getchar() != '\n');
        pauseScreen();
        return;
    }
    
    index_tiket = searchTiketIndexById(id_booking_refund);
    if (index_tiket < 0) {
        printf("\n❌ Tiket dengan ID Booking %d tidak ditemukan.", id_booking_refund);
        pauseScreen();
        return;
    }
    
    struct Tiket *tiket_refund = &daftar_tiket[index_tiket];
    index_film = searchFilmIndexById(tiket_refund->id_film);
    
    if (index_film != -1) {
        daftar_film[index_film].kursi_tersedia += tiket_refund->jumlah_kursi;
        printf("✅ %d kursi berhasil dikembalikan ke Film \"%s\".\n", 
               tiket_refund->jumlah_kursi, daftar_film[index_film].judul);
    } else {
        printf("⚠️ Film asli tidak ditemukan, kursi tidak dikembalikan.\n");
    }
    
    printf("   ID Booking: %d\n", tiket_refund->id_booking);
    printf("   Total Refund: Rp %.0f\n", tiket_refund->total_harga);
    
    for (int i = index_tiket; i < jumlah_tiket - 1; i++) {
        daftar_tiket[i] = daftar_tiket[i+1];
    }
    jumlah_tiket--;
    
    printf("\n✅ REFUND BERHASIL! Tiket telah dihapus.\n");
    pauseScreen();
}


void tampilkanTiket() {
    clearScreen();
    printf("\n--- DAFTAR TIKET YANG DIBOOKING ---\n");

    if (jumlah_tiket == 0) {
        printf("\nℹ️ Belum ada tiket yang dibooking.\n");
        pauseScreen();
        return;
    }
    
    printf("+-------+----+--------------------------------------------------+-------+----------+\n");
    printf("| ID BK | ID | JUDUL FILM                                       | KURSI | TOTAL RP |\n");
    printf("+-------+----+--------------------------------------------------+-------+----------+\n");

    for (int i = 0; i < jumlah_tiket; i++) {
        printf("| %-5d | %-2d | %-48s | %-5d | %-8.0f |\n", 
               daftar_tiket[i].id_booking, 
               daftar_tiket[i].id_film,
               daftar_tiket[i].judul_film,
               daftar_tiket[i].jumlah_kursi,
               daftar_tiket[i].total_harga);
    }
    printf("+-------+----+--------------------------------------------------+-------+----------+\n");
    pauseScreen();
}

int login() {
    int pilihan;
    while(1) {	
		clearScreen();
		
	    printf("============= LOGIN =============\n");
	    printf("= 1. Login Admin                =\n");
	    printf("= 2. Keluar Program             =\n");
	    printf("=================================\n");
	    printf("Pilih menu: ");
	    
        if (scanf("%d", &pilihan) != 1) {
            pilihan = -1;
            while(getchar() != '\n'); 
        }
	
	    if (pilihan == 1) {
	        clearScreen();
	        
	        char username[50];
            char password[50];
            
	        printf("====== MENU LOGIN ======\n");
            printf("Username : ");
            scanf("%s", username);
            printf("Password : ");
            scanf("%s", password);

            if(strcmp(username, "admin") == 0 && strcmp(password, "admin123") == 0) {
                clearScreen();
                printf("Login berhasil!\n");
                pauseScreen();
                return 1; 
            } else {
            	printf("\nLogin salah!\n");
                printf("Tekan Enter untuk mencoba ulang...");
                getchar(); 
                getchar(); 
            }
	   	}
			    	
	    else if (pilihan == 2) {
	        return 0; 
	    }
	    else {
	        printf("\nPilihan tidak valid!\n");
            printf("Tekan Enter untuk mencoba lagi...");
            getchar();
            getchar();
		}	
	}
}

void menuUtama() {
    int pilihan;
    
    do {
        clearScreen();
        printf("\n--- 🎬 MENU UTAMA BIOSKOP --- \n");
        printf("==================================\n");
        printf("1. CRUD FILM (Admin)\n");
        printf("2. Film yang Sedang Tayang\n");
        printf("3. Booking Tiket\n");
        printf("4. Refund Tiket\n");
        printf("5. Tampilkan Tiket (Booking)\n");
        printf("6. Rumah Pola\n");
        printf("7. Bendera Negara\n");
        printf("8. Author Program\n");
        printf("9. Reset Pilihan\n");
        printf("0. Keluar Program\n");
        printf("==================================\n");
        printf("Pilih menu: ");
        
        if (scanf("%d", &pilihan) != 1) {
            pilihan = -1; 
            while(getchar() != '\n');
        }
        
        switch (pilihan) {
            case 1: menuFilm(); break;
            case 2: clearScreen(); showAllFilm(0); pauseScreen(); break; 
            case 3: bookingTiket(); break;
            case 4: refundTiket(); break;
            case 5: tampilkanTiket(); break;
            case 6: clearScreen(); printf("\n--- Rumah Pola (Belum diimplementasikan) ---\n"); pauseScreen(); break;
            case 7: clearScreen(); printf("\n--- Bendera Negara (Belum diimplementasikan) ---\n"); pauseScreen(); break;
            case 8: clearScreen(); printf("\n--- Author Program (Belum diimplementasikan) ---\n"); pauseScreen(); break;
            case 9: break; 
            case 0: printf("\nTerima kasih! Sampai jumpa.\n"); break;
            default: printf("\n⚠️ Pilihan tidak valid.\n"); pauseScreen();
        }
    } while (pilihan != 0);
}


int main() {
    addFilm(101, "Komang", 192, 50, 45000.0);
    addFilm(102, "Boboiboy", 180, 75, 40000.0);
    addFilm(103, "ONE PIECE", 166, 0, 42000.0); 
    
    if (login() == 1) {
        menuUtama();
    }
    
    return 0;
}