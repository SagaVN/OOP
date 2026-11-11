#pragma once

#include "Command.h"
#include <memory>
#include <vector>

class CommandHistory {
private:
    // Container da hinh su dung con tro thong minh unique_ptr (Yeu cau 4.3 & 4.4)
    std::vector<std::unique_ptr<Command>> danhSachUndo;
    std::vector<std::unique_ptr<Command>> danhSachRedo;

public:
    CommandHistory() = default;

    void thucThiLenh(std::unique_ptr<Command> lenh, Document& taiLieu);
    void undo(Document& taiLieu);
    void redo(Document& taiLieu);
    void xoaLichSu();

    void inLichSuUndo() const;
    size_t laySoLenhUndo() const { return danhSachUndo.size(); }
    size_t laySoLenhRedo() const { return danhSachRedo.size(); }
};
