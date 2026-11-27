// Tao phan cai dat cua thu vien Miniaudio
#define MINIAUDIO_IMPLEMENTATION

#include "miniaudio.h" // Thu vien am thanh
#include <iostream>
#include "sound.h"
using namespace std;

ma_engine engine;// Doi tuong quan ly he thong am thanh
// Cac doi tuong am thanh trong game
ma_sound nhacNen;
ma_sound diChuyen;
ma_sound xoayKhoi;
ma_sound xoaKhoi;
ma_sound ketThuc;

// Kiem tra he thong am thanh
bool soundOK = false;


// Khoi tao he thong am thanh
void initSound() {

    // Khoi tao engine am thanh
    if (ma_engine_init(NULL, &engine) != MA_SUCCESS) {
        cout << "Khong khoi tao duoc am thanh!" << endl;
        return;
    }

    // Nap file nhac nen
    ma_sound_init_from_file(
        &engine, "sound/Nhac_nen.wav",
        0, NULL, NULL, &nhacNen
    );

    // Nap am thanh di chuyen
    ma_sound_init_from_file(
        &engine, "sound/di_chuyen_ngang.wav",
        0, NULL, NULL, &diChuyen
    );

    // Nap am thanh xoay khoi
    ma_sound_init_from_file(
        &engine, "sound/Xoay_khoi.wav",
        0, NULL, NULL, &xoayKhoi
    );

    // Nap am thanh xoa hang
    ma_sound_init_from_file(
        &engine, "sound/xoa_khoi.wav",
        0, NULL, NULL, &xoaKhoi
    );

    // Nap am thanh ket thuc game
    ma_sound_init_from_file(
        &engine, "sound/ket_thuc.wav",
        0, NULL, NULL, &ketThuc
    );

    // Cho nhac nen lap lai lien tuc
    ma_sound_set_looping(&nhacNen, MA_TRUE);

    // Danh dau he thong da khoi tao
    soundOK = true;
}

// Ham phat am thanh dung chung
void playSound(ma_sound* sound) {
    if (!soundOK) return; // Neu am thanh chua khoi tao thi khong phat
    ma_sound_seek_to_pcm_frame(sound, 0); // Dua am thanh ve vi tri ban dau
    ma_sound_start(sound);// Phat am thanh
}

// Bat dau phat nhac nen
void sound_BatDauNhacNen() {
    if (!soundOK) return; // Neu he thong chua san sang thi khong phat
    ma_sound_start(&nhacNen); // Phat nhac nen
}


// Phat am thanh khi di chuyen ngang
void sound_DiChuyenNgang() {
    playSound(&diChuyen); // Phat am thanh di chuyen
}


// Phat am thanh khi xoay khoi
void sound_XoayKhoi() {
    playSound(&xoayKhoi); // Phat am thanh xoay
} 

// Phat am thanh khi xoa hang
void sound_XoaKhoi() {
    playSound(&xoaKhoi); // Phat am thanh xoa hang
}

// Phat am thanh khi ket thuc game
void sound_KetThuc() {
    if (!soundOK) return;// Neu he thong chua khoi tao thi khong xu ly
    ma_sound_stop(&nhacNen);  // Dung nhac nen
    playSound(&ketThuc);// Phat am thanh ket thuc
}


// Dong he thong am thanh
void closeSound() {

    if (!soundOK) return;   // Neu he thong chua khoi tao thi khong dong
    // Giai phong cac am thanh
    ma_sound_uninit(&ketThuc);
    ma_sound_uninit(&xoaKhoi);
    ma_sound_uninit(&xoayKhoi);
    ma_sound_uninit(&diChuyen);
    ma_sound_uninit(&nhacNen);
    // Giai phong engine
    ma_engine_uninit(&engine);
    // Danh dau he thong da dong
    soundOK = false;
}