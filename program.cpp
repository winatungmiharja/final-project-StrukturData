#include <bits/stdc++.h>
#include <fstream>
using namespace std;

const char space    = ' ';
const int col     	= 25;

struct identitas {
    string nama;
    string NIP;
};

bool isSmaller(string a, string b){
    int startA=0, startB=0;
    bool ans = false;
    
    if(a.front()=='0')
    {
        while(a[startA]=='0')
        {
            startA++;
        }
    }

    if(b.front()=='0')
    {
        while(b[startB]=='0')
        {
            startB++;
        }
    }

    int lengthA=a.length()-startA;
    int lengthB=b.length()-startB;
    
    if(lengthA!=lengthB)
    {
        return (lengthA<lengthB)? 1:0;
    }
    else
    {
        int c = min(lengthA,lengthB);
        for(int i = 0; i < c ; ++i) {
            if ((a[i+startA] - '0') < (b[i+startB] - '0'))
            { 
                ans=true;
                break; 
            }
            else if ((a[i+startA] - '0') > (b[i+startB] - '0'))
            {
                ans=false;
                break;
            }
            else{
                continue;
            }
        }
        return ans;
    }
    return ans;
}

bool operator<(const identitas& a, const identitas& b)
{
    return a.NIP < b.NIP;
}

bool sortNama (const identitas& a, const identitas& b)
{
    return a.nama < b.nama;
}

bool sortNIP (const identitas& a, const identitas& b)
{
    return isSmaller(a.NIP , b.NIP);
}

class CRUD {
    public :
    vector<identitas>data;
    unsigned int size;

    CRUD(){
        data.clear();
        size=0;
        identitas mahasiswa;
        ifstream read;
        read.open("nip.txt");
        while (getline(read , mahasiswa.nama)) {
            read >> mahasiswa.NIP;
            data.push_back({_formatNama(mahasiswa.nama),mahasiswa.NIP});
            read.ignore();
        }
        size = data.size();
        read.close();
    }

    int query() {
        int option;
        _printQueryDivider();
        cout << "==> MENU UTAMA \n \n";
        cout << "1. Tampilkan Data Mahasiswa" << endl;
        cout << "2. Tambahkan Data Mahasiswa" << endl;
        cout << "3. Perbarui Data Mahasiswa" << endl;
        cout << "4. Hapus Data Mahasiswa" << endl;
        cout << "5. Keluar dari Program" << endl;
        _printQueryDivider();
        cout << "Masukan Pilihan : ";
        cin >> option;
        system("cls");
        if(option>=1 && option<=5)
        {
            return option;
        }
        else
        {
            return 0;
        }
    };

    void printData(){
        switch(_printQuery())
        {
            case 0: //salah
                cout << "Input Anda Salah" << endl;
                break;

            case 1: {
                vector<identitas>urutNIP;
                urutNIP.assign(data.begin(),data.end());
                sort(urutNIP.begin(),urutNIP.end(),sortNIP);
                _printData(urutNIP);
                break;
            }

            case 2: {
                vector<identitas>urutNama;
                urutNama.assign(data.begin(),data.end());
                sort(urutNama.begin(),urutNama.end(),sortNama);
                _printData(urutNama);
                break;
            }

            case 3: //raw
                _printData(data);
                break;

            case 4: //batal
                return;
                
            default :
                cout << "Input yang dimasukkan salah \n";
                return;
        }
    }

    void addData() {
        identitas mahasiswa;
        cout << "Input Data Mahasiswa \n";
        cout << "Nama Lengkap : ";
        cin.get();
        getline(cin,mahasiswa.nama);
        cout << "NIP : ";
        cin >> mahasiswa.NIP;

        if(_findNIP(mahasiswa.NIP))
        {
            cout << endl << "Data mahasiswa gagal di-input" << endl; 
            cout << endl << "karena data mahasiswa sudah ada di Database" << endl;
        }
        else
        {
            _addData(mahasiswa);
            size++;
            cout << endl << "Data mahasiswa berhasil di-input" << endl;    
        }
        return;
    }

    void updateData() {
        cout << "INPUT NIP MAHASISWA YANG AKAN DIPERBARUI" << endl;
        int idMahasiswa = _searchData();
        if(idMahasiswa == -1)
        {   _printQueryDivider();
            cout << "Data Mahasiswa tidak ditemukan" << endl;
            _printQueryDivider();
            return;
        }
        else
        {
            identitas mahasiswaBaru;
            cout << "Anda akan melakukan pembaruan \ndata Mahasiswa dengan \n";
            cout << "Nama : " << data[idMahasiswa].nama << endl;
            cout << "NIP : " << data[idMahasiswa].NIP << endl;
            switch(_updateQuery())
            {
                case 0:
                    cout << "Input anda salah! \n";
                    break;

                case 1: //update Nama
                    cout << "NIP : " << data[idMahasiswa].NIP << endl;
                    cout << "Nama : " ;
                    cin.get();
                    getline(cin,mahasiswaBaru.nama);
                    _updateNama(data[idMahasiswa].NIP,_formatNama(mahasiswaBaru.nama));
                    break;

                case 2: //update NIP
                    cout << "Nama : " << data[idMahasiswa].nama << endl;
                    cout << "NIP : " ;
                    cin.get();
                    getline(cin,mahasiswaBaru.NIP);
                    _updateNIP(data[idMahasiswa].nama,mahasiswaBaru.NIP);
                    break;

                case 3:
                    cout << "Nama : " ;
                    cin.get();
                    getline(cin,mahasiswaBaru.nama);
                    cout << "NIP : " ;
                    getline(cin,mahasiswaBaru.NIP);
                    _updateData(mahasiswaBaru,idMahasiswa);
                    return;

                case 4:
                    cout << "Perbarui data dibatalkan \n";
                    return;

                default :
                    cout << "Input yang dimasukkan salah \n";
                    return;
            }
            cout << "Data berhasil diubah! \n";
        }
    }

    void deleteData() {
        cout << "INPUT NIP MAHASISWA YANG AKAN DIHAPUS" << endl;
        int idMahasiswa = _searchData();
        if(idMahasiswa == -1)
        {
            cout << "Data Mahasiswa tidak ditemukan" << endl;
            return;
        }
        else
        {
            char opt;
            identitas mahasiswaBaru;
            _printQueryDivider();
            cout << "Anda akan menghapus data Mahasiswa dengan \n";
            cout << "Nama : " << data[idMahasiswa].nama << endl;
            cout << "NIP : " << data[idMahasiswa].NIP << endl;
            _printQueryDivider();
            cout << "Apakah Anda yakin?" << endl;
            cout << "(y/n) ";
            cin >> opt;
            if(opt =='y')
            {
                _deleteData(data[idMahasiswa].NIP);
                cout << "Data berhasil dihapus! \n";
            }
            else
            {
                cout << "Penghapusan data dibatalkan \n";
            }
        }
        size--;
        return;
    }

    bool exitProgram(){
        char opt;
        cout << "Apakah anda ingin keluar dari program ini? \n";
        cout << "(y/n)? ";
        cin>>opt;
        return (opt=='y')?true:false;
    }

    private :
    
    bool _findNIP(string NIP) {
        for(int i=0; i<data.size();i++)
        {
            if(data[i].NIP == NIP)
            {
                return true;
            }
        }
        return false;
    }

    string _formatNama (string &temp) {
        string nama=temp;
        int sz = nama.length();
        for(int i=0;i<sz;++i)
        {
            if(i==0)
            {
                if(nama[i]>='a') nama[i]=toupper(nama[i]);
            }
            else if (nama[i]==' ' )
            {
            nama[i+1]=toupper(nama[i+1]);
            }
            else
            {
                if(i>0 && nama[i-1]!=' ')
                nama[i]=tolower(nama[i]);
            }
        }
        return nama;
    }

    int _searchData() {
        string temp;
        cout << "NIP Mahasiswa : ";
        cin >> temp;
        for(int i=0; i<data.size();i++)
        {
            if(data[i].NIP == temp)
            {
                return i;
            }
        }
        return -1;
    }

    //updateData
    int _updateQuery() {
        int option;
        _printQueryDivider();
        cout << "==> PILIH DATA YANG AKAN DIPERBARUI \n \n";
        cout << "1. Perbarui Nama" << endl;
        cout << "2. Perbarui NIP" << endl;
        cout << "3. Perbarui Nama dan NRP" << endl;
        cout << "4. Batal" << endl;
        _printQueryDivider();
        cout << "Masukan Pilihan : ";
        cin >> option;
        if(option<=4 && option>=1)
        {
            return option;
        }
        else
        {
            return 0;
        }
    };

    void _updateNama(string NIP, string namaBaru) {
        ofstream temp;
        ifstream read;
        temp.open("temp.txt");
        read.open("nip.txt");
        identitas mahasiswa;
        while (getline(read, mahasiswa.nama)) {
            read >> mahasiswa.NIP;
            read.ignore();

            if (mahasiswa.NIP == NIP) {
            temp <<  namaBaru << endl;
            }
            else {
            temp <<  mahasiswa.nama << endl;
            }
            
            temp << mahasiswa.NIP << endl;
        }
        read.close();
        temp.close();
        remove("nip.txt");
        rename("temp.txt", "nip.txt");
        return;
    }

    void _updateNIP(string nama, string NIPBaru) {
        ofstream temp;
        ifstream read;
        temp.open("temp.txt");
        read.open("nip.txt");
        identitas mahasiswa;
        while (getline(read, mahasiswa.nama)) {
            read >> mahasiswa.NIP;
            read.ignore();

            temp << mahasiswa.nama << endl;
            if (mahasiswa.nama == nama) {
            temp <<  NIPBaru << endl;
            }
            else {
            temp <<  mahasiswa.NIP << endl;
            }
        }
        read.close();
        temp.close();
        remove("nip.txt");
        rename("temp.txt", "nip.txt");
        return;
    }

    void _updateData(identitas mahasiswaBaru, int id) {
        ofstream temp;
        ifstream read;
        temp.open("temp.txt");
        read.open("nip.txt");
        identitas mahasiswa;
        int counter=0;
        while (getline(read, mahasiswa.nama)) {
            read >> mahasiswa.NIP;
            read.ignore();
            if(counter==id)
            {
                temp << _formatNama(mahasiswaBaru.nama) << endl;
                temp << mahasiswaBaru.NIP << endl;
            }
            else {
                temp << mahasiswa.nama << endl;
                temp << mahasiswa.NIP << endl;
            }
            counter++;
        }
        read.close();
        temp.close();
        remove("nip.txt");
        rename("temp.txt", "nip.txt");
        return;
    }

    //deleteData
    void _deleteData(string NIP){
        ofstream temp;
        ifstream read;
        temp.open("temp.txt");
        read.open("nip.txt");
        identitas mahasiswa;
        while (getline(read, mahasiswa.nama)) {
            read >> mahasiswa.NIP;
            read.ignore();
            if (mahasiswa.NIP != NIP) {
                temp <<  mahasiswa.nama << endl;
                temp << mahasiswa.NIP << endl;
            }
            else {
                continue;
            }
        }
        read.close();
        temp.close();
        remove("nip.txt");
        rename("temp.txt", "nip.txt");
        return;
    }

    //addData
    void _addData(identitas mahasiswa) {
        ofstream write;
        write.open("nip.txt", ios::app);
        write << _formatNama(mahasiswa.nama) << endl << mahasiswa.NIP << endl;
        write.close();
        return;
    }

    //printData
    int _printQuery() {
        int option;
        _printQueryDivider();
        cout << "==> PILIH TAMPILAN URUTAN YANG DIINGINKAN \n \n";
        cout << "1. Tampilkan dengan urut NIP" << endl;
        cout << "2. Tampilkan dengan urut Nama" << endl;
        cout << "3. Tampilkan data" << endl;
        cout << "4. Batal" << endl;
        _printQueryDivider();
        cout << "Masukan Pilihan : ";
        cin >> option;
        system("cls");
        if(option<=4 && option>=1)
        {
            return option;
        }
        else
        {
            return -1;
        }
    };

    void _printHeader(string col_1, string col_2) {
    _printDivider();
    cout 	<< "| " << left << setw(col) << setfill(space) << col_1
    	 	<< " | " << left << setw(col) << setfill(space) << col_2
    	 	<< " |" << endl;
    _printDivider();
    return;
    }

    void _printData(vector<identitas>&data) {
        _printHeader("Nama","NIP");
        int sz = data.size();
        for(int i=0; i<sz;i++) {
            _printMhs(data[i]);
        }
        _printDivider();
    }

    void _printDivider(){
        cout << "+" << setw(col+2) << setfill('-') << ""
         << "+" << setw(col+2) << setfill('-') << "" << "+" << endl;
    }

    void _printQueryDivider(){
        cout << left << setw(50) << setfill('_') << ""<<'\n';
    }

    void _printMhs(identitas mahasiswa) {
        cout 	<< "| " << left << setw(col) << setfill(space) << mahasiswa.nama
                << " | " << left << setw(col) << setfill(space) << mahasiswa.NIP
                << " |" << endl;
        return;
    };

};

void printWelcome() {
    cout << "==> PROGRAM CRUD MAHASISWA \n \n";
    cout << left << setw(10) << "Oleh" << " : " << setw(10) << "Wina Tungmiharja" << endl;
    cout << left << setw(10) << "NRP" << " : " << setw(10) << "5025201242" << endl;
    cout << left << setw(10) << "Kelas" << " : " << setw(10) << "Struktur Data H" << endl;
}

bool goBack() {
    char opt;
    cout << "Apakah anda ingin kembali ke Menu Utama? \n";
    cout << "(y/n)? ";
    cin>>opt;
    if(opt == 'y' || opt == 'n')
    {
        return (opt=='y')?true:false;
    }
    cout << "Input anda salah!" << endl;
    return goBack();
}

int main() {
    system("cls");
    printWelcome();
    while(true){
        CRUD data;
        switch(data.query()) {
        case 0:
            cout << "Input anda salah! \n";
            break;
        case 1:
            data.printData();
            break;
        case 2:
            data.addData();
            break;
        case 3:
            data.updateData();
            break;
        case 4:
            data.deleteData();
            break;
        case 5:
            if(data.exitProgram()) return 0;
            else{
                system("cls");
                continue;
            };
        default :
            cout << "opsi anda salah" << endl;
            break;
        }

        if(goBack())
        {
            system("cls");
            continue;
        }
        else
        {
            if(data.exitProgram()) return 0;
            else{
                system("cls");
                continue;
            };
        }
    }

    return 0;
}