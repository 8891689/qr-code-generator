// author：8891689
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <qrencode.h>
#include <SDL2/SDL.h>

#define SCALE 14  // 放大倍数

int main(int argc, char *argv[]) {
  // 檢查是否提供了至少一個參數 (除了程式名稱本身)
  if (argc < 2) {
    fprintf(stderr, "用法: %s <要編碼成二維碼的文本...>\n", argv[0]);
    fprintf(stderr, "提示: 直接輸入文本即可，空格會被自動處理。\n");
    fprintf(stderr, "     如果文本包含特殊 Shell 字符，您可能仍需使用引號。\n");
    fprintf(stderr, "範例 1: %s Hello World\n", argv[0]);
    fprintf(stderr, "範例 2: %s \"Special & Characters!\"\n", argv[0]);
    return 1;
  }

  // --- 合併命令列參數 ---
  // 1. 計算總長度
  size_t total_len = 0;
  for (int i = 1; i < argc; i++) {
    total_len += strlen(argv[i]);
  }
  // 添加空格的長度 (參數數量 - 1 個參數之間，需要 argc - 1 - 1 = argc - 2 個空格)
  if (argc > 2) {
    total_len += (argc - 2);
  }
  total_len += 1; // 為結尾的 '\0' 預留空間

  // 2. 分配記憶體
  char *input_text_buffer = (char *)malloc(total_len);
  if (input_text_buffer == NULL) {
    fprintf(stderr, "错误: 無法分配記憶體來合併輸入文本。\n");
    return 1;
  }
  input_text_buffer[0] = '\0'; // 初始化為空字串，方便 strcat 或 strcpy

  // 3. 合併字串
  char *current_pos = input_text_buffer;
  for (int i = 1; i < argc; i++) {
    // 複製參數到緩衝區
    strcpy(current_pos, argv[i]);
    // 移動指針到剛複製的字串末尾
    current_pos += strlen(argv[i]);

    // 如果不是最後一個參數，添加空格
    if (i < argc - 1) {
      *current_pos = ' '; // 添加空格
      current_pos++;      // 移動指針
    }
  }
  *current_pos = '\0'; // 確保字串以 null 結尾

  // 現在 input_text_buffer 包含了完整的、用空格連接的輸入文本
  const char *input_text = input_text_buffer; // 可以用 const 指針指向它

  printf("準備編碼的文本: \"%s\"\n", input_text); // 打印合併後的文本，方便調試

  // --- 後續代碼與之前類似，使用 input_text ---

  QRcode *qrcode = QRcode_encodeString(input_text, 0, QR_ECLEVEL_L, QR_MODE_8, 1);

  if (qrcode == NULL) {
    fprintf(stderr, "错误: QR 碼編碼失敗。可能是文本過長或包含無法編碼的字元。\n");
    perror("QRcode_encodeString failed");
    free(input_text_buffer); // *** 記得釋放記憶體 ***
    return 1;
  }

  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    fprintf(stderr, "SDL 初始化失败: %s\n", SDL_GetError());
    QRcode_free(qrcode);
    free(input_text_buffer); // *** 記得釋放記憶體 ***
    return 1;
  }

  SDL_Window *window = SDL_CreateWindow(
      "QR Code Generator",
      SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
      qrcode->width * SCALE, qrcode->width * SCALE,
      SDL_WINDOW_SHOWN
  );

  if (window == NULL) {
    fprintf(stderr, "窗口创建失败: %s\n", SDL_GetError());
    SDL_Quit();
    QRcode_free(qrcode);
    free(input_text_buffer); // *** 記得釋放記憶體 ***
    return 1;
  }

  SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

  if (renderer == NULL)
  {
    fprintf(stderr, "渲染器创建失败: %s\n", SDL_GetError());
    SDL_DestroyWindow(window);
    SDL_Quit();
    QRcode_free(qrcode);
    free(input_text_buffer); // *** 記得釋放記憶體 ***
    return 1;
  }

  SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
  SDL_RenderClear(renderer);

  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  for (int y = 0; y < qrcode->width; y++) {
    for (int x = 0; x < qrcode->width; x++) {
      if (qrcode->data[y * qrcode->width + x] & 1) {
        SDL_Rect rect = { x * SCALE, y * SCALE, SCALE, SCALE };
        SDL_RenderFillRect(renderer, &rect);
      }
    }
  }

  SDL_RenderPresent(renderer);

  SDL_Event event;
  int quit = 0;
  while (!quit) {
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) {
        quit = 1;
      }
    }
     SDL_Delay(100); // 短暫延遲避免 CPU 空轉
  }

  // --- 清理資源 ---
  free(input_text_buffer);       // *** 釋放合併字串的記憶體 ***
  QRcode_free(qrcode);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
}
