// author：8891689
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <qrencode.h>


// ANSI 转义码，用于设置终端颜色
#define ANSI_COLOR_BLACK_BG   "\x1b[40m" // 黑色背景
#define ANSI_COLOR_WHITE_BG   "\x1b[47m" // 白色背景
#define ANSI_COLOR_RESET   "\x1b[0m"  // 重置颜色
#define MODULE_STRING "  "         // 用兩個空格代表一個 QR 模塊，讓方塊更明顯

int main(int argc, char *argv[]) {
    // 1. 檢查參數數量
    if (argc < 2) {
        fprintf(stderr, "用法: %s <要編碼成二維碼的文本...>\n", argv[0]);
        fprintf(stderr, "提示: 直接輸入文本即可，空格會被自動處理。\n");
        fprintf(stderr, "     如果文本包含特殊 Shell 字符(例如 * & | ! $)，建議使用引號括起來。\n");
        fprintf(stderr, "範例 1: %s Hello World\n", argv[0]);
        fprintf(stderr, "範例 2: %s \"Special & Characters!\"\n", argv[0]);
        return 1;
    }

    // --- 合併命令列參數 ---
    // 2. 計算總長度
    size_t total_len = 0;
    for (int i = 1; i < argc; i++) {
        total_len += strlen(argv[i]);
    }
    // 添加空格的長度 (argc - 2 個空格)
    if (argc > 2) {
        total_len += (argc - 2);
    }
    total_len += 1; // 為結尾的 '\0'

    // 3. 分配記憶體
    char *input_text_buffer = (char *)malloc(total_len);
    if (input_text_buffer == NULL) {
        fprintf(stderr, "错误: 無法分配記憶體來合併輸入文本。\n");
        return 1;
    }
    input_text_buffer[0] = '\0'; // 初始化

    // 4. 合併字串
    char *current_pos = input_text_buffer;
    for (int i = 1; i < argc; i++) {
        strcpy(current_pos, argv[i]);
        current_pos += strlen(argv[i]);
        if (i < argc - 1) {
            *current_pos = ' ';
            current_pos++;
        }
    }
    *current_pos = '\0';

    // 5. 使用合併後的字串
    const char *input_text = input_text_buffer;
    printf("準備編碼的文本: \"%s\"\n", input_text); // 調試輸出



    // 6. 編碼 QR 碼
    QRcode *qrcode = QRcode_encodeString(input_text, 0, QR_ECLEVEL_L, QR_MODE_8, 1);
    if (qrcode == NULL) {
        fprintf(stderr, "错误: QR 碼編碼失敗。\n");
        perror("QRcode_encodeString failed");
        free(input_text_buffer); // *** 釋放記憶體 ***
        return 1;
    }

    // --- 在終端打印 QR 碼 ---
    // 計算邊框寬度（使用 MODULE_STRING 的寬度）
    // 這裡我們使用一個模塊寬度的白色邊框
    const int border_width = 1;
    const int qr_dimension = qrcode->width;
    const int total_dimension = qr_dimension + border_width * 2;

    // 打印上邊框
    printf(ANSI_COLOR_WHITE_BG);
    for (int i = 0; i < total_dimension; i++) {
        printf("%s", MODULE_STRING);
    }
    printf(ANSI_COLOR_RESET "\n");

    // 打印 QR 碼內容和左右邊框
    for (int y = 0; y < qr_dimension; y++) {
        // 左邊框
        printf(ANSI_COLOR_WHITE_BG);
        for(int b=0; b<border_width; ++b) printf("%s", MODULE_STRING);
        printf(ANSI_COLOR_RESET);

        // QR 碼行
        for (int x = 0; x < qr_dimension; x++) {
            if (qrcode->data[y * qr_dimension + x] & 1) {
                printf(ANSI_COLOR_BLACK_BG "%s" ANSI_COLOR_RESET, MODULE_STRING); // 黑色块
            } else {
                printf(ANSI_COLOR_WHITE_BG "%s" ANSI_COLOR_RESET, MODULE_STRING); // 白色块
            }
        }

        // 右邊框
        printf(ANSI_COLOR_WHITE_BG);
         for(int b=0; b<border_width; ++b) printf("%s", MODULE_STRING);
        printf(ANSI_COLOR_RESET);

        printf("\n");
    }

    // 打印下邊框
    printf(ANSI_COLOR_WHITE_BG);
    for (int i = 0; i < total_dimension; i++) {
        printf("%s", MODULE_STRING);
    }
    printf(ANSI_COLOR_RESET "\n");

    // --- 清理資源 ---
    QRcode_free(qrcode);
    free(input_text_buffer); // *** 釋放合併字串的記憶體 ***

    return 0;
}
