#include <iostream>
#include <string.h>
#include <fstream>
#include <sstream>
#include <cmath>
using namespace std;


struct Notes
{
    string top[5];
    string middle[5];
    string base[5];
};

struct Parfum
{
    int id_parfum;
    string nama;
    string merek;
    string deskripsi;
    Notes notes;
    Parfum *next;
};

struct Review
{
    int id_review;
    int id_parfum;
    string username;
    int rating;
    string komentar;
    Review *next;
};

struct User
{
    int id_user;
    string username;
    string password;
    char role;
};

User user[100];
int jumlahuser = 0;
Parfum *HeadParfum = nullptr;
Parfum *TailParfum = nullptr;
int jumlahparfum = 0;
Review *top = nullptr;
Review *Headreview = nullptr;
Review *Tailreview = nullptr;
int jumlahreview = 0;

void splitNotes(const string &notes, string *arr, int maxSize);
void clearReviews();

bool inputkosong(const string &input)
{
    return !input.empty() && input.find_first_not_of(" \t\n\r\f\v") != string::npos;
}

void clearInputBuffer()
{
    cin.clear();
    cin.ignore();
}

int inputAngka()
{
    int angka;
    while (true)
    {
        if (cin >> angka)
        {
            clearInputBuffer();
            return angka;
        }
        else
        {
            cout << "Input tidak valid. Harap masukkan angka: ";
            clearInputBuffer();
        }
    }
}

void clear()
{
    system("cls");
}

void read_file_user()
{
    ifstream file("akun.csv", ios::in);
    if (!file.is_open())
    {
        cout << "file akun.csv tidak ditemukan" << endl;
        return;
    }

    string header;
    getline(file, header);

    jumlahuser = 0;
    string line;
    while (getline(file, line) && jumlahuser < 100)
    {
        stringstream ss(line);
        string temp;
        getline(ss, temp, ',');
        user[jumlahuser].id_user = stoi(temp);
        getline(ss, user[jumlahuser].username, ',');
        getline(ss, user[jumlahuser].password, ',');
        getline(ss, temp);
        user[jumlahuser].role = temp[0];

        jumlahuser++;
    }

    file.close();
}

void save_file_user()
{
    ofstream file("akun.csv", ios::out);
    if (!file.is_open())
    {
        cout << "Gagal membuka file akun.csv untuk menyimpan data" << endl;
        return;
    }

    file << "id_user,username,password,role" << endl;

    for (int i = 0; i < jumlahuser; i++)
    {
        file << user[i].id_user << ","
             << user[i].username << ","
             << user[i].password << ","
             << user[i].role << endl;
    }

    file.close();
}

void read_file_parfum()
{
    while (HeadParfum != nullptr)
    {
        Parfum *temp = HeadParfum;
        HeadParfum = HeadParfum->next;
        delete temp;
    }
    HeadParfum = nullptr;
    TailParfum = nullptr;
    jumlahparfum = 0;

    ifstream file("parfum.csv", ios::in);
    if (!file.is_open())
    {
        cout << "File parfum.csv tidak ditemukan" << endl;
        return;
    }

    string line;
    getline(file, line); 

    while (getline(file, line))
    {
        Parfum *newParfum = new Parfum();
        newParfum->next = nullptr;

        stringstream ss(line);
        string id_parfum, top_notes, middle_notes, base_notes;

        getline(ss, id_parfum, ',');
        newParfum->id_parfum = stoi(id_parfum);
        getline(ss, newParfum->nama, ',');
        getline(ss, newParfum->merek, ',');
        getline(ss, newParfum->deskripsi, ',');
        getline(ss, top_notes, ',');
        getline(ss, middle_notes, ',');
        getline(ss, base_notes, ',');

        splitNotes(top_notes, newParfum->notes.top, 5);
        splitNotes(middle_notes, newParfum->notes.middle, 5);
        splitNotes(base_notes, newParfum->notes.base, 5);

        if (HeadParfum == nullptr)
        {
            HeadParfum = newParfum;
            TailParfum = newParfum;
        }
        else
        {
            TailParfum->next = newParfum;
            TailParfum = newParfum;
        }
        jumlahparfum++;
    }
    file.close();
}

void save_file_parfum()
{
    ofstream file("parfum.csv", ios::out);
    if (!file.is_open())
    {
        cout << "Gagal membuka file parfum.csv untuk menyimpan data" << endl;
        return;
    }

    file << "id_parfum,nama,merek,deskripsi,top_notes,middle_notes,base_notes" << endl;

    Parfum *current = HeadParfum;
    while (current != nullptr)
    {
        file << current->id_parfum << ","
             << current->nama << ","
             << current->merek << ","
             << current->deskripsi << ",";

        for (int j = 0; j < 5; j++)
        {
            file << current->notes.top[j];
            if (j < 4)
                file << ";";
        }
        file << ",";

        for (int j = 0; j < 5; j++)
        {
            file << current->notes.middle[j];
            if (j < 4)
                file << ";";
        }
        file << ",";

        for (int j = 0; j < 5; j++)
        {
            file << current->notes.base[j];
            if (j < 4)
                file << ";";
        }
        file << endl;

        current = current->next;
    }

    file.close();
}

void read_file_review()
{
    jumlahreview = 0;
    clearReviews();

    ifstream file("review.csv", ios::in);
    if (!file.is_open())
    {
        cout << "File review.csv tidak ditemukan" << endl;
        return;
    }

    string header;
    getline(file, header);

    string line;
    while (getline(file, line))
    {
        stringstream ss(line);
        Review *newReview = new Review();

        string temp;

        getline(ss, temp, ',');
        newReview->id_review = stoi(temp);
        getline(ss, temp, ',');
        newReview->id_parfum = stoi(temp);
        getline(ss, newReview->username, ',');
        getline(ss, temp, ',');
        newReview->rating = stoi(temp);
        getline(ss, newReview->komentar);

        newReview->next = top;
        top = newReview;

        jumlahreview++;
    }

    file.close();
}

void save_file_review()
{
    ofstream file("review.csv");
    if (!file.is_open())
    {
        cout << "Gagal membuka file review.csv" << endl;
        return;
    }

    file << "id_review,id_parfum,username,rating,komentar\n";

    Review *current = top;
    while (current != nullptr)
    {
        file << current->id_review << ","
             << current->id_parfum << ","
             << current->username << ","
             << current->rating << ","
             << current->komentar << "\n";
        current = current->next;
    }

    file.close();
    cout << "Data review berhasil disimpan!" << endl;
}

void clearReviews()
{
    while (top != nullptr)
    {
        Review *temp = top;
        top = top->next;
        delete temp;
    }
}

void splitNotes(const string &notes, string *arr, int maxjumlahnotes)
{
    stringstream ss(notes);
    string note;
    int i = 0;

    while (getline(ss, note, ';') && i < maxjumlahnotes)
    {
        arr[i++] = note;
    }
}

void tampilkannotes(const string notes[], int size, const string &label)
{
    cout << label << ": ";
    for (int i = 0; i < size; i++)
    {
        if (!notes[i].empty())
            cout << notes[i] << " ";
    }
    cout << endl;
}

void shellSortParfum()
{
    int n = jumlahparfum;

    Parfum **arr = new Parfum *[n];
    Parfum *current = HeadParfum;
    for (int i = 0; i < n; i++)
    {
        arr[i] = current;
        current = current->next;
    }

    for (int gap = n / 2; gap > 0; gap /= 2)
    {
        for (int i = gap; i < n; i++)
        {
            Parfum *temp = arr[i];
            int j = i;

            while (j >= gap && arr[j - gap]->nama.compare(temp->nama) > 0)
            {
                arr[j] = arr[j - gap];
                j -= gap;
            }
            arr[j] = temp;
        }
    }

    HeadParfum = arr[0];
    current = HeadParfum;
    for (int i = 1; i < n; i++)
    {
        current->next = arr[i];
        current = current->next;
    }
    current->next = nullptr;
    TailParfum = current;

    delete[] arr;
}

void mergeParfum(Parfum arr[], int left, int mid, int right)
{
    int n1 = mid - left + 1;
    int n2 = right - mid;

    Parfum *leftArr = new Parfum[n1];
    Parfum *rightArr = new Parfum[n2];

    for (int i = 0; i < n1; i++)
    {
        leftArr[i] = arr[left + i];
    }
    for (int i = 0; i < n2; i++)
    {
        rightArr[i] = arr[mid + 1 + i];
    }

    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2)
    {
        if (leftArr[i].nama > rightArr[j].nama)
        {
            arr[k] = leftArr[i];
            i++;
        }
        else
        {
            arr[k] = rightArr[j];
            j++;
        }
        k++;
    }

    while (i < n1)
    {
        arr[k] = leftArr[i];
        i++;
        k++;
    }

    while (j < n2)
    {
        arr[k] = rightArr[j];
        j++;
        k++;
    }

    delete[] leftArr;
    delete[] rightArr;
}

// Function untuk membagi linked list
Parfum *findMiddle(Parfum *head)
{
    if (head == nullptr || head->next == nullptr)
    {
        return head;
    }

    Parfum *slow = head;
    Parfum *fast = head->next;

    while (fast != nullptr && fast->next != nullptr)
    {
        slow = slow->next;
        fast = fast->next->next;
    }

    return slow;
}

Parfum *merge(Parfum *left, Parfum *right)
{
    if (left == nullptr)
        return right;
    if (right == nullptr)
        return left;

    Parfum *result = nullptr;

    if (left->nama >= right->nama)
    {
        result = left;
        result->next = merge(left->next, right);
    }
    else
    {
        result = right;
        result->next = merge(left, right->next);
    }

    return result;
}

Parfum *mergeSortParfum(Parfum *head)
{
    if (head == nullptr || head->next == nullptr)
    {
        return head;
    }

    Parfum *middle = findMiddle(head);
    Parfum *nextToMiddle = middle->next;

    middle->next = nullptr;

    Parfum *left = mergeSortParfum(head);
    Parfum *right = mergeSortParfum(nextToMiddle);

    return merge(left, right);
}

void daftarParfumDesc()
{
    read_file_parfum();

    HeadParfum = mergeSortParfum(HeadParfum);

    TailParfum = HeadParfum;
    while (TailParfum != nullptr && TailParfum->next != nullptr)
    {
        TailParfum = TailParfum->next;
    }

    Parfum *current = HeadParfum;
    while (current != nullptr)
    {
        cout << "ID Parfum: " << current->id_parfum << endl;
        cout << "Nama: " << current->nama << endl;
        cout << "Merek: " << current->merek << endl;
        cout << "Deskripsi: " << current->deskripsi << endl;

        tampilkannotes(current->notes.top, 5, "Top Notes");
        tampilkannotes(current->notes.middle, 5, "Middle Notes");
        tampilkannotes(current->notes.base, 5, "Base Notes");

        cout << endl;
        current = current->next;
    }
}

void lihat_data_parfum()
{
    read_file_parfum();
    shellSortParfum();

    Parfum *current = HeadParfum;
    while (current != nullptr)
    {
        cout << "ID Parfum: " << current->id_parfum << endl;
        cout << "Nama: " << current->nama << endl;
        cout << "Merek: " << current->merek << endl;
        cout << "Deskripsi: " << current->deskripsi << endl;

        tampilkannotes(current->notes.top, 5, "Top Notes");
        tampilkannotes(current->notes.middle, 5, "Middle Notes");
        tampilkannotes(current->notes.base, 5, "Base Notes");

        cout << endl;
        current = current->next;
    }
}

void enqueue(Parfum *&head, int id, string nama, string merek, string deskripsi, Notes notes)
{
    Parfum *newParfum = new Parfum();
    newParfum->id_parfum = id;
    newParfum->nama = nama;
    newParfum->merek = merek;
    newParfum->deskripsi = deskripsi;
    newParfum->notes = notes;
    newParfum->next = nullptr;

    if (head == nullptr)
    {
        head = newParfum;
        TailParfum = newParfum;
    }
    else
    {
        TailParfum->next = newParfum;
        TailParfum = newParfum;
    }

    cout << "\nData parfum yang baru anda tambah:\n";
    cout << "ID: " << newParfum->id_parfum << endl;
    cout << "Nama: " << newParfum->nama << endl;
    cout << "Merek: " << newParfum->merek << endl;
    cout << "Deskripsi: " << newParfum->deskripsi << endl;
    cout << "Top Notes: ";
    for (int i = 0; i < 5 && !newParfum->notes.top[i].empty(); i++)
    {
        cout << newParfum->notes.top[i] << " ";
    }
    cout << "\nMiddle Notes: ";
    for (int i = 0; i < 5 && !newParfum->notes.middle[i].empty(); i++)
    {
        cout << newParfum->notes.middle[i] << " ";
    }
    cout << "\nBase Notes: ";
    for (int i = 0; i < 5 && !newParfum->notes.base[i].empty(); i++)
    {
        cout << newParfum->notes.base[i] << " ";
    }
    cout << "\n\n";
}

void tambahparfum()
{
    read_file_parfum();
    clear();

    int berapa;
    cout << "Tambah Data Parfum." << endl;
    cout << "Berapa data yang ingin ditambah: ";
    berapa = inputAngka();
    clear();
    int maxIdParfum = 0;
    Parfum *current = HeadParfum;
    while (current != nullptr)
    {
        if (current->id_parfum > maxIdParfum)
        {
            maxIdParfum = current->id_parfum;
        }
        current = current->next;
    }

    for (int i = 0; i < berapa; i++)
    {
        cout << "Data ke-" << i + 1 << ":" << endl;

        Parfum *newParfum = new Parfum();
        newParfum->next = nullptr;

        newParfum->id_parfum = maxIdParfum + 1;
        maxIdParfum++;

        cout << "Nama Parfum: ";
        getline(cin, newParfum->nama);

        cout << "Brand: ";
        getline(cin, newParfum->merek);

        cout << "Deskripsi: ";
        getline(cin, newParfum->deskripsi);

        cout << "Masukkan hingga 5 Top Notes (tekan Enter untuk berhenti): " << endl;
        for (int j = 0; j < 5; j++)
        {
            cout << "Top Note " << j + 1 << ": ";
            getline(cin, newParfum->notes.top[j]);
            if (newParfum->notes.top[j].empty())
            {
                for (int k = j; k < 5; k++)
                {
                    newParfum->notes.top[k] = "";
                }
                break;
            }
        }

        cout << "Masukkan hingga 5 Middle Notes (tekan Enter untuk berhenti): " << endl;
        for (int j = 0; j < 5; j++)
        {
            cout << "Middle Note " << j + 1 << ": ";
            getline(cin, newParfum->notes.middle[j]);
            if (newParfum->notes.middle[j].empty())
            {
                for (int k = j; k < 5; k++)
                {
                    newParfum->notes.middle[k] = "";
                }
                break;
            }
        }

        cout << "Masukkan hingga 5 Base Notes (tekan Enter untuk berhenti): " << endl;
        for (int j = 0; j < 5; j++)
        {
            cout << "Base Note " << j + 1 << ": ";
            getline(cin, newParfum->notes.base[j]);
            if (newParfum->notes.base[j].empty())
            {
                for (int k = j; k < 5; k++)
                {
                    newParfum->notes.base[k] = "";
                }
                break;
            }
        }

        enqueue(HeadParfum, newParfum->id_parfum, newParfum->nama, newParfum->merek, newParfum->deskripsi, newParfum->notes);

        jumlahparfum++;
        save_file_parfum();
    }
}

void dequeue(Parfum *&head)
{
    clear();
    read_file_parfum();
    if (head == nullptr)
    {
        cout << "Tidak ada parfum untuk dihapus." << endl;
        return;
    }

    Parfum *temp = head;
    head = head->next; 
    cout << "Parfum dengan ID " << temp->id_parfum << " telah dihapus." << endl;

    delete temp;
    jumlahparfum--;
    save_file_parfum();
}

void ubahparfum()
{
    clear();
    read_file_parfum();

    if (HeadParfum != nullptr)
    {
        lihat_data_parfum();
        int pilih;

        cout << "Pilih id data yang ingin diubah: ";
        pilih = inputAngka();

        Parfum *current = HeadParfum;
        bool ketemu = false;

        while (current != nullptr)
        {
            if (current->id_parfum == pilih)
            {
                ketemu = true;
                break;
            }
            current = current->next;
        }

        if (ketemu)
        {
            cout << "\nMengubah data parfum dengan ID " << pilih << endl;

            cout << "Nama Parfum: ";
            getline(cin, current->nama);

            cout << "Brand: ";
            getline(cin, current->merek);

            cout << "Deskripsi: ";
            getline(cin, current->deskripsi);

            for (int j = 0; j < 5; j++)
            {
                current->notes.top[j] = "";
                current->notes.middle[j] = "";
                current->notes.base[j] = "";
            }

            cout << "Masukkan hingga 5 Top Notes (tekan Enter untuk berhenti): " << endl;
            for (int j = 0; j < 5; j++)
            {
                cout << "Top Note " << j + 1 << ": ";
                getline(cin, current->notes.top[j]);
                if (current->notes.top[j].empty())
                    break;
            }

            cout << "Masukkan hingga 5 Middle Notes (tekan Enter untuk berhenti): " << endl;
            for (int j = 0; j < 5; j++)
            {
                cout << "Middle Note " << j + 1 << ": ";
                getline(cin, current->notes.middle[j]);
                if (current->notes.middle[j].empty())
                    break;
            }

            cout << "Masukkan hingga 5 Base Notes (tekan Enter untuk berhenti): " << endl;
            for (int j = 0; j < 5; j++)
            {
                cout << "Base Note " << j + 1 << ": ";
                getline(cin, current->notes.base[j]);
                if (current->notes.base[j].empty())
                    break;
            }

            save_file_parfum();
            cout << "\nData parfum berhasil diubah!" << endl;
        }
        else
        {
            cout << "ID parfum tidak ditemukan!" << endl;
        }
    }
    else
    {
        cout << "Data Kosong!" << endl;
    }
}

void shellSortIdParfum()
{
    int n = jumlahparfum;

    Parfum **arr = new Parfum *[n];
    Parfum *current = HeadParfum;

    for (int i = 0; i < n; i++)
    {
        arr[i] = current;
        current = current->next;
    }

    for (int gap = n / 2; gap > 0; gap /= 2)
    {
        for (int i = gap; i < n; i++)
        {
            Parfum *temp = arr[i];
            int j = i;

            while (j >= gap && arr[j - gap]->id_parfum > temp->id_parfum)
            {
                arr[j] = arr[j - gap];
                j -= gap;
            }
            arr[j] = temp;
        }
    }

    HeadParfum = arr[0];
    current = HeadParfum;
    for (int i = 1; i < n; i++)
    {
        current->next = arr[i];
        current = current->next;
    }
    current->next = nullptr; 
    TailParfum = current;

    delete[] arr;
}

void hapusparfum()
{
    clear();
    read_file_parfum();

    if (HeadParfum != nullptr)
    {
        lihat_data_parfum();
        int pilihhapus;

        cout << "Pilih ID parfum yang ingin dihapus: ";
        pilihhapus = inputAngka();

        Parfum *current = HeadParfum;
        Parfum *prev = nullptr;
        bool ketemu = false;

        while (current != nullptr)
        {
            if (current->id_parfum == pilihhapus)
            {
                ketemu = true;
                break;
            }
            prev = current;
            current = current->next;
        }

        if (ketemu)
        {
            if (prev == nullptr)
            {
                HeadParfum = current->next;
                if (HeadParfum == nullptr)
                {
                    TailParfum = nullptr;
                }
            }
            else
            {
                prev->next = current->next;
                if (current->next == nullptr)
                {
                    TailParfum = prev; 
                }
            }

            delete current;
            jumlahparfum--;
            shellSortIdParfum();
            save_file_parfum();

            cout << "Parfum dengan ID " << pilihhapus << " berhasil dihapus!" << endl;
        }
        else
        {
            cout << "ID parfum tidak ditemukan!" << endl;
        }
    }
    else
    {
        cout << "Data Kosong!" << endl;
    }
}
void splitList(Review *source, Review **frontRef, Review **backRef)
{
    Review *fast;
    Review *slow;
    slow = source;
    fast = source->next;

    while (fast != nullptr)
    {
        fast = fast->next;
        if (fast != nullptr)
        {
            slow = slow->next;
            fast = fast->next;
        }
    }

    *frontRef = source;
    *backRef = slow->next;
    slow->next = nullptr;
}

Review *sortedMerge(Review *a, Review *b, bool ascending)
{
    Review *result = nullptr;

    if (a == nullptr)
        return b;
    else if (b == nullptr)
        return a;

    if ((ascending && a->id_review <= b->id_review) || (!ascending && a->id_review > b->id_review))
    {
        result = a;
        result->next = sortedMerge(a->next, b, ascending);
    }
    else
    {
        result = b;
        result->next = sortedMerge(a, b->next, ascending);
    }

    return result;
}

Review *mergeSort(Review *head, bool ascending)
{
    if (head == nullptr || head->next == nullptr)
    {
        return head;
    }

    Review *a;
    Review *b;
    splitList(head, &a, &b);

    a = mergeSort(a, ascending);
    b = mergeSort(b, ascending);

    return sortedMerge(a, b, ascending);
}

void lihat_data_review()
{
    int pilihParfum, urutan;
    read_file_parfum();
    read_file_review();
    clear();

    lihat_data_parfum();
    if (HeadParfum == nullptr)
    {
        cout << "Tidak ada parfum yang tersedia!" << endl;
        return;
    }

    cout << "\nMasukkan ID parfum untuk melihat review: ";
    cin >> pilihParfum;
    cin.ignore();

    while (cin.fail())
    {
        cout << "Input harus berupa angka!" << endl;
        cin.clear();
        cin.ignore();
        cout << "Masukkan ID parfum untuk melihat review: ";
        cin >> pilihParfum;
        cin.ignore();
    }

    cout << "\nPilih Urutan Review:\n";
    cout << "1. Urutkan Ascending \n";
    cout << "2. Urutkan Descending \n";
    cout << "Pilih Urutan: ";
    cin >> urutan;
    cin.ignore();

    if (urutan == 1)
    {
        cout << "\n==== Daftar Review (Ascending) ====\n";
        top = mergeSort(top, true);
    }
    else if (urutan == 2)
    {
        cout << "\n==== Daftar Review (Descending) ====\n";
        top = mergeSort(top, false);
    }
    else
    {
        cout << "Pilihan tidak valid.\n";
        return;
    }

    bool ketemu = false;
    Parfum *current = HeadParfum;

    while (current != nullptr)
    {
        if (current->id_parfum == pilihParfum)
        {
            ketemu = true;

            cout << "======== Review untuk " << current->nama << " ========\n";

            bool adareview = false;
            Review *reviewCurrent = top;
            while (reviewCurrent != nullptr)
            {
                if (reviewCurrent->id_parfum == pilihParfum)
                {
                    adareview = true;
                    cout << "ID Review: " << reviewCurrent->id_review << endl;
                    cout << "User: " << reviewCurrent->username << endl;
                    cout << "Rating: " << reviewCurrent->rating << "/5" << endl;
                    cout << "Komentar: " << reviewCurrent->komentar << endl;
                    cout << "--------------------------------\n";
                }
                reviewCurrent = reviewCurrent->next;
            }

            if (!adareview)
            {
                cout << "Belum ada review untuk parfum ini.\n";
            }

            break;
        }
        current = current->next;
    }

    if (!ketemu)
    {
        cout << "Parfum dengan ID " << pilihParfum << " tidak ditemukan!\n";
    }
}

void tambahreview(const string &username)
{
    clear();
    read_file_review();
    lihat_data_parfum();

    Review *newReview = new Review();
    int pilihParfum;

    cout << "Pilih parfum untuk direview (ID): ";
    pilihParfum = inputAngka();

    bool ketemu = false;
    Parfum *current = HeadParfum;

    while (current != nullptr)
    {
        if (current->id_parfum == pilihParfum)
        {
            ketemu = true;

            int maxIdReview = 0;
            Review *reviewCurrent = top;
            while (reviewCurrent != nullptr)
            {
                if (reviewCurrent->id_review > maxIdReview)
                {
                    maxIdReview = reviewCurrent->id_review;
                }
                reviewCurrent = reviewCurrent->next;
            }

            newReview->id_review = maxIdReview + 1;
            newReview->id_parfum = pilihParfum;
            newReview->username = username;

            do
            {
                cout << "Berikan rating (1-5): ";
                cin >> newReview->rating;
                cin.ignore();

                if (cin.fail() || newReview->rating < 1 || newReview->rating > 5)
                {
                    cout << "Rating harus berupa angka 1-5!\n";
                    cin.clear();
                    cin.ignore();
                }
                else
                {
                    break;
                }
            } while (true);

            do
            {
                cout << "Masukkan komentar review: ";
                getline(cin, newReview->komentar);

                if (!inputkosong(newReview->komentar))
                {
                    cout << "Komentar tidak boleh kosong! Silakan coba lagi.\n";
                }
                else
                {
                    break;
                }
            } while (!inputkosong(newReview->komentar));

            newReview->next = top;
            top = newReview;

            jumlahreview++;
            save_file_review();
            cout << "Review berhasil ditambahkan!" << endl;

            break;
        }
        current = current->next;
    }

    if (!ketemu)
    {
        cout << "Parfum dengan ID " << pilihParfum << " tidak ditemukan!\n";
        delete newReview;
    }
}

void ubahreview(int id_review)
{
    clear();
    read_file_review();

    Review *current = Headreview;
    bool ketemu = false;
    while (current != nullptr)
    {
        if (current->id_review == id_review)
        {
            ketemu = true;
            cout << "Review ditemukan. Silakan ubah data:\n";
            cout << "Rating lama: " << current->rating << "\nMasukkan rating baru (1-5): ";
            cin >> current->rating;
            cin.ignore();

            cout << "Komentar lama: " << current->komentar << endl;
            do
            {
                cout << "Masukkan komentar baru: ";
                getline(cin, current->komentar);
                if (!inputkosong(current->komentar))
                {
                    cout << "Komentar tidak boleh kosong! Silakan coba lagi.\n";
                }
            } while (!inputkosong(current->komentar));
            cout << "Review berhasil diubah!\n";
            save_file_review();
            break;
        }
        current = current->next;
    }

    if (!ketemu)
    {
        cout << "ID review " << id_review << " tidak ditemukan!\n";
    }
}

void tampilkanreviewuser(const string &username)
{
    read_file_review();

    bool ketemu = false;
    cout << "Review oleh " << username << ":\n";
    int no = 1;

    top = mergeSort(top, false);

    Review *temp = top;
    while (temp != nullptr)
    {
        if (temp->username == username)
        {
            cout << no << ". ID Review: " << temp->id_review
                 << ", Rating: " << temp->rating
                 << ", Komentar: " << temp->komentar << endl;
            ketemu = true;
            no++;
        }
        temp = temp->next;
    }

    if (!ketemu)
    {
        cout << "Anda belum membuat review.\n";
    }
}

void editreview(const string &username)
{
    read_file_review();

    tampilkanreviewuser(username);

    int pilih;
    cout << "Pilih nomor review yang ingin diedit (0 untuk batal): ";
    pilih = inputAngka();

    if (pilih == 0)
    {
        cout << "Batal mengedit review.\n";
        return;
    }

    Review *temp = top;
    int no = 1;
    bool ketemu = false;

    while (temp != nullptr)
    {
        if (temp->username == username)
        {
            if (no == pilih)
            {
                ketemu = true;

                string komentarBaru;
                int ratingBaru;

                do
                {
                    cout << "Rating lama: " << temp->rating << "\nMasukkan rating baru (1-5): ";
                    cin >> ratingBaru;
                    cin.ignore();

                    if (cin.fail() || ratingBaru < 1 || ratingBaru > 5)
                    {
                        cout << "Rating harus berupa angka 1-5!\n";
                        cin.clear();
                        cin.ignore();
                    }
                    else
                    {
                        break;
                    }
                } while (true);

                temp->rating = ratingBaru;

                do
                {
                    cout << "Komentar lama: " << temp->komentar << "\nMasukkan komentar baru: ";
                    getline(cin, komentarBaru);

                    if (!inputkosong(komentarBaru))
                    {
                        cout << "Komentar tidak boleh kosong! Silakan coba lagi.\n";
                    }
                    else
                    {
                        break;
                    }
                } while (true);

                temp->komentar = komentarBaru;

                cout << "Review berhasil diperbarui!\n";
                save_file_review();
                return;
            }
            no++;
        }
        temp = temp->next;
    }

    if (!ketemu)
    {
        cout << "Review tidak ditemukan.\n";
    }
}

void hapusreviewStack(const string &username)
{
    read_file_parfum();
    tampilkanreviewuser(username);

    if (top == nullptr)
    {
        cout << "Tidak ada review yang dapat dihapus!" << endl;
        return;
    }

    char konfirmasi;
    cout << "Yakin ingin menghapus review terbaru? (y/n): ";
    cin >> konfirmasi;

    if (konfirmasi == 'y' || konfirmasi == 'Y')
    {
        Review *reviewToDelete = top;
        top = top->next;

        delete reviewToDelete;
        jumlahreview--;

        save_file_review();
        cout << "Review terbaru berhasil dihapus!" << endl;
    }
    else
    {
        cout << "Penghapusan dibatalkan." << endl;
    }
}

void hapusreview(const string &username)
{
    read_file_review();
    tampilkanreviewuser(username);

    if (top == nullptr)
    {
        cout << "Tidak ada review untuk dihapus!" << endl;
        return;
    }

    int pilihhapus;
    cout << "Pilih ID review yang ingin dihapus: ";
    pilihhapus = inputAngka();

    Review *current = top;
    Review *previous = nullptr;

    while (current != nullptr)
    {
        if (current->id_review == pilihhapus)
        {
            if (previous == nullptr)
            {
                top = current->next;
            }
            else
            {
                previous->next = current->next;
            }

            delete current;
            jumlahreview--;

            save_file_review();
            cout << "Review berhasil dihapus!" << endl;
            return;
        }

        previous = current;
        current = current->next;
    }

    cout << "Review dengan ID tersebut tidak ditemukan!" << endl;
}

void hapusreviewAdmin()
{
    lihat_data_review();

    if (top == nullptr)
    {
        cout << "Tidak ada review untuk dihapus!" << endl;
        return;
    }

    int pilihhapus;

    cout << "Pilih ID review yang ingin dihapus: ";
    cin >> pilihhapus;

    Review *current = top;
    Review *previous = nullptr;

    while (current != nullptr)
    {
        if (current->id_review == pilihhapus)
        {
            if (previous == nullptr)
            {
                top = current->next;
            }
            else
            {
                previous->next = current->next;
            }

            delete current;
            jumlahreview--;

            save_file_review();
            cout << "Review berhasil dihapus!" << endl;
            return;
        }

        previous = current;
        current = current->next;
    }

    cout << "Review dengan ID tersebut tidak ditemukan!" << endl;
}

bool cekUsername(const string &username)
{
    for (int i = 0; i < jumlahuser; i++)
    {
        if (username == user[i].username)
        {
            return true;
        }
    }
    return false;
}

void registrasi()
{
    string username, password;

    cout << "======== Registrasi User ========\n";
    do
    {
        cout << "Masukkan Username: ";
        getline(cin, username);
        if (!inputkosong(username))
        {
            cout << "Username tidak boleh kosong atau hanya berisi spasi!" << endl;
        }
        else if (cekUsername(username))
        {
            cout << "Username sudah terpakai, coba gunakan username lainnya" << endl;
        }
    } while (!inputkosong(username) || cekUsername(username));

    do
    {
        cout << "Masukkan Password: ";
        getline(cin, password);
        if (!inputkosong(password))
        {
            cout << "Password tidak boleh kosong atau hanya berisi spasi!\n";
        }
    } while (!inputkosong(password));

    user[jumlahuser].id_user = jumlahuser + 1;
    user[jumlahuser].username = username;
    user[jumlahuser].password = password;
    user[jumlahuser].role = 'u';

    jumlahuser++;

    save_file_user();
}

User *login()
{
    string username, password;
    int coba = 0;
    read_file_user();

    do
    {
        cout << "==== Login ====\n";
        cout << "Masukkan username: ";
        getline(cin, username);
        cout << "Masukkan password: ";
        getline(cin, password);

        for (int i = 0; i < jumlahuser; i++)
        {
            if (user[i].username == username && user[i].password == password)
            {
                cout << "Login berhasil!\n";
                return &user[i];
            }
        }

        cout << "Login gagal. Username atau password salah.\n";
        coba++;
    } while (coba < 3);

    cout << "Anda telah gagal login 3 kali, program akan berhenti!\n";
    exit(0);
}

const int NO_OF_CHARS = 256;

void badCharHeuristic(string str, int size, int badchar[NO_OF_CHARS])
{
    for (int i = 0; i < NO_OF_CHARS; i++)
    {
        badchar[i] = -1;
    }

    for (int i = 0; i < size; i++)
    {
        badchar[(int)str[i]] = i;
    }
}

int searchBoyerMoore(string pat, string txt)
{
    int M = pat.length(); 
    int N = txt.length(); 

    int badchar[NO_OF_CHARS] = {-1};
    badCharHeuristic(pat, M, badchar);

    int s = 0;

    while (s <= (N - M))
    {
        int j = M - 1;

        while (j >= 0 && pat[j] == txt[s + j])
            j--;

        if (j < 0)
        {
            return s;
            s += (s + M < N) ? M - badchar[txt[s + M]] : 1;
        }
        else
        {
            s += max(1, j - badchar[txt[s + j]]);
        }
    }

    return -1;
}

Parfum *searching_data(const string &nama)
{
    Parfum *current = HeadParfum;

    while (current != nullptr)
    {
        if (searchBoyerMoore(current->nama, nama) != -1)
        {
            return current;
        }
        current = current->next;
    }
    return nullptr; 
}

void menu_cari_parfum()
{
    string masuk;
    clear();
    read_file_parfum();
    shellSortParfum();

    cout << "===================================" << endl;
    cout << "|          CARI PARFUM            |" << endl;
    cout << "===================================" << endl;
    cout << " Nama parfum : ";

    getline(cin, masuk);
    fflush(stdin);

    Parfum *hasil = searching_data(masuk);

    if (hasil == nullptr)
    {
        clear();
        cout << "| =============================== |" << endl;
        cout << "| ==  Parfum tidak ditemukan   == |" << endl;
        cout << "| =============================== |" << endl;
    }
    else
    {
        clear();
        cout << "========================" << endl;
        cout << hasil->nama << endl;
        cout << "========================" << endl;
        cout << " Merk     : " << hasil->merek << endl;
        cout << " Deskripsi : " << hasil->deskripsi << endl;

        cout << " Top Notes : ";
        for (int i = 0; i < 5; i++)
        {
            if (!hasil->notes.top[i].empty())
            {
                cout << hasil->notes.top[i];
                if (i < 4 && !hasil->notes.top[i + 1].empty())
                    cout << ", ";
            }
        }
        cout << endl;

        cout << " Middle Notes : ";
        for (int i = 0; i < 5; i++)
        {
            if (!hasil->notes.middle[i].empty())
            {
                cout << hasil->notes.middle[i];
                if (i < 4 && !hasil->notes.middle[i + 1].empty())
                    cout << ", ";
            }
        }
        cout << endl;

        cout << " Base Notes : ";
        for (int i = 0; i < 5; i++)
        {
            if (!hasil->notes.base[i].empty())
            {
                cout << hasil->notes.base[i];
                if (i < 4 && !hasil->notes.base[i + 1].empty())
                    cout << ", ";
            }
        }
        cout << endl;
        cout << "========================" << endl;
    }
}

void sortUsersByUsername()
{
    for (int i = 0; i < jumlahuser - 1; i++)
    {
        for (int j = 0; j < jumlahuser - i - 1; j++)
        {
            if (user[j].username > user[j + 1].username)
            {
                User temp = user[j];
                user[j] = user[j + 1];
                user[j + 1] = temp;
            }
        }
    }
}

int jumpSearchByUsername(string target)
{
    int step = sqrt(jumlahuser); 
    int prev = 0;

    while (user[min(step, jumlahuser) - 1].username < target)
    {
        prev = step;
        step += sqrt(jumlahuser);
        if (prev >= jumlahuser)
            return -1;
    }

    for (int i = prev; i < min(step, jumlahuser); i++)
    {
        if (user[i].username == target)
        {
            return i; 
        }
    }

    return -1;
}

void menu_cari_user()
{
    clear();

    string username;
    read_file_user();
    sortUsersByUsername();

    cout << "===================================" << endl;
    cout << "|           CARI USER             |" << endl;
    cout << "===================================" << endl;
    cout << " Username User : ";

    getline(cin, username);
    fflush(stdin);

    int dapat = jumpSearchByUsername(username);

    if (dapat == -1)
    {
        clear();
        cout << "| =============================== |" << endl;
        cout << "| ==    User tidak ditemukan   == |" << endl;
        cout << "| =============================== |" << endl;
    }
    else
    {
        clear();
        cout << "========================" << endl;
        cout << user[dapat].username << endl;
        cout << "========================" << endl;
        cout << " ID User     : " << user[dapat].id_user << endl;
        cout << " Role : " << user[dapat].role << endl;
        cout << "========================" << endl;
    }
}

Review *getReviewByIndex(int index)
{
    Review *current = top;
    int count = 0;
    while (current != nullptr && count < index)
    {
        current = current->next;
        count++;
    }
    return current;
}

Review *fibonacciSearchByRating(int rating)
{
    if (top == nullptr)
        return nullptr;

    int fibMMm2 = 0;              
    int fibMMm1 = 1;              
    int fibM = fibMMm2 + fibMMm1;

    while (fibM < jumlahreview)
    {
        fibMMm2 = fibMMm1;
        fibMMm1 = fibM;
        fibM = fibMMm2 + fibMMm1;
    }

    int offset = -1; 

    while (fibM > 1)
    {
        int i = min(offset + fibMMm2, jumlahreview - 1);
        Review *current = getReviewByIndex(i);

        if (current == nullptr)
            break;

        if (current->rating < rating)
        {
            fibM = fibMMm1;
            fibMMm1 = fibMMm2;
            fibMMm2 = fibM - fibMMm1;
            offset = i;
        }
        else if (current->rating > rating)
        {

            fibM = fibMMm2;
            fibMMm1 = fibMMm1 - fibMMm2;
            fibMMm2 = fibM - fibMMm1;
        }
        else
        {
            return current;
        }
    }

    Review *lastReview = getReviewByIndex(offset + 1);
    if (lastReview && lastReview->rating == rating)
    {
        return lastReview;
    }

    return nullptr;
}

void menu_cari_review()
{
    int rating;
    read_file_review();
    cout << "===================================" << endl;
    cout << "|       CARI BERDASARKAN RATING   |" << endl;
    cout << "===================================" << endl;
    cout << "Masukkan Rating (1-5): ";
    cin >> rating;
    cin.ignore();

    if (rating < 1 || rating > 5)
    {
        cout << "Rating tidak valid!" << endl;
        return;
    }

    Review *result = fibonacciSearchByRating(rating);

    if (result == nullptr)
    {
        cout << "| =============================== |" << endl;
        cout << "| ==  Review tidak ditemukan   == |" << endl;
        cout << "| =============================== |" << endl;
    }
    else
    {
        cout << "========================" << endl;
        cout << "ID Review: " << result->id_review << endl;
        cout << "Username: " << result->username << endl;
        cout << "Rating: " << result->rating << endl;
        cout << "Komentar: " << result->komentar << endl;
        cout << "========================" << endl;
    }
}

void menuadmin()
{
    int pilihan, pilih, pilihhapus;

    do
    {
        cout << "======== Menu Admin ========\n";
        cout << "1. Lihat Daftar Parfum.\n";
        cout << "2. Tambah Parfum.\n";
        cout << "3. Edit Parfum.\n";
        cout << "4. Hapus Parfum\n";
        cout << "5. Cari Data\n";
        cout << "6. Lihat Review Parfum\n";
        cout << "7. Hapus Review User\n";
        cout << "0. Logout\n";
        cout << "Pilih Menu: ";
        pilihan = inputAngka();

        switch (pilihan)
        {
        case 1:
        {
            cout << "========= Daftar Parfum ========\n";
            cout << "1. Lihat Daftar Terurut berdasarkan Nama(A-Z).\n";
            cout << "2. Lihat Daftar Terurut berdasarkan Nama(Z-A)\n";
            cout << "Pilih: ";
            pilih = inputAngka();

            if (pilih == 1)
            {
                lihat_data_parfum();
                break;
            }
            else if (pilih == 2)
            {
                daftarParfumDesc();
                break;
            }
            else
            {
                cout << "Pilihan tidak valid." << endl;
            }
        }

        case 2:
        {
            cout << "\n==== Tambah Parfum ====\n";
            tambahparfum();
            break;
        }

        case 3:
        {
            ubahparfum();
            break;
        }

        case 4:
        {
            int menuHapus;
            cout << "\n==== Menu Hapus ====\n";
            cout << "1. Hapus Parfum Berdasarkan ID\n";
            cout << "2. Hapus Parfum Terlama\n";
            cout << "Pilih Pencarian: ";
            menuHapus = inputAngka();

            switch (menuHapus)
            {
            case 1:
                hapusparfum();
                break;
            case 2:
                dequeue(HeadParfum);
                break;
            default:
                cout << "Pilihan tidak valid!" << endl;
            }
            break;
        }

        case 5:
        {
            int menuCari;
            cout << "\n==== Menu Pencarian ====\n";
            cout << "1. Cari Berdasarkan Username\n";
            cout << "2. Cari Berdasarkan Nama Parfum\n";
            cout << "3. Cari Berdasarkan Rating Ulasan\n";
            cout << "Pilih Pencarian: ";
            menuCari = inputAngka();

            switch (menuCari)
            {
            case 1:
                menu_cari_user();
                break;
            case 2:
                menu_cari_parfum();
                break;
            case 3:
                menu_cari_review();
                break;
            default:
                cout << "Pilihan tidak valid!" << endl;
            }
            break;
        }

        case 6:
        {
            lihat_data_review();
            break;
        }

        case 7:
        {
            hapusreviewAdmin();
        }

        case 0:
        {
            cout << "Logout berhasil!\n";
            break;
        }

        default:
        {
            cout << "Pilihan tidak valid!\n";
            break;
        }
        }

    } while (pilihan != 0);
}

void menuuser(User *userLogin)
{
    int pilihan, pilih, pilihhapus;

    do
    {
        cout << "======== Menu User ========\n";
        cout << "1. Lihat Daftar Parfum.\n";
        cout << "2. Cari Parfum\n";
        cout << "3. Lihat Review\n";
        cout << "4. Lihat Review Anda\n";
        cout << "5. Tambah Review\n";
        cout << "6. Edit Review\n";
        cout << "7. Hapus Review\n";
        cout << "0. Logout\n";
        cout << "Pilih Menu: ";
        pilihan = inputAngka();

        switch (pilihan)
        {
        case 1:
            cout << "========= Daftar Parfum ========\n";
            cout << "1. Lihat Daftar Terurut berdasarkan Nama(A-Z).\n";
            cout << "2. Lihat Daftar Terurut berdasarkan Nama(Z-A)\n";
            cout << "Pilih: ";
            pilih = inputAngka();

            if (pilih == 1)
            {
                lihat_data_parfum();
                break;
            }
            else if (pilih == 2)
            {
                daftarParfumDesc();
                break;
            }
            else
            {
                cout << "Pilihan tidak valid." << endl;
            }
        case 2:
            menu_cari_parfum();
            break;
        case 3:
            lihat_data_review();
            break;
        case 4:
            tampilkanreviewuser(userLogin->username);
            break;
        case 5:
            tambahreview(userLogin->username);
            break;
        case 6:
            editreview(userLogin->username);
            break;
        case 7:
        {
            cout << "========= Menu Hapus ========\n";
            cout << "1. Hapus Review Berdasarkan ID.\n";
            cout << "2. Hapus Review terbaru\n";
            cout << "Pilih: ";
            pilihhapus = inputAngka();

            if (pilihhapus == 1)
            {
                hapusreview(userLogin->username);
                break;
            }
            else if (pilihhapus == 2)
            {
                hapusreviewStack(userLogin->username);
                break;
            }
            else
            {
                cout << "Pilihan tidak valid." << endl;
            }
        }
        case 0:
            cout << "Anda telah logout.\n";
            break;
        default:
            cout << "Pilihan tidak valid, coba lagi.\n";
            break;
        }
    } while (pilihan != 0);
}

int main()
{
    int pilihan;
    User *userLogin = nullptr;
    read_file_user();
    do
    {
        cout << "======== Sistem Review Parfum ========\n";
        cout << "1. Registrasi\n";
        cout << "2. Login\n";
        cout << "0. Keluar\n";
        cout << "Pilih menu: ";
        pilihan = inputAngka();

        switch (pilihan)
        {
        case 1:
            registrasi();
            break;

        case 2:
            userLogin = login();
            if (userLogin != nullptr)
            {
                if (userLogin->role == 'a')
                {
                    cout << "\nSelamat datang Admin!" << endl;
                    menuadmin();
                }
                else if (userLogin->role == 'u')
                {
                    cout << "\nSelamat datang " << userLogin->username << endl;
                    menuuser(userLogin);
                }
            }
            else
            {
                cout << "Login gagal, coba lagi.\n";
            }
            break;

        case 0:
            cout << "Keluar dari program.\n";
            break;

        default:
            cout << "Pilihan tidak valid.\n";
        }
    } while (pilihan != 0);
    return 0;
}