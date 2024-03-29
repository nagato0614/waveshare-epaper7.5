#include "DEV_Config.h"
#include "Debug.h"
#include "EPD_7in5_V2.h"
#include "GUI_BMPfile.h"
#include "GUI_Paint.h"
#include "ImageData.h"
#include "curl.hpp"
#include "weather.hpp"

#include <iostream>
#include <random>
#include <signal.h>  //signal()
#include <stdlib.h>  //exit()
#include <stdlib.h>  // malloc() free()
#include <time.h>

int EPD_7in5_V2_test(void)
{
  printf("EPD_7IN5_V2_test Demo\r\n");
  if (DEV_Module_Init() != 0)
  {
    return -1;
  }

  printf("e-Paper Init and Clear...\r\n");
  EPD_7IN5_V2_Init();

  struct timespec start = {0, 0}, finish = {0, 0};
  clock_gettime(CLOCK_REALTIME, &start);
  EPD_7IN5_V2_Clear();
  clock_gettime(CLOCK_REALTIME, &finish);
  printf("%ld S\r\n", finish.tv_sec - start.tv_sec);
  DEV_Delay_ms(500);

  // Create a new image cache
  UBYTE *BlackImage;
  /* you have to edit the startup_stm32fxxx.s file and set a big enough heap size */
  UWORD Imagesize =
      ((EPD_7IN5_V2_WIDTH % 8 == 0) ? (EPD_7IN5_V2_WIDTH / 8) : (EPD_7IN5_V2_WIDTH / 8 + 1)) * EPD_7IN5_V2_HEIGHT;
  if ((BlackImage = (UBYTE *) malloc(Imagesize)) == NULL)
  {
    printf("Failed to apply for black memory...\r\n");
    return -1;
  }
  printf("Paint_NewImage\r\n");
  Paint_NewImage(BlackImage, EPD_7IN5_V2_WIDTH, EPD_7IN5_V2_HEIGHT, 0, WHITE);

#if 1  // show bmp
  printf("show window BMP-----------------\r\n");
  Paint_SelectImage(BlackImage);
  Paint_Clear(WHITE);
  GUI_ReadBmp("../pic/800x480.bmp", 0, 0);
  EPD_7IN5_V2_Display(BlackImage);
  DEV_Delay_ms(10000);

  printf("show bmp------------------------\r\n");
  Paint_SelectImage(BlackImage);
  Paint_Clear(WHITE);
  GUI_ReadBmp("../pic/100x100.bmp", 0, 0);
  EPD_7IN5_V2_Display(BlackImage);
  DEV_Delay_ms(2000);
#endif

#if 1  // show image for array
  EPD_7IN5_V2_Init_Fast();
  printf("show image for array\r\n");
  Paint_SelectImage(BlackImage);
  Paint_Clear(WHITE);
  Paint_DrawBitMap(gImage_7in5_V2);
  EPD_7IN5_V2_Display(BlackImage);
  DEV_Delay_ms(2000);
#endif

#if 1  // Drawing on the image
  // 1.Select Image
  //  EPD_7IN5_V2_Init();
  printf("SelectImage:BlackImage\r\n");
  Paint_SelectImage(BlackImage);
  Paint_Clear(WHITE);

  // 2.Drawing on the image
  printf("Drawing:BlackImage\r\n");
  Paint_DrawPoint(10, 80, BLACK, DOT_PIXEL_1X1, DOT_STYLE_DFT);
  Paint_DrawPoint(10, 90, BLACK, DOT_PIXEL_2X2, DOT_STYLE_DFT);
  Paint_DrawPoint(10, 100, BLACK, DOT_PIXEL_3X3, DOT_STYLE_DFT);
  Paint_DrawLine(20, 70, 70, 120, BLACK, DOT_PIXEL_1X1, LINE_STYLE_SOLID);
  Paint_DrawLine(70, 70, 20, 120, BLACK, DOT_PIXEL_1X1, LINE_STYLE_SOLID);
  Paint_DrawRectangle(20, 70, 70, 120, BLACK, DOT_PIXEL_1X1, DRAW_FILL_EMPTY);
  Paint_DrawRectangle(80, 70, 130, 120, BLACK, DOT_PIXEL_1X1, DRAW_FILL_FULL);
  Paint_DrawCircle(45, 95, 20, BLACK, DOT_PIXEL_1X1, DRAW_FILL_EMPTY);
  Paint_DrawCircle(105, 95, 20, WHITE, DOT_PIXEL_1X1, DRAW_FILL_FULL);
  Paint_DrawLine(85, 95, 125, 95, BLACK, DOT_PIXEL_1X1, LINE_STYLE_DOTTED);
  Paint_DrawLine(105, 75, 105, 115, BLACK, DOT_PIXEL_1X1, LINE_STYLE_DOTTED);
  Paint_DrawString_EN(10, 0, "waveshare", &Font16, BLACK, WHITE);
  Paint_DrawString_EN(10, 20, "hello world", &Font12, WHITE, BLACK);
  Paint_DrawNum(10, 33, 123456789, &Font12, BLACK, WHITE);
  Paint_DrawNum(10, 50, 987654321, &Font16, WHITE, BLACK);
  Paint_DrawString_CN(130, 0, " ���abc", &Font12CN, BLACK, WHITE);
  Paint_DrawString_CN(130, 20, "΢ѩ����", &Font24CN, WHITE, BLACK);

  printf("EPD_Display\r\n");
  EPD_7IN5_V2_Display(BlackImage);
  DEV_Delay_ms(2000);
#endif

#if 1  // Partial refresh, example shows time
  EPD_7IN5_V2_Init_Part();
  Paint_NewImage(BlackImage, Font20.Width * 7, Font20.Height, 0, WHITE);
  Debug("Partial refresh\r\n");
  Paint_SelectImage(BlackImage);
  Paint_Clear(WHITE);

  PAINT_TIME sPaint_time;
  sPaint_time.Hour = 12;
  sPaint_time.Min = 34;
  sPaint_time.Sec = 56;
  UBYTE num = 10;
  for (;;)
  {
    sPaint_time.Sec = sPaint_time.Sec + 1;
    if (sPaint_time.Sec == 60)
    {
      sPaint_time.Min = sPaint_time.Min + 1;
      sPaint_time.Sec = 0;
      if (sPaint_time.Min == 60)
      {
        sPaint_time.Hour = sPaint_time.Hour + 1;
        sPaint_time.Min = 0;
        if (sPaint_time.Hour == 24)
        {
          sPaint_time.Hour = 0;
          sPaint_time.Min = 0;
          sPaint_time.Sec = 0;
        }
      }
    }
    Paint_ClearWindows(0, 0, Font20.Width * 7, Font20.Height, WHITE);
    Paint_DrawTime(0, 0, &sPaint_time, &Font20, WHITE, BLACK);

    num = num - 1;
    if (num == 0)
    {
      break;
    }
    EPD_7IN5_V2_Display_Part(BlackImage, 150, 80, 150 + Font20.Width * 7, 80 + Font20.Height);
    DEV_Delay_ms(500);  // Analog clock 1s
  }
#endif

  printf("Clear...\r\n");
  EPD_7IN5_V2_Init();
  EPD_7IN5_V2_Clear();

  printf("Goto Sleep...\r\n");
  EPD_7IN5_V2_Sleep();
  // free(BlackImage);
  // BlackImage = NULL;
  DEV_Delay_ms(2000);  // important, at least 2s
  // close 5V
  printf("close 5V, Module enters 0 power consumption ...\r\n");
  DEV_Module_Exit();

  return 0;
}

void Handler(int signo)
{
  // System Exit
  printf("\r\nHandler:exit\r\n");
  DEV_Module_Exit();

  exit(0);
}

int print_usagi()
{
  printf("show usagi.....\n");

  if (DEV_Module_Init() != 0)
  {
    return -1;
  }

  printf("e-Paper Init and Clear...\r\n");
  EPD_7IN5_V2_Init();

  struct timespec start = {0, 0}, finish = {0, 0};
  clock_gettime(CLOCK_REALTIME, &start);
  EPD_7IN5_V2_Clear();
  clock_gettime(CLOCK_REALTIME, &finish);
  printf("%ld S\r\n", finish.tv_sec - start.tv_sec);
  DEV_Delay_ms(500);

  // Create a new image cache
  UBYTE *BlackImage;
  /* you have to edit the startup_stm32fxxx.s file and set a big enough heap size */
  UWORD Imagesize =
      ((EPD_7IN5_V2_WIDTH % 8 == 0) ? (EPD_7IN5_V2_WIDTH / 8) : (EPD_7IN5_V2_WIDTH / 8 + 1)) * EPD_7IN5_V2_HEIGHT;
  if ((BlackImage = (UBYTE *) malloc(Imagesize)) == NULL)
  {
    printf("Failed to apply for black memory...\r\n");
    return -1;
  }
  printf("Paint_NewImage\r\n");
  Paint_NewImage(BlackImage, EPD_7IN5_V2_WIDTH, EPD_7IN5_V2_HEIGHT, 0, WHITE);

  while (true)
  {
    // メルセンヌツイスタで0~51までの整数を生成
    std::random_device rnd;
    std::mt19937 mt(rnd());
    std::uniform_int_distribution<> rand(0, 51);
    int i = rand(mt);

    std::cout << "i: " << i << std::endl;
    Paint_SelectImage(BlackImage);
    Paint_Clear(WHITE);
    const std::string file_path = "../picture/" + std::to_string(i) + ".bmp";
    GUI_ReadBmp_onCenter(file_path.c_str());
    EPD_7IN5_V2_Display(BlackImage);
    DEV_Delay_ms(10000);
  }

  printf("finish printing\n");
  return 0;
}

int print_time()
{
  if (DEV_Module_Init() != 0)
  {
    return -1;
  }

  EPD_7IN5_V2_Init();
  struct timespec start = {0, 0}, finish = {0, 0};
  clock_gettime(CLOCK_REALTIME, &start);
  EPD_7IN5_V2_Clear();
  clock_gettime(CLOCK_REALTIME, &finish);
  printf("%ld S\r\n", finish.tv_sec - start.tv_sec);
  DEV_Delay_ms(500);

  // Create a new image cache
  UBYTE *BlackImage;
  /* you have to edit the startup_stm32fxxx.s file and set a big enough heap size */
  UWORD Imagesize =
      ((EPD_7IN5_V2_WIDTH % 8 == 0) ? (EPD_7IN5_V2_WIDTH / 8) : (EPD_7IN5_V2_WIDTH / 8 + 1)) * EPD_7IN5_V2_HEIGHT;
  if ((BlackImage = (UBYTE *) malloc(Imagesize)) == NULL)
  {
    printf("Failed to apply for black memory...\r\n");
    return -1;
  }
  printf("Paint_NewImage\r\n");
  Paint_NewImage(BlackImage, EPD_7IN5_V2_WIDTH, EPD_7IN5_V2_HEIGHT, 0, WHITE);

  // 2.Drawing on the image
  printf("Drawing:BlackImage\r\n");
  PAINT_TIME time;
  time.Year = 2024;
  time.Day = 15;
  time.Hour = 20;
  time.Min = 54;
  time.Sec = 36;
  Paint_Clear(WHITE);
  Paint_DrawTime(0, 0, &time, &Font24, WHITE, BLACK);

  printf("EPD_Display\r\n");
  EPD_7IN5_V2_Display(BlackImage);
  DEV_Delay_ms(2000);
}

int main(void)
{
  // Exception handling:ctrl + c
  signal(SIGINT, Handler);
  print_usagi();

  using namespace nagato;

  //   Weather weather;

  //   std::string weather_json = weather.get_weather_json();

  //   std::cout << "weather_json: " << weather_json << std::endl;
  return 0;
}