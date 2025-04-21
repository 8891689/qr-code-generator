// author：8891689
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <qrencode.h>
#include <png.h> // 需要 libpng 庫


#define OUTPUT_FILENAME "png_qrcode.png" // 修改輸出檔名
#define PNG_SCALE 14                       // PNG 放大倍數 (每個 QR 模塊對應的像素邊長)

// 保存 PNG 函數 (基本保持不變，除了錯誤訊息微調)
void save_png(const char *filename, QRcode *qrcode, int scale) {
    FILE *fp = fopen(filename, "wb");
    if (!fp) {
        fprintf(stderr, "错误: 無法打開文件 %s 進行寫入。\n", filename);
        perror("fopen failed"); // 提供更詳細的系統錯誤信息
        return; // 返回，讓主函數知道出錯
    }

    png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) {
        fprintf(stderr, "错误: 無法創建 PNG 寫入結構。\n");
        fclose(fp);
        return;
    }

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        fprintf(stderr, "错误: 無法創建 PNG 信息結構。\n");
        png_destroy_write_struct(&png_ptr, (png_infopp)NULL);
        fclose(fp);
        return;
    }

    // 設置錯誤處理
    if (setjmp(png_jmpbuf(png_ptr))) {
        fprintf(stderr, "错误: 寫入 PNG 文件時發生錯誤。\n");
        png_destroy_write_struct(&png_ptr, &info_ptr);
        fclose(fp);
        return;
    }

    png_init_io(png_ptr, fp);

    // 計算實際的 PNG 圖像寬高
    int width = qrcode->width * scale;
    int height = qrcode->width * scale;

    // 設置 PNG 頭部信息
    // 使用 8 位灰度圖 (0=黑, 255=白)
    png_set_IHDR(png_ptr, info_ptr, width, height, 8, PNG_COLOR_TYPE_GRAY,
                   PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);

    // 可選：設置物理像素密度 (dpi)，例如 300 dpi
    // png_set_pHYs(png_ptr, info_ptr, 300 * 39.37, 300 * 39.37, PNG_RESOLUTION_METER);

    png_write_info(png_ptr, info_ptr);

    // 準備行緩衝區
    png_bytep row = (png_bytep)malloc(width * sizeof(png_byte));
    if (!row) {
         fprintf(stderr, "错误: 無法分配 PNG 行緩衝區記憶體。\n");
         png_destroy_write_struct(&png_ptr, &info_ptr);
         fclose(fp);
         return;
    }

    // 逐行寫入 PNG 數據
    for (int y = 0; y < qrcode->width; y++) {
        // 填充一行放大後的數據
        for (int x = 0; x < qrcode->width; x++) {
            // 根據 QR 碼數據設置灰度值 (0=黑, 255=白)
            unsigned char color = (qrcode->data[y * qrcode->width + x] & 1) ? 0 : 255;
            // 將這個顏色填充到放大的區域
            for (int i = 0; i < scale; i++) {
                row[x * scale + i] = color;
            }
        }
        // 將這一行數據寫入 scale 次 (垂直放大)
        for (int i = 0; i < scale; i++) {
            png_write_row(png_ptr, row);
        }
    }

    free(row); // 釋放行緩衝區

    png_write_end(png_ptr, NULL); // 結束 PNG 寫入

    // 清理 libpng 資源
    png_destroy_write_struct(&png_ptr, &info_ptr);
    fclose(fp); // 關閉文件

    printf("The QR code has been successfully saved to(二維碼成功保存到) %s\n", filename); // 成功消息移到這裡，確保文件操作都完成
}

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
    if (argc > 2) {
        total_len += (argc - 2); // 空格數量
    }
    total_len += 1; // '\0'

    // 3. 分配記憶體
    char *input_text_buffer = (char *)malloc(total_len);
    if (input_text_buffer == NULL) {
        fprintf(stderr, "错误: 無法分配記憶體來合併輸入文本。\n");
        return 1;
    }
    input_text_buffer[0] = '\0';

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
    printf("The text to be encoded(待编码的文本): \"%s\"\n", input_text);


    // 6. 編碼 QR 碼
    // 版本設為 0，讓 libqrencode 自動選擇
    QRcode *qrcode = QRcode_encodeString(input_text, 0, QR_ECLEVEL_L, QR_MODE_8, 1);
    if (qrcode == NULL) {
        fprintf(stderr, "错误: QR 碼編碼失敗。\n");
        perror("QRcode_encodeString failed");
        free(input_text_buffer); // *** 釋放記憶體 ***
        return 1;
    }

    // 7. 將 QR 碼保存為 PNG
    // save_png 函數內部會處理文件操作並打印成功/失敗信息
    save_png(OUTPUT_FILENAME, qrcode, PNG_SCALE);

    // 8. 清理資源
    QRcode_free(qrcode);
    free(input_text_buffer); // *** 釋放合併字串的記憶體 ***

    // 注意：成功消息已移至 save_png 內部，確保文件真正寫入成功才顯示
    // printf("QR 碼已保存到 %s\n", OUTPUT_FILENAME); // 原來的成功消息位置

    return 0; // 假設 save_png 內部處理了錯誤，這裡返回 0
              // 更健壯的做法是讓 save_png 返回狀態值，main 據此返回 0 或 1
}
