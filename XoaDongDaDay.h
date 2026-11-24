#ifndef REMOVELINE_H
#define REMOVELINE_H

// Kiem tra mot hang trong vung choi co day khoi hay khong
bool laHangDay(int chiSoHang) {
    for (int cot = 1; cot < W - 1; cot++) {
        if (board[chiSoHang][cot] == ' ') {
            return false; // Con it nhat 1 o trong -> chua day
        }
    }
    return true;
}

// Xoa tat ca cac hang day, don cac hang phia tren xuong va tra ve so dong da xoa
int removeLine() {
    int soDongDaXoa = 0;
    int hangGhi = H - 2; // Con tro ghi bat dau tu hang choi thap nhat

    // Duyet nguoc tu day vung choi len dinh vung choi
    for (int hangDoc = H - 2; hangDoc >= 1; hangDoc--) {
        if (laHangDay(hangDoc)) {
            soDongDaXoa++;
        } else {
            // Chi sao chep khi co it nhat 1 hang ben duoi da bi xoa
            if (hangGhi != hangDoc) {
                for (int cot = 1; cot < W - 1; cot++) {
                    board[hangGhi][cot] = board[hangDoc][cot];
                }
            }
            hangGhi--;
        }
    }

    // Xoa trang cac hang con thua o phia tren cung cua vung choi
    for (int hangTrong = hangGhi; hangTrong >= 1; hangTrong--) {
        for (int cot = 1; cot < W - 1; cot++) {
            board[hangTrong][cot] = ' ';
        }
    }

    return soDongDaXoa;
}

#endif
