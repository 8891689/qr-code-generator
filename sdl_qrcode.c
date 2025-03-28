//author：8891689
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <qrencode.h>
#include <SDL2/SDL.h>

#define MAX_PRIVATE_KEY_LENGTH 256
#define SCALE 14  // 放大倍数

int main(int argc, char *argv[]) {
  if (argc != 2) {
    fprintf(stderr, "用法: %s <比特币私钥>\n", argv[0]);
    return 1;
  }

  const char *private_key = argv[1];
  if (strlen(private_key) > MAX_PRIVATE_KEY_LENGTH) {
    fprintf(stderr, "错误: 比特币私钥过长。\n", MAX_PRIVATE_KEY_LENGTH);
    return 1;
  }

  QRcode *qrcode = QRcode_encodeString(private_key, 0, QR_ECLEVEL_L, QR_MODE_8, 1);
  if (qrcode == NULL) {
    fprintf(stderr, "错误: QR 码编码失败。\n");
    return 1;
  }

  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    fprintf(stderr, "SDL 初始化失败: %s\n", SDL_GetError());
    QRcode_free(qrcode);
    return 1;
  }

  SDL_Window *window = SDL_CreateWindow(
      "Bitcoin Private Key QR Code",
      SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
      qrcode->width * SCALE, qrcode->width * SCALE,
      SDL_WINDOW_SHOWN
  );
  if (window == NULL) {
    fprintf(stderr, "窗口创建失败: %s\n", SDL_GetError());
    SDL_Quit();
    QRcode_free(qrcode);
    return 1;
  }

  SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  if (renderer == NULL)
  {
    fprintf(stderr, "renderer创建失败: %s\n", SDL_GetError());
    SDL_DestroyWindow(window);
    SDL_Quit();
      QRcode_free(qrcode);
    return 1;
  }


  SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // White background
  SDL_RenderClear(renderer);

  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Black for QR code
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
  }


    SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
  QRcode_free(qrcode);

  return 0;
}
