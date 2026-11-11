#include "CommandHistory.h"
#include <iostream>
#include <utility>

void CommandHistory::thucThiLenh(std::unique_ptr<Command> lenh, Document& taiLieu) {
    if (!lenh) return;

    // Goi execute() TRUOC khi dua vao danhSachUndo.
    // Neu execute() bi loi (nem InvalidPosition), danhSachUndo khong bi hong!
    lenh->execute(taiLieu);
    danhSachUndo.push_back(std::move(lenh));
    danhSachRedo.clear(); // Khi co thao tac moi thi xoa ngan xep Redo
}

void CommandHistory::undo(Document& taiLieu) {
    if (danhSachUndo.empty()) {
        throw NothingToUndo("Lich su Undo dang trong, khong the hoan tac!");
    }
    // Lay lenh o dinh ngan xep Undo ra
    std::unique_ptr<Command> lenh = std::move(danhSachUndo.back());
    danhSachUndo.pop_back();

    // Goi da hinh ham undo()
    lenh->undo(taiLieu);

    // Chuyen lenh sang ngan xep Redo
    danhSachRedo.push_back(std::move(lenh));
}

void CommandHistory::redo(Document& taiLieu) {
    if (danhSachRedo.empty()) {
        throw NothingToUndo("Lich su Redo dang trong, khong the lam lai!");
    }
    std::unique_ptr<Command> lenh = std::move(danhSachRedo.back());
    danhSachRedo.pop_back();

    // Thuc thi lai lenh
    lenh->execute(taiLieu);
    danhSachUndo.push_back(std::move(lenh));
}

void CommandHistory::xoaLichSu() {
    danhSachUndo.clear();
    danhSachRedo.clear();
}

void CommandHistory::inLichSuUndo() const {
    if (danhSachUndo.empty()) {
        std::cout << "  (Chua co lenh nao trong lich su)\n";
        return;
    }
    for (size_t i = 0; i < danhSachUndo.size(); ++i) {
        std::cout << "  " << (i + 1) << ". " << danhSachUndo[i]->layMoTa() << "\n";
    }
}
