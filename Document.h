#pragma once

#include "ThuVienChung.h"
#include <string>
#include <vector>
#include <map>

class Document {
private:
    std::string noiDung;
    std::string tenFile{"vanban.txt"};

public:
    Document() = default;
    explicit Document(const std::string& noiDungBanDau);

    const std::string& layNoiDung() const;
    size_t layDoDai() const;
    std::string layChuoiCon(const VungChon& vung) const;

    // 4 thao tac chinh sua van ban cot loi
    void insert(size_t viTri, const std::string& chuoiThem);
    std::string deleteText(const VungChon& vung);
    std::string replace(const VungChon& vung, const std::string& chuoiMoi);
    std::string format(const VungChon& vung, bool inHoa);

    // Chuc nang tim kiem & thong ke su dung STL (vector, map, algorithm, lambda)
    std::vector<size_t> search(const std::string& tuKhoa) const;
    std::map<std::string, int> thongKeTanSuatTu() const;
    int demSoTuDaiHon(size_t doDaiToiThieu) const;

    // Luu va doc file (RAII voi ifstream / ofstream)
    void save(const std::string& duongDanFile) const;
    void load(const std::string& duongDanFile);
};
