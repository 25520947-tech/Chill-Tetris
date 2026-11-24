//Phần 2: chỉnh sửa lại khung viền và khối sao cho vuông vức
void draw() {
    system("cls");
    for (int i = 0; i < H; i++) {
        for (int j = 0; j < W; j++) {
            // Vẽ 4 góc khung viền
            if (i == 0 && j == 0) cout << " ╔";
            else if (i == 0 && j == W - 1) cout << "╗ ";
            else if (i == H - 1 && j == 0) cout << " ╚";
            else if (i == H - 1 && j == W - 1) cout << "╝ ";
            // Vẽ cạnh trên và cạnh dưới
            else if (i == 0 || i == H - 1) cout << "══";
            // Vẽ cạnh trái và cạnh phải (áp sát vào lòng bảng)
            else if (j == 0) cout << " ║";
            else if (j == W - 1) cout << "║ ";
            // Vẽ lòng bảng: khoảng trống hoặc block vuông
            else if (board[i][j] == ' ') cout << "  ";
            else cout << "██";
        }
        cout << '\n';
    }
}
