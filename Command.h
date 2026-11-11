#pragma once

#include "Document.h"
#include <memory>
#include <string>

// ============================================================================
// CAY KE THUA 5 LOP COMMAND (Yeu cau 4.1 & 4.3: Co virtual destructor & da hinh)
// ============================================================================
class Command {
public:
    virtual ~Command() = default; // Ham huy ao bat buoc cho lop co so da hinh
    virtual void execute(Document& taiLieu) = 0;
    virtual void undo(Document& taiLieu) = 0;
    virtual std::string layMoTa() const = 0;
};

// Lop con 1: Lenh Chen van ban
class InsertCommand : public Command {
private:
    size_t viTri;
    std::string chuoiThem;

public:
    InsertCommand(size_t viTriChen, const std::string& chuoi);
    void execute(Document& taiLieu) override;
    void undo(Document& taiLieu) override;
    std::string layMoTa() const override;
};

// Lop con 2: Lenh Xoa van ban
class DeleteCommand : public Command {
private:
    VungChon vungXoa;
    std::string chuoiBiXoa; // Luu lai de khoi phuc khi Undo

public:
    explicit DeleteCommand(const VungChon& vung);
    void execute(Document& taiLieu) override;
    void undo(Document& taiLieu) override;
    std::string layMoTa() const override;
};

// Lop con 3: Lenh Thay the van ban
class ReplaceCommand : public Command {
private:
    VungChon vungThay;
    std::string chuoiMoi;
    std::string chuoiCu; // Luu lai de khoi phuc khi Undo

public:
    ReplaceCommand(const VungChon& vung, const std::string& moi);
    void execute(Document& taiLieu) override;
    void undo(Document& taiLieu) override;
    std::string layMoTa() const override;
};

// Lop con 4: Lenh Dinh dang van ban (In hoa / In thuong)
class FormatCommand : public Command {
private:
    VungChon vungDinhDang;
    bool inHoa;
    std::string chuoiGoc; // Luu lai de khoi phuc khi Undo

public:
    FormatCommand(const VungChon& vung, bool chonInHoa);
    void execute(Document& taiLieu) override;
    void undo(Document& taiLieu) override;
    std::string layMoTa() const override;
};

// ============================================================================
// MAU THIET KE FACTORY PATTERN (Yeu cau 4.8: CommandFactory)
// ============================================================================
class CommandFactory {
public:
    static std::unique_ptr<Command> taoLenhInsert(size_t viTri, const std::string& chuoi);
    static std::unique_ptr<Command> taoLenhDelete(size_t viTri, size_t doDai);
    static std::unique_ptr<Command> taoLenhReplace(size_t viTri, size_t doDai, const std::string& chuoiMoi);
    static std::unique_ptr<Command> taoLenhFormat(size_t viTri, size_t doDai, bool inHoa);
};
