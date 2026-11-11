#include "Document.h"
#include <algorithm>
#include <cctype>
#include <fstream>
#include <sstream>

Document::Document(const std::string& noiDungBanDau)
    : noiDung{noiDungBanDau} {}

const std::string& Document::layNoiDung() const {
    return noiDung;
}

size_t Document::layDoDai() const {
    return noiDung.size();
}

std::string Document::layChuoiCon(const VungChon& vung) const {
    vung.kiemTraHopLe(noiDung.size());
    return noiDung.substr(vung.layViTri(), vung.layDoDai());
}

void Document::insert(size_t viTri, const std::string& chuoiThem) {
    // Kiem tra dieu kien truoc khi sua de dam bao An toan ngoai le (Exception Safety)
    if (viTri > noiDung.size()) {
        throw InvalidPosition(
            "Vi tri chen (" + std::to_string(viTri) +
            ") vuot qua do dai van ban (" + std::to_string(noiDung.size()) + ")!"
        );
    }
    noiDung.insert(viTri, chuoiThem);
}

std::string Document::deleteText(const VungChon& vung) {
    vung.kiemTraHopLe(noiDung.size());
    std::string chuoiBiXoa = noiDung.substr(vung.layViTri(), vung.layDoDai());
    noiDung.erase(vung.layViTri(), vung.layDoDai());
    return chuoiBiXoa; // Tra ve chuoi bi xoa de phuc vu Undo
}

std::string Document::replace(const VungChon& vung, const std::string& chuoiMoi) {
    vung.kiemTraHopLe(noiDung.size());
    std::string chuoiCu = noiDung.substr(vung.layViTri(), vung.layDoDai());
    noiDung.replace(vung.layViTri(), vung.layDoDai(), chuoiMoi);
    return chuoiCu; // Tra ve chuoi cu de phuc vu Undo
}

std::string Document::format(const VungChon& vung, bool inHoa) {
    vung.kiemTraHopLe(noiDung.size());
    std::string chuoiGoc = noiDung.substr(vung.layViTri(), vung.layDoDai());
    std::string chuoiBienDoi = chuoiGoc;

    // Dung thuat toan STL std::transform ket hop bieu thuc Lambda
    std::transform(chuoiBienDoi.begin(), chuoiBienDoi.end(), chuoiBienDoi.begin(),
                   [inHoa](unsigned char kyTu) {
                       return inHoa ? static_cast<char>(std::toupper(kyTu))
                                    : static_cast<char>(std::tolower(kyTu));
                   });

    noiDung.replace(vung.layViTri(), vung.layDoDai(), chuoiBienDoi);
    return chuoiGoc; // Tra ve chuoi goc truoc khi format de Undo
}

std::vector<size_t> Document::search(const std::string& tuKhoa) const {
    std::vector<size_t> danhSachViTri;
    if (tuKhoa.empty()) return danhSachViTri;

    size_t viTriTimThay = noiDung.find(tuKhoa, 0);
    while (viTriTimThay != std::string::npos) {
        danhSachViTri.push_back(viTriTimThay);
        viTriTimThay = noiDung.find(tuKhoa, viTriTimThay + 1);
    }
    return danhSachViTri;
}

std::map<std::string, int> Document::thongKeTanSuatTu() const {
    std::map<std::string, int> bangTanSuat;
    std::istringstream luongChuoi(noiDung);
    std::string tu;
    while (luongChuoi >> tu) {
        bangTanSuat[tu]++;
    }
    return bangTanSuat;
}

int Document::demSoTuDaiHon(size_t doDaiToiThieu) const {
    std::istringstream luongChuoi(noiDung);
    std::vector<std::string> danhSachTu;
    std::string tu;
    while (luongChuoi >> tu) {
        danhSachTu.push_back(tu);
    }
    // Dung std::count_if trong <algorithm> va Lambda
    return static_cast<int>(std::count_if(
        danhSachTu.begin(), danhSachTu.end(),
        [doDaiToiThieu](const std::string& tuHienTai) {
            return tuHienTai.size() > doDaiToiThieu;
        }
    ));
}

void Document::save(const std::string& duongDanFile) const {
    std::ofstream fileGhi(duongDanFile);
    if (!fileGhi.is_open()) {
        throw std::runtime_error("Khong the mo file de ghi: " + duongDanFile);
    }
    fileGhi << noiDung;
    // Khi thoat khoi ham, fileGhi tu dong dong lai nho co che RAII
}

void Document::load(const std::string& duongDanFile) {
    std::ifstream fileDoc(duongDanFile);
    if (!fileDoc.is_open()) {
        throw std::runtime_error("Khong the mo file de doc: " + duongDanFile);
    }
    std::ostringstream boDem;
    boDem << fileDoc.rdbuf();
    // Chi cap nhat noiDung sau khi doc xong hoan toan (An toan ngoai le)
    noiDung = boDem.str();
    tenFile = duongDanFile;
}
