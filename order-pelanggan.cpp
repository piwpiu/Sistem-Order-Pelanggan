#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <limits>
#include <algorithm>
#include <ctime>

#define cout std::cout
#define cin std::cin

using namespace std;

typedef struct individualMenu {
    string namaMenu;
    int harga;
} im;
im *createIndividualMenu(string namaMenu, int harga);

int tentukanDiskon(const int &totalBayar);
void masukanKedalamRiwayat(vector<im *> v, vector<int> w,
                           string namaPelanggan, int totalBayar,
                           int diskon);
void masukan(const vector<im *> &v, const vector<int> &w,int index, ofstream &file);
void programPencatatan();
void printSeluruhRiwayat();
void urutkanTerkecil();
void urutkanTerbesar();
void tampilkanOrderSeseorang(string nama);
void hapusSeluruhRiwayat();
void hapusRiwayatPelanggan();
void bersihkanBuffer();
bool handleInvalidInput();
void printMenu(im *menu[], int menuSize);
void printWhiteSpace(size_t n);
vector<size_t> getMenuMaxLength(im *menu[],const int menuSize);
string getTodayTime();
string getMonth(int month);
void intiProgramPencatatan();
bool konfirmasi(string s);
void dealokasiMenu(im *menu[], int menuSize);

int main() {
    cout << "========================================\n"
         << "Sistem Manajemen Riwayat Order Pelanggan\n"
         << "========================================\n";

    while (true) {
        int pilihan = 0;

        cout << "pilihan: \n"
             << "[1] catat order pelanggan\n"
             << "[2] print seluruh riwayat\n"
             << "[3] urutkan riwayat dari total bayar terkecil\n"
             << "[4] urutkan riwayat dari total bayar terbesar\n"
             << "[5] tampilkan order seorang pelanggan\n"
             << "[6] hapus riwayat seorang pelanggan\n"
             << "[7] hapus seluruh riwayat\n"
             << "[0] keluar dari program\n"
             << "-> ";
        cin  >> pilihan;

        if (handleInvalidInput())
            continue;
        cout << "\n\n";

        string namaPelanggan;
        switch (pilihan) {
            case 7:
                hapusSeluruhRiwayat();
                break;
            case 6:
                hapusRiwayatPelanggan();
                break;
            case 5:
                cout << "masukan nama pelanggan: ";
                getline(cin, namaPelanggan);
                tampilkanOrderSeseorang(namaPelanggan);
                break;
            case 4:
                urutkanTerbesar();
                break;
            case 3:
                urutkanTerkecil();
                break;
            case 2:
                printSeluruhRiwayat();
                break;
            case 1:
                programPencatatan();
                break;
            case 0:
                cout << "\nAnda keluar dari program\n\n\n";
                return 0;
                break;
            default:
                cout << "pilihan tidak valid\n\n\n";
                break;
        }
    }
    return 0;
}

im *createIndividualMenu(string namaMenu, int harga) {
    im *newIM = new im;

    newIM->namaMenu = namaMenu;
    newIM->harga = harga;

    return newIM;
}

int tentukanDiskon(const int &totalBayar) {
    int diskon = 0;
    if (totalBayar >= 200000) {
        diskon = 20;
        cout << "selamat anda mendapatkan diskon 20% \n"
             << "karena sudah belanja melebihi 200 ribu rupiah\n"
             << "\n\n";
    }
    else if (totalBayar >= 100000) {
        diskon = 10;
        cout << "selamat anda mendapatkan diskon 10% \n"
             << "karena sudah belanja melebihi 100 ribu rupiah\n"
             << "\n\n";
    }
    return diskon;
}

void masukanKedalamRiwayat(vector<im *> v, vector<int> w,
                           string namaPelanggan, int totalBayar,
                           int diskon) {

    ofstream file("riwayat.txt", ios::app);

    file << "{\nNama pelanggan: " << namaPelanggan << "\n"
         << "Pesanan: \n";
    masukan(v, w, 0, file);
    file << "Dapat diskon: " << diskon     << "%\n"
         << "Total Bayar: "  << totalBayar << '\n'
         << getTodayTime()   << "}\n";
    file.close();
}

void masukan(const vector<im *> &v, const vector<int> &w,int index, ofstream &file) {
    if (index >= v.size())
        return;

    file << v[index]->namaMenu << "  -  "
         << v[index]->harga    << " berjumlah "
         << w[index]           << '\n';

    masukan(v, w, index + 1, file);
}

void programPencatatan() {
    char keluarProgram = 't';
    while (keluarProgram != 'y') {
        switch (keluarProgram) {
        case 'y':
            return;
            break;

        default:
            intiProgramPencatatan();

        while (true) {
            cout << "apakah ingin keluar program pencatatan pesanan [y/t]? \n";
            cin  >> keluarProgram;

            bool inputValid = false;

            if (cin.fail() || cin.get() != '\n') {
                cout << "input inginKeluarProgram tidak valid"
                     << "\n\n";
                bersihkanBuffer();
                continue;
            }
            keluarProgram = tolower(keluarProgram);

            if(keluarProgram == 'y' || keluarProgram == 't')
                inputValid = true;

            cout << "\n\n";

            if (inputValid) {
                if (keluarProgram == 'y')
                    return;
                else
                    break;
                }
            }
        }
    }
}

void printSeluruhRiwayat() {
    ifstream file("riwayat.txt");
    if (file.is_open()) {
        string line;
        cout << "Riwayat Order\n"
             << "===============================================" << '\n';
        while (getline(file, line)) {
            if (line.find("{") != string::npos)
                continue;
            else if (line.find("}") != string::npos) {
                cout << "===============================================" << '\n';
                continue;
            }
            cout << line << '\n';
        }
        file.close();
        cout << "\n\n";
    }
    else
        cout << "file tidak bisa dibuka.\n\n\n" ;
}

void urutkanTerkecil() {
    ifstream file("riwayat.txt");

    if (!file.is_open()) {
        cout << "gagal membuka file untuk sorting\n\n\n";
        return;
    }

    string line;
    string block = "";

    vector<string> blockList;
    vector<int> listTotalHarga;

    while (getline(file, line)) {
        if (line.find("}") != string::npos) {
            block.append(line);
            blockList.push_back(block);
            block = "";
            continue;
        }
        else if (line.find("Total Bayar: ") != string::npos) {
            int x = stoi( line.substr( 12 , string::npos) );
            listTotalHarga.push_back(x);
        }
        block.append(line + "\n");
    }

    for (int i = 0; i < listTotalHarga.size() - 1; ++i) {
        int minIndex = i;
        for (int j = i + 1; j < listTotalHarga.size(); ++j) {
            if (listTotalHarga[j] < listTotalHarga[minIndex]) {
                minIndex = j;
            }
        }
        swap(listTotalHarga[i], listTotalHarga[minIndex]);
        swap(blockList[i], blockList[minIndex]);
    }
    file.close();

    ofstream writeFile("riwayat.txt");
    if (writeFile.is_open()) {
        for (size_t i = 0; i < blockList.size(); i++)
            writeFile << blockList[i] << '\n';
    }
    else {
        cout << "gagal membuka file untuk memasukan hasil sorting\n\n";
        return;
    }
    cout << "sorting selesai\n\n\n";
    writeFile.close();
}

void urutkanTerbesar() {
    ifstream file("riwayat.txt");
    if (!file.is_open()) {
        cout << "gagal membuka file untuk sorting\n\n\n";
        return;
    }
    string line;
    string block = "";
    vector<string> blockList;
    vector<int>listTotalHarga;

    while (getline(file, line)) {
        if (line.find("}") != string::npos) {
            block.append(line);
            blockList.push_back(block);
            block = "";
            continue;
        }
        else if (line.find("Total Bayar: ") != string::npos) {
            int x = stoi( line.substr( 12 , string::npos) );
            listTotalHarga.push_back(x);
        }
        block.append(line + "\n");
    }

    for (int i = 0; i < listTotalHarga.size() - 1; ++i) {
        int minIndex = i;
        for (int j = i + 1; j < listTotalHarga.size(); ++j) {
            if (listTotalHarga[j] > listTotalHarga[minIndex]) {
                minIndex = j;
            }
        }
        swap(listTotalHarga[i], listTotalHarga[minIndex]);
        swap(blockList[i], blockList[minIndex]);
    }
    file.close();

    ofstream writeFile("riwayat.txt");
    if (writeFile.is_open()) {
        for (size_t i = 0; i < blockList.size(); i++)
            writeFile << blockList[i] << '\n';
    }
    else {
        cout << "gagal membuka file untuk memasukan hasil sorting\n\n";
        return;
    }
    cout << "sorting selesai\n\n\n";
    writeFile.close();
}
void tampilkanOrderSeseorang(string nama) {
    ifstream file("riwayat.txt");

    if (file.is_open()) {
        string line, block, result;
        bool ketemu = false;

        while (getline(file, line)) {
            if (line.find("}") != string::npos) {
                block.append(line);

                if (block.find(nama) != string::npos) {
                    string delim = "===============================================\n";

                    size_t pos1 = block.find("{");

                    if (!ketemu)
                        block.replace(pos1, 2, delim);
                    else
                        block.erase(pos1, 2);

                    size_t pos2 = block.find("}");
                    block.erase(pos2, 2);
                    block.append(delim);

                    result.append(block);

                    block = "";

                    ketemu = true;
                    continue;
                }
                block = "";
                continue;
            }
            block.append(line + "\n");
        }
        if (ketemu) {
            cout << "order ketemu\n" << "Riwayat Order\n" << result << "\n\n";
        }
        else if (!ketemu)
            cout << "tidak ada order dengan nama " << nama << "\n\n\n";
        file.close();
    }
    else
        cout << "file tidak bisa dibuka\n\n\n";
}

void hapusSeluruhRiwayat() {
    if ( konfirmasi("hapus seluruh riwayat") )
        return;

    ofstream file("riwayat.txt");

    if (file.is_open()) {
        file.close();
        cout << "riwayat berhasil di hapus\n\n\n";
    }
    else
        cout << "file tidak dapat dibuka\n\n\n";
}

void hapusRiwayatPelanggan() {
    string nama;
    cout << "masukan nama pelanggan: ";
    getline(cin, nama);

    string pesanKonfirmasi = "menghapus riwayat order dengan nama " + nama;
    if ( konfirmasi(pesanKonfirmasi) )
        return;

    ifstream file("riwayat.txt");

    if (file.is_open()) {
        string line, block, newFilecontent = "";
        bool ketemu = false;

        while (getline(file, line)) {
            block.append(line + "\n");

            if (line.find("}") != string::npos) {
                if (block.find(nama) != string::npos && !ketemu) {
                    cout << block << "\n\n"
                         << "berhasil dihapus\n\n\n" ;
                    block = "";
                    ketemu = true;
                    continue;
                }
                newFilecontent += block;
                block = "";
                continue;
            }
        }

        ofstream writeFile("riwayat.txt");

        writeFile << newFilecontent;

        if (!ketemu)
            cout << "tidak ada order dengan nama " << nama << "\n\n\n";

        writeFile.close();
        file.close();
    }
    else
        cout << "file tidak bisa dibuka\n\n\n";
}

void bersihkanBuffer() {
    cin.clear();
    cin.ignore(numeric_limits<int>::max(), '\n');
    cout << "input tidak valid"
         << "\n\n";
}

bool handleInvalidInput() {
    cin.ignore(numeric_limits<int>::max(), '\n');
    if (cin.fail()){
        bersihkanBuffer();
        return true;
    }
    return false;
}

void printMenu(im *menu[], int menuSize) {
    /*
    [0] = nama menu max length
    [1] = harga menu max length
    [2] = menuSize string length
    */
    vector<size_t> maxMenuLength = getMenuMaxLength(menu, menuSize);

    cout << "Masukan pesanan pelanggan\n"
         << "Nama Menu ";
    printWhiteSpace(maxMenuLength[0] + maxMenuLength[1] + 1);
    cout << "Harga \n"
         << "===============================================\n";

    int j = 1;
    for(size_t i = 0; i < menuSize; i++) {
        cout << "[" <<j<< "] " << menu[i]->namaMenu ;
        size_t spaceAmount = ( maxMenuLength[0] - (menu[i]->namaMenu.length()) ) + maxMenuLength[2] - to_string(j).length();
        printWhiteSpace(spaceAmount);
        cout << menu[i]->harga << '\n';
        j++;
    }
    cout << "===============================================\n";
}

void printWhiteSpace(size_t n){
    for (size_t i = 0; i < n; i++)
        cout << " ";
}

vector<size_t> getMenuMaxLength(im *menu[],const int menuSize) {
    vector<size_t> arr1;
    vector<size_t> arr2;

    for (size_t i = 0; i < menuSize; i++) {
        arr1.push_back( menu[i]->namaMenu.length() );
        arr2.push_back( to_string( menu[i]->harga ).length() );
    }

    sort(arr1.begin(), arr1.end(), greater<int>());
    sort(arr2.begin(), arr2.end(), greater<int>());

    vector<size_t> result {arr1[0], arr2[0], arr1.size()};

    return result;
}

string getTodayTime() {
    // ambil waktu saat ini
    time_t currentTime = time(nullptr);

    // konversi ke waktu lokal
    tm* localTime = localtime(&currentTime);

    // ambil tahun, bulan, hari
    int year = localTime->tm_year + 1900;  // tahun sejak 1900
    int month = localTime->tm_mon;         // bulan (0-11)
    int day = localTime->tm_mday;          // hari pada bulan (1-31)

    // ambil menit dan jam
    int hour = localTime->tm_hour;     // jam (0-23)
    int minutes = localTime->tm_min;   // menit (0-59)

    string hourStr, minutesStr;
    if (hour < 10){
        hourStr = "0";
        hourStr.append(to_string(hour));
    }
    else
        hourStr = to_string(hour);

    if (minutes < 10){
        minutesStr = "0";
        minutesStr.append(to_string(minutes));
    }
    else
        minutesStr = to_string(minutes);

    string result = "Waktu order: " + to_string(day) + "-" + getMonth(month)
                  + "-" + to_string(year) + " " + hourStr + ":"
                  + minutesStr + '\n';
    return result;
}

string getMonth(int month) {
    string months[] = {"januari", "februari", "maret"
                     "april", "mei", "juni",
                     "juli", "agustus", "september",
                     "oktober", "november", "desember"};
    return months[month];
}

void intiProgramPencatatan(){
    string namaPelanggan;
    cout << "selamat datang di program pencatatan order pelanggan\n"
            << "masukan nama pelanggan: ";
    getline(cin, namaPelanggan);
    cout << "\n\n";

    im* menu[] = {
        createIndividualMenu("Double cheese burger", 25000),
        createIndividualMenu("Big mac burger", 35000),
        createIndividualMenu("Big big mac burger", 45000),
        createIndividualMenu("Chicken muffin with egg", 25000),
        createIndividualMenu("Pancakes 3pcs", 20000),
        createIndividualMenu("Fried chicken dengan nasi", 20000),
        createIndividualMenu("Nasi uduk McD", 25000),
        createIndividualMenu("Coca cola", 5000),
        createIndividualMenu("Mango float", 12000),
        createIndividualMenu("Fanta float", 10000),
        createIndividualMenu("French fries", 15000),
        createIndividualMenu("Spicy chicken bites", 20000),
        createIndividualMenu("McNuggets 10pcs", 25000)
    };

    int menuSize = sizeof(menu) / sizeof(menu[0]);

    vector<im *> pesananPelanggan;
    vector<int> jmlPesananPelanggan;
    int jumlahPesanan;
    int totalBayar = 0;
    /*
    0 = neutral state, langsung nanya mo ke state mana
    1 = tampilkan order saat ini
    2 = sedang input order
    3 = sedang hapus order
    4 = cancel order pelanggan,
        tidak dimasukan ke dalam riwayat,
        lanjut ke apakah ingin ulang program pencatatan
    5 = selesai mencatat order pelanggan,
        order dimasukan kedalam riwayat,
        lanjut ke pembayaran
    */
    int state = 2;
    do {
        if (state == 1) {
            cout << "pesanan " << namaPelanggan << " saat ini:\n";
            for (size_t i = 0; i < pesananPelanggan.size(); i++) {
                cout <<"[" << i+1 << "] " << pesananPelanggan[i]->namaMenu
                     << " berjumlah " << jmlPesananPelanggan[i] << '\n';
            }
        }
        else if (state == 2) {
            printMenu(menu, menuSize);

            int pilihanMenu = 0;
            cout << "pilihan anda: ";
            cin  >> pilihanMenu;

            if (handleInvalidInput())
                continue;
            else if (pilihanMenu > menuSize || pilihanMenu < 1) {
                cout << "tidak ada pilihan untuk menu tersebut"
                        << "\n\n";
                continue;
            }

            cout << "masukan jumlah pesanan: ";
            cin  >> jumlahPesanan;

            if (handleInvalidInput())
                continue;
            else if (jumlahPesanan <= 0) {
                cout << "jumlah pesanan tidak boleh 0 atau negatif"
                     << "\n\n";
                continue;
            }
            else if(jumlahPesanan > 100) {
                cout << "tidak dapat memesan lebih dari 100"
                     << "\n\n";
                continue;
            }

            totalBayar = totalBayar + (menu[pilihanMenu-1]->harga * jumlahPesanan);
            pesananPelanggan.push_back(menu[pilihanMenu-1]);
            jmlPesananPelanggan.push_back(jumlahPesanan);
        }
        else if(state == 3) {
            // jika kosong
            if (!pesananPelanggan.size())
                continue;

            cout << "pesanan " << namaPelanggan << " saat ini:\n";
            for (size_t i = 0; i < pesananPelanggan.size(); i++) {
                cout <<"[" << i+1 << "] " << pesananPelanggan[i]->namaMenu
                     << " berjumlah " << jmlPesananPelanggan[i] << '\n';
            }

            int menuYgDihapus;
            cout << "hapus nomor: ";
            cin  >> menuYgDihapus;
            if( handleInvalidInput() ) continue;
            else if(menuYgDihapus < 1 || menuYgDihapus > pesananPelanggan.size()) continue;

            auto it1 = pesananPelanggan.begin();
            auto it2 = jmlPesananPelanggan.begin();
            for (size_t i = 1; i < menuYgDihapus; i++) {
                ++it1;
                ++it2;
            }

            pesananPelanggan.erase(it1);
            jmlPesananPelanggan.erase(it2);
            totalBayar = totalBayar - pesananPelanggan[menuYgDihapus-1]->harga * jumlahPesanan;
        }
        else if (state == 4) {
            dealokasiMenu(menu, menuSize);
            cout << "pencatatan order atas nama " << namaPelanggan << " dibatalkan\n\n\n";
            return;
        }
        else if (state == 5) {
            break;
        }

        while (true) {
            int pilihanMenuLanjutan;
            cout << "\nmenu lanjutan:\n"
                 << "[1] tampilkan order saat ini\n"
                 << "[2] input order pelanggan lagi\n"
                 << "[3] hapus salah satu order pelanggan\n"
                 << "[4] cancel order pelanggan\n"
                 << "[5] lanjut ke pembayaran\n";
            cin  >> pilihanMenuLanjutan;

            if(handleInvalidInput())
                continue;

            if (pilihanMenuLanjutan == 1) {
                state = 1;
                break;
            }
            else if (pilihanMenuLanjutan == 2) {
                state = 2;
                break;
            }
            else if (pilihanMenuLanjutan == 3) {
                if ( konfirmasi("hapus salah satu order pelanggan") ){
                    state = 0;
                    break;
                }
                state = 3;
                break;
            }
            else if(pilihanMenuLanjutan == 4) {
                if ( konfirmasi("cancel order pelanggan") ) {
                    state = 0;
                    break;
                }
                state = 4;
                break;
            }
            else if(pilihanMenuLanjutan == 5) {
                if ( konfirmasi("lanjut ke pembayaran") ) {
                    state = 0;
                    break;
                }
                state = 5;
                break;
            }
            else {
                cout << "pilihan tidak valid\n\n";
                continue;
            }
        }
        cout << "\n\n";
    } while (state != 4 || state != 5);

    // jika order kosong tapi lanjut ke pembayaran
    if(!pesananPelanggan.size()) {
        dealokasiMenu(menu, menuSize);
        cout << "order " << namaPelanggan << " kosong, tidak dapat melakukan pembayaran\n\n\n";
        return;
    }

    int diskon = tentukanDiskon(totalBayar);

    // hitung total bayar setelah didiskon
    int potonganHarga = (diskon * totalBayar) / 100;

    totalBayar = totalBayar - potonganHarga;

    int uangPembeli = 0;

    while (uangPembeli < totalBayar) {
        cout << "total harga pesanan " << totalBayar << " rupiah\n"
             << "masukan uang pelanggan: ";
        cin  >> uangPembeli;

        if (handleInvalidInput())
            continue;
        else if (uangPembeli < totalBayar) {
            cout << "nominal uang tidak cukup\n"
                    << "silahkan bayar lagi\n"
                    << "\n\n";
            continue;
        }
    }

    cout << "kembalian: " << uangPembeli - totalBayar << " rupiah\n"
         << "\n\n";

    masukanKedalamRiwayat(pesananPelanggan, jmlPesananPelanggan,
                          namaPelanggan, totalBayar, diskon);

    dealokasiMenu(menu, menuSize);
}

bool konfirmasi(string s) {
    int pilihan;
    cout << "Apakah yakin ingin " << s <<" ?\n"
         << "[1] Ya\n"
         << "[2] Tidak\n";
    cin  >> pilihan;

    if (handleInvalidInput()) {
        cout << "input tidak valid\n"
             << "operasi gagal, ulangi lagi\n\n\n";
        return true;
    }

    if (pilihan == 2) {
        cout << "tidak jadi " << s << "\n\n\n";
        return true;
    }
    return false;
}

void dealokasiMenu(im *menu[], int menuSize) {
    for (size_t i = 0; i < menuSize; i++) {
        im *hapus = menu[i];
        delete hapus;
    }
}
