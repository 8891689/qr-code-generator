//author：8891689
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <qrencode.h>

#define MAX_PRIVATE_KEY_LENGTH 256

// ANSI 转义码，用于设置终端颜色
#define ANSI_COLOR_BLACK   "\x1b[40m" // 黑色背景
#define ANSI_COLOR_WHITE   "\x1b[47m" // 白色背景
#define ANSI_COLOR_RESET   "\x1b[0m"  // 重置颜色

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "用法: %s <比特币私钥>\n", argv[0]);
        return 1;
    }

    const char *private_key = argv[1];
    if (strlen(private_key) > MAX_PRIVATE_KEY_LENGTH) {
        fprintf(stderr, "错误: 比特币私钥过长，最大长度为 %d 字符。\n", MAX_PRIVATE_KEY_LENGTH);
        return 1;
    }

    QRcode *qrcode = QRcode_encodeString(private_key, 0, QR_ECLEVEL_L, QR_MODE_8, 1);
    if (qrcode == NULL) {
        fprintf(stderr, "错误: QR 码编码失败。\n");
        return 1;
    }
     // 添加边框（可选）
    printf(ANSI_COLOR_WHITE);
    for (int i = 0; i < qrcode->width + 4; i++) {
        printf("  ");
    }
    printf(ANSI_COLOR_RESET "\n");


    for (int y = 0; y < qrcode->width; y++) {
         // 添加边框（可选）
        printf(ANSI_COLOR_WHITE "    " ANSI_COLOR_RESET);

        for (int x = 0; x < qrcode->width; x++) {
            if (qrcode->data[y * qrcode->width + x] & 1) {
                printf(ANSI_COLOR_BLACK "  " ANSI_COLOR_RESET); // 黑色块 (两个空格)
            } else {
                printf(ANSI_COLOR_WHITE "  " ANSI_COLOR_RESET); // 白色块 (两个空格)
            }
        }
          // 添加边框（可选）
         printf(ANSI_COLOR_WHITE "    " ANSI_COLOR_RESET);

        printf("\n");
    }

      // 添加边框（可选）
    printf(ANSI_COLOR_WHITE);
    for (int i = 0; i < qrcode->width + 4; i++) {
        printf("  ");
    }
    printf(ANSI_COLOR_RESET "\n");


    QRcode_free(qrcode);

    return 0;
}
