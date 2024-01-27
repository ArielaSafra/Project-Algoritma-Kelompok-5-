#include <iostream>
#include <string>
#include <ctime>

using namespace std;

// memeber
struct Member {
    string name;
    int memberId;
    int borrowedBooks;
};

// book
struct Book {
    string title;
    string author;
    int stock;
    int borrowed;
    Member borrowedBy;
    time_t borrowDate;
};

// get tanggal saat ini
string getCurrentDateAsString() {
    time_t now = time(0);
    tm* currentDate = localtime(&now);

    char buffer[80];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d", currentDate);

    return string(buffer);
}

// Menambahkan jumlah hari ke tanggal saat ini
string addDaysToCurrentDate(int daysToAdd) {
    time_t now = time(0);
    now += daysToAdd * 24 * 60 * 60; // Menambahkan detik ke waktu saat ini

    tm* newDate = localtime(&now);

    char buffer[80];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d", newDate);

    return string(buffer);
}

// Fungsi login
void login() {
    string username, password;

    cout << "Masukkan username: ";
    cin >> username;

    cout << "Masukkan password: ";
    cin >> password;

    // user login
    if (username != "mahasiswa" || password != "123") {
        cout << "Login gagal. Periksa kembali username dan password." << endl;
        exit(0);
    }

    cout << "Login berhasil. Selamat datang, " << username << "!" << endl;
}

// Menampilkan daftar member
void viewMembers(const Member members[], int memberCount) {
    cout << "Daftar Member:" << endl;
    cout << "==============" << endl;
    for (int i = 0; i < memberCount; i++) {
        cout << i + 1 << ". " << members[i].name << " - ID: " << members[i].memberId << endl;
    }
}

// Fungsi input member
void inputMember(Member members[], int& memberCount) {
    cin.ignore(); // Membersihkan newline character dari input sebelumnya

    cout << "Masukkan nama member:\t";
    getline(cin, members[memberCount].name);

    members[memberCount].memberId = memberCount + 1; // Member ID diisi secara otomatis sesuai dengan urutan penambahan
    members[memberCount].borrowedBooks = 0; // Inisialisasi jumlah buku yang dipinjam oleh member

    cout << "Member berhasil ditambahkan. ID member baru:\t" << members[memberCount].memberId << endl;

    memberCount++;
}

// Input buku
void inputBook(Book books[], int& bookCount) {
    cout << "Masukkan judul buku:\t";
    cin.ignore();
    getline(cin, books[bookCount].title);

    cout << "Masukkan nama penulis:\t";
    getline(cin, books[bookCount].author);

    cout << "Masukkan jumlah stok:\t";
    cin >> books[bookCount].stock;

    books[bookCount].borrowed = 0; // Inisialisasi jumlah buku yang dipinjam
    bookCount++;
}

// Menampilkan daftar buku
void showBookList(const Book books[], int bookCount) {
    cout << "Daftar buku yang tersedia:" << endl;
    int totalStock = 0;
    for (int i = 0; i < bookCount; i++) {
        cout << i + 1 << ". " << books[i].title << " - " << books[i].author << " - " << books[i].stock << " stok" << endl;
        totalStock += books[i].stock;
    }
    cout << "Total stok saat ini: " << totalStock << "." << endl;
}

// Peminjaman buku
void borrowBook(Book books[], int bookCount, Member members[], int memberCount) {
    cout << "Masukkan nomor buku yang ingin dipinjam: ";
    int bookNumber;
    cin >> bookNumber;

    if (bookNumber < 1 || bookNumber > bookCount) {
        cout << "Nomor buku yang Anda masukkan salah. Silakan coba lagi." << endl;
        return;
    }

    if (books[bookNumber - 1].stock == 0) {
        cout << "Maaf, stok buku tersebut saat ini kosong. Silakan coba lagi nanti." << endl;
        return;
    }

    cout << "Masukkan nomor member yang meminjam: ";
    int memberNumber;
    cin >> memberNumber;

    if (memberNumber < 1 || memberNumber > memberCount) {
        cout << "Nomor member yang Anda masukkan salah. Silakan coba lagi." << endl;
        return;
    }

    // Periksa batas peminjaman member
    if (members[memberNumber - 1].borrowedBooks >= 3) {
        cout << "Member ini telah mencapai batas peminjaman (maksimum 3 buku)." << endl;
        return;
    }

    books[bookNumber - 1].stock--;
    books[bookNumber - 1].borrowed++;
    books[bookNumber - 1].borrowedBy = members[memberNumber - 1];
    books[bookNumber - 1].borrowDate = time(0);

    members[memberNumber - 1].borrowedBooks++;

    cout << "Buku berhasil dipinjam oleh " << members[memberNumber - 1].name << ". Jumlah stok saat ini: " << books[bookNumber - 1].stock << "." << endl;
    cout << "Tanggal peminjaman: " << getCurrentDateAsString() << endl;
    // maks 7 hari
    string futureDate = addDaysToCurrentDate(7);
    cout << "Maksimal Pengembalian 7 hari: " << futureDate << endl;
}

// Menampilkan daftar buku yang sedang dipinjam
void showBorrowedBooks(const Book books[], int bookCount, const Member members[], int memberCount) {
    bool borrowedBooksExist = false;
    int totalBorrowed = 0;

	cout << "=================================" << endl;
    cout << "Daftar buku yang sedang dipinjam:" << endl;
    cout << "=================================" << endl;
    for (int i = 0; i < bookCount; i++) {
        if (books[i].borrowed > 0) {
            borrowedBooksExist = true;
            cout << i + 1 << ". " << books[i].title << " - Dipinjam: " << books[i].borrowed << " buku oleh " << members[books[i].borrowedBy.memberId - 1].name << endl;
        }
        totalBorrowed += books[i].borrowed;
    }
    cout << "Total buku yang sedang dipinjam: [" << totalBorrowed << "]." << endl;

    if (!borrowedBooksExist) {
        cout << "Tidak ada buku yang sedang dipinjam." << endl;
    }
}

// Menghitung selisih hari antara dua tanggal
int calculateDaysDifference(time_t startDate, time_t endDate) {
    // Menghitung selisih detik
    int difference = difftime(endDate, startDate);

    // Mengonversi selisih detik menjadi hari
    int daysDifference = difference / (60 * 60 * 24);

    return daysDifference;
}

// Pengembalian buku
void returnBook(Book books[], int bookCount, Member members[], int memberCount) {
    cout << "Masukkan nomor buku yang ingin dikembalikan:\t";
    int bookNumber;
    cin >> bookNumber;

    cout << "Masukkan nomor member yang melakukan peminjaman: ";
    int memberNumber;
    cin >> memberNumber;
    
    cout << "Masukkan tanggal pengembalian (YYYY-MM-DD): ";
    string returnDate;
    cin >> returnDate;

    // Memeriksa apakah nomor buku dan nomor member yang dimasukkan adalah valid
    if (books[bookNumber - 1].borrowDate != 0) {
        // Mengonversi tanggal pengembalian ke dalam time_t
        tm returnDate_tm = {};
        sscanf(returnDate.c_str(), "%d-%d-%d", &returnDate_tm.tm_year, &returnDate_tm.tm_mon, &returnDate_tm.tm_mday);
        returnDate_tm.tm_year -= 1900; // Tahun dimulai dari 1900
        returnDate_tm.tm_mon -= 1;    // Bulan dimulai dari 0
        time_t returnDateTime = mktime(&returnDate_tm);

        int daysOverdue = calculateDaysDifference(books[bookNumber - 1].borrowDate, returnDateTime);

        // logika denda
        if (daysOverdue > 7) {
            int fine = (daysOverdue - 7) * 500; // Denda Rp. 500 per hari
            cout << "==========================================" << endl;
            cout << "Denda sebesar Rp. " << fine << " dikenakan karena melebihi batas waktu peminjaman." << endl;
            cout << "Keterlambatan: " << daysOverdue - 7 << " hari." << endl;
            cout << "==========================================" << endl;
        }
    }

    // Mengembalikan buku
    books[bookNumber - 1].stock++;
    books[bookNumber - 1].borrowed--;

    // Mengatur ulang informasi peminjaman pada buku
    books[bookNumber - 1].borrowDate = 0;

    // Mengurangi jumlah buku yang dipinjam oleh member
    members[memberNumber - 1].borrowedBooks--;

    cout << "Buku berhasil dikembalikan oleh " << members[memberNumber - 1].name << ". Jumlah stok saat ini: " << books[bookNumber - 1].stock << "." << endl;
}

// Laporan stok
void showReport(const Book books[], int bookCount) {
    int totalStock = 0;
    int totalBorrowed = 0;
    for (int i = 0; i < bookCount; i++) {
        totalStock += books[i].stock;
        totalBorrowed += books[i].borrowed;
    }
    cout << "\nTotal stok stok buku saat ini:\t" << totalStock << "." << endl;
    cout << "Total buku yang sedang dipinjam:\t" << totalBorrowed << "." << endl;
}

int main() {
    cout << "Selamat datang di perpustakaan sederhana!" << endl;

    // Login
    login();

    // Menambahkan data dummy
    Book books[100] = {
        {"Harry Potter", "J.K. Rowling", 5, 0},
        {"To Kill a Mockingbird", "Harper Lee", 3, 0},
        {"1984", "George Orwell", 7, 0},
        {"Malioboro at midnight", "Skysphire", 4, 0},
        {"Hello (Again), Cello","Nadia Ristivani" , 8, 0},
        {"Loversation","Valerie Patkar", 3, 0},
        {"Lentera Senja", "Alfia Ramadhani", 7, 0},
        {"Ghost Girl, Banana", "Wiz Wharton", 5, 0}
        // Tambahkan data buku dummy sesuai kebutuhan
    };
    int bookCount = 8; // Sesuaikan dengan jumlah buku dummy yang ditambahkan

    Member members[50] = {
        {"Ahmad", 1},
        {"Ariel", 2},
        {"Kanaya", 3}
    };
    int memberCount = 3;

    while (true) {
        cout << "Pilih menu yang Anda inginkan:" << endl;
        cout << "1. Input Data Buku" << endl;
        cout << "2. Lihat Data Buku" << endl;
        cout << "3. Input Data Member" << endl;
        cout << "4. Lihat Data Member" << endl;
        cout << "5. Peminjaman" << endl;
        cout << "6. Pengembalian" << endl;
        cout << "7. Buku yang sedang dipinjam" << endl;
        cout << "8. Laporan" << endl;
        cout << "9. Exit" << endl;
        cout << "Pilih : ";

        int menu;
        cin >> menu;

        switch (menu) {
            case 1:
                // Input buku
                inputBook(books, bookCount);
                break;
            case 2:
                // Daftar buku
                showBookList(books, bookCount);
                break;
            case 3:
                // Input member
                inputMember(members, memberCount);
                break;
            case 4:
                // Daftar member
                viewMembers(members, memberCount);
                break;
            case 5:
                // Peminjaman
                borrowBook(books, bookCount, members, memberCount);
                break;
            case 6:
                // Pengembalian
                returnBook(books, bookCount, members, memberCount);
                break;
            case 7:
                // Daftar buku yang sedang dipinjam
                showBorrowedBooks(books, bookCount, members, memberCount);
                break;
            case 8:
                // Laporan
                showReport(books, bookCount);
                break;
            case 9:
                // Exit
                return 0;
            default:
                cout << "Menu yang Anda masukkan salah. Silakan coba lagi." << endl;
        }
    }

    return 0;
}
