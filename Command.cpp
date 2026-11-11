#include "Command.h"
#include <sstream>

// --- 1. InsertCommand ---
InsertCommand::InsertCommand(size_t viTriChen, const std::string& chuoi)
    : viTri{viTriChen}, chuoiThem{chuoi} {}

void InsertCommand::execute(Document& taiLieu) {
    taiLieu.insert(viTri, chuoiThem);
}

void InsertCommand::undo(Document& taiLieu) {
    // Hoan tac lenh chen = xoa doan chuoi vua chen
    taiLieu.deleteText(VungChon{viTri, chuoiThem.size()});
}

std::string InsertCommand::layMoTa() const {
    return "Insert(\"" + chuoiThem + "\" tai viTri=" + std::to_string(viTri) + ")";
}

// --- 2. DeleteCommand ---
DeleteCommand::DeleteCommand(const VungChon& vung)
    : vungXoa{vung} {}

void DeleteCommand::execute(Document& taiLieu) {
    chuoiBiXoa = taiLieu.deleteText(vungXoa);
}

void DeleteCommand::undo(Document& taiLieu) {
    // Hoan tac lenh xoa = chen lai chuoiBiXoa vao vi tri cu
    taiLieu.insert(vungXoa.layViTri(), chuoiBiXoa);
}

std::string DeleteCommand::layMoTa() const {
    std::ostringstream luong;
    luong << "Delete(" << vungXoa << ")";
    return luong.str();
}

// --- 3. ReplaceCommand ---
ReplaceCommand::ReplaceCommand(const VungChon& vung, const std::string& moi)
    : vungThay{vung}, chuoiMoi{moi} {}

void ReplaceCommand::execute(Document& taiLieu) {
    chuoiCu = taiLieu.replace(vungThay, chuoiMoi);
}

void ReplaceCommand::undo(Document& taiLieu) {
    // Hoan tac lenh thay the = thay chuoiMoi bang chuoiCu ban dau
    VungChon vungHienTai{vungThay.layViTri(), chuoiMoi.size()};
    taiLieu.replace(vungHienTai, chuoiCu);
}

std::string ReplaceCommand::layMoTa() const {
    std::ostringstream luong;
    luong << "Replace(" << vungThay << " -> \"" << chuoiMoi << "\")";
    return luong.str();
}

// --- 4. FormatCommand ---
FormatCommand::FormatCommand(const VungChon& vung, bool chonInHoa)
    : vungDinhDang{vung}, inHoa{chonInHoa} {}

void FormatCommand::execute(Document& taiLieu) {
    chuoiGoc = taiLieu.format(vungDinhDang, inHoa);
}

void FormatCommand::undo(Document& taiLieu) {
    // Hoan tac lenh format = thay the lai bang chuoiGoc ban dau
    taiLieu.replace(vungDinhDang, chuoiGoc);
}

std::string FormatCommand::layMoTa() const {
    std::ostringstream luong;
    luong << "Format(" << vungDinhDang << ", kieu=" << (inHoa ? "IN_HOA" : "in_thuong") << ")";
    return luong.str();
}

// --- CommandFactory ---
std::unique_ptr<Command> CommandFactory::taoLenhInsert(size_t viTri, const std::string& chuoi) {
    return std::make_unique<InsertCommand>(viTri, chuoi);
}

std::unique_ptr<Command> CommandFactory::taoLenhDelete(size_t viTri, size_t doDai) {
    return std::make_unique<DeleteCommand>(VungChon{viTri, doDai});
}

std::unique_ptr<Command> CommandFactory::taoLenhReplace(size_t viTri, size_t doDai, const std::string& chuoiMoi) {
    return std::make_unique<ReplaceCommand>(VungChon{viTri, doDai}, chuoiMoi);
}

std::unique_ptr<Command> CommandFactory::taoLenhFormat(size_t viTri, size_t doDai, bool inHoa) {
    return std::make_unique<FormatCommand>(VungChon{viTri, doDai}, inHoa);
}
