#include <windows.h>
#include "sound.h"
#include <iostream>

void sound_BatDauNhacNen() {
    Beep(196, 90);  
    Beep(220, 90);  
    Beep(262, 110);
}
 
namespace {
    // Phát một âm thanh hệ thống 
    void phatAmHeThong(LPCWSTR aliasId) {
        PlaySoundW(aliasId, NULL, SND_ALIAS_ID | SND_ASYNC | SND_NODEFAULT);
    }
}

void sound_DiChuyenNgang() {
    phatAmHeThong((LPCWSTR)SND_ALIAS_SYSTEMASTERISK); 
}

void sound_XoayKhoi() {
    std::cout << "\a";
    
}

void sound_XoaKhoi() {
    Beep(262, 110); 
    Beep(330, 110); 
    Beep(392, 110); 
    Beep(523, 280); 
}

void sound_KetThuc() {
    Beep(392, 120);
    Beep(293, 180);
    Beep(193, 240);
}