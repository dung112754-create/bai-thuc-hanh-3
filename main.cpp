#include <iostream>
#include <string>
using namespace std;

class phuong_tien {
private:
    int ma_xe;
    string loai_xe;
    double gio_do;
public:
    phuong_tien() : ma_xe(0), loai_xe(""), gio_do(0) {}
    phuong_tien(int ma, string loai, double gio) : ma_xe(ma), loai_xe(loai), gio_do(gio) {}
    int lay_ma_xe() const { return ma_xe; }
    string lay_loai_xe() const { return loai_xe; }
    double tinh_phi() const {
        double gia = (loai_xe == "Oto") ? 20000 : (loai_xe == "Xetai") ? 30000 : 5000;
        return gia * gio_do;
    }
    void hien_thi() const {
        cout << "  Xe " << ma_xe << " | " << loai_xe << " | " << gio_do << "h | Phi: " << tinh_phi() << "\n";
    }
};

class bai_xe {
private:
    int ma_bai;
    char kich_thuoc;
    int suc_chua;
    int so_xe;
    phuong_tien** xe;
    void resize(int cap_moi);
public:
    bai_xe(int ma, char kt);
    ~bai_xe();
    bai_xe(const bai_xe& o);
    bai_xe& operator=(const bai_xe& o);
    int lay_ma_bai() const { return ma_bai; }
    int lay_so_xe() const { return so_xe; }
    bool them(const phuong_tien& v);
    bool xoa(int ma_xe);
    bool chua(int ma_xe) const;
    void hien_thi() const;
    bool co_loai(string loai, int& dem, double& tong) const;
    double tong_phi() const;
};

bai_xe::bai_xe(int ma, char kt) {
    ma_bai = ma;
    kich_thuoc = kt;
    suc_chua = (kt == 'S') ? 1 : (kt == 'M') ? 2 : 3;
    so_xe = 0;
    xe = new phuong_tien*[suc_chua];
    for (int i = 0; i < suc_chua; i++) xe[i] = nullptr;
}

bai_xe::~bai_xe() {
    for (int i = 0; i < so_xe; i++) delete xe[i];
    delete[] xe;
}

bai_xe::bai_xe(const bai_xe& o) {
    ma_bai = o.ma_bai;
    kich_thuoc = o.kich_thuoc;
    suc_chua = o.suc_chua;
    so_xe = o.so_xe;
    xe = new phuong_tien*[suc_chua];
    for (int i = 0; i < suc_chua; i++) xe[i] = nullptr;
    for (int i = 0; i < so_xe; i++) xe[i] = new phuong_tien(*o.xe[i]);
}

bai_xe& bai_xe::operator=(const bai_xe& o) {
    if (this == &o) return *this;
    for (int i = 0; i < so_xe; i++) delete xe[i];
    delete[] xe;
    ma_bai = o.ma_bai;
    kich_thuoc = o.kich_thuoc;
    suc_chua = o.suc_chua;
    so_xe = o.so_xe;
    xe = new phuong_tien*[suc_chua];
    for (int i = 0; i < suc_chua; i++) xe[i] = nullptr;
    for (int i = 0; i < so_xe; i++) xe[i] = new phuong_tien(*o.xe[i]);
    return *this;
}

void bai_xe::resize(int cap_moi) {
    phuong_tien** moi = new phuong_tien*[cap_moi];
    for (int i = 0; i < cap_moi; i++) moi[i] = nullptr;
    for (int i = 0; i < so_xe; i++) moi[i] = xe[i];
    delete[] xe;
    xe = moi;
    suc_chua = cap_moi;
}

bool bai_xe::them(const phuong_tien& v) {
    if (so_xe == suc_chua) resize(suc_chua * 2);
    xe[so_xe] = new phuong_tien(v);
    so_xe++;
    return true;
}

bool bai_xe::xoa(int ma_xe) {
    for (int i = 0; i < so_xe; i++) {
        if (xe[i]->lay_ma_xe() == ma_xe) {
            delete xe[i];
            for (int j = i; j < so_xe - 1; j++) xe[j] = xe[j + 1];
            xe[so_xe - 1] = nullptr;
            so_xe--;
            return true;
        }
    }
    return false;
}

bool bai_xe::chua(int ma_xe) const {
    for (int i = 0; i < so_xe; i++)
        if (xe[i]->lay_ma_xe() == ma_xe) return true;
    return false;
}

void bai_xe::hien_thi() const {
    cout << "Bai " << ma_bai << " [" << kich_thuoc << "] "
         << so_xe << "/" << suc_chua << " xe\n";
    for (int i = 0; i < so_xe; i++) xe[i]->hien_thi();
}

bool bai_xe::co_loai(string loai, int& dem, double& tong) const {
    dem = 0; tong = 0;
    for (int i = 0; i < so_xe; i++) {
        if (xe[i]->lay_loai_xe() == loai) { ++dem; tong += xe[i]->tinh_phi(); }
    }
    return dem > 0;
}

double bai_xe::tong_phi() const {
    double t = 0;
    for (int i = 0; i < so_xe; i++) t += xe[i]->tinh_phi();
    return t;
}

class SystemInterface {
private:
    int lotCapacity;
    int currentLotCount;
    bai_xe** parkingLots;
    int timBai(int id) const;
public:
    SystemInterface();
    ~SystemInterface();
    SystemInterface(const SystemInterface& o);
    SystemInterface& operator=(const SystemInterface& o);
    void resizeSystem(int newCapacity);
    void runMenu();
};

SystemInterface::SystemInterface() {
    lotCapacity = 5;
    currentLotCount = 0;
    parkingLots = new bai_xe*[lotCapacity];
    for (int i = 0; i < lotCapacity; i++) parkingLots[i] = nullptr;
}

SystemInterface::~SystemInterface() {
    for (int i = 0; i < currentLotCount; i++) delete parkingLots[i];
    delete[] parkingLots;
}

SystemInterface::SystemInterface(const SystemInterface& o) {
    lotCapacity = o.lotCapacity;
    currentLotCount = o.currentLotCount;
    parkingLots = new bai_xe*[lotCapacity];
    for (int i = 0; i < lotCapacity; i++) parkingLots[i] = nullptr;
    for (int i = 0; i < currentLotCount; i++) parkingLots[i] = new bai_xe(*o.parkingLots[i]);
}

SystemInterface& SystemInterface::operator=(const SystemInterface& o) {
    if (this == &o) return *this;
    for (int i = 0; i < currentLotCount; i++) delete parkingLots[i];
    delete[] parkingLots;
    lotCapacity = o.lotCapacity;
    currentLotCount = o.currentLotCount;
    parkingLots = new bai_xe*[lotCapacity];
    for (int i = 0; i < lotCapacity; i++) parkingLots[i] = nullptr;
    for (int i = 0; i < currentLotCount; i++) parkingLots[i] = new bai_xe(*o.parkingLots[i]);
    return *this;
}

int SystemInterface::timBai(int id) const {
    for (int i = 0; i < currentLotCount; i++)
        if (parkingLots[i]->lay_ma_bai() == id) return i;
    return -1;
}

void SystemInterface::resizeSystem(int newCapacity) {
    bai_xe** moi = new bai_xe*[newCapacity];
    for (int i = 0; i < newCapacity; i++) moi[i] = nullptr;
    for (int i = 0; i < currentLotCount; i++) moi[i] = parkingLots[i];
    delete[] parkingLots;
    parkingLots = moi;
    lotCapacity = newCapacity;
    cout << "He thong da mo rong len " << lotCapacity << " bai\n";
}

void SystemInterface::runMenu() {
    int chon;
    while (true) {
        cout << "\n===== QUAN LY BAI DO XE =====\n";
        cout << "1. Tao bai do\n";
        cout << "2. Them xe\n";
        cout << "3. Xoa bai do\n";
        cout << "4. Lay xe ra\n";
        cout << "5. Hien thi tat ca bai do\n";
        cout << "6. Hien thi xe theo bai\n";
        cout << "7. Liet ke xe theo loai\n";
        cout << "8. Bao cao phi trung binh\n";
        cout << "9. Thoat\n";
        cout << "Chon: ";
        cin >> chon;

        if (chon == 1) {
            int id; char size;
            cout << "Nhap ma bai: "; cin >> id;
            cout << "Nhap kich thuoc (S/M/L): "; cin >> size;
            if (timBai(id) != -1) { cout << "Ma bai da ton tai!\n"; continue; }
            if (size != 'S' && size != 'M' && size != 'L') { cout << "Kich thuoc khong hop le!\n"; continue; }
            if (currentLotCount == lotCapacity) resizeSystem(lotCapacity * 2);
            parkingLots[currentLotCount++] = new bai_xe(id, size);
            cout << "Da tao bai " << id << "\n";
        }
        else if (chon == 2) {
            int idBai, maXe, loai; double gio;
            cout << "Nhap ma bai: "; cin >> idBai;
            int vt = timBai(idBai);
            if (vt == -1) { cout << "Khong tim thay bai!\n"; continue; }
            cout << "Nhap ma xe: "; cin >> maXe;
            bool trung = false;
            for (int i = 0; i < currentLotCount; i++)
                if (parkingLots[i]->chua(maXe)) trung = true;
            if (trung) { cout << "Ma xe da ton tai!\n"; continue; }
            cout << "Loai xe (1. Xemay, 2. Oto, 3. Xetai): "; cin >> loai;
            string ten = (loai == 1) ? "Xemay" : (loai == 2) ? "Oto" : "Xetai";
            cout << "Thoi gian do (gio): "; cin >> gio;
            parkingLots[vt]->them(phuong_tien(maXe, ten, gio));
            cout << "Da them xe " << maXe << " vao bai " << idBai << "\n";
        }
        else if (chon == 3) {
            int id;
            cout << "Nhap ma bai can xoa: "; cin >> id;
            int vt = timBai(id);
            if (vt == -1) { cout << "Khong tim thay bai!\n"; continue; }
            delete parkingLots[vt];
            for (int i = vt; i < currentLotCount - 1; i++) parkingLots[i] = parkingLots[i + 1];
            parkingLots[currentLotCount - 1] = nullptr;
            currentLotCount--;
            cout << "Da xoa bai " << id << "\n";
        }
        else if (chon == 4) {
            int maXe;
            cout << "Nhap ma xe can lay: "; cin >> maXe;
            bool ok = false;
            for (int i = 0; i < currentLotCount && !ok; i++)
                if (parkingLots[i]->xoa(maXe)) ok = true;
            if (ok) cout << "Da lay xe ra\n";
            else cout << "Khong tim thay xe!\n";
        }
        else if (chon == 5) {
            if (currentLotCount == 0) cout << "Chua co bai do nao\n";
            for (int i = 0; i < currentLotCount; i++)
                if (parkingLots[i] != nullptr) parkingLots[i]->hien_thi();
        }
        else if (chon == 6) {
            int id;
            cout << "Nhap ma bai: "; cin >> id;
            int vt = timBai(id);
            if (vt == -1) cout << "Khong tim thay bai!\n";
            else parkingLots[vt]->hien_thi();
        }
        else if (chon == 7) {
            int loai;
            cout << "Loai xe (1. Xemay, 2. Oto, 3. Xetai): "; cin >> loai;
            string ten = (loai == 1) ? "Xemay" : (loai == 2) ? "Oto" : "Xetai";
            int tongDem = 0;
            for (int i = 0; i < currentLotCount; i++) {
                int dem; double tong;
                if (parkingLots[i]->co_loai(ten, dem, tong)) {
                    cout << "Bai " << parkingLots[i]->lay_ma_bai() << ": " << dem
                         << " xe " << ten << ", tong phi " << tong << "\n";
                    tongDem += dem;
                }
            }
            if (tongDem == 0) cout << "Khong co xe loai nay\n";
        }
        else if (chon == 8) {
            int tongXe = 0; double tongPhi = 0;
            for (int i = 0; i < currentLotCount; i++) {
                tongXe += parkingLots[i]->lay_so_xe();
                tongPhi += parkingLots[i]->tong_phi();
            }
            if (tongXe == 0) cout << "Chua co xe nao\n";
            else cout << "Phi trung binh: " << tongPhi / tongXe << " (" << tongXe << " xe)\n";
        }
        else if (chon == 9) {
            cout << "Thoat chuong trinh\n";
            break;
        }
        else {
            cout << "Lua chon khong hop le!\n";
        }
    }
}

int main() {
    SystemInterface he_thong;
    he_thong.runMenu();
    return 0;
}